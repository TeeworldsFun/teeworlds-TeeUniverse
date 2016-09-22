#include "guitabsstyle.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_GuiTabsStyle::CAsset_GuiTabsStyle() :
	m_ButtonListFill(true)
{
	
}

/* IO *****************************************************************/

void CAsset_GuiTabsStyle::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
	
	m_LayoutPath = pItem->m_LayoutPath;
	m_ContentPath = pItem->m_ContentPath;
	m_ButtonListPath = pItem->m_ButtonListPath;
	m_InactiveButtonPath = pItem->m_InactiveButtonPath;
	m_ActiveButtonPath = pItem->m_ActiveButtonPath;
	m_ButtonListFill = pItem->m_ButtonListFill;
	m_ButtonListText = pItem->m_ButtonListText;
}

void CAsset_GuiTabsStyle::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_LayoutPath = m_LayoutPath;
	Item.m_ContentPath = m_ContentPath;
	Item.m_ButtonListPath = m_ButtonListPath;
	Item.m_InactiveButtonPath = m_InactiveButtonPath;
	Item.m_ActiveButtonPath = m_ActiveButtonPath;
	Item.m_ButtonListFill = m_ButtonListFill;
	Item.m_ButtonListText = m_ButtonListText;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

}
