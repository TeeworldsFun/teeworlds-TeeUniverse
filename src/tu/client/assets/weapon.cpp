#include "weapon.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_Weapon::CAsset_Weapon()
{
	
}

/* IO *****************************************************************/

void CAsset_Weapon::InitFromAssetsFile(CDataFileReader* pFileReader, const CAsset_Weapon::CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
				
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
	
	Item.m_CharacterPath = m_CharacterPath;
	Item.m_CursorPath = m_CursorPath;
	Item.m_SkinPath = m_SkinPath;
	Item.m_AttackAnimationPath = m_AttackAnimationPath;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE ASSETPATH *******************************************************/

template<>
CAssetPath CAsset_Weapon::GetValue(int ValueType, int Path, CAssetPath DefaultValue) const
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
	}

	return CAsset::GetValue(ValueType, Path, DefaultValue);
}
	
template<>
bool CAsset_Weapon::SetValue(int ValueType, int Path, CAssetPath Value)
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
	
	return CAsset::SetValue(ValueType, Path, Value);
}

}
