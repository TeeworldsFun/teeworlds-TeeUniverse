#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "panellayout.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT PANEL LAYOUT **********************************************/

CAbstractPanelLayout::CAbstractPanelLayout(CContext *pContext) :
	CWidget(pContext),
	m_BoxStylePath(CAssetPath::Null())
{
	
}

CAbstractPanelLayout::~CAbstractPanelLayout()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i].m_pWidget;
	}
}

void CAbstractPanelLayout::Clear()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i].m_pWidget;
	}
	m_Childs.clear();
}

void CAbstractPanelLayout::Add(CWidget* pWidget, int Size)
{
	CChild Child;
	Child.m_pWidget = pWidget;
	Child.m_Size = Size;
	Child.m_Fill = (Size < 0);
	Child.m_SliderClicked = false;
	m_Childs.add(Child);
}

void CAbstractPanelLayout::Update()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i].m_pWidget->Update();
	}
}
	
void CAbstractPanelLayout::Render()
{
	//Background
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		CRect Rect = m_DrawRect;
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	
		AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetRectPath());
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
	}
	
	//Childs
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_pWidget->IsDisabled())
			continue;
			
		m_Childs[i].m_pWidget->Render();
	}
}

void CAbstractPanelLayout::OnButtonClick(int X, int Y, int Button, int Count)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_pWidget->IsDisabled())
			continue;
			
		if(m_Childs[i].m_SliderRect.IsInside(X, Y))
			m_Childs[i].m_SliderClicked = true;
		
		m_Childs[i].m_pWidget->OnButtonClick(X, Y, Button, Count);
	}
}

void CAbstractPanelLayout::OnButtonRelease(int Button)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_pWidget->IsDisabled())
			continue;
			
		m_Childs[i].m_SliderClicked = false;
		m_Childs[i].m_pWidget->OnButtonRelease(Button);
	}
}

void CAbstractPanelLayout::OnInputEvent()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_pWidget->IsDisabled())
			continue;
			
		m_Childs[i].m_pWidget->OnInputEvent();
	}
}


/* H PANEL LAYOUT *****************************************************/

CHPanelLayout::CHPanelLayout(CContext *pContext) :
	CAbstractPanelLayout(pContext)
{
	
}

void CHPanelLayout::UpdateBoundingSize()
{
	int Spacing = 0;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
		Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
	
	m_BoundingSizeRect.BSNoConstraint();
	
	bool FirstChild = true;
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_pWidget->IsDisabled())
			continue;
		
		m_Childs[i].m_pWidget->UpdateBoundingSize();
		
		if(FirstChild)
			FirstChild = false;
		else
			m_BoundingSizeRect.BSAddSpacing(Spacing, 0);
			
		m_BoundingSizeRect.BSHorizontalAdd(m_Childs[i].m_pWidget->GetBS());
	}
	
	if(pBoxStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
}

void CHPanelLayout::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	m_ClipRect = m_DrawRect;
	int Spacing = 10;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		//~ Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
	
	int AvailableSpace = m_ClipRect.w;
	int NumFill = 0;
	int NumChilds = 0;
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_pWidget->IsDisabled())
			continue;
		
		NumChilds++;
		
		if(m_Childs[i].m_Fill)
			NumFill++;
		else
			AvailableSpace -= max(m_Childs[i].m_Size, m_Childs[i].m_pWidget->GetBS().minw);
	}
	if(NumChilds > 1)
		AvailableSpace -= (NumChilds-1)*Spacing;
	
	int FillSize = 0;
	if(NumFill > 0)
		FillSize = AvailableSpace/NumFill;
	
	if(Localization()->GetWritingDirection() == CLocalization::DIRECTION_RTL)
	{
		int PosX = m_ClipRect.x + m_ClipRect.w;
		for(int i=0; i<m_Childs.size(); i++)
		{
			if(m_Childs[i].m_pWidget->IsDisabled())
				continue;
				
			int ChildWidth;
			if(m_Childs[i].m_Fill)
				ChildWidth = FillSize;
			else
				ChildWidth = max(m_Childs[i].m_Size, m_Childs[i].m_pWidget->GetBS().minw);
			
			CRect ChildRect(
				PosX - ChildWidth,
				m_ClipRect.y,
				ChildWidth,
				m_ClipRect.h
			);
			m_Childs[i].m_pWidget->UpdatePosition(ChildRect);
			m_Childs[i].m_Size = ChildWidth;
			
			PosX -= ChildRect.w;
			
			m_Childs[i].m_SliderRect = CRect(PosX - Spacing, ChildRect.y, Spacing, ChildRect.h);
			
			PosX -= Spacing;
		}
	}
	else
	{
		int PosX = m_ClipRect.x;
		for(int i=0; i<m_Childs.size(); i++)
		{
			if(m_Childs[i].m_pWidget->IsDisabled())
				continue;
				
			int ChildWidth;
			if(m_Childs[i].m_Fill)
				ChildWidth = FillSize;
			else
				ChildWidth = max(m_Childs[i].m_Size, m_Childs[i].m_pWidget->GetBS().minw);
			
			CRect ChildRect(
				PosX,
				m_ClipRect.y,
				ChildWidth,
				m_ClipRect.h
			);
			m_Childs[i].m_pWidget->UpdatePosition(ChildRect);
			m_Childs[i].m_Size = ChildWidth;
			
			PosX += ChildRect.w;
			
			m_Childs[i].m_SliderRect = CRect(PosX, ChildRect.y, Spacing, ChildRect.h);
			
			PosX += Spacing;
		}
	}
}

