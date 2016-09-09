#include "mapgroup.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_MapGroup::CAsset_MapGroup() :
	m_Position(0.0f, 0.0f),
	m_HardParallax(1.0f, 1.0f)
{
	
}
	
/* IO *****************************************************************/

void CAsset_MapGroup::InitFromAssetsFile(tu::IAssetsFile* pAssetsFile, const CAsset_MapGroup::CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	m_Position.x = pItem->m_PositionX;
	m_Position.y = pItem->m_PositionY;
	m_HardParallax.x = pItem->m_HardParallaxX;
	m_HardParallax.y = pItem->m_HardParallaxY;

	// load layers
	const int* pLayers = static_cast<int*>(pAssetsFile->GetData(pItem->m_LayersData));
	for(int i=0; i<pItem->m_NumLayers; i++)
	{
		m_Layers.add(pLayers[i]);
	}
}

void CAsset_MapGroup::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CAsset_MapGroup::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_PositionX = m_Position.x;
	Item.m_PositionY = m_Position.y;
	Item.m_HardParallaxX = m_HardParallax.x;
	Item.m_HardParallaxY = m_HardParallax.y;
	
	{
		int* pLayers = new int[m_Layers.size()];
		for(int i=0; i<m_Layers.size(); i++)
		{
			pLayers[i] = m_Layers[i];
		}
		Item.m_NumLayers = m_Layers.size();
		Item.m_LayersData = pFileWriter->AddData(Item.m_NumLayers * sizeof(int), pLayers);
		delete[] pLayers;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE FLOAT ********************************************************/

template<>
float CAsset_MapGroup::GetValue<float>(int ValueType, int PathInt, float DefaultValue)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(float, POSITION_X, GetPosition().x)
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(float, POSITION_Y, GetPosition().y)
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(float, HARDPARALLAX_X, GetHardParallax().x)
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(float, HARDPARALLAX_Y, GetHardParallax().y)
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(float)
}
	
template<>
bool CAsset_MapGroup::SetValue<float>(int ValueType, int PathInt, float Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(float, POSITION_X, m_Position.x)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(float, POSITION_Y, m_Position.y)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(float, HARDPARALLAX_X, m_HardParallax.x)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(float, HARDPARALLAX_Y, m_HardParallax.y)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(float)
}

}
