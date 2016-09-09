#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "button.h"

namespace tu
{
	
namespace gui
{

CAbstractButton::CAbstractButton(CConfig *pConfig) :
	CWidget(pConfig),
	m_Clicked(false),
	m_UnderMouse(false),
	m_ButtonStyle(CConfig::BUTTONSTYLE_DEFAULT)
{	
	m_Rect.w = m_pConfig->m_ButtonHeight;
	m_Rect.h = m_pConfig->m_ButtonHeight;
}
	
void CAbstractButton::Render()
{
	if(m_UnderMouse)
		TUGraphics()->DrawGuiRect(&m_Rect, m_pConfig->m_ButtonStyles[m_ButtonStyle].m_StylePath_UnderMouse);
	else
		TUGraphics()->DrawGuiRect(&m_Rect, m_pConfig->m_ButtonStyles[m_ButtonStyle].m_StylePath_Normal);
}

void CAbstractButton::SetButtonStyle(int Style)
{
	m_ButtonStyle = Style;
}

void CAbstractButton::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_Rect.IsInside(X, Y))
	{
		m_UnderMouse = true;
	}
	else
	{
		m_UnderMouse = false;
	}
}

void CAbstractButton::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_Rect.IsInside(X, Y))
	{
		m_Clicked = true;
	}
}

void CAbstractButton::OnButtonRelease(int Button)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_UnderMouse && m_Clicked)
	{
		m_Clicked = false;
		MouseClickAction();
	}
}

/* ICON BUTTON ********************************************************/

CIconButton::CIconButton(CConfig *pConfig, CAssetPath IconPath) :
	CAbstractButton(pConfig),
	m_IconPath(IconPath)
{
	
}

void CIconButton::Render()
{
	CAbstractButton::Render();
	
	int PosX = m_Rect.x + m_Rect.w/2;
	int PosY = m_Rect.y + m_Rect.h/2;
	
	TUGraphics()->DrawSprite(
		m_IconPath,
		vec2(PosX, PosY),
		1.0f, 0.0f, 0x0, 1.0f
	);
}

void CIconButton::SetIcon(CAssetPath IconPath)
{
	m_IconPath = IconPath;
}

/* ICON TOGGLE ********************************************************/

CIconToggle::CIconToggle(CConfig *pConfig, CAssetPath IconPath) :
	CIconButton(pConfig, IconPath)
{
	
}

void CIconToggle::MouseClickAction()
{
	m_Toggle = !m_Toggle;
	OnToggle(m_Toggle);
}

/* TEXT BUTTON ********************************************************/

CTextButton::CTextButton(CConfig *pConfig, const char* pText, CAssetPath IconPath) :
	CAbstractButton(pConfig)
{
	SetText(pText);
	SetIcon(IconPath);
	
	m_Centered = true;
	
	m_Rect.w = m_pConfig->m_ButtonHeight;
	m_Rect.h = m_pConfig->m_ButtonHeight;
}

void CTextButton::Render()
{
	CAbstractButton::Render();
		
	int TextWidth = TextRender()->TextWidth(0, m_pConfig->m_TextSize[TEXTSTYLE_NORMAL], m_aText, -1);
	
	int PosX;
	if(m_Centered)
	{
		int CenterX = m_Rect.x + m_Rect.w/2;
		
		if(!m_IconPath.IsNull())
			CenterX += m_pConfig->m_IconSize/2;
		
		PosX = CenterX - TextWidth/2;
	}
	else
	{
		PosX = m_Rect.x + m_pConfig->m_LabelMargin;
		
		if(!m_IconPath.IsNull())
			PosX += m_pConfig->m_IconSize;
	}
	
	int CenterY = m_Rect.y + m_Rect.h/2;
	int PosY = CenterY - m_pConfig->m_TextSize[TEXTSTYLE_NORMAL]*0.7;
	
	CTextCursor Cursor;
	TextRender()->SetCursor(&Cursor, PosX, PosY, m_pConfig->m_TextSize[TEXTSTYLE_NORMAL], TEXTFLAG_RENDER);
	TextRender()->TextColor(m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].x, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].y, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].z, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].w);
	TextRender()->TextEx(&Cursor, m_aText, -1);
	
	if(!m_IconPath.IsNull())
	{
		TUGraphics()->DrawSprite(
			m_IconPath,
			vec2(PosX-m_pConfig->m_IconSize/2-4, CenterY),
			1.0f, 0.0f, 0x0, 1.0f
		);
	}
}

void CTextButton::SetText(const char* pText)
{
	if(pText)
		str_copy(m_aText, pText, sizeof(m_aText));
}

void CTextButton::SetIcon(CAssetPath IconPath)
{
	m_IconPath = IconPath;
}

/* EXTERNAL TEXT BUTTON ********************************************************/

CExternalTextButton::CExternalTextButton(class CConfig *pConfig, const char* pText, CAssetPath IconPath) :
	CAbstractButton(pConfig),
	m_pText(pText),
	m_Centered(true)
{
	SetIcon(IconPath);
	
	m_Rect.w = m_pConfig->m_ButtonHeight;
	m_Rect.h = m_pConfig->m_ButtonHeight;
}

void CExternalTextButton::Render()
{
	CAbstractButton::Render();
	
	if(!m_pText)
		return;
		
	int TextWidth = TextRender()->TextWidth(0, m_pConfig->m_TextSize[TEXTSTYLE_NORMAL], m_pText, -1);
	
	int PosX;
	if(m_Centered)
	{
		int CenterX = m_Rect.x + m_Rect.w/2;
		
		if(!m_IconPath.IsNull())
			CenterX += m_pConfig->m_IconSize/2;
		
		PosX = CenterX - TextWidth/2;
	}
	else
	{
		PosX = m_Rect.x + m_pConfig->m_LabelMargin;
		
		if(!m_IconPath.IsNull())
			PosX += m_pConfig->m_IconSize;
	}
	
	int CenterY = m_Rect.y + m_Rect.h/2;
	int PosY = CenterY - m_pConfig->m_TextSize[TEXTSTYLE_NORMAL]*0.7;
	
	CTextCursor Cursor;
	TextRender()->SetCursor(&Cursor, PosX, PosY, m_pConfig->m_TextSize[TEXTSTYLE_NORMAL], TEXTFLAG_RENDER);
	TextRender()->TextColor(m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].x, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].y, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].z, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].w);
	TextRender()->TextEx(&Cursor, m_pText, -1);
	
	if(!m_IconPath.IsNull())
	{		
		TUGraphics()->DrawSprite(
			m_IconPath,
			vec2(PosX-m_pConfig->m_IconSize/2-4, CenterY),
			1.0f, 0.0f, 0x0, 1.0f
		);
	}
}

void CExternalTextButton::SetIcon(CAssetPath IconPath)
{
	m_IconPath = IconPath;
}

}

}
