#include "image.h"

#include <engine/storage.h>
#include <engine/shared/datafile.h>
#include <tu/client/assetsmanager.h>

#include <engine/external/pnglite/pnglite.h>

namespace tu
{

CAsset_Image::CAsset_Image() :
	m_pData(0),
	m_NeedUpdate(false),
	m_LastUpdate(-1),
	m_GridWidth(1),
	m_GridHeight(1),
	m_External(false)
{
	
}

CAsset_Image::CAsset_Image(const CAsset_Image& Image) :
	m_pData(0),
	m_NeedUpdate(false),
	m_LastUpdate(-1),
	m_GridWidth(1),
	m_GridHeight(1),
	m_External(false)
{
	*this = Image;
}

CAsset_Image::~CAsset_Image()
{
	if(m_pData)
		delete[] m_pData;
}

CAsset_Image& CAsset_Image::operator=(const CAsset_Image& Image)
{
	CAsset::operator=(Image);
	
	str_copy(m_aFilename, Image.m_aFilename, sizeof(m_aFilename));
	
	if(Image.m_pData)
	{
		m_LastUpdate = Image.m_LastUpdate;
		SetData(Image.m_Width, Image.m_Height, Image.m_GridWidth, Image.m_GridHeight, Image.m_Format, Image.m_pData);
		m_Texture = Image.m_Texture;
	}
	else
	{
		m_Width = 0;
		m_Height = 0;
		m_GridWidth = 1;
		m_GridHeight = 1;
		m_NeedUpdate = false;
		m_External = false;
		m_LastUpdate = -1;
	}
	
	return *this;
}

void CAsset_Image::Update()
{
	if(!m_NeedUpdate)
		return;
	
	if(m_LastUpdate != -1 && (time_get()-m_LastUpdate < time_freq()/60))
		return;
	
	AssetsManager()->Graphics()->UnloadTexture(&m_Texture);
	
	int Flags = 0;
	if(m_GridWidth > 1 || m_GridHeight > 1)
		Flags = IGraphics::TEXLOAD_MULTI_DIMENSION;
	
	m_Texture = AssetsManager()->Graphics()->LoadTextureRaw(m_Width, m_Height, m_GridWidth, m_GridHeight, m_Format, m_pData, CImageInfo::FORMAT_AUTO, Flags);

	m_NeedUpdate = false;
	m_LastUpdate = time_get();
}

void CAsset_Image::Unload()
{
	if(m_Texture.IsValid())
	{
		AssetsManager()->Graphics()->UnloadTexture(&m_Texture);
	}
	
	if(m_pData)
	{
		delete[] m_pData;
		m_pData = 0;
	}
}

int CAsset_Image::GetDataSize() const
{
	int PixelSize;
	switch(m_Format)
	{
		case FORMAT_ALPHA:
			PixelSize = 1;
			break;
		case FORMAT_RGB:
			PixelSize = 3;
			break;
		case FORMAT_RGBA:
			PixelSize = 4;
			break;
	}
	
	return m_Width*m_Height*PixelSize;
}

void CAsset_Image::LoadData(const char* pFilename, int GridWidth, int GridHeight)
{
	char aCompleteFilename[512];
	unsigned char *pBuffer;
	png_t Png; // ignore_convention

	// open file for reading
	png_init(0,0); // ignore_convention

	IOHANDLE File = AssetsManager()->Storage()->OpenFile(pFilename, IOFLAG_READ, IStorage::TYPE_ALL, aCompleteFilename, sizeof(aCompleteFilename));
	if(File)
	{
		io_close(File);
	}
	else
	{
		dbg_msg("mod", "failed to open file. filename='%s'", pFilename);
		return;
	}

	int Error = png_open_file(&Png, aCompleteFilename); // ignore_convention
	if(Error != PNG_NO_ERROR)
	{
		dbg_msg("mod", "failed to open file. filename='%s'", aCompleteFilename);
		if(Error != PNG_FILE_ERROR)
		{
			png_close_file(&Png); // ignore_convention
		}
		return;
	}

	if(Png.depth != 8 || (Png.color_type != PNG_TRUECOLOR && Png.color_type != PNG_TRUECOLOR_ALPHA) || Png.width > (2<<12) || Png.height > (2<<12)) // ignore_convention
	{
		dbg_msg("mod", "invalid format. filename='%s'", aCompleteFilename);
		png_close_file(&Png); // ignore_convention
		return;
	}

	pBuffer = (unsigned char *) mem_alloc(Png.width * Png.height * Png.bpp, 1); // ignore_convention
	png_get_data(&Png, pBuffer); // ignore_convention
	png_close_file(&Png); // ignore_convention

	int Format;
	if(Png.color_type == PNG_TRUECOLOR) 
		Format = CImageInfo::FORMAT_RGB;
	else if(Png.color_type == PNG_TRUECOLOR_ALPHA)
		Format = CImageInfo::FORMAT_RGBA;
	
	if(GridWidth < 0)
		GridWidth = Png.width/64;
	if(GridHeight < 0)
		GridHeight = Png.height/64;
	
	m_External = true;
	SetData(Png.width, Png.height, GridWidth, GridHeight, Format, pBuffer);
	mem_free(pBuffer);
}

void CAsset_Image::SetData(int Width, int Height, int GridWidth, int GridHeight, int Format, unsigned char* pData)
{
	if(m_pData)
		delete[] m_pData;
	
	m_Width = Width;
	m_Height = Height;
	m_GridWidth = GridWidth;
	m_GridHeight = GridHeight;
	m_Format = Format;
	
	int PixelSize;
	switch(m_Format)
	{
		case FORMAT_ALPHA:
			PixelSize = 1;
			break;
		case FORMAT_RGB:
			PixelSize = 3;
			break;
		case FORMAT_RGBA:
			PixelSize = 4;
			break;
	}
	
	int DataSize = GetDataSize();
	m_pData = new unsigned char[DataSize];
	mem_copy(m_pData, pData, DataSize*sizeof(unsigned char));
	
	m_NeedUpdate = true;
}

void CAsset_Image::SetGridSize(int GridWidth, int GridHeight)
{
	m_GridWidth = max(GridWidth, 1);
	m_GridHeight = max(GridHeight, 1);
	m_NeedUpdate = true;
}

vec4 CAsset_Image::GetColor(int x, int y)
{
	switch(m_Format)
	{
		case FORMAT_RGB:
			return vec4(m_pData[(y*m_Width+x)*3+0]/255.0f, m_pData[(y*m_Width+x)*3+1]/255.0f, m_pData[(y*m_Width+x)*3+2]/255.0f, 1.0f);
		case FORMAT_RGBA:
			return vec4(m_pData[(y*m_Width+x)*4+0]/255.0f, m_pData[(y*m_Width+x)*4+1]/255.0f, m_pData[(y*m_Width+x)*4+2]/255.0f, m_pData[(y*m_Width+x)*4+3]/255.0f);
		case FORMAT_ALPHA:
			return vec4(0.0f, 0.0f, 0.0f, m_pData[y*m_Width+x]/255.0f);
	}
	
	return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

vec4 CAsset_Image::Sample(vec2 uv)
{
    int x0 = clamp(static_cast<int>(floor(uv.x * m_Width)), 0, m_Width-1);
    int x1 = clamp(x0 + 1, 0, m_Width - 1);
    int y0 = clamp(static_cast<int>(floor(uv.y * m_Height)), 0, m_Height-1);
    int y1 = clamp(y0 + 1, 0, m_Height - 1);
    
    float wx = uv.x * m_Width - floor(uv.x * m_Width);
    float wy = uv.y * m_Height - floor(uv.y * m_Height);
    
    vec4 value_y0 = GetColor(x0, y0) * (1.0f - wx) + GetColor(x1, y0) * wx;
    vec4 value_y1 = GetColor(x0, y1) * (1.0f - wx) + GetColor(x1, y1) * wx;
    vec4 value = value_y0 * (1.0f - wy) + value_y1 * wy;
    
    return value;
}

/* IO *****************************************************************/

void CAsset_Image::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
				
	// copy info
	m_External = (pItem->m_External == 1);
	
	// copy image data
	if(m_External)
	{
		char* pFilename = (char*) pFileReader->GetData(pItem->m_FilenameData);
		mem_copy(m_aFilename, pFilename, sizeof(m_aFilename));
		pFileReader->UnloadData(pItem->m_FilenameData);
		
		LoadData(m_aFilename, m_GridWidth, m_GridHeight);
	}
	else
	{
		unsigned char* pData = (unsigned char*) pFileReader->GetData(pItem->m_ImageData);
		SetData(pItem->m_Width, pItem->m_Height, max(1, pItem->m_GridWidth), max(1, pItem->m_GridHeight), pItem->m_Format, pData);
		pFileReader->UnloadData(pItem->m_ImageData);
	}
}

void CAsset_Image::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	Item.m_Width = m_Width; // ignore_convention
	Item.m_Height = m_Height; // ignore_convention
	Item.m_Format = m_Format;
	Item.m_GridWidth = m_GridWidth;
	Item.m_GridHeight = m_GridHeight;
	Item.m_External = (m_External ? 1 : 0);
	
