#include "guilinestyle.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_GuiLineStyle::CAsset_GuiLineStyle()
{
	
}

/* IO *****************************************************************/

void CAsset_GuiLineStyle::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
	
	m_Flags = pItem->m_Flags;
	m_BorderColor = tu::IntToColor(pItem->m_BorderColor);
	m_ImagePath = pItem->m_ImagePath;
	m_ImageUV_Min.x = pItem->m_ImageUV_X_Min;
	m_ImageUV_Min.y = pItem->m_ImageUV_Y_Min;
	m_ImageUV_Max.x = pItem->m_ImageUV_X_Max;
	m_ImageUV_Max.y = pItem->m_ImageUV_Y_Max;
}

void CAsset_GuiLineStyle::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_Flags = m_Flags;
	Item.m_BorderColor = tu::ColorToInt(m_BorderColor);
	Item.m_ImagePath = m_ImagePath;
	Item.m_ImageUV_X_Min = m_ImageUV_Min.x;
	Item.m_ImageUV_X_Max = m_ImageUV_Max.x;
	Item.m_ImageUV_Y_Min = m_ImageUV_Min.y;
	Item.m_ImageUV_Y_Max = m_ImageUV_Max.y;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

}
