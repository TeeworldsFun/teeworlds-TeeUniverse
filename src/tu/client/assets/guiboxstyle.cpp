#include "guiboxstyle.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_GuiBoxStyle::CAsset_GuiBoxStyle() :
	m_TextAlignment(TEXTALIGNMENT_LEFT),
	m_Margin(0),
	m_Padding(0),
	m_Spacing(0),
	m_FontSize(12)
{
	
}

/* IO *****************************************************************/

void CAsset_GuiBoxStyle::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
		
	m_DefaultRectPath = pItem->m_DefaultRectPath;
	m_MouseOverRectPath = pItem->m_MouseOverRectPath;
	m_Margin = pItem->m_Margin;
	m_Padding = pItem->m_Padding;
	m_Spacing = pItem->m_Spacing;
	m_FontSize = pItem->m_FontSize;
	m_TextColor = tu::IntToColor(pItem->m_TextColor);
	m_TextAlignment = pItem->m_TextAlignment;
}

void CAsset_GuiBoxStyle::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_DefaultRectPath = m_DefaultRectPath;
	Item.m_MouseOverRectPath = m_MouseOverRectPath;
	Item.m_Margin = m_Margin;
	Item.m_Padding = m_Padding;
	Item.m_Spacing = m_Spacing;
	Item.m_FontSize = m_FontSize;
	Item.m_TextColor = tu::ColorToInt(m_TextColor);
	Item.m_TextAlignment = m_TextAlignment;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/

template<>
int CAsset_GuiBoxStyle::GetValue(int ValueType, int PathInt, int DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_FUNC(int, MARGIN, GetMargin)
		TU_ASSET_GET_FUNC_IMPL_FUNC(int, PADDING, GetPadding)
		TU_ASSET_GET_FUNC_IMPL_FUNC(int, SPACING, GetSpacing)
		TU_ASSET_GET_FUNC_IMPL_FUNC(int, FONTSIZE, GetFontSize)
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(int)
}
	
template<>
bool CAsset_GuiBoxStyle::SetValue(int ValueType, int PathInt, int Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_FUNC(int, MARGIN, SetMargin)
		TU_ASSET_SET_FUNC_IMPL_FUNC(int, PADDING, SetPadding)
		TU_ASSET_SET_FUNC_IMPL_FUNC(int, SPACING, SetSpacing)
		TU_ASSET_SET_FUNC_IMPL_FUNC(int, FONTSIZE, SetFontSize)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(int)
}

/* VALUE ASSETPATH **********************************************************/

template<>
CAssetPath CAsset_GuiBoxStyle::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, DEFAULTRECTPATH, GetDefaultRectPath)
		TU_ASSET_GET_FUNC_IMPL_FUNC(CAssetPath, MOUSEOVERRECTPATH, GetMouseOverRectPath)
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_GuiBoxStyle::SetValue(int ValueType, int PathInt, CAssetPath Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, DEFAULTRECTPATH, SetDefaultRectPath)
		TU_ASSET_SET_FUNC_IMPL_FUNC(CAssetPath, MOUSEOVERRECTPATH, SetMouseOverRectPath)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

}
