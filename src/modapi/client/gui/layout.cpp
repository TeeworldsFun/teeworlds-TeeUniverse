#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "layout.h"

/* H LAYOUT *************************************************************/

namespace tu
{
	
namespace gui
{
		
CHListLayout::CSeparator::CSeparator(CConfig* pConfig) :
	CWidget(pConfig)
{
	SetWidth(10);
}

void CHListLayout::CSeparator::Render()
{
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(1.0f, 1.0f, 0.5f, 1.0f);
	
	float x = m_Rect.x + m_Rect.w/2;
	IGraphics::CLineItem Line(x, m_Rect.y, x, m_Rect.y + m_Rect.h);
	Graphics()->LinesDraw(&Line, 1);
	
	Graphics()->LinesEnd();
}

CHListLayout::CHListLayout(CConfig *pConfig, int Style, int Model) :
	CWidget(pConfig),
	m_Style(Style),
	m_Model(Model)
{
	
}

CHListLayout::~CHListLayout()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i];
	}
}

void CHListLayout::Clear()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i];
	}
	m_Childs.clear();
	
	Update();
}

void CHListLayout::Update_FillingFirst()
{
	if(m_Childs.size() == 0)
		return;
	
	int Margin = (m_Style == LAYOUTSTYLE_FRAME ? m_pConfig->m_LayoutMargin : 0);
	int AvailableSpace = m_Rect.w - 2*Margin;
		
	if(m_Childs.size() > 1)
		AvailableSpace -= m_pConfig->m_LayoutSpacing * (m_Childs.size() - 1);
		
	for(int i=1; i<m_Childs.size(); i++)
	{
		AvailableSpace -= m_Childs[i]->m_Rect.w;
	}
	
	m_Childs[0]->SetRect(CRect(
		m_Rect.x + Margin,
		m_Rect.y + Margin,
		AvailableSpace,
		m_Rect.h - Margin*2
	));
	
	int PosX = m_Childs[0]->m_Rect.x + m_Childs[0]->m_Rect.w;
	for(int i=1; i<m_Childs.size(); i++)
	{
		m_Childs[i]->SetRect(CRect(
			PosX + m_pConfig->m_LayoutSpacing,
			m_Rect.y + Margin,
			m_Childs[i]->m_Rect.w,
			m_Rect.h - Margin*2
		));
		
		PosX += m_pConfig->m_LayoutSpacing + m_Childs[i]->m_Rect.w;
	}
}

void CHListLayout::Update_FillingLast()
{
	if(m_Childs.size() == 0)
		return;
	
	int Margin = (m_Style == LAYOUTSTYLE_FRAME ? m_pConfig->m_LayoutMargin : 0);
	int AvailableSpace = m_Rect.w - 2*Margin;
		
	if(m_Childs.size() > 1)
		AvailableSpace -= m_pConfig->m_LayoutSpacing * (m_Childs.size() - 1);
		
	for(int i=0; i<m_Childs.size()-1; i++)
	{
		AvailableSpace -= m_Childs[i]->m_Rect.w;
	}
	
	int PosX = m_Rect.x + Margin;
	for(int i=0; i<m_Childs.size()-1; i++)
	{
		m_Childs[i]->SetRect(CRect(
			PosX,
			m_Rect.y + Margin,
			m_Childs[i]->m_Rect.w,
			m_Rect.h - Margin*2
		));
		
		PosX += m_Childs[i]->m_Rect.w + m_pConfig->m_LayoutSpacing;
	}
	
	m_Childs[m_Childs.size()-1]->SetRect(CRect(
		PosX,
		m_Rect.y + Margin,
		AvailableSpace,
		m_Rect.h - Margin*2
	));
}

