#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "expand.h"

namespace tu
{
	
namespace gui
{

CExpand::CExpand(CConfig *pConfig) :
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
	
	Update();
}

void CExpand::Update()
{
	m_pTitle->m_Rect.x = m_Rect.x;
	m_pTitle->m_Rect.y = m_Rect.y;
	m_pTitle->m_Rect.w = m_Rect.w;
	m_pTitle->Update();
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Update();
	}
	
	int PosY = m_Rect.y + m_pTitle->m_Rect.h + m_pConfig->m_ExpandSpacing;
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->SetRect(CRect(
			m_Rect.x + m_pConfig->m_ExpandShift,
			PosY,
			m_Rect.w - m_pConfig->m_ExpandShift,
			m_Childs[i]->m_Rect.h
		));
		
		PosY += m_Childs[i]->m_Rect.h + m_pConfig->m_ExpandSpacing;
	}
	
	m_Rect.h = m_pTitle->m_Rect.h + m_pConfig->m_ExpandSpacing*m_Childs.size();
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Update();
		
		m_Rect.h += m_Childs[i]->m_Rect.h;
	}
}
	
void CExpand::Render()
{
	m_pTitle->Render();
	
	//Childs
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->Render();
	}
}

void CExpand::SetTitle(CWidget* pWidget)
{
	m_pTitle = pWidget;
}

void CExpand::Add(CWidget* pWidget)
{
	m_Childs.add(pWidget);
}

void CExpand::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	m_pTitle->OnMouseOver(X, Y, RelX, RelY, KeyState);
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CExpand::OnButtonClick(int X, int Y, int Button, int Count)
{
	m_pTitle->OnButtonClick(X, Y, Button, Count);
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnButtonClick(X, Y, Button, Count);
	}
}

void CExpand::OnButtonRelease(int Button)
{
	m_pTitle->OnButtonRelease(Button);
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnButtonRelease(Button);
	}
}

void CExpand::OnInputEvent()
{
	m_pTitle->OnInputEvent();
	
	for(int i=0; i<m_Childs.size(); i++)
	{
		m_Childs[i]->OnInputEvent();
	}
}

}

}
