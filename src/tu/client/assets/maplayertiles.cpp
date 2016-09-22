#include "maplayertiles.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_MapLayerTiles::CAsset_MapLayerTiles() :
	m_Width(0),
	m_Height(0),
	m_pTiles(0),
	m_Color(vec4(1.0f, 1.0f, 1.0f, 1.0f))
{
	
}

CAsset_MapLayerTiles::CAsset_MapLayerTiles(const CAsset_MapLayerTiles& Layer) :
	m_Width(0),
	m_Height(0),
	m_pTiles(0),
	m_Color(vec4(1.0f, 1.0f, 1.0f, 1.0f))
{
	*this = Layer;
}

CAsset_MapLayerTiles::~CAsset_MapLayerTiles()
{
	if(m_pTiles)
		delete[] m_pTiles;
}

CAsset_MapLayerTiles& CAsset_MapLayerTiles::operator=(const CAsset_MapLayerTiles& Layer)
{
	CAsset::operator=(Layer);
	
	if(Layer.m_pTiles)
	{
		SetSize(Layer.m_Width, Layer.m_Height);
		mem_copy(m_pTiles, Layer.m_pTiles, m_Width*m_Height*sizeof(CTile));
		
		m_ImagePath = Layer.m_ImagePath;
		m_Color = Layer.m_Color;
	}
	else
	{
		m_Width = 0;
		m_Height = 0;
		m_pTiles = 0;
		
		m_ImagePath = CAssetPath::Null();
		m_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	return *this;
}

void CAsset_MapLayerTiles::SetSize(int Width, int Height)
{
	m_Width = max(1, Width);
	m_Height = max(1, Height);
	
	if(m_pTiles)
		delete[] m_pTiles;
	
	m_pTiles = new CTile[m_Width*m_Height];
	
	for(int j=0; j<m_Height; j++)
	{
		for(int i=0; i<m_Width; i++)
		{
			m_pTiles[j*m_Width+i] = CTile();
		}
	}
}

void CAsset_MapLayerTiles::Resize(int Width, int Height)
{
	int NewWidth = max(1, Width);
	int NewHeight = max(1, Height);
	CTile* pNewTiles = new CTile[NewWidth*NewHeight];
	
	if(m_pTiles)
	{
		for(int j=0; j<NewHeight; j++)
		{
			for(int i=0; i<NewWidth; i++)
			{
				if(j<m_Height && i<m_Width)
					pNewTiles[j*NewWidth+i] = m_pTiles[j*m_Width+i];
				else
					pNewTiles[j*NewWidth+i] = CTile();
			}
		}
		
		delete[] m_pTiles;
	}
	
	m_Width = NewWidth;
	m_Height = NewHeight;
	m_pTiles = pNewTiles;
}

const CAsset_MapLayerTiles::CTile* CAsset_MapLayerTiles::GetTilePointer(int x, int y) const
{
	if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
		return &m_pTiles[y*m_Width+x];
	else
		return 0;
}

/* IO *****************************************************************/

void CAsset_MapLayerTiles::InitFromAssetsFile(CDataFileReader* pFileReader, const CAsset_MapLayerTiles::CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
	
	SetSize(pItem->m_Width, pItem->m_Height);
	
	m_ImagePath = pItem->m_ImagePath;
	m_Color = tu::IntToColor(pItem->m_Color);
	
	// load tiles
	int nbTiles = pItem->m_Width * pItem->m_Height;
	const CStorageType::CTile* pTiles = static_cast<CStorageType::CTile*>(pFileReader->GetData(pItem->m_TilesData));
	for(int i=0; i<nbTiles; i++)
	{
		m_pTiles[i].m_Index = pTiles[i].m_Index;
		m_pTiles[i].m_Flags = pTiles[i].m_Flags;
	}
}

void CAsset_MapLayerTiles::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CAsset_MapLayerTiles::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_ImagePath = m_ImagePath;
	Item.m_Color = tu::ColorToInt(m_Color);
	
	{
		int nbTiles = m_Width * m_Height;
		CStorageType::CTile* pTiles = new CStorageType::CTile[nbTiles];
		for(int i=0; i<nbTiles; i++)
		{
			pTiles[i].m_Index = m_pTiles[i].m_Index;
			pTiles[i].m_Flags = m_pTiles[i].m_Flags;
		}
		Item.m_Width = m_Width;
		Item.m_Height = m_Height;
		Item.m_TilesData = pFileWriter->AddData(nbTiles * sizeof(CStorageType::CTile), pTiles);
		delete[] pTiles;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/

template<>
int CAsset_MapLayerTiles::GetValue(int ValueType, int PathInt, int DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_FUNC(int, WIDTH, GetWidth);
		TU_ASSET_GET_FUNC_IMPL_FUNC(int, HEIGHT, GetHeight);
		TU_ASSET_GET_FUNC_IMPL_SUBFUNC(int, TILE_INDEX, GetTileIndex);
		TU_ASSET_GET_FUNC_IMPL_SUBFUNC(int, TILE_FLAGS, GetTileFlags);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(int)
}
	
template<>
bool CAsset_MapLayerTiles::SetValue(int ValueType, int PathInt, int Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_FUNC(int, WIDTH, SetWidth);
		TU_ASSET_SET_FUNC_IMPL_FUNC(int, HEIGHT, SetHeight);
		TU_ASSET_SET_FUNC_IMPL_SUBFUNC(int, TILE_INDEX, SetTileIndex);
		TU_ASSET_SET_FUNC_IMPL_SUBFUNC(int, TILE_FLAGS, SetTileFlags);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(int)
}

/* VALUE ASSETPATH ****************************************************/
	
template<>
CAssetPath CAsset_MapLayerTiles::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, IMAGEPATH, GetImagePath());
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_MapLayerTiles::SetValue(int ValueType, int PathInt, CAssetPath Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, IMAGEPATH, m_ImagePath);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

/* VALUE VEC4 *********************************************************/
	
template<>
vec4 CAsset_MapLayerTiles::GetValue<vec4>(int ValueType, int PathInt, vec4 DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, COLOR, GetColor());
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(vec4)
}
	
template<>
bool CAsset_MapLayerTiles::SetValue<vec4>(int ValueType, int PathInt, vec4 Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(vec4, COLOR, m_Color);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(vec4)
}

}