void CHListLayout::Update_FillingAll()
{
	if(m_Childs.size() == 0)
		return;
	
	int Margin = (m_Style == LAYOUTSTYLE_FRAME ? m_pConfig->m_LayoutMargin : 0);
	int AvailableSpace = m_Rect.w - 2*Margin;

	if(m_Childs.size() > 1)
		AvailableSpace -= m_pConfig->m_LayoutSpacing * (m_Childs.size() - 1);
		
	int Width = AvailableSpace / m_Childs.size();
	
	int PosX = m_Rect.x + Margin;
	for(int i=0; i<m_Childs.size()-1; i++)
	{
		m_Childs[i]->SetRect(CRect(
			PosX,
			m_Rect.y + Margin,
			Width,
			m_Rect.h - Margin*2
		));
		
		PosX += Width + m_pConfig->m_LayoutSpacing;
	}
	
	m_Childs[m_Childs.size()-1]->SetRect(CRect(
		PosX,
		m_Rect.y + Margin,
		m_Rect.x + m_Rect.w - Margin - PosX,
		m_Rect.h - Margin*2
	));
}

void CHListLayout::Update_FillingNone()
{
	int Margin = (m_Style == LAYOUTSTYLE_FRAME ? m_pConfig->m_LayoutMargin : 0);
	
	int PosX = m_Rect.x + Margin;
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->SetRect(CRect(
			PosX,
			m_Rect.y + Margin,
			m_Childs[i]->m_Rect.w,
			m_Rect.h - Margin*2
		));
		
		PosX += m_Childs[i]->m_Rect.w + m_pConfig->m_LayoutSpacing;
	}
}

void CHListLayout::Update()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Update();
	}
	
	switch(m_Model)
	{
		case LAYOUTFILLING_NONE:
			Update_FillingNone();
			break;
		case LAYOUTFILLING_FIRST:
			Update_FillingFirst();
			break;
		case LAYOUTFILLING_LAST:
			Update_FillingLast();
			break;
		case LAYOUTFILLING_ALL:
			Update_FillingAll();
			break;
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Update();
	}
}
	
void CHListLayout::Render()
{
	//Background
	if(m_Style == LAYOUTSTYLE_FRAME)
	{
		CUIRect rect;
		rect.x = m_Rect.x;
		rect.y = m_Rect.y;
		rect.w = m_Rect.w;
		rect.h = m_Rect.h;
		RenderTools()->DrawRoundRect(&rect, vec4(0.0f, 0.0f, 0.0f, s_LayoutOpacity), s_LayoutCornerRadius);
	}
	
	//Childs
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Render();
	}
}

void CHListLayout::Add(CWidget* pWidget)
{
	m_Childs.add(pWidget);
}

void CHListLayout::AddSeparator()
{
	Add(new CHListLayout::CSeparator(m_pConfig));
}

void CHListLayout::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CHListLayout::OnButtonClick(int X, int Y, int Button)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnButtonClick(X, Y, Button);
	}
}

void CHListLayout::OnButtonRelease(int Button)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnButtonRelease(Button);
	}
}

void CHListLayout::OnInputEvent()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnInputEvent();
	}
}

/* V LAYOUT *************************************************************/

CVListLayout::CSeparator::CSeparator(CConfig* pConfig) :
	CWidget(pConfig)
{
	SetHeight(20);
}

void CVListLayout::CSeparator::Render()
{
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(1.0f, 1.0f, 0.5f, 1.0f);
	
	float y = m_Rect.y + m_Rect.h * 0.7;
	IGraphics::CLineItem Line(m_Rect.x, y, m_Rect.x + m_Rect.w, y);
	Graphics()->LinesDraw(&Line, 1);
	
	Graphics()->LinesEnd();
}

CVListLayout::CSlider::CSlider(CVListLayout* pLayout) :
	CVSlider(pLayout->m_pConfig),
	m_pLayout(pLayout)
{
	
}

void CVListLayout::CSlider::OnNewPosition(float Pos)
{
	m_pLayout->OnNewScrollPos(Pos);
}

CVListLayout::CVListLayout(CConfig *pConfig, int Style) :
	CWidget(pConfig),
	m_Style(Style)
{
	m_pSlider = new CVListLayout::CSlider(this);
	m_ShowScrollBar = false;
	m_ScrollValue = 0;
}

