#include "guibuttonstyle.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_GuiButtonStyle::CAsset_GuiButtonStyle()
{
	
}

/* IO *****************************************************************/

void CAsset_GuiButtonStyle::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
		
	m_IdleStylePath = pItem->m_IdleStylePath;
	m_MouseOverStylePath = pItem->m_MouseOverStylePath;
}

void CAsset_GuiButtonStyle::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_IdleStylePath = m_IdleStylePath;
	Item.m_MouseOverStylePath = m_MouseOverStylePath;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE ASSETPATH **********************************************************/

template<>
CAssetPath CAsset_GuiButtonStyle::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, IDLESTYLEPATH, GetIdleStylePath)
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, MOUSEOVERSTYLEPATH, GetMouseOverStylePath)
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_GuiButtonStyle::SetValue(int ValueType, int PathInt, CAssetPath Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, IDLESTYLEPATH, SetIdleStylePath)
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, MOUSEOVERSTYLEPATH, SetMouseOverStylePath)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

}