	int PixelSize = (m_Format == CImageInfo::FORMAT_RGB ? 3 : 4);
	int ImageSize = m_Width * m_Height * PixelSize;
	
	if(m_External)
	{
		Item.m_FilenameData = pFileWriter->AddData(str_length(m_aFilename)+1, m_aFilename);
	}
	else
	{
		Item.m_ImageData = pFileWriter->AddData(ImageSize, m_pData);
	}
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/

template<>
int CAsset_Image::GetValue(int ValueType, int PathInt, int DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, WIDTH, GetWidth())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, HEIGHT, GetHeight())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, GRIDWIDTH, GetGridWidth())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, GRIDHEIGHT, GetGridHeight())
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(int)
}
	
template<>
bool CAsset_Image::SetValue(int ValueType, int PathInt, int Value)
{
	switch(ValueType)
	{
		case GRIDWIDTH:
		{
			int NewVal = max(1, Value);
			if(NewVal != m_GridWidth)
			{
				m_GridWidth = NewVal;
				m_NeedUpdate = true;
			}
			return true;
		}
		case GRIDHEIGHT:
			int NewVal = max(1, Value);
			if(NewVal != m_GridHeight)
			{
				m_GridHeight = NewVal;
				m_NeedUpdate = true;
			}
			return true;
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(int)
}

}
