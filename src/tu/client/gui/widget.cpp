#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "widget.h"

namespace tu
{
	
namespace gui
{

CRect::CRect() :
	x(0),
	y(0),
	w(0),
	h(0)
{
	
}

CRect::CRect(int X, int Y, int W, int H) :
	x(X),
	y(Y),
	w(W),
	h(H)
{
	
}

void CRect::CenterIn(const CRect& Rect)
{
	x = Rect.x + Rect.w/2 - w/2;
	y = Rect.y + Rect.h/2 - h/2;
}

bool CRect::IsInside(int X, int Y)
{
	return (X >= x && X <= x + w && Y >= y && Y <= y + h);
}

CWidget::CWidget(CConfig *pConfig) :
	m_pConfig(pConfig)
{
	
}

CWidget::~CWidget()
{
	
}

void CWidget::Update()
{
	
}

void CWidget::SetRect(const CRect& Rect)
{
	m_Rect = Rect;
}

void CWidget::SetX(int x)
{
	SetRect(CRect(
		x,
		m_Rect.y,
		m_Rect.w,
		m_Rect.h	
	));
}

void CWidget::SetY(int y)
{
	SetRect(CRect(
		m_Rect.x,
		y,
		m_Rect.w,
		m_Rect.h	
	));
}

void CWidget::SetWidth(int w)
{
	SetRect(CRect(
		m_Rect.x,
		m_Rect.y,
		w,
		m_Rect.h	
	));
}

void CWidget::SetHeight(int h)
{
	SetRect(CRect(
		m_Rect.x,
		m_Rect.y,
		m_Rect.w,	
		h
	));
}

void CWidget::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	
}

void CWidget::OnButtonClick(int X, int Y, int Button, int Count)
{
	
}

void CWidget::OnButtonRelease(int Button)
{
	
}

void CWidget::OnInputEvent()
{
	
}

float CWidget::s_LayoutOpacity = 0.25f;
float CWidget::s_LayoutCornerRadius = 5.0f;

int CWidget::s_ButtonInnerMargin = 5;
float CWidget::s_ButtonOpacity = 0.25f;
float CWidget::s_ButtonCornerRadius = 5.0f;

}

}
