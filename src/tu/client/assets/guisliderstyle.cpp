#include "guisliderstyle.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_GuiSliderStyle::CAsset_GuiSliderStyle()
{
	
}

/* IO *****************************************************************/

void CAsset_GuiSliderStyle::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
	
	m_DefaultRailPath = pItem->m_DefaultRailPath;
	m_CursorPath = pItem->m_CursorPath;
	m_RectPath = pItem->m_RectPath;
	m_Margin = pItem->m_Margin;
	m_Padding = pItem->m_Padding;
}

void CAsset_GuiSliderStyle::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_DefaultRailPath = m_DefaultRailPath;
	Item.m_CursorPath = m_CursorPath;
	Item.m_RectPath = m_RectPath;
	Item.m_Margin = m_Margin;
	Item.m_Padding = m_Padding;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

}