void CHPanelLayout::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_pWidget->IsDisabled())
			continue;
			
		if(m_Childs[i].m_SliderClicked && (i+1)<m_Childs.size())
		{
			int Diff = RelX;
			
			if(Localization()->GetWritingDirection() == CLocalization::DIRECTION_RTL)
				Diff = -Diff;
			
			if(m_Childs[i].m_Size >= 0)
			{
				if(m_Childs[i].m_Size + Diff < m_Childs[i].m_pWidget->GetBS().minw)
					Diff = m_Childs[i].m_pWidget->GetBS().minw - m_Childs[i].m_Size;
			}			
			if(m_Childs[i+1].m_Size >= 0)
			{
				if(m_Childs[i+1].m_Size - Diff < m_Childs[i+1].m_pWidget->GetBS().minw)
					Diff = m_Childs[i+1].m_Size - m_Childs[i+1].m_pWidget->GetBS().minw;
			}
			
			if(m_Childs[i].m_Size >= 0)
				m_Childs[i].m_Size += Diff;
			if((i+1)<m_Childs.size() &&  m_Childs[i+1].m_Size >= 0)
				m_Childs[i+1].m_Size -= Diff;
		}
		
		if(m_Childs[i].m_SliderRect.IsInside(X, Y) || m_Childs[i].m_SliderClicked)
			Context()->SetCursorSprite(CAssetPath::SpriteSystem(SPRITE_CURSOR_TRANSLATEX));
		
		m_Childs[i].m_pWidget->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}


/* V PANEL LAYOUT *****************************************************/

CVPanelLayout::CVPanelLayout(CContext *pContext) :
	CAbstractPanelLayout(pContext)
{
	
}

void CVPanelLayout::UpdateBoundingSize()
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
			
		m_BoundingSizeRect.BSVerticalAdd(m_Childs[i].m_pWidget->GetBS());
	}
	
	if(pBoxStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
}

void CVPanelLayout::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	m_ClipRect = m_DrawRect;
	int Spacing = 10;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		//~ Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
	
	int AvailableSpace = m_ClipRect.h;
	int NumFill = 0;
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_Size < 0)
			NumFill++;
		else
			AvailableSpace -= max(m_Childs[i].m_Size, m_Childs[i].m_pWidget->GetBS().minw);
	}
	if(m_Childs.size() > 1)
		AvailableSpace -= (m_Childs.size()-1)*Spacing;
	
	int FillSize = 0;
	if(NumFill > 0)
		FillSize = AvailableSpace/NumFill;
	
	int PosY = m_ClipRect.y;
	for(int i=0; i<m_Childs.size(); i++)
	{
		int ChildHeight = max(m_Childs[i].m_Size, m_Childs[i].m_pWidget->GetBS().minh);
		if(m_Childs[i].m_Size < 0)
			ChildHeight = FillSize;
		
		CRect ChildRect(
			m_ClipRect.x,
			PosY,
			m_ClipRect.w,
			ChildHeight
		);
		m_Childs[i].m_pWidget->UpdatePosition(ChildRect);
		m_Childs[i].m_Size = ChildHeight;
		
		PosY += ChildRect.h;
		
		m_Childs[i].m_SliderRect = CRect(ChildRect.x, PosY, ChildRect.w, Spacing);
		
		PosY += Spacing;
	}
}

void CVPanelLayout::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_Childs[i].m_SliderClicked && (i+1)<m_Childs.size())
		{
			int Diff = RelY;
			
			if(Localization()->GetWritingDirection() == CLocalization::DIRECTION_RTL)
				Diff = -Diff;
			
			if(m_Childs[i].m_Size >= 0)
			{
				if(m_Childs[i].m_Size + Diff < m_Childs[i].m_pWidget->GetBS().minh)
					Diff = m_Childs[i].m_pWidget->GetBS().minh - m_Childs[i].m_Size;
			}			
			if(m_Childs[i+1].m_Size >= 0)
			{
				if(m_Childs[i+1].m_Size - Diff < m_Childs[i+1].m_pWidget->GetBS().minh)
					Diff = m_Childs[i+1].m_Size - m_Childs[i+1].m_pWidget->GetBS().minh;
			}
			
			if(m_Childs[i].m_Size >= 0)
				m_Childs[i].m_Size += Diff;
			if((i+1)<m_Childs.size() &&  m_Childs[i+1].m_Size >= 0)
				m_Childs[i+1].m_Size -= Diff;
		}
		
		if(m_Childs[i].m_SliderRect.IsInside(X, Y) || m_Childs[i].m_SliderClicked)
			Context()->SetCursorSprite(CAssetPath::SpriteSystem(SPRITE_CURSOR_TRANSLATEY));
		
		m_Childs[i].m_pWidget->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

}

}
