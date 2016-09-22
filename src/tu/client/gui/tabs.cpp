#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "tabs.h"

namespace tu
{
	
namespace gui
{

/* TABS ***************************************************************/

	//Button
CTabs::CTabButton::CTabButton(CContext *pContext, CTabs *pTabs, int Id, const char* pName, CAssetPath IconPath) :
	CButton(pContext, pName, IconPath),
	m_pTabs(pTabs),
	m_Id(Id)
{
	
}

void CTabs::CTabButton::MouseClickAction()
{
	m_pTabs->OpenTab(m_Id);
}

	//Tabs
CTabs::CTabs(CContext *pContext) :
	CWidget(pContext),
	m_SelectedTab(-1),
	m_pButtonList(0),
	m_TabsStylePath(Context()->GetTabsStyle())
{
	m_pButtonList = new CHListLayout(pContext);
}

CTabs::~CTabs()
{
	if(m_pButtonList)
		delete m_pButtonList;
	
	for(int i=0; i<m_Tabs.size(); i++)
		delete m_Tabs[i].m_pWidget;
}

void CTabs::OpenTab(int TabId)
{
	if(TabId >= 0 && TabId < m_Tabs.size())
		m_SelectedTab = TabId;
	else
		m_SelectedTab = -1;
}

void CTabs::AddTab(CWidget* pWidget, const char* pName, CAssetPath IconPath)
{
	bool Fill = true;
	bool ButtonListText = true;
	const CAsset_GuiTabsStyle* pTabsStyle = AssetsManager()->GetAsset<CAsset_GuiTabsStyle>(m_TabsStylePath);
	if(pTabsStyle)
	{
		Fill = pTabsStyle->m_ButtonListFill;
		ButtonListText = pTabsStyle->m_ButtonListText;
	}
	int TabId = m_Tabs.add(CTab());
	CTab* pTab = &m_Tabs[TabId];
	
	pTab->m_IconPath = IconPath;
	pTab->m_Disabled = pWidget->IsDisabled();
	pTab->m_pWidget = pWidget;
	pTab->m_pTabButton = 0;
	str_copy(pTab->m_aName, pName, sizeof(pTab->m_aName));
	
	if(!pTab->m_Disabled)
	{
		pTab->m_pTabButton = new CTabButton(Context(), this, TabId, (ButtonListText ? pTab->m_aName : ""), IconPath);
		m_pButtonList->Add(pTab->m_pTabButton, Fill);
	}
	
	if(m_SelectedTab < 0)
		m_SelectedTab = TabId;
}

void CTabs::Clear()
{
	m_SelectedTab = -1;
	for(int i=0; i<m_Tabs.size(); i++)
	{
		delete m_Tabs[i].m_pWidget;
	}
	m_Tabs.clear();
	m_pButtonList->Clear();
}

void CTabs::UpdateBoundingSize()
{	
	m_BoundingSizeRect.BSNoConstraint();
	
	const CAsset_GuiTabsStyle* pTabsStyle = AssetsManager()->GetAsset<CAsset_GuiTabsStyle>(m_TabsStylePath);
	const CAsset_GuiBoxStyle* pLayoutStyle = 0;
	const CAsset_GuiBoxStyle* pContentStyle = 0;
	if(pTabsStyle)
	{
		pLayoutStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(pTabsStyle->m_LayoutPath);
		pContentStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(pTabsStyle->m_ContentPath);
	}
	
	m_pButtonList->UpdateBoundingSize();
	m_BoundingSizeRect.BSVerticalAdd(m_pButtonList->GetBS());
	
	if(pTabsStyle)
		m_BoundingSizeRect.BSAddSpacing(0, Context()->ApplyGuiScale(pLayoutStyle->GetSpacing()));
	
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->UpdateBoundingSize();
		
		CRect BSSelectedTab = m_Tabs[m_SelectedTab].m_pWidget->GetBS();
		
		if(pLayoutStyle)
		{
			BSSelectedTab.BSAddMargin(Context()->ApplyGuiScale(pContentStyle->GetPadding()));
			BSSelectedTab.BSAddMargin(Context()->ApplyGuiScale(pContentStyle->GetMargin()));
		}
		
		m_BoundingSizeRect.BSVerticalAdd(BSSelectedTab);
	}
	
	if(pTabsStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pLayoutStyle->GetPadding()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pLayoutStyle->GetMargin()));
	}
}

void CTabs::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	const CAsset_GuiTabsStyle* pTabsStyle = AssetsManager()->GetAsset<CAsset_GuiTabsStyle>(m_TabsStylePath);
	const CAsset_GuiBoxStyle* pLayoutStyle = 0;
	const CAsset_GuiBoxStyle* pContentStyle = 0;
	if(pTabsStyle)
	{
		pLayoutStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(pTabsStyle->m_LayoutPath);
		pContentStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(pTabsStyle->m_ContentPath);
	}
	
	int Spacing;
	CRect TabsRect = m_DrawRect;
	if(pLayoutStyle)
	{
		Spacing = Context()->ApplyGuiScale(pLayoutStyle->GetSpacing());
		TabsRect.RemoveMargin(Context()->ApplyGuiScale(pLayoutStyle->GetPadding()));
		TabsRect.RemoveMargin(Context()->ApplyGuiScale(pLayoutStyle->GetMargin()));
	}
	
	CRect ButtonListRect(
		TabsRect.x,
		TabsRect.y,
		TabsRect.w,
		m_pButtonList->GetBS().minh
	);
	m_pButtonList->UpdatePosition(ButtonListRect);
	
	m_ClipRect = CRect(
		TabsRect.x,
		ButtonListRect.y + ButtonListRect.h + Spacing,
		TabsRect.w,
		TabsRect.h - ButtonListRect.h - Spacing
	);
	
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->UpdatePosition(m_ClipRect);
	}
}

