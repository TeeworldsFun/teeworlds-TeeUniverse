#include "character.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_Character::CAsset_Character()
{
	
}

CAsset_Character::CPart& CAsset_Character::AddPart()
{
	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "part%d", m_Parts.size());
	m_Parts.add(CAsset_Character::CPart());
	return m_Parts[m_Parts.size()-1].Name(aBuf);
}

/* IO *****************************************************************/

void CAsset_Character::InitFromAssetsFile(tu::IAssetsFile* pAssetsFile, const CStorageType* pItem)
{
	// load name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	// load parts
	const CStorageType::CPart* pParts = static_cast<CStorageType::CPart*>(pAssetsFile->GetData(pItem->m_PartsData));
	for(int i=0; i<pItem->m_NumParts; i++)
	{
		m_Parts.add(CPart());
		CPart& Part = m_Parts[m_Parts.size()-1];
		str_copy(Part.m_aName, pParts[i].m_aName, sizeof(Part.m_aName));
	}
	
	m_IdlePath = CAssetPath(pItem->m_IdlePath);
	m_WalkPath = CAssetPath(pItem->m_WalkPath);
	m_ControlledJumpPath = CAssetPath(pItem->m_ControlledJumpPath);
	m_UncontrolledJumpPath = CAssetPath(pItem->m_UncontrolledJumpPath);
}

void CAsset_Character::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	{
		CStorageType::CPart* pParts = new CStorageType::CPart[m_Parts.size()];
		for(int i=0; i<m_Parts.size(); i++)
		{
			str_copy(pParts[i].m_aName, m_Parts[i].m_aName, sizeof(pParts[i].m_aName));
		}
		Item.m_NumParts = m_Parts.size();
		Item.m_PartsData = pFileWriter->AddData(Item.m_NumParts * sizeof(CStorageType::CPart), pParts);
		delete[] pParts;
	}
	
	Item.m_IdlePath = m_IdlePath;
	Item.m_WalkPath = m_WalkPath;
	Item.m_ControlledJumpPath = m_ControlledJumpPath;
	Item.m_UncontrolledJumpPath = m_UncontrolledJumpPath;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE STRING *******************************************************/

template<>
char* CAsset_Character::GetValue(int ValueType, int PathInt, char* DefaultValue)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case PART_NAME:
			if(Path.GetType() == CSubPath::TYPE_PART && Path.GetId() >= 0 && Path.GetId() < m_Parts.size())
				return m_Parts[Path.GetId()].m_aName;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue<char*>(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_Character::SetValue<const char*>(int ValueType, int PathInt, const char* pText)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case PART_NAME:
			if(Path.GetType() == CSubPath::TYPE_PART && Path.GetId() >= 0 && Path.GetId() < m_Parts.size())
			{
				str_copy(m_Parts[Path.GetId()].m_aName, pText, sizeof(m_Parts[Path.GetId()].m_aName));
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue<const char*>(ValueType, PathInt, pText);
}

/* VALUE ASSETPATH ****************************************************/

template<>
CAssetPath CAsset_Character::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, IDLEPATH, GetIdlePath())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, WALKPATH, GetWalkPath())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, CONTROLLEDJUMPPATH, GetControlledJumpPath())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, UNCONTROLLEDJUMPPATH, GetUncontrolledJumpPath())
		TU_ASSET_GET_FUNC_IMPL_ARRAY_VARIABLE(CAssetPath, PART_DEFAULTPATH, TYPE_PART, m_Parts, m_DefaultPath)
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_Character::SetValue<CAssetPath>(int ValueType, int PathInt, CAssetPath Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, IDLEPATH, m_IdlePath)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, WALKPATH, m_WalkPath)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, CONTROLLEDJUMPPATH, m_ControlledJumpPath)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, UNCONTROLLEDJUMPPATH, m_UncontrolledJumpPath)
		TU_ASSET_SET_FUNC_IMPL_ARRAY_VARIABLE(CAssetPath, PART_DEFAULTPATH, TYPE_PART, m_Parts, m_DefaultPath)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

}
