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
			m_Rect.x,
			m_Rect.y + m_pConfig->m_IconSize + m_pConfig->m_LabelMargin*2.0f,
			m_Rect.w,
			m_Rect.h - m_pConfig->m_IconSize - m_pConfig->m_LabelMargin*2.0f
		);
	
	for(int i=0; i<m_Tabs.size(); i++)
	{
		m_Tabs[i].m_pWidget->SetRect(m_ContentRect);
		m_Tabs[i].m_pWidget->Update();
	}
}
	
void CTabs::Render()
{	
	for(int i=0; i<m_Tabs.size(); i++)
	{
		CUIRect rect;
		rect.x = m_Tabs[i].m_Rect.x;
		rect.y = m_Tabs[i].m_Rect.y;
		rect.w = m_Tabs[i].m_Rect.w;
		rect.h = m_Tabs[i].m_Rect.h;
	
		if(m_SelectedTab == i)
			TUGraphics()->Draw_GuiRect(&m_Tabs[i].m_Rect, m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_StylePath_ButtonHighlight);
		else
			TUGraphics()->Draw_GuiRect(&m_Tabs[i].m_Rect, m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_StylePath_Button);
			
		int SubX = m_Tabs[i].m_IconId%16;
		int SubY = m_Tabs[i].m_IconId/16;
		
		Graphics()->TextureSet(m_pConfig->m_Texture);
		Graphics()->QuadsBegin();
		Graphics()->QuadsSetSubset(SubX/16.0f, SubY/16.0f, (SubX+1)/16.0f, (SubY+1)/16.0f);
		IGraphics::CQuadItem QuadItem(rect.x+m_pConfig->m_LabelMargin, rect.y+m_pConfig->m_LabelMargin, m_pConfig->m_IconSize, m_pConfig->m_IconSize);
		Graphics()->QuadsDrawTL(&QuadItem, 1);
		Graphics()->QuadsEnd();
	}
	
	TUGraphics()->Draw_GuiRect(&m_ContentRect, m_pConfig->m_TabStyles[CConfig::TABSTYLE_DEFAULT].m_StylePath_Content);
	
	if(m_SelectedTab >= 0)
	{
		m_Tabs[m_SelectedTab].m_pWidget->Render();
	}
}

void CTabs::AddTab(CWidget* pWidget, int IconId, const char* pHint)
{
	float TabButtonSize = m_pConfig->m_IconSize + m_pConfig->m_LabelMargin*2.0f;
	
	int TabId = m_Tabs.size();
	m_Tabs.add(CTab());
	CTab& Tab = m_Tabs[TabId];
	
	Tab.m_pWidget = pWidget;
	Tab.m_IconId = IconId;
	str_copy(Tab.m_aHint, pHint, sizeof(Tab.m_aHint));
	Tab.m_Rect.x = m_Rect.x + s_LayoutCornerRadius + TabButtonSize*TabId + TabId;
	Tab.m_Rect.y = m_Rect.y;
	Tab.m_Rect.w = m_pConfig->m_IconSize + m_pConfig->m_LabelMargin*2.0f;
	Tab.m_Rect.h = Tab.m_Rect.w;
	
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
