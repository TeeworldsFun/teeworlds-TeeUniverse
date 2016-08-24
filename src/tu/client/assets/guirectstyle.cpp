#include "guirectstyle.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_GuiRectStyle::CAsset_GuiRectStyle()
{
	
}

/* IO *****************************************************************/

void CAsset_GuiRectStyle::InitFromAssetsFile(CAssetsManager* pAssetsManager, IAssetsFile* pAssetsFile, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	m_Flags = pItem->m_Flags;
	m_BackgroundColor = tu::IntToColor(pItem->m_BackgroundColor);
	m_BorderColor = tu::IntToColor(pItem->m_BorderColor);
	m_BorderFlags = pItem->m_BorderFlags;
	m_CornerRadius = pItem->m_CornerRadius;
	m_CornerFlags = pItem->m_CornerFlags;
}

void CAsset_GuiRectStyle::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_Flags = m_Flags;
	Item.m_BackgroundColor = tu::ColorToInt(m_BackgroundColor);
	Item.m_BorderColor = tu::ColorToInt(m_BorderColor);
	Item.m_BorderFlags = m_BorderFlags;
	Item.m_CornerRadius = m_CornerRadius;
	Item.m_CornerFlags = m_CornerFlags;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

}
