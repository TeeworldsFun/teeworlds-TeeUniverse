#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "scrollbar.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT SCROLLBAR *************************************************/

CAbstractScrollBar::CAbstractScrollBar(CContext *pContext) :
	CWidget(pContext),
	m_ViewSize(0),
	m_ContentSize(0),
	m_ContentPos(0.0f),
	m_Clicked(false),
	m_ClickShift(0)
{
	SetScrollbarStyle(Context()->GetScrollbarStyle());
}

void CAbstractScrollBar::UpdateBoundingSize()
{
	m_BoundingSizeRect.BSNoConstraint();
	
	const CAsset_GuiScrollbarStyle* pScrollbarStyle = AssetsManager()->GetAsset<CAsset_GuiScrollbarStyle>(m_ScrollbarStylePath);
	if(pScrollbarStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pScrollbarStyle->m_Padding));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pScrollbarStyle->m_Margin));
	}
}

void CAbstractScrollBar::SetContentInformations(int ContentSize, int ViewSize)
{
	m_ContentSize = ContentSize;
	m_ViewSize = ViewSize;
}

void CAbstractScrollBar::OnButtonRelease(int Button)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	m_Clicked = false;
}

void CAbstractScrollBar::OnWheelDown()
{
	m_ContentPos += 100.0f;
	
	if(m_ContentPos < 0)
		m_ContentPos = 0;
	if(m_ContentPos + m_ViewSize > m_ContentSize)
		m_ContentPos = m_ContentSize - m_ViewSize;
}

void CAbstractScrollBar::OnWheelUp()
{
	m_ContentPos -= 100.0f;
	
	if(m_ContentPos < 0)
		m_ContentPos = 0;
	if(m_ContentPos + m_ViewSize > m_ContentSize)
		m_ContentPos = m_ContentSize - m_ViewSize;
}

int CAbstractScrollBar::GetContentPos() const
{
	return m_ContentPos;
}

/* H SCROLLBAR *************************************************/

CHScrollBar::CHScrollBar(CContext *pContext) :
	CAbstractScrollBar(pContext)
{
	
}

void CHScrollBar::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	int Padding = 0;
	m_ClipRect = m_DrawRect;
	const CAsset_GuiScrollbarStyle* pScrollbarStyle = AssetsManager()->GetAsset<CAsset_GuiScrollbarStyle>(m_ScrollbarStylePath);
	if(pScrollbarStyle)
	{
		Padding = Context()->ApplyGuiScale(pScrollbarStyle->m_Padding);
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pScrollbarStyle->m_Margin));
		m_ClipRect.RemoveMargin(Padding);
	}
	
	m_ClipRect.y -= Padding;
	m_ClipRect.h += 2*Padding;
	
	int SliderPos = (m_ClipRect.w * m_ContentPos) / m_ContentSize;
	int SliderSize = (m_ClipRect.w * m_ViewSize) / m_ContentSize;
	
	m_SliderRect = CRect(
		m_ClipRect.x + SliderPos,
		m_ClipRect.y,
		SliderSize,
		m_ClipRect.h
	);
}
	
void CHScrollBar::Render()
{
	const CAsset_GuiScrollbarStyle* pScrollbarStyle = AssetsManager()->GetAsset<CAsset_GuiScrollbarStyle>(m_ScrollbarStylePath);
	if(pScrollbarStyle)
	{
		CRect Rect = m_DrawRect;
		Rect.RemoveMargin(Context()->ApplyGuiScale(pScrollbarStyle->m_Margin));
	
		AssetsRenderer()->DrawGuiRect(&Rect, pScrollbarStyle->m_RectPath);
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pScrollbarStyle->m_Padding));
	
		ivec2 MousePos = Context()->GetMousePos();
		
		ivec2 RailPos0(m_ClipRect.x, m_ClipRect.y + m_ClipRect.h/2);
		ivec2 RailPos1(m_ClipRect.x + m_ClipRect.w, m_ClipRect.y + m_ClipRect.h/2);
		AssetsRenderer()->DrawGuiLine(RailPos0, RailPos1, pScrollbarStyle->m_DefaultRailPath);
		
		ivec2 SliderPos0(m_SliderRect.x, m_SliderRect.y + m_SliderRect.h/2);
		ivec2 SliderPos1(m_SliderRect.x + m_SliderRect.w, m_SliderRect.y + m_SliderRect.h/2);
		if(m_SliderRect.IsInside(MousePos.x, MousePos.y))
			AssetsRenderer()->DrawGuiLine(SliderPos0, SliderPos1, pScrollbarStyle->m_MouseOverSliderPath);
		else
			AssetsRenderer()->DrawGuiLine(SliderPos0, SliderPos1, pScrollbarStyle->m_DefaultSliderPath);
	}
}

