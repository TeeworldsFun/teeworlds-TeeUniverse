#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>
#include <tu/client/assetsrenderer.h>

#include "expand.h"

namespace tu
{
	
namespace gui
{

CExpand::CExpand(CContext *pConfig) :
	CWidget(pConfig),
	m_pTitle(0)
{
	
}

CExpand::~CExpand()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i];
	}
	
	if(m_pTitle)
		delete m_pTitle;
}

void CExpand::Clear()
{
	for(int i=0; i<m_Childs.size(); i++)
	{
		delete m_Childs[i];
	}
	m_Childs.clear();
}

void CExpand::UpdateBoundingSize()
{	
	m_BoundingSizeRect.BSNoConstraint();
	
	int Spacing = 0;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
		Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
	
	if(m_pTitle)
	{
		m_pTitle->UpdateBoundingSize();
		m_BoundingSizeRect.BSVerticalAdd(m_pTitle->GetBS());
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->UpdateBoundingSize();
		
		if(m_pTitle || i>0)
			m_BoundingSizeRect.BSAddSpacing(0, Spacing);
		
		m_BoundingSizeRect.BSVerticalAdd(m_Childs[i]->GetBS());
	}
	
	if(pBoxStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
	}
}

void CExpand::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	int Spacing = 0;
	CRect ContentRect = m_DrawRect;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		Spacing = pBoxStyle->GetSpacing();
		ContentRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		ContentRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
	
	int PosY = ContentRect.y;
	if(m_pTitle)
	{
		CRect TitleRect(
			ContentRect.x,
			PosY,
			ContentRect.w,
			m_pTitle->GetBS().minh
		);
		m_pTitle->UpdatePosition(TitleRect);
		PosY += TitleRect.h;
	}
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		if(m_pTitle || i>0)
			PosY += Spacing;
		
		CRect ChildRect(
			ContentRect.x,
			PosY,
			ContentRect.w - GetShift(),
			m_Childs[i]->GetBS().minh
		);
		
		if(Context()->GetGuiDirection() != CContext::DIRECTION_RTL)
			ChildRect.x += GetShift();
		
		m_Childs[i]->UpdatePosition(ChildRect);
		PosY += ChildRect.h;
	}
}

void CExpand::Update()
{
	if(m_pTitle)
		m_pTitle->Update();
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Update();
	}
}
	
void CExpand::Render()
{
	//Background
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		CRect Rect = m_DrawRect;
		Rect.RemoveMargin(pBoxStyle->GetMargin());
	
		ivec2 MousePos = Context()->GetMousePos();
		if(Rect.IsInside(MousePos.x, MousePos.y))
			AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetMouseOverRectPath());
		else
			AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetDefaultRectPath());
			
		Rect.RemoveMargin(pBoxStyle->GetPadding());
	}
	
	if(m_pTitle)
		m_pTitle->Render();
	
	//Childs
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Render();
	}
}

void CExpand::SetTitle(CWidget* pWidget)
{
	if(m_pTitle)
		delete m_pTitle;
	
	m_pTitle = pWidget;
}

void CExpand::Add(CWidget* pWidget)
{
	m_Childs.add(pWidget);
}

void CExpand::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_pTitle)
		m_pTitle->OnMouseOver(X, Y, RelX, RelY, KeyState);
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CExpand::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(m_pTitle)
		m_pTitle->OnButtonClick(X, Y, Button, Count);
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnButtonClick(X, Y, Button, Count);
	}
}

void CExpand::OnButtonRelease(int Button)
{
	if(m_pTitle)
		m_pTitle->OnButtonRelease(Button);
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnButtonRelease(Button);
	}
}

void CExpand::OnInputEvent()
{
	if(m_pTitle)
		m_pTitle->OnInputEvent();
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnInputEvent();
	}
}

}

}
