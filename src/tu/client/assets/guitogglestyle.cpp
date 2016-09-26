#include "guitogglestyle.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_GuiToggleStyle::CAsset_GuiToggleStyle()
{
	
}

/* IO *****************************************************************/

void CAsset_GuiToggleStyle::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
		
	m_SwitchIcon = pItem->m_SwitchIcon;
	m_IdleTrueStylePath = pItem->m_IdleTrueStylePath;
	m_IdleFalseStylePath = pItem->m_IdleFalseStylePath;
	m_MouseOverTrueStylePath = pItem->m_MouseOverTrueStylePath;
	m_MouseOverFalseStylePath = pItem->m_MouseOverFalseStylePath;
	m_IconTruePath = pItem->m_IconTruePath;
	m_IconFalsePath = pItem->m_IconFalsePath;
}

void CAsset_GuiToggleStyle::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_SwitchIcon = m_SwitchIcon;
	Item.m_IdleTrueStylePath = m_IdleTrueStylePath;
	Item.m_IdleFalseStylePath = m_IdleFalseStylePath;
	Item.m_MouseOverTrueStylePath = m_MouseOverTrueStylePath;
	Item.m_MouseOverFalseStylePath = m_MouseOverFalseStylePath;
	Item.m_IconTruePath = m_IconTruePath;
	Item.m_IconFalsePath = m_IconFalsePath;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE BOOL *********************************************************/

template<>
bool CAsset_GuiToggleStyle::GetValue(int ValueType, int PathInt, bool DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_FUNC(bool, SWITCHICON, GetSwitchIcon)
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(bool)
}
	
template<>
bool CAsset_GuiToggleStyle::SetValue(int ValueType, int PathInt, bool Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_FUNC(bool, SWITCHICON, SetSwitchIcon)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(bool)
}

/* VALUE ASSETPATH **********************************************************/

template<>
CAssetPath CAsset_GuiToggleStyle::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, IDLETRUESTYLEPATH, GetIdleTrueStylePath)
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, IDLEFALSESTYLEPATH, GetIdleFalseStylePath)
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, MOUSEOVERTRUESTYLEPATH, GetMouseOverTrueStylePath)
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, MOUSEOVERFALSESTYLEPATH, GetMouseOverFalseStylePath)
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, ICONTRUEPATH, GetIconTruePath)
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, ICONFALSEPATH, GetIconFalsePath)
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_GuiToggleStyle::SetValue(int ValueType, int PathInt, CAssetPath Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, IDLETRUESTYLEPATH, SetIdleTrueStylePath)
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, IDLEFALSESTYLEPATH, SetIdleFalseStylePath)
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, MOUSEOVERTRUESTYLEPATH, SetMouseOverTrueStylePath)
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, MOUSEOVERFALSESTYLEPATH, SetMouseOverFalseStylePath)
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, ICONTRUEPATH, SetIconTruePath)
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, ICONFALSEPATH, SetIconFalsePath)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

}
