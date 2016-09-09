#include "characterpart.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>
#include <tu/client/assets/character.h>

namespace tu
{
	
CAsset_CharacterPart::CAsset_CharacterPart()
{
	
}

/* IO *****************************************************************/

void CAsset_CharacterPart::InitFromAssetsFile(tu::IAssetsFile* pAssetsFile, const CStorageType* pItem)
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
	
	Item.m_CharacterPath = m_CharacterPath;
	Item.m_CharacterPart = m_CharacterPart;
	Item.m_SkeletonSkinPath = m_SkeletonSkinPath;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/

template<>
int CAsset_CharacterPart::GetValue(int ValueType, int PathInt, int DefaultValue)
{	
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, CHARACTERPART, GetCharacterPart())
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(int)
}
	
template<>
bool CAsset_CharacterPart::SetValue<int>(int ValueType, int PathInt, int Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(int, CHARACTERPART, m_CharacterPart)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(int)
}

/* VALUE ASSETPATH *******************************************************/

template<>
CAssetPath CAsset_CharacterPart::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue)
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, CHARACTERPATH, GetCharacterPath())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, SKELETONSKINPATH, GetSkeletonSkinPath())
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_CharacterPart::SetValue<CAssetPath>(int ValueType, int PathInt, CAssetPath Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, CHARACTERPATH, m_CharacterPath)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, SKELETONSKINPATH, m_SkeletonSkinPath)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

}
