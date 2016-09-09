#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "tabs.h"

namespace tu
{
	
namespace gui
{

CTabs::CTabs(CConfig *pConfig) :
	CWidget(pConfig),
	m_SelectedTab(-1)
{	

}

CTabs::~CTabs()
{
	for(int i=0; i<m_Tabs.size(); i++)
	{
		delete m_Tabs[i].m_pWidget;
	}
}

void CTabs::Update()
{
	m_ContentRect = CRect(
			m_Rect.x + m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_LayoutPadding,
			m_Rect.y + m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_LayoutPadding + m_pConfig->m_IconSize + 2*m_pConfig->m_LabelMargin,
			m_Rect.w - 2*m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_LayoutPadding,
			m_Rect.h - m_pConfig->m_IconSize - 2*m_pConfig->m_LabelMargin - 2*m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_LayoutPadding
		);
	
	for(int i=0; i<m_Tabs.size(); i++)
	{
		m_Tabs[i].m_pWidget->SetRect(m_ContentRect);
		m_Tabs[i].m_pWidget->Update();
	}
}
	
void CTabs::Render()
{
	TUGraphics()->DrawGuiRect(&m_Rect, m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_StylePath_Layout);
	
	for(int i=0; i<m_Tabs.size(); i++)
	{
		if(m_SelectedTab == i)
			TUGraphics()->DrawGuiRect(&m_Tabs[i].m_Rect, m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_StylePath_ButtonHighlight);
		else
			TUGraphics()->DrawGuiRect(&m_Tabs[i].m_Rect, m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_StylePath_Button);
		
		TUGraphics()->DrawSprite(
			m_Tabs[i].m_IconPath,
			vec2(m_Tabs[i].m_Rect.x+m_Tabs[i].m_Rect.w/2, m_Tabs[i].m_Rect.y+m_Tabs[i].m_Rect.h/2),
			1.0f, 0.0f, 0x0, 1.0f
		);
	}
	
	TUGraphics()->DrawGuiRect(&m_ContentRect, m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_StylePath_Content);
	
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->Render();
	}
}

void CTabs::AddTab(CWidget* pWidget, CAssetPath IconPath, const char* pHint)
{
	float TabButtonSize = m_pConfig->m_IconSize + m_pConfig->m_LabelMargin*2.0f;
	
	int TabId = m_Tabs.size();
	m_Tabs.add(CTab());
	CTab& Tab = m_Tabs[TabId];
	
	Tab.m_pWidget = pWidget;
	Tab.m_IconPath = IconPath;
	str_copy(Tab.m_aHint, pHint, sizeof(Tab.m_aHint));
	Tab.m_Rect.x = m_Rect.x + TabButtonSize*TabId + m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_LayoutPadding;
	Tab.m_Rect.y = m_Rect.y + m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_LayoutPadding;
	Tab.m_Rect.w = TabButtonSize;
	Tab.m_Rect.h = TabButtonSize;
	
	for(int i=0; i<m_Tabs.size(); i++)
	{
		pWidget->SetRect(CRect(
			m_Rect.x,
			m_Rect.y + m_pConfig->m_IconSize + m_pConfig->m_LabelMargin*2.0f,
			m_Rect.w,
			m_Rect.h - m_pConfig->m_IconSize - m_pConfig->m_LabelMargin*2.0f
		));
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
}

void CTabs::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	for(int i=0; i<m_Tabs.size(); i++)
	{
		if(m_Tabs[i].m_Rect.IsInside(X, Y))
		{
			ShowHint(m_Tabs[i].m_aHint);
			break;
		}
	}
		
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CTabs::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1)
	{
		for(int i=0; i<m_Tabs.size(); i++)
		{
			if(m_Tabs[i].m_Rect.IsInside(X, Y))
			{
				m_SelectedTab = i;
				return;
			}
		}
	}
	
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->OnButtonClick(X, Y, Button, Count);
	}
}

void CTabs::OnButtonRelease(int Button)
{
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->OnButtonRelease(Button);
	}
}

void CTabs::OnInputEvent()
{
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->OnInputEvent();
	}
}

}

}
