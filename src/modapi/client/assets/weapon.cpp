#include "weapon.h"

#include <engine/shared/datafile.h>
#include <modapi/client/graphics.h>

void CModAPI_Asset_Weapon::InitFromAssetsFile(CModAPI_AssetManager* pAssetManager, IModAPI_AssetsFile* pAssetsFile, const CModAPI_Asset_Weapon::CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
				
	// copy info
	m_CharacterPath = CModAPI_AssetPath(pItem->m_CharacterPath);
	m_CursorPath = CModAPI_AssetPath(pItem->m_CursorPath);
	m_SkinPath = CModAPI_AssetPath(pItem->m_SkinPath);
	m_AttackAnimationPath = CModAPI_AssetPath(pItem->m_AttackAnimationPath);
}

void CModAPI_Asset_Weapon::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CModAPI_Asset_Weapon::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_CharacterPath = m_CharacterPath.ConvertToInteger();
	Item.m_CursorPath = m_CursorPath.ConvertToInteger();
	Item.m_SkinPath = m_SkinPath.ConvertToInteger();
	Item.m_AttackAnimationPath = m_AttackAnimationPath.ConvertToInteger();
	
	pFileWriter->AddItem(CModAPI_AssetPath::TypeToStoredType(CModAPI_Asset_Weapon::TypeId), Position, sizeof(CModAPI_Asset_Weapon::CStorageType), &Item);
}

void CModAPI_Asset_Weapon::Unload(class CModAPI_AssetManager* pAssetManager)
{
	
}

/* VALUE ASSETPATH *******************************************************/

template<>
CModAPI_AssetPath CModAPI_Asset_Weapon::GetValue(int ValueType, int Path, CModAPI_AssetPath DefaultValue)
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
			return CModAPI_Asset::GetValue<CModAPI_AssetPath>(ValueType, Path, DefaultValue);
	}
}
	
template<>
bool CModAPI_Asset_Weapon::SetValue<CModAPI_AssetPath>(int ValueType, int Path, CModAPI_AssetPath Value)
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
	
	return CModAPI_Asset::SetValue<CModAPI_AssetPath>(ValueType, Path, Value);
}
