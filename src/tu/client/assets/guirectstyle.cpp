#include "guirectstyle.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_GuiRectStyle::CAsset_GuiRectStyle() :
	m_BackgroundPadding(0)
{
	
}

/* IO *****************************************************************/

void CAsset_GuiRectStyle::InitFromAssetsFile(IAssetsFile* pAssetsFile, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	m_Flags = pItem->m_Flags;
	m_BackgroundColor = tu::IntToColor(pItem->m_BackgroundColor);
	m_BackgroundPadding = pItem->m_BackgroundPadding;
	m_BorderColor = tu::IntToColor(pItem->m_BorderColor);
	m_BorderFlags = pItem->m_BorderFlags;
	m_CornerRadius = pItem->m_CornerRadius;
	m_CornerFlags = pItem->m_CornerFlags;
	m_ImagePath = pItem->m_ImagePath;
	m_ImageUV_Min.x = pItem->m_ImageUV_X_Min;
	m_ImageUV_Min.y = pItem->m_ImageUV_Y_Min;
	m_ImageUV_Max.x = pItem->m_ImageUV_X_Max;
	m_ImageUV_Max.y = pItem->m_ImageUV_Y_Max;
}

void CAsset_GuiRectStyle::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_Flags = m_Flags;
	Item.m_BackgroundColor = tu::ColorToInt(m_BackgroundColor);
	Item.m_BackgroundPadding = m_BackgroundPadding;
	Item.m_BorderColor = tu::ColorToInt(m_BorderColor);
	Item.m_BorderFlags = m_BorderFlags;
	Item.m_CornerRadius = m_CornerRadius;
	Item.m_CornerFlags = m_CornerFlags;
	Item.m_ImagePath = m_ImagePath;
	Item.m_ImageUV_X_Min = m_ImageUV_Min.x;
	Item.m_ImageUV_X_Max = m_ImageUV_Max.x;
	Item.m_ImageUV_Y_Min = m_ImageUV_Min.y;
	Item.m_ImageUV_Y_Max = m_ImageUV_Max.y;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

}
