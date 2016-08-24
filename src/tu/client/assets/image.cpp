#include "image.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsmanager.h>

namespace tu
{

CAsset_Image::CAsset_Image() :
	m_pAssetsManager(0),
	m_pData(0),
	m_NeedUpdate(false),
	m_LastUpdate(-1),
	m_GridWidth(1),
	m_GridHeight(1)
{

}

CAsset_Image::CAsset_Image(class CAssetsManager* pAssetsManager) :
	m_pAssetsManager(pAssetsManager),
	m_pData(0),
	m_NeedUpdate(false),
	m_LastUpdate(0),
	m_GridWidth(1),
	m_GridHeight(1)
{
	
}

void CAsset_Image::Update()
{
	if(!m_NeedUpdate)
		return;
	
	if(m_LastUpdate != -1 && (time_get()-m_LastUpdate < time_freq()/60))
		return;
	
	if(m_Texture.IsValid())
	{
		m_pAssetsManager->Graphics()->UnloadTexture(&m_Texture);
	}
	
	int Flags = 0;
	if(m_GridWidth > 1 || m_GridHeight > 1)
		Flags = IGraphics::TEXLOAD_MULTI_DIMENSION;
	
	m_Texture = m_pAssetsManager->Graphics()->LoadTextureRaw(m_Width, m_Height, m_GridWidth, m_GridHeight, m_Format, m_pData, CImageInfo::FORMAT_AUTO, Flags);

	m_NeedUpdate = false;
	m_LastUpdate = time_get();
}

void CAsset_Image::Unload()
{
	if(m_Texture.IsValid())
	{
		m_pAssetsManager->Graphics()->UnloadTexture(&m_Texture);
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

/* IO *****************************************************************/

void CAsset_Image::InitFromAssetsFile(CAssetsManager* pAssetsManager, tu::IAssetsFile* pAssetsFile, const CStorageType* pItem)
{
	m_pAssetsManager = pAssetsManager;
	
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
				
	// copy info
	m_Width = pItem->m_Width;
	m_Height = pItem->m_Height;
	m_Format = pItem->m_Format;
	m_GridWidth = max(1, pItem->m_GridWidth);
	m_GridHeight = max(1, pItem->m_GridHeight);
	int PixelSize = (pItem->m_Format == CImageInfo::FORMAT_RGB ? 3 : 4);
	int ImageSize = m_Width * m_Height * PixelSize;
	
	// copy image data
	void *pData = pAssetsFile->GetData(pItem->m_ImageData);
	m_pData = (unsigned char*) mem_alloc(ImageSize, 1);
	mem_copy(m_pData, pData, ImageSize);
	m_NeedUpdate = true;
	
	// unload image
	pAssetsFile->UnloadData(pItem->m_ImageData);
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
	
	int PixelSize = (m_Format == CImageInfo::FORMAT_RGB ? 3 : 4);
	int ImageSize = m_Width * m_Height * PixelSize;
	
	Item.m_ImageData = pFileWriter->AddData(ImageSize, m_pData);
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/
	
template<>
bool CAsset_Image::SetValue<int>(int ValueType, int Path, int Value)
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
	
	return CAsset::SetValue<int>(ValueType, Path, Value);
}

template<>
int CAsset_Image::GetValue(int ValueType, int Path, int DefaultValue)
{
	switch(ValueType)
	{
		case GRIDWIDTH:
			return max(1, m_GridWidth);
		case GRIDHEIGHT:
			return max(1, m_GridHeight);
		case WIDTH:
			return m_Width;
		case HEIGHT:
			return m_Height;
		default:
			return CAsset::GetValue<int>(ValueType, Path, DefaultValue);
	}
}

}
