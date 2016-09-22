#include "map.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_Map::CAsset_Map()
{
	
}
	
/* IO *****************************************************************/

void CAsset_Map::InitFromAssetsFile(CDataFileReader* pFileReader, const CAsset_Map::CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
	
	// load bg groups
	const int* pBgGroups = static_cast<int*>(pFileReader->GetData(pItem->m_BgGroupsData));
	for(int i=0; i<pItem->m_NumBgGroups; i++)
	{
		m_BgGroups.add(pBgGroups[i]);
	}
	
	// load fg groups
	const int* pFgGroups = static_cast<int*>(pFileReader->GetData(pItem->m_FgGroupsData));
	for(int i=0; i<pItem->m_NumFgGroups; i++)
	{
		m_FgGroups.add(pFgGroups[i]);
	}
	
	// load zone layers
	const int* pZoneLayers = static_cast<int*>(pFileReader->GetData(pItem->m_ZoneLayersData));
	for(int i=0; i<pItem->m_NumZoneLayers; i++)
	{
		m_ZoneLayers.add(pZoneLayers[i]);
	}
}

void CAsset_Map::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CAsset_Map::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	{
		int* pBgGroups = new int[m_BgGroups.size()];
		for(int i=0; i<m_BgGroups.size(); i++)
		{
			pBgGroups[i] = m_BgGroups[i];
		}
		Item.m_NumBgGroups = m_BgGroups.size();
		Item.m_BgGroupsData = pFileWriter->AddData(Item.m_NumBgGroups * sizeof(int), pBgGroups);
		delete[] pBgGroups;
	}
	{
		int* pFgGroups = new int[m_FgGroups.size()];
		for(int i=0; i<m_FgGroups.size(); i++)
		{
			pFgGroups[i] = m_FgGroups[i];
		}
		Item.m_NumFgGroups = m_FgGroups.size();
		Item.m_FgGroupsData = pFileWriter->AddData(Item.m_NumFgGroups * sizeof(int), pFgGroups);
		delete[] pFgGroups;
	}
	{
		int* pZoneLayers = new int[m_ZoneLayers.size()];
		for(int i=0; i<m_ZoneLayers.size(); i++)
		{
			pZoneLayers[i] = m_ZoneLayers[i];
		}
		Item.m_NumZoneLayers = m_ZoneLayers.size();
		Item.m_ZoneLayersData = pFileWriter->AddData(Item.m_NumZoneLayers * sizeof(int), pZoneLayers);
		delete[] pZoneLayers;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE ASSETPATH ****************************************************/
	
template<>
CAssetPath CAsset_Map::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_ARRAY(CAssetPath, BGGROUP, TYPE_BGGROUP, m_BgGroups)
		TU_ASSET_GET_FUNC_IMPL_ARRAY(CAssetPath, FGGROUP, TYPE_FGGROUP, m_FgGroups)
		TU_ASSET_GET_FUNC_IMPL_ARRAY(CAssetPath, ZONELAYER, TYPE_ZONELAYER, m_ZoneLayers)
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_Map::SetValue(int ValueType, int PathInt, CAssetPath Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_ARRAY(CAssetPath, BGGROUP, TYPE_BGGROUP, m_BgGroups)
		TU_ASSET_SET_FUNC_IMPL_ARRAY(CAssetPath, FGGROUP, TYPE_FGGROUP, m_FgGroups)
		TU_ASSET_SET_FUNC_IMPL_ARRAY(CAssetPath, ZONELAYER, TYPE_ZONELAYER, m_ZoneLayers)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

}
