#include "weapon.h"

#include <engine/shared/datafile.h>
#include <modapi/client/graphics.h>

namespace tu
{

void CAsset_Weapon::InitFromAssetsFile(CAssetManager* pAssetManager, tu::IAssetsFile* pAssetsFile, const CAsset_Weapon::CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
				
	// copy info
	m_CharacterPath = CAssetPath(pItem->m_CharacterPath);
	m_CursorPath = CAssetPath(pItem->m_CursorPath);
	m_SkinPath = CAssetPath(pItem->m_SkinPath);
	m_AttackAnimationPath = CAssetPath(pItem->m_AttackAnimationPath);
}

void CAsset_Weapon::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CAsset_Weapon::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_CharacterPath = m_CharacterPath.ConvertToInteger();
	Item.m_CursorPath = m_CursorPath.ConvertToInteger();
	Item.m_SkinPath = m_SkinPath.ConvertToInteger();
	Item.m_AttackAnimationPath = m_AttackAnimationPath.ConvertToInteger();
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(CAsset_Weapon::TypeId), Position, sizeof(CAsset_Weapon::CStorageType), &Item);
}

void CAsset_Weapon::Unload(class CAssetManager* pAssetManager)
{
	
}

/* VALUE ASSETPATH *******************************************************/

template<>
CAssetPath CAsset_Weapon::GetValue(int ValueType, int Path, CAssetPath DefaultValue)
{
	switch(ValueType)
	{
		case CHARACTERPATH:
			return m_CharacterPath;
		case CURSORPATH:
			return m_CursorPath;
		case SKINPATH:
			return m_SkinPath;
		case ATTACKANIMATIONPATH:
			return m_AttackAnimationPath;
		default:
			return CAsset::GetValue<CAssetPath>(ValueType, Path, DefaultValue);
	}
}
	
template<>
bool CAsset_Weapon::SetValue<CAssetPath>(int ValueType, int Path, CAssetPath Value)
{
	switch(ValueType)
	{
		case CHARACTERPATH:
			m_CharacterPath = Value;
			return true;
		case CURSORPATH:
			m_CursorPath = Value;
			return true;
		case SKINPATH:
			m_SkinPath = Value;
			return true;
		case ATTACKANIMATIONPATH:
			m_AttackAnimationPath = Value;
			return true;
	}
	
	return CAsset::SetValue<CAssetPath>(ValueType, Path, Value);
}

}
