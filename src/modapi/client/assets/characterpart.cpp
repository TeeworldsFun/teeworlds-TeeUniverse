#include "characterpart.h"

#include <engine/shared/datafile.h>
#include <modapi/client/graphics.h>
#include <modapi/client/assets/character.h>

namespace tu
{

/* IO *****************************************************************/

void CAsset_CharacterPart::InitFromAssetsFile(CAssetManager* pAssetManager, tu::IAssetsFile* pAssetsFile, const CStorageType* pItem)
{
	// load name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	m_CharacterPath = CAssetPath(pItem->m_CharacterPath);
	m_CharacterPart = CAsset_Character::CSubPath(pItem->m_CharacterPart);
	m_SkeletonSkinPath = CAssetPath(pItem->m_SkeletonSkinPath);
}

void CAsset_CharacterPart::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_CharacterPath = m_CharacterPath.ConvertToInteger();
	Item.m_CharacterPart = m_CharacterPart.ConvertToInteger();
	Item.m_SkeletonSkinPath = m_SkeletonSkinPath.ConvertToInteger();
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

void CAsset_CharacterPart::Unload(class CAssetManager* pAssetManager)
{
	
}

/* VALUE INT **********************************************************/

template<>
int CAsset_CharacterPart::GetValue(int ValueType, int Path, int DefaultValue)
{
	switch(ValueType)
	{
		case CHARACTERPART:
			return m_CharacterPart.ConvertToInteger();
		default:
			return CAsset::GetValue<int>(ValueType, Path, DefaultValue);
	}
}
	
template<>
bool CAsset_CharacterPart::SetValue<int>(int ValueType, int Path, int Value)
{
	switch(ValueType)
	{
		case CHARACTERPART:
			m_CharacterPart = Value;
			return true;
	}
	
	return CAsset::SetValue<int>(ValueType, Path, Value);
}

/* VALUE ASSETPATH *******************************************************/

template<>
CAssetPath CAsset_CharacterPart::GetValue(int ValueType, int Path, CAssetPath DefaultValue)
{
	switch(ValueType)
	{
		case CHARACTERPATH:
			return m_CharacterPath;
		case SKELETONSKINPATH:
			return m_SkeletonSkinPath;
		default:
			return CAsset::GetValue<CAssetPath>(ValueType, Path, DefaultValue);
	}
}
	
template<>
bool CAsset_CharacterPart::SetValue<CAssetPath>(int ValueType, int Path, CAssetPath Value)
{
	switch(ValueType)
	{
		case CHARACTERPATH:
			m_CharacterPath = Value;
			return true;
		case SKELETONSKINPATH:
			m_SkeletonSkinPath = Value;
			return true;
	}
	
	return CAsset::SetValue<CAssetPath>(ValueType, Path, Value);
}

}
