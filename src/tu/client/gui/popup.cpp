#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "popup.h"

namespace tu
{
	
namespace gui
{

CPopup::CPopup(CContext *pConfig, const CRect& CreatorRect, int Width, int Height, int Alignment) :
	CWidget(pConfig),
	m_pChild(0),
	m_IsClosed(false),
	m_Width(Width),
	m_Height(Height),
	m_Alignment(Alignment),
	m_CreatorRect(CreatorRect),
	m_BoxStylePath(Context()->GetPopupStyle())
{

}

CPopup::~CPopup()
{
	if(m_pChild)
		delete m_pChild;
}

void CPopup::Clear()
{
	if(m_pChild)
	{
		delete m_pChild;
		m_pChild = 0;
		m_ChildWidth = 0;
		m_ChildHeight = 0;
	}
}

void CPopup::UpdateBoundingSize()
{
	if(m_pChild)
		m_pChild->UpdateBoundingSize();
	
	m_BoundingSizeRect.BSFixed(m_Width, m_Height);
}

void CPopup::UpdatePosition(CRect BoundingRect)
{
	if(m_Alignment == ALIGNMENT_INNER)
	{
		m_DrawRect = m_CreatorRect;
	}
	else
	{
		int TryRight = m_CreatorRect.x;
		int TryLeft = m_CreatorRect.x - m_Width;
		int TryTop = m_CreatorRect.y + m_CreatorRect.h - m_Height;
		int TryBottom = m_CreatorRect.y;
		
		if((m_CreatorRect.x + m_CreatorRect.w/2) > (BoundingRect.x + BoundingRect.w/2))
		{
			if(TryRight + m_Width < BoundingRect.x + BoundingRect.w)
				m_DrawRect.x = TryRight;
			else
				m_DrawRect.x = TryLeft;
		}
		else
		{
			if(TryLeft >= BoundingRect.x)
				m_DrawRect.x = TryLeft;
			else
				m_DrawRect.x = TryRight;
		}
			
		if(TryBottom + m_Height < BoundingRect.y + BoundingRect.h)
			m_DrawRect.y = TryBottom;
		else
			m_DrawRect.y = TryTop;
	}
	
	m_DrawRect.w = m_Width;
	m_DrawRect.h = m_Height;
	
	if(m_pChild)
	{
		CRect ContentRect = m_DrawRect;
		const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
		if(pBoxStyle)
		{
			ContentRect.RemoveMargin(pBoxStyle->GetMargin());
			ContentRect.RemoveMargin(pBoxStyle->GetPadding());
		}
		m_pChild->UpdatePosition(ContentRect);
	}
}

void CPopup::Update()
{
	if(m_pChild)
	{
		m_pChild->Update();
	}
}
	
void CPopup::Render()
{
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		CRect Rect = m_DrawRect;
		Rect.RemoveMargin(pBoxStyle->GetMargin());
	
		AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetRectPath());
			
		Rect.RemoveMargin(pBoxStyle->GetPadding());
	}
	
	if(m_pChild)
		m_pChild->Render();
}

void CPopup::Add(CWidget* pWidget)
{
	if(m_pChild)
		delete m_pChild;
	
	m_pChild = pWidget;
}

void CPopup::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_pChild)
	{
		m_pChild->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CPopup::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(m_DrawRect.IsInside(X, Y))
	{
		if(m_pChild)
		{
			m_pChild->OnButtonClick(X, Y, Button, Count);
		}
	}
	else if(Button == KEY_MOUSE_1)
	{
		Close();
	}
}

void CPopup::OnButtonRelease(int Button)
{
	if(m_pChild)
	{
		m_pChild->OnButtonRelease(Button);
	}
}

void CPopup::OnInputEvent()
{
	if(m_pChild)
	{
		m_pChild->OnInputEvent();
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
