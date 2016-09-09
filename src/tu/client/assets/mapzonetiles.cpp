#include "mapzonetiles.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_MapZoneTiles::CAsset_MapZoneTiles() :
	m_pTiles(0),
	m_Width(0),
	m_Height(0)
{
	
}

CAsset_MapZoneTiles::CAsset_MapZoneTiles(const CAsset_MapZoneTiles& Layer) :
	m_pTiles(0),
	m_Width(0),
	m_Height(0)
{
	*this = Layer;
}

CAsset_MapZoneTiles::~CAsset_MapZoneTiles()
{
	if(m_pTiles)
		delete[] m_pTiles;
}

CAsset_MapZoneTiles& CAsset_MapZoneTiles::operator=(const CAsset_MapZoneTiles& Layer)
{
	CAsset::operator=(Layer);
	
	if(Layer.m_pTiles)
	{
		SetSize(Layer.m_Width, Layer.m_Height);
		mem_copy(m_pTiles, Layer.m_pTiles, m_Width*m_Height*sizeof(CTile));
		
		m_ZoneTypePath = Layer.m_ZoneTypePath;
	}
	else
	{
		m_Width = 0;
		m_Height = 0;
		m_pTiles = 0;
		
		m_ZoneTypePath = CAssetPath::Null();
	}
	
	return *this;
}

void CAsset_MapZoneTiles::SetSize(int Width, int Height)
{
	if(m_pTiles)
		delete[] m_pTiles;
		
	m_Width = max(1, Width);
	m_Height = max(1, Height);
	
	m_pTiles = new CTile[m_Width*m_Height];
	
	for(int j=0; j<m_Height; j++)
	{
		for(int i=0; i<m_Width; i++)
		{
			m_pTiles[j*m_Width+i] = CTile();
		}
	}
}

void CAsset_MapZoneTiles::Resize(int Width, int Height)
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

/* IO *****************************************************************/

void CAsset_MapZoneTiles::InitFromAssetsFile(tu::IAssetsFile* pAssetsFile, const CAsset_MapZoneTiles::CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	SetSize(pItem->m_Width, pItem->m_Height);
	
	// load tiles
	int nbTiles = pItem->m_Width * pItem->m_Height;
	const CStorageType::CTile* pTiles = static_cast<CStorageType::CTile*>(pAssetsFile->GetData(pItem->m_TilesData));
	for(int i=0; i<nbTiles; i++)
	{
		m_pTiles[i].m_Index = pTiles[i].m_Index;
	}
}

void CAsset_MapZoneTiles::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CAsset_MapZoneTiles::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	{
		int nbTiles = m_Width * m_Height;
		CStorageType::CTile* pTiles = new CStorageType::CTile[nbTiles];
		for(int i=0; i<nbTiles; i++)
		{
			pTiles[i].m_Index = m_pTiles[i].m_Index;
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
int CAsset_MapZoneTiles::GetValue(int ValueType, int PathInt, int DefaultValue)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_FUNC(int, WIDTH, GetWidth);
		TU_ASSET_GET_FUNC_IMPL_FUNC(int, HEIGHT, GetHeight);
		TU_ASSET_GET_FUNC_IMPL_SUBFUNC(int, TILE_INDEX, GetTileIndex);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(int)
}
	
template<>
bool CAsset_MapZoneTiles::SetValue<int>(int ValueType, int PathInt, int Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_FUNC(int, WIDTH, SetWidth);
		TU_ASSET_SET_FUNC_IMPL_FUNC(int, HEIGHT, SetHeight);
		TU_ASSET_SET_FUNC_IMPL_SUBFUNC(int, TILE_INDEX, SetTileIndex);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(int)
}

/* VALUE ASSETPATH ****************************************************/
	
template<>
CAssetPath CAsset_MapZoneTiles::GetValue<CAssetPath>(int ValueType, int PathInt, CAssetPath DefaultValue)
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, ZONETYPEPATH, GetZoneTypePath);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_MapZoneTiles::SetValue<CAssetPath>(int ValueType, int PathInt, CAssetPath Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, ZONETYPEPATH, SetZoneTypePath);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

}
