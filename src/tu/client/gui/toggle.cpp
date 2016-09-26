#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>
#include <tu/client/assetsrenderer.h>

#include "toggle.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT TOGGLE ****************************************************/

CAbstractToggle::CAbstractToggle(CContext *pConfig) :
	CAbstractLabel(pConfig),
	m_Clicked(false),
	m_MouseOver(false)
{
	m_ToggleStylePath = Context()->GetToggleStyle();
}

void CAbstractToggle::RefreshLabelStyle()
{
	const CAsset_GuiToggleStyle* pToggleStyle = AssetsManager()->GetAsset<CAsset_GuiToggleStyle>(m_ToggleStylePath);
	if(pToggleStyle)
	{
		if(GetValue())
		{
			if(pToggleStyle->GetSwitchIcon())
				SetIcon(pToggleStyle->GetIconTruePath());
			
			if(m_MouseOver)
				SetLabelStyle(pToggleStyle->GetMouseOverTrueStylePath());
			else
				SetLabelStyle(pToggleStyle->GetIdleTrueStylePath());
		}
		else
		{
			if(pToggleStyle->GetSwitchIcon())
				SetIcon(pToggleStyle->GetIconFalsePath());
			
			if(m_MouseOver)
				SetLabelStyle(pToggleStyle->GetMouseOverFalseStylePath());
			else
				SetLabelStyle(pToggleStyle->GetIdleFalseStylePath());
		}
	}
	else
		SetLabelStyle(CAssetPath::Null());
}

void CAbstractToggle::SetToggleStyle(CAssetPath StylePath)
{
	m_ToggleStylePath = StylePath;
	RefreshLabelStyle();
}
	
void CAbstractToggle::Update()
{
	RefreshLabelStyle();
	CAbstractLabel::Update();
}

void CAbstractToggle::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_DrawRect.IsInside(X, Y))
	{
		m_Clicked = true;
	}
}

void CAbstractToggle::OnButtonRelease(int Button)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(!m_Clicked)
		return;
	
	if(m_MouseOver)
	{
		m_Clicked = false;
		SetValue(!GetValue());
	}
}

void CAbstractToggle::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_DrawRect.IsInside(X, Y))
		m_MouseOver = true;
	else
		m_MouseOver = false;
}

/* TOGGLE *************************************************************/

CToggle::CToggle(CContext *pConfig, CAssetPath IconPath) :
	CAbstractToggle(pConfig)
{
	SetText("");
	SetIcon(IconPath);
}

CToggle::CToggle(CContext *pConfig, const char* pText) :
	CAbstractToggle(pConfig)
{
	SetText(pText);
}

CToggle::CToggle(CContext *pConfig, const CLocalizableString& LocalizableString) :
	CAbstractToggle(pConfig)
{
	SetText(LocalizableString.m_pText, true);
}

CToggle::CToggle(CContext *pConfig, const char* pText, CAssetPath IconPath) :
	CAbstractToggle(pConfig)
{
	SetText(pText);
	SetIcon(IconPath);
}

CToggle::CToggle(CContext *pConfig, const CLocalizableString& LocalizableString, CAssetPath IconPath) :
	CAbstractToggle(pConfig)
{
	SetText(LocalizableString.m_pText, true);
	SetIcon(IconPath);
}

}

}