void CHScrollBar::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_SliderRect.IsInside(X, Y))
	{
		m_Clicked = true;
		m_ClickShift = X - m_SliderRect.x;
	}
}

void CHScrollBar::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_Clicked)
	{
		int SliderPos = X - m_SliderRect.x + m_ClickShift;
		m_ContentPos = (SliderPos * m_ContentSize)/m_ClipRect.w;
		if(m_ContentPos < 0)
			m_ContentPos = 0;
		if(m_ContentPos + m_ViewSize > m_ContentSize)
			m_ContentPos = m_ContentSize - m_ViewSize;
	}
}

/* V SCROLLBAR *************************************************/

CVScrollBar::CVScrollBar(CContext *pContext) :
	CAbstractScrollBar(pContext)
{
	
}

void CVScrollBar::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	int Padding = 0;
	m_ClipRect = m_DrawRect;
	const CAsset_GuiScrollbarStyle* pScrollbarStyle = AssetsManager()->GetAsset<CAsset_GuiScrollbarStyle>(m_ScrollbarStylePath);
	if(pScrollbarStyle)
	{
		Padding = Context()->ApplyGuiScale(pScrollbarStyle->m_Padding);
		
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pScrollbarStyle->m_Margin));
		m_ClipRect.RemoveMargin(Padding);
	}
	
	int SliderPos = (m_ClipRect.h * m_ContentPos) / m_ContentSize;
	int SliderSize = (m_ClipRect.h * m_ViewSize) / m_ContentSize;
	
	m_ClipRect.x -= Padding;
	m_ClipRect.w += 2*Padding;
	
	m_SliderRect = CRect(
		m_ClipRect.x,
		m_ClipRect.y + SliderPos,
		m_ClipRect.w,
		SliderSize
	);
}
	
void CVScrollBar::Render()
{
	const CAsset_GuiScrollbarStyle* pScrollbarStyle = AssetsManager()->GetAsset<CAsset_GuiScrollbarStyle>(m_ScrollbarStylePath);
	if(pScrollbarStyle)
	{
		CRect Rect = m_DrawRect;
		Rect.RemoveMargin(Context()->ApplyGuiScale(pScrollbarStyle->m_Margin));
		AssetsRenderer()->DrawGuiRect(&Rect, pScrollbarStyle->m_RectPath);
		Rect.RemoveMargin(Context()->ApplyGuiScale(pScrollbarStyle->m_Padding));
	
		ivec2 MousePos = Context()->GetMousePos();
		
		ivec2 RailPos0(m_ClipRect.x + m_ClipRect.w/2, m_ClipRect.y);
		ivec2 RailPos1(m_ClipRect.x + m_ClipRect.w/2, m_ClipRect.y + m_ClipRect.h);
		AssetsRenderer()->DrawGuiLine(RailPos0, RailPos1, pScrollbarStyle->m_DefaultRailPath);
		
		ivec2 SliderPos0(m_SliderRect.x + m_SliderRect.w/2, m_SliderRect.y);
		ivec2 SliderPos1(m_SliderRect.x + m_SliderRect.w/2, m_SliderRect.y + m_SliderRect.h);
		if(m_SliderRect.IsInside(MousePos.x, MousePos.y))
			AssetsRenderer()->DrawGuiLine(SliderPos0, SliderPos1, pScrollbarStyle->m_MouseOverSliderPath);
		else
			AssetsRenderer()->DrawGuiLine(SliderPos0, SliderPos1, pScrollbarStyle->m_DefaultSliderPath);
	}
}

void CVScrollBar::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_SliderRect.IsInside(X, Y))
	{
		m_Clicked = true;
	}
}

void CVScrollBar::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_Clicked)
	{
		int SliderPos = m_SliderRect.y - m_ClipRect.y + RelY;
		m_ContentPos = static_cast<float>(SliderPos * m_ContentSize)/m_ClipRect.h;
		if(m_ContentPos < 0)
			m_ContentPos = 0;
		if(m_ContentPos + m_ViewSize > m_ContentSize)
			m_ContentPos = m_ContentSize - m_ViewSize;
	}
}

}

}
