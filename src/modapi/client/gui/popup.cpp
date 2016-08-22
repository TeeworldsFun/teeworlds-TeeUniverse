#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "popup.h"

namespace tu
{
	
namespace gui
{

CPopup::CPopup(CConfig *pConfig, const CRect& CreatorRect, int Width, int Height, int Alignment) :
	CWidget(pConfig),
	m_Child(0),
	m_IsClosed(false)
{
	int ScreenWidth = Graphics()->ScreenWidth();
	int ScreenHeight = Graphics()->ScreenHeight();
	
	switch(Alignment)
	{
		case ALIGNMENT_LEFT:
			SetRect(CRect(
				CreatorRect.x - Width - m_pConfig->m_LayoutSpacing,
				min(CreatorRect.y, ScreenHeight - Height - m_pConfig->m_LayoutSpacing),
				Width,
				Height
			));
			break;
		case ALIGNMENT_RIGHT:
		default:
			SetRect(CRect(
				CreatorRect.x + CreatorRect.w + m_pConfig->m_LayoutSpacing,
				min(CreatorRect.y, ScreenHeight - Height - m_pConfig->m_LayoutSpacing),
				Width,
				Height
			));
	}
}

CPopup::~CPopup()
{
	if(m_Child)
		delete m_Child;
}

void CPopup::Clear()
{
	if(m_Child)
	{
		delete m_Child;
		m_Child = 0;
		m_ChildWidth = 0;
		m_ChildHeight = 0;
	}
}

void CPopup::Update()
{
	if(m_Child)
	{
		m_Child->SetRect(CRect(
			m_Rect.x + m_pConfig->m_LayoutMargin,
			m_Rect.y + m_pConfig->m_LayoutMargin,
			m_Rect.w - m_pConfig->m_LayoutMargin*2,
			m_Rect.h - m_pConfig->m_LayoutMargin*2)
		);
		m_Child->Update();
	}
}
	
void CPopup::Render()
{
	if(!m_Child)
		return;
		
	//Background	
	{
		CUIRect rect;
		rect.x = m_Rect.x;
		rect.y = m_Rect.y;
		rect.w = m_Rect.w;
		rect.h = m_Rect.h;
		RenderTools()->DrawRoundRect(&rect, vec4(0.5f, 0.5f, 0.5f, 0.8f), s_LayoutCornerRadius);
	}
	
	m_Child->Render();
}

void CPopup::Add(CWidget* pWidget)
{
	if(m_Child) delete m_Child;
	m_Child = pWidget;
}

void CPopup::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_Child)
	{
		m_Child->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CPopup::OnButtonClick(int X, int Y, int Button)
{
	if(m_Rect.IsInside(X, Y))
	{
		if(m_Child)
		{
			m_Child->OnButtonClick(X, Y, Button);
		}
	}
	else if(Button == KEY_MOUSE_1)
	{
		Close();
	}
}

void CPopup::OnButtonRelease(int Button)
{
	if(m_Child)
	{
		m_Child->OnButtonRelease(Button);
	}
}

void CPopup::OnInputEvent()
{
	if(m_Child)
	{
		m_Child->OnInputEvent();
	}
}

void CPopup::Close()
{
	m_IsClosed = true;
}

bool CPopup::IsClosed()
{
	return m_IsClosed;
}

}

}