void CTabs::Update()
{
	const CAsset_GuiTabsStyle* pTabsStyle = AssetsManager()->GetAsset<CAsset_GuiTabsStyle>(m_TabsStylePath);
	if(pTabsStyle)
		m_pButtonList->SetBoxStyle(pTabsStyle->m_ButtonListPath);
	
	bool NeedUpdate = false;
	
	for(int i=0; i<m_Tabs.size(); i++)
		m_Tabs[i].m_pWidget->Update();
	
	for(int i=0; i<m_Tabs.size(); i++)
	{
		if(m_Tabs[i].m_pWidget->IsDisabled() != m_Tabs[i].m_Disabled)
		{
			m_Tabs[i].m_Disabled = m_Tabs[i].m_pWidget->IsDisabled();
			NeedUpdate = true;
		}
	}
	
	if(NeedUpdate)
		RegenerateButtons();
	
	if(pTabsStyle)
	{
		for(int i=0; i<m_Tabs.size(); i++)
		{
			if(m_Tabs[i].m_pTabButton)
			{
				if(i == m_SelectedTab)
					m_Tabs[i].m_pTabButton->SetBoxStyle(pTabsStyle->m_ActiveButtonPath);
				else
					m_Tabs[i].m_pTabButton->SetBoxStyle(pTabsStyle->m_InactiveButtonPath);
			}	
		}
	}
	
	m_pButtonList->Update();
}

void CTabs::RegenerateButtons()
{
	bool Fill = true;
	bool ButtonListText = true;
	const CAsset_GuiTabsStyle* pTabsStyle = AssetsManager()->GetAsset<CAsset_GuiTabsStyle>(m_TabsStylePath);
	if(pTabsStyle)
	{
		Fill = pTabsStyle->m_ButtonListFill;
		ButtonListText = pTabsStyle->m_ButtonListText;
	}
	
	m_pButtonList->Clear();
	int FirstEnabledChild = -1;
	for(int i=0; i<m_Tabs.size(); i++)
	{
		m_Tabs[i].m_pTabButton = 0;
		
		if(!m_Tabs[i].m_pWidget->IsDisabled())
		{
			m_Tabs[i].m_pTabButton = new CTabButton(Context(), this, i, (ButtonListText ? m_Tabs[i].m_aName : ""), m_Tabs[i].m_IconPath);
			m_pButtonList->Add(m_Tabs[i].m_pTabButton, Fill);
			if(FirstEnabledChild < 0)
				FirstEnabledChild = i;
		}
	}
	
	if(m_Tabs[m_SelectedTab].m_pWidget->IsDisabled())
		m_SelectedTab = max(FirstEnabledChild, 0);
}

void CTabs::Render()
{
	const CAsset_GuiTabsStyle* pTabsStyle = AssetsManager()->GetAsset<CAsset_GuiTabsStyle>(m_TabsStylePath);
	const CAsset_GuiBoxStyle* pLayoutStyle = 0;
	const CAsset_GuiBoxStyle* pContentStyle = 0;
	if(pTabsStyle)
	{
		pLayoutStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(pTabsStyle->m_LayoutPath);
		pContentStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(pTabsStyle->m_ContentPath);
	}
	
	ivec2 MousePos = Context()->GetMousePos();
	
	if(pLayoutStyle)
	{
		CRect Rect = m_DrawRect;
		Rect.RemoveMargin(Context()->ApplyGuiScale(pLayoutStyle->GetMargin()));
	
		if(Rect.IsInside(MousePos.x, MousePos.y))
			AssetsRenderer()->DrawGuiRect(&Rect, pLayoutStyle->GetMouseOverRectPath());
		else
			AssetsRenderer()->DrawGuiRect(&Rect, pLayoutStyle->GetDefaultRectPath());
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pLayoutStyle->GetPadding()));
	}
	
	if(pContentStyle)
	{
		CRect Rect = m_ClipRect;
		Rect.RemoveMargin(Context()->ApplyGuiScale(pContentStyle->GetMargin()));
	
		if(Rect.IsInside(MousePos.x, MousePos.y))
			AssetsRenderer()->DrawGuiRect(&Rect, pContentStyle->GetMouseOverRectPath());
		else
			AssetsRenderer()->DrawGuiRect(&Rect, pContentStyle->GetDefaultRectPath());
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pContentStyle->GetPadding()));
	}
		
	m_pButtonList->Render();
	
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->Render();
	}
}

void CTabs::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	m_pButtonList->OnMouseOver(X, Y, RelX, RelY, KeyState);
		
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CTabs::OnButtonClick(int X, int Y, int Button, int Count)
{
	m_pButtonList->OnButtonClick(X, Y, Button, Count);
		
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->OnButtonClick(X, Y, Button, Count);
	}
}

void CTabs::OnButtonRelease(int Button)
{
	m_pButtonList->OnButtonRelease(Button);
		
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->OnButtonRelease(Button);
	}
}

void CTabs::OnInputEvent()
{
	m_pButtonList->OnInputEvent();
		
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->OnInputEvent();
	}
}

}

}
