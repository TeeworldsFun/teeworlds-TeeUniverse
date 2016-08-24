#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "slider.h"

namespace tu
{
	
namespace gui
{

CAbstractSlider::CAbstractSlider(CConfig *pConfig) :
	CWidget(pConfig)
{	
	m_UnderMouse = false;
	m_ButtonDown = false;
	m_Pos = 0.0f;
}
	
void CAbstractSlider::Render()
{
	CUIRect rail;
	rail.x = m_RailRect.x;
	rail.y = m_RailRect.y;
	rail.w = m_RailRect.w;
	rail.h = m_RailRect.h;
	RenderTools()->DrawRoundRect(&rail, vec4(1.0f, 1.0f, 1.0f, 0.25f), m_pConfig->m_SliderWidth/2);
	
	CUIRect slider;
	slider.x = m_SliderRect.x;
	slider.y = m_SliderRect.y;
	slider.w = m_SliderRect.w;
	slider.h = m_SliderRect.h;
	
	if(m_UnderMouse || m_ButtonDown)
	{
		RenderTools()->DrawRoundRect(&slider, vec4(1.0f, 1.0f, 1.0f, 1.0f), m_pConfig->m_SliderWidth/2);
	}
	else
	{
		RenderTools()->DrawRoundRect(&slider, vec4(1.0f, 1.0f, 1.0f, 0.5f), m_pConfig->m_SliderWidth/2);
	}
}

void CAbstractSlider::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_SliderRect.IsInside(X, Y))
	{
		m_UnderMouse = true;
	}
	else
	{
		m_UnderMouse = false;
	}
}

void CAbstractSlider::OnButtonRelease(int Button)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	m_ButtonDown = false;
}

float CAbstractSlider::GetSliderPos()
{
	return m_Pos;
}

void CAbstractSlider::SetSliderPos(float Pos, bool Trigger)
{
	m_Pos = Pos;
	
	if(Trigger)
		OnNewPosition(Pos);
}

CHSlider::CHSlider(CConfig *pConfig) :
	CAbstractSlider(pConfig)
{
	m_SliderRect.w = m_pConfig->m_SliderHeight;
	m_SliderRect.h = m_pConfig->m_SliderWidth;
	
	m_RailRect.h = m_SliderRect.h;
	m_Rect.h = m_SliderRect.h;
}

void CHSlider::Update()
{
	m_RailRect.x = m_Rect.x;
	m_RailRect.y = m_Rect.y + m_Rect.h/2 -m_RailRect.h/2;
	m_RailRect.w = m_Rect.w;
	
	m_SliderRect.x = m_Rect.x + (int)(m_Pos*static_cast<float>(m_Rect.w - m_SliderRect.w));
	m_SliderRect.y = m_Rect.y + m_Rect.h/2 -m_SliderRect.h/2;
}

void CHSlider::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_Rect.IsInside(X, Y))
	{
		m_SliderRect.x = clamp(X - m_SliderRect.w/2, m_Rect.x, m_Rect.x + m_Rect.w - m_SliderRect.w);
		
		m_Pos = static_cast<float>(m_SliderRect.x - m_Rect.x)/static_cast<float>(m_Rect.w - m_SliderRect.w);
		OnNewPosition(m_Pos);
		
		m_ButtonDown = true;
	}
}

void CHSlider::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	CAbstractSlider::OnMouseOver(X, Y, RelX, RelY, KeyState);
	
	if(m_ButtonDown && RelX != 0)
	{
		m_SliderRect.x = clamp(m_SliderRect.x + RelX, m_Rect.x, m_Rect.x + m_Rect.w - m_SliderRect.w);
		
		m_Pos = static_cast<float>(m_SliderRect.x - m_Rect.x)/static_cast<float>(m_Rect.w - m_SliderRect.w);
		OnNewPosition(m_Pos);
	}
}
	
void CHSlider::SetSliderSize(int Size)
{
	m_SliderRect.w = Size;
}

CVSlider::CVSlider(CConfig *pConfig) :
	CAbstractSlider(pConfig)
{
	m_SliderRect.w = m_pConfig->m_SliderWidth;
	m_SliderRect.h = m_pConfig->m_SliderHeight;
	
	m_RailRect.w = m_SliderRect.w;
	m_Rect.w = m_SliderRect.w;
}

void CVSlider::Update()
{
	m_RailRect.x = m_Rect.x;
	m_RailRect.y = m_Rect.y;
	m_RailRect.h = m_Rect.h;
	
	m_SliderRect.x = m_Rect.x;
	m_SliderRect.y = m_Rect.y + (int)(m_Pos*static_cast<float>(m_Rect.h - m_SliderRect.h));
}

void CVSlider::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_Rect.IsInside(X, Y))
	{
		m_SliderRect.y = clamp(Y - m_SliderRect.h/2, m_Rect.y, m_Rect.y + m_Rect.h - m_SliderRect.h);
		
		m_Pos = static_cast<float>(m_SliderRect.y - m_Rect.y)/static_cast<float>(m_Rect.h - m_SliderRect.h);
		OnNewPosition(m_Pos);
		
		m_ButtonDown = true;
	}
}

void CVSlider::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	CAbstractSlider::OnMouseOver(X, Y, RelX, RelY, KeyState);
	
	if(m_ButtonDown && RelY != 0)
	{
		m_SliderRect.y = clamp(m_SliderRect.y + RelY, m_Rect.y, m_Rect.y + m_Rect.h - m_SliderRect.h);
		
		m_Pos = static_cast<float>(m_SliderRect.y - m_Rect.y)/static_cast<float>(m_Rect.h - m_SliderRect.h);
		OnNewPosition(m_Pos);
	}
}
	
void CVSlider::SetSliderSize(int Size)
{
	m_SliderRect.h = Size;
}

}

}
