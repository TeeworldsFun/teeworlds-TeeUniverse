#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "listlayout.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT LIST LAYOUT ***********************************************/

CAbstractListLayout::CAbstractListLayout(CContext *pConfig) :
	CWidget(pConfig),
	m_BoxStylePath(CAssetPath::Null())
{
	
}

CAbstractListLayout::~CAbstractListLayout()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i].m_pWidget;
	}
}

void CAbstractListLayout::Clear()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i].m_pWidget;
	}
	m_Childs.clear();
}

void CAbstractListLayout::Add(CWidget* pWidget, bool Fill)
{
	CChild Child;
	Child.m_pWidget = pWidget;
	Child.m_Fill = Fill;
	m_Childs.add(Child);
}

void CAbstractListLayout::Update()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->Update();
	}
}
	
void CAbstractListLayout::Render()
{
	//Background
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		CRect Rect = m_DrawRect;
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	
		ivec2 MousePos = Context()->GetMousePos();
		if(Rect.IsInside(MousePos.x, MousePos.y))
			AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetMouseOverRectPath());
		else
			AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetDefaultRectPath());
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
	}
	
	//Childs
	Graphics()->ClipPush(m_ClipRect.x, m_ClipRect.y, m_ClipRect.w, m_ClipRect.h);
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->Render();
	}
	Graphics()->ClipPop();
}

void CAbstractListLayout::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CAbstractListLayout::OnButtonClick(int X, int Y, int Button, int Count)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->OnButtonClick(X, Y, Button, Count);
	}
}

void CAbstractListLayout::OnButtonRelease(int Button)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->OnButtonRelease(Button);
	}
}

void CAbstractListLayout::OnInputEvent()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->OnInputEvent();
	}
}

/* H LIST LAYOUT ******************************************************/
	
	//Separator	
CHListLayout::CSeparator::CSeparator(CContext* pConfig) :
	CWidget(pConfig)
{
	
}

void CHListLayout::CSeparator::UpdateBoundingSize()
{
	m_BoundingSizeRect.BSMinimum(5, 5);
}

void CHListLayout::CSeparator::Render()
{
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(1.0f, 1.0f, 0.5f, 1.0f);
	
	float x = m_DrawRect.x + m_DrawRect.w/2;
	IGraphics::CLineItem Line(x+0.5f, m_DrawRect.y, x+0.5f, m_DrawRect.y + m_DrawRect.h);
	Graphics()->LinesDraw(&Line, 1);
	
	Graphics()->LinesEnd();
}

	//List
CHListLayout::CHListLayout(CContext *pConfig) :
	CAbstractListLayout(pConfig)
{
	
}

void CHListLayout::UpdateBoundingSize()
{
	int Spacing = 0;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
		Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
	
	m_BoundingSizeRect.BSNoConstraint();
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->UpdateBoundingSize();
		
		if(i != 0)
			m_BoundingSizeRect.BSAddSpacing(Spacing, 0);
			
		m_BoundingSizeRect.BSHorizontalAdd(m_Childs[i].m_pWidget->GetBS());
	}
	
	if(pBoxStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
}

void CHListLayout::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	m_ClipRect = m_DrawRect;
	int Spacing = 0;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
	
	int NumFill = 0;
	int AvailableSpace = m_ClipRect.w;
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_Fill)
			NumFill++;
		else
			AvailableSpace -= m_Childs[i].m_pWidget->GetBS().minw;
	}
	int FillSize = 0;
	if(NumFill > 0)
		FillSize = AvailableSpace/NumFill;
	
	if(Context()->GetGuiDirection() == CContext::DIRECTION_RTL)
	{
		int PosX = m_ClipRect.x + m_ClipRect.w;
		for(int i=0; i<m_Childs.size(); i++)
		{
			int ChildWidth = (m_Childs[i].m_Fill ? FillSize : m_Childs[i].m_pWidget->GetBS().minw);
			CRect ChildRect(
				PosX - ChildWidth,
				m_ClipRect.y,
				ChildWidth,
				m_ClipRect.h
			);
			m_Childs[i].m_pWidget->UpdatePosition(ChildRect);
			PosX -= ChildRect.w + Spacing;
		}
	}
	else
	{
		int PosX = m_ClipRect.x;
		for(int i=0; i<m_Childs.size(); i++)
		{
			int ChildWidth = (m_Childs[i].m_Fill ? FillSize : m_Childs[i].m_pWidget->GetBS().minw);
			CRect ChildRect(
				PosX,
				m_ClipRect.y,
				ChildWidth,
				m_ClipRect.h
			);
			m_Childs[i].m_pWidget->UpdatePosition(ChildRect);
			PosX += ChildRect.w + Spacing;
		}
	}
}

void CHListLayout::AddSeparator()
{
	Add(new CHListLayout::CSeparator(Context()), false);
}

/* V LIST LAYOUT ******************************************************/

	//Separator
CVListLayout::CSeparator::CSeparator(CContext* pConfig) :
	CWidget(pConfig)
{
	
}

void CVListLayout::CSeparator::UpdateBoundingSize()
{
	m_BoundingSizeRect.BSMinimum(5, 5);
}

void CVListLayout::CSeparator::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
}

