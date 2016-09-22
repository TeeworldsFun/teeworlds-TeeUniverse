#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>
#include <tu/client/assetsrenderer.h>

#include "button.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT BUTTON ****************************************************/

CAbstractButton::CAbstractButton(CContext *pContext) :
	CAbstractLabel(pContext),
	m_Clicked(false)
{
	SetBoxStyle(Context()->GetButtonStyle());
}

void CAbstractButton::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_DrawRect.IsInside(X, Y))
	{
		m_Clicked = true;
	}
}

void CAbstractButton::OnButtonRelease(int Button)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(!m_Clicked)
		return;
	
	ivec2 MousePos = Context()->GetMousePos();
	if(m_DrawRect.IsInside(MousePos.x, MousePos.y))
	{
		m_Clicked = false;
		MouseClickAction();
	}
}

/* BUTTON *************************************************************/

CButton::CButton(CContext *pConfig, const char* pText) :
	CAbstractButton(pConfig)
{
	SetText(pText);
	SetIcon(CAssetPath::Null());
}

CButton::CButton(CContext *pConfig, CAssetPath IconPath) :
	CAbstractButton(pConfig)
{
	SetText("");
	SetIcon(IconPath);
}

CButton::CButton(CContext *pConfig, const char* pText, CAssetPath IconPath) :
	CAbstractButton(pConfig)
{
	SetText(pText);
	SetIcon(IconPath);
}

/* ABSTRACT TOGGLE ****************************************************/

CAbstractToggle::CAbstractToggle(CContext *pConfig) :
	CAbstractButton(pConfig)
{
	
}

void CAbstractToggle::MouseClickAction()
{
	m_Toggle = !m_Toggle;
	OnToggle(m_Toggle);
}

/* TOGGLE *************************************************************/

CToggle::CToggle(CContext *pConfig, const char* pText) :
	CAbstractToggle(pConfig)
{
	SetText(pText);
	SetIcon(CAssetPath::Null());
}

CToggle::CToggle(CContext *pConfig, CAssetPath IconPath) :
	CAbstractToggle(pConfig)
{
	SetText("");
	SetIcon(IconPath);
}

CToggle::CToggle(CContext *pConfig, const char* pText, CAssetPath IconPath) :
	CAbstractToggle(pConfig)
{
	SetText(pText);
	SetIcon(IconPath);
}

}

}
