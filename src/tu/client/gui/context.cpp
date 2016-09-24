#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/input.h>
#include <game/client/render.h>
#include <tu/client/textrenderer.h>

#include "context.h"
#include "widget.h"
#include "popup.h"

namespace tu
{
	
namespace gui
{
	
int CContext::s_aButtons[] = {
	KEY_MOUSE_1,
	KEY_MOUSE_2,
	KEY_MOUSE_3,
	KEY_MOUSE_WHEEL_UP,
	KEY_MOUSE_WHEEL_DOWN
};

CContext::CContext(CKernel* pKernel) :
	CKernel::CGuest(pKernel),
	m_pMainWidget(0),
	m_GuiScale(1.0f),
	m_GuiDirection(DIRECTION_LTR)
{
	m_aLocalizationContext[0] = 0;
}

CContext::~CContext()
{
	for(int i=0; i<m_pPopups.size(); i++)
		delete m_pPopups[i];
	if(m_pMainWidget)
		delete m_pMainWidget;
}
	
void CContext::Init(const CRect& DrawRect)
{
	m_DrawRect = DrawRect;
	m_ShowCursor = true;
	
	for(int i=0; i<sizeof(s_aButtons)/sizeof(int); i++)
		m_Button[i] = 0;
	
	CreateMainWidget();
}

void CContext::Update()
{
	//Remove closed popups
	for(int i=0; i<m_pPopups.size(); i++)
	{
		if(m_pPopups[i]->IsClosed())
		{
			delete m_pPopups[i];
			m_pPopups.remove_index(i);
		}
	}
	
	DoShortcuts();
	
	float MousePosX;
	float MousePosY;
	Input()->MouseRelative(&MousePosX, &MousePosY);
	m_MouseDelta.x = MousePosX;
	m_MouseDelta.y = MousePosY;
	m_MousePos.x += m_MouseDelta.x;
	m_MousePos.y += m_MouseDelta.y;
	m_MousePos.x = clamp(m_MousePos.x, 0, Graphics()->ScreenWidth()-1);
	m_MousePos.y = clamp(m_MousePos.y, 0, Graphics()->ScreenHeight()-1);
	
	m_CursorPath = CAssetPath::SpriteSystem(SPRITE_CURSOR);
	
	int Keys = 0x0;
		
	if(Input()->KeyIsPressed(KEY_LCTRL) || Input()->KeyIsPressed(KEY_RCTRL))
		Keys |= TU_INPUT_CTRL;
		
	if(Input()->KeyIsPressed(KEY_LSHIFT) || Input()->KeyIsPressed(KEY_RSHIFT))
		Keys |= TU_INPUT_SHIFT;
	
	if(Input()->KeyIsPressed(KEY_LALT) || Input()->KeyIsPressed(KEY_RALT))
		Keys |= TU_INPUT_ALT;
	
	for(int i=0; i<m_pPopups.size(); i++)
	{
		m_pPopups[i]->OnInputEvent();
		m_pPopups[i]->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
	}
	if(m_pMainWidget && m_pPopups.size() == 0)
	{
		m_pMainWidget->OnInputEvent();
		m_pMainWidget->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);	
	}
	
	for(int i=0; i<(sizeof(s_aButtons)/sizeof(int)); i++)
	{
		int Button = s_aButtons[i];
		
		int Count = 0;
		if(Button == KEY_MOUSE_WHEEL_UP || Button == KEY_MOUSE_WHEEL_DOWN)
			Count = Input()->KeyPress(Button);
		else
			Count = (Input()->KeyIsPressed(Button) ? 1 : 0);
		
		if(Count)
		{
			if(m_Button[i] == 0)
			{
				m_Button[i] = 1;
				
				for(int i=0; i<m_pPopups.size(); i++)
					m_pPopups[i]->OnButtonClick(m_MousePos.x, m_MousePos.y, Button, Count);
				if(m_pMainWidget && m_pPopups.size() == 0)
					m_pMainWidget->OnButtonClick(m_MousePos.x, m_MousePos.y, Button, Count);
			}
		}
		else
		{
			if(m_Button[i] == 1)
			{
				m_Button[i] = 0;
				
				for(int i=0; i<m_pPopups.size(); i++)
					m_pPopups[i]->OnButtonRelease(Button);
				if(m_pMainWidget && m_pPopups.size() == 0)
					m_pMainWidget->OnButtonRelease(Button);
			}
		}
	}

	if(m_pMainWidget)
	{
		m_pMainWidget->Update();
		m_pMainWidget->UpdateBoundingSize();
		m_pMainWidget->UpdatePosition(m_DrawRect);
	}
	for(int i=0; i<m_pPopups.size(); i++)
	{
		m_pPopups[i]->Update();
		m_pPopups[i]->UpdateBoundingSize();
		m_pPopups[i]->UpdatePosition(m_DrawRect);
	}
	
	Input()->Clear();
}

void CContext::Render()
{
	Graphics()->MapScreen(m_DrawRect.x, m_DrawRect.y, m_DrawRect.w, m_DrawRect.h);
	
	RenderBackground();
	
	if(m_pMainWidget)
		m_pMainWidget->Render();
	
	for(int i=0; i<m_pPopups.size(); i++)
		m_pPopups[i]->Render();
	
	//Cursor
	if(m_ShowCursor)
	{	
		AssetsRenderer()->DrawSprite(
			m_CursorPath,
			vec2(m_MousePos.x,m_MousePos.y),
			1.0f, 0.0f, 0x0, 1.0f
		);
	}
}
	
void CContext::DisplayPopup(CPopup* pPopup)
{
	m_pPopups.add(pPopup);
}

}

}
