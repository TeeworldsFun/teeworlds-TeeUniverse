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

void CAsset_MapGroup::InitFromAssetsFile(CAssetsManager* pAssetsManager, tu::IAssetsFile* pAssetsFile, const CAsset_MapGroup::CStorageType* pItem)
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
			pLayers[i] = m_Layers[i].ConvertToInteger();
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
		case POSITION_X:
			return m_Position.x;
		case POSITION_Y:
			return m_Position.y;
		case HARDPARALLAX_X:
			return m_HardParallax.x;
		case HARDPARALLAX_Y:
			return m_HardParallax.x;
		default:
			return CAsset::GetValue<float>(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_MapGroup::SetValue<float>(int ValueType, int PathInt, float Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case POSITION_X:
			m_Position.x = Value;
			return true;
		case POSITION_Y:
			m_Position.y = Value;
			return true;
		case HARDPARALLAX_X:
			m_HardParallax.x = Value;
			return true;
		case HARDPARALLAX_Y:
			m_HardParallax.y = Value;
			return true;
	}
	
	return CAsset::SetValue<float>(ValueType, PathInt, Value);
}

}
