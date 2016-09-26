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
	m_Clicked(false),
	m_MouseOver(false)
{
	m_ButtonStylePath = Context()->GetButtonStyle();
}

void CAbstractButton::RefreshLabelStyle()
{
	const CAsset_GuiButtonStyle* pButtonStyle = AssetsManager()->GetAsset<CAsset_GuiButtonStyle>(m_ButtonStylePath);
	if(pButtonStyle)
	{
		if(m_MouseOver)
			SetLabelStyle(pButtonStyle->GetMouseOverStylePath());
		else
			SetLabelStyle(pButtonStyle->GetIdleStylePath());
	}
	else
		SetLabelStyle(CAssetPath::Null());
}

void CAbstractButton::SetButtonStyle(CAssetPath StylePath)
{
	m_ButtonStylePath = StylePath;
	RefreshLabelStyle();
}
	
void CAbstractButton::Update()
{
	RefreshLabelStyle();
	CAbstractLabel::Update();
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
	
	if(m_MouseOver)
	{
		m_Clicked = false;
		MouseClickAction();
	}
}

void CAbstractButton::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_DrawRect.IsInside(X, Y))
		m_MouseOver = true;
	else
		m_MouseOver = false;
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

}

}