void CVListLayout::CSeparator::Render()
{
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(1.0f, 1.0f, 0.5f, 1.0f);
	
	float y = m_DrawRect.y + m_DrawRect.h * 0.7;
	IGraphics::CLineItem Line(m_DrawRect.x, y+0.5f, m_DrawRect.x + m_DrawRect.w, y+0.5f);
	Graphics()->LinesDraw(&Line, 1);
	
	Graphics()->LinesEnd();
}

	//List
CVListLayout::CVListLayout(CContext *pContext) :
	CAbstractListLayout(pContext),
	m_ShowScrollBar(false),
	m_pScrollBar(0)
{
	m_pScrollBar = new CVScrollBar(pContext);
}

CVListLayout::~CVListLayout()
{
	if(m_pScrollBar)
		delete m_pScrollBar;
}

void CVListLayout::UpdateBoundingSize()
{
	m_BoundingSizeRect.BSNoConstraint();
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->UpdateBoundingSize();
	}
	
	int Spacing = 0;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
	}
	
	if(m_ShowScrollBar)
	{
		m_pScrollBar->UpdateBoundingSize();
		
		m_BoundingSizeRect.BSAddSpacing(Spacing, 0);
		m_BoundingSizeRect.BSHorizontalAdd(m_pScrollBar->GetBS());
	}
}

void CVListLayout::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	m_ClipRect = m_DrawRect;
	int Spacing = 0;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
	
	int NumFill = 0;
	int AvailableSpace = m_ClipRect.h;
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_Fill)
			NumFill++;
		else
			AvailableSpace -= m_Childs[i].m_pWidget->GetBS().minh;
	}
	int FillSize = 0;
	if(NumFill > 0)
		FillSize = AvailableSpace/NumFill;
	
	//Compute total size
	m_ChildrenLength = 0;
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_ChildrenLength += (m_Childs[i].m_Fill ? FillSize : m_Childs[i].m_pWidget->GetBS().minh);
		
		if(i != 0)
			m_ChildrenLength += Spacing;
	}
	
	//Check if scrollbar is needed
	m_ShowScrollBar = false;
	if(m_pScrollBar && m_ChildrenLength > m_ClipRect.h)
	{
		m_pScrollBar->SetContentInformations(m_ChildrenLength, m_ClipRect.h);
		
		m_ClipRect.w -= m_pScrollBar->GetBS().minw;
		m_ClipRect.w -= Spacing;
	
		CRect ScrollBarRect = m_ClipRect;
		ScrollBarRect.w = m_pScrollBar->GetBS().minw;
		if(Context()->GetGuiDirection() == CContext::DIRECTION_RTL)
		{
			ScrollBarRect.x = m_ClipRect.x;
			m_ClipRect.x += ScrollBarRect.w + Spacing;
		}
		else
			ScrollBarRect.x = m_ClipRect.x + m_ClipRect.w + Spacing;
		
		m_pScrollBar->UpdatePosition(ScrollBarRect);
		
		m_ShowScrollBar = true;
	}
	
	//Update children position
	int PosY = m_ClipRect.y;
	if(m_ShowScrollBar)
	{
		PosY -= m_pScrollBar->GetContentPos();
	}
	for(int i=0; i<m_Childs.size(); i++)
	{
		CRect ChildRect(
			m_ClipRect.x,
			PosY,
			m_ClipRect.w,
			(m_Childs[i].m_Fill ? FillSize : m_Childs[i].m_pWidget->GetBS().minh)
		);
		m_Childs[i].m_pWidget->UpdatePosition(ChildRect);
		PosY += ChildRect.h + Spacing;
	}
}

void CVListLayout::Update()
{
	CAbstractListLayout::Update();
	
	if(m_pScrollBar && m_ShowScrollBar)
		m_pScrollBar->Update();
}
	
void CVListLayout::Render()
{
	CAbstractListLayout::Render();
	
	if(m_pScrollBar && m_ShowScrollBar)
		m_pScrollBar->Render();
}

void CVListLayout::AddSeparator()
{
	Add(new CVListLayout::CSeparator(Context()), false);
}

void CVListLayout::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_pScrollBar && m_ShowScrollBar)
		m_pScrollBar->OnMouseOver(X, Y, RelX, RelY, KeyState);
		
	CAbstractListLayout::OnMouseOver(X, Y, RelX, RelY, KeyState);
}

void CVListLayout::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(m_ShowScrollBar && m_DrawRect.IsInside(X, Y))
	{
		if(Button == KEY_MOUSE_WHEEL_UP)
		{
			m_pScrollBar->OnWheelUp();
			return;
		}
		else if(Button == KEY_MOUSE_WHEEL_DOWN)
		{
			m_pScrollBar->OnWheelDown();
			return;
		}
	}
	
	if(m_pScrollBar && m_ShowScrollBar)
		m_pScrollBar->OnButtonClick(X, Y, Button, Count);
		
	CAbstractListLayout::OnButtonClick(X, Y, Button, Count);
}

void CVListLayout::OnButtonRelease(int Button)
{
	if(m_pScrollBar && m_ShowScrollBar)
		m_pScrollBar->OnButtonRelease(Button);
		
	CAbstractListLayout::OnButtonRelease(Button);
}

void CVListLayout::OnInputEvent()
{
	if(m_pScrollBar && m_ShowScrollBar)
		m_pScrollBar->OnInputEvent();
		
	CAbstractListLayout::OnInputEvent();
}

}

}