CVListLayout::~CVListLayout()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i];
	}
	if(m_pSlider) delete m_pSlider;
}

void CVListLayout::Clear()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i];
	}
	m_Childs.clear();
	
	Update();
}

void CVListLayout::Update()
{
	m_ChildrenHeight = 0;
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Update();
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->SetRect(CRect(
			m_Rect.x + m_pConfig->m_LayoutSpacing,
			m_Rect.y + m_ChildrenHeight,
			m_Rect.w - m_pConfig->m_LayoutSpacing*2,
			m_Childs[i]->m_Rect.h
		));
		
		m_ChildrenHeight += m_Childs[i]->m_Rect.h + m_pConfig->m_LayoutSpacing;
	}
	
	//Add scrole bar
	if(m_ChildrenHeight > m_Rect.h)
	{
		m_ShowScrollBar = true;
		m_ScrollValue = 0;
		
		m_pSlider->SetRect(CRect(
			m_Rect.x + m_Rect.w - m_pSlider->m_Rect.w,
			m_Rect.y,
			m_pSlider->m_Rect.w,
			m_Rect.h
		));
		
		m_pSlider->Update();
		
		for(int i=0; i<m_Childs.size(); i++)
		{
			m_Childs[i]->SetWidth(m_Rect.w - m_pConfig->m_LayoutSpacing*3 - m_pSlider->m_Rect.w);
		}
	}
	else
	{
		m_ShowScrollBar = false;
		m_ScrollValue = 0;
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Update();
	}
}
	
void CVListLayout::Render()
{
	//Background
	if(m_Style == LAYOUTSTYLE_FRAME)
	{
		CUIRect rect;
		rect.x = m_Rect.x;
		rect.y = m_Rect.y;
		rect.w = m_Rect.w;
		rect.h = m_Rect.h;
		RenderTools()->DrawRoundRect(&rect, vec4(0.0f, 0.0f, 0.0f, s_LayoutOpacity), s_LayoutCornerRadius);
	}
	
	//Childs
	Graphics()->ClipEnable(m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h);
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Render();
	}
	Graphics()->ClipDisable();
	
	//Scrollbar
	if(m_ShowScrollBar)
	{
		m_pSlider->Render();
	}
}

void CVListLayout::Add(CWidget* pWidget)
{
	m_Childs.add(pWidget);
}

void CVListLayout::AddSeparator()
{
	Add(new CVListLayout::CSeparator(m_pConfig));
}

void CVListLayout::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_ShowScrollBar)
	{
		m_pSlider->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CVListLayout::OnButtonClick(int X, int Y, int Button)
{
	if(m_ShowScrollBar)
	{
		m_pSlider->OnButtonClick(X, Y, Button);
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnButtonClick(X, Y, Button);
	}
}

void CVListLayout::OnButtonRelease(int Button)
{
	if(m_ShowScrollBar)
	{
		m_pSlider->OnButtonRelease(Button);
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnButtonRelease(Button);
	}
}

void CVListLayout::OnInputEvent()
{
	if(m_ShowScrollBar)
	{
		m_pSlider->OnInputEvent();
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnInputEvent();
	}
}

void CVListLayout::OnNewScrollPos(float Pos)
{
	int ScrollHeight = m_ChildrenHeight - m_Rect.h;
	int LastScrollValue = m_ScrollValue;
	m_ScrollValue = ScrollHeight * Pos;
	int ScrollDelta = LastScrollValue - m_ScrollValue;
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->SetY(m_Childs[i]->m_Rect.y + ScrollDelta);
		m_Childs[i]->Update();
	}
}

void CVListLayout::SetScrollPos(float Pos)
{
	m_pSlider->SetSliderPos(Pos);
}

float CVListLayout::GetScrollPos()
{
	return m_pSlider->GetSliderPos();
}

}

}
