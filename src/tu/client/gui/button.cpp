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
		TUGraphics()->Draw_GuiRect(&m_Rect, m_pConfig->m_ButtonStyles[m_ButtonStyle].m_StylePath_UnderMouse);
	else
		TUGraphics()->Draw_GuiRect(&m_Rect, m_pConfig->m_ButtonStyles[m_ButtonStyle].m_StylePath_Normal);
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

CIconButton::CIconButton(CConfig *pConfig, int IconId) :
	CAbstractButton(pConfig),
	m_IconId(IconId)
{
	
}

void CIconButton::Render()
{
	CAbstractButton::Render();
	
	int PosX = m_Rect.x + m_Rect.w/2;
	int PosY = m_Rect.y + m_Rect.h/2;
	
	int SubX = m_IconId%16;
	int SubY = m_IconId/16;
	
	Graphics()->TextureSet(m_pConfig->m_Texture);
	Graphics()->QuadsBegin();
	Graphics()->QuadsSetSubset(SubX/16.0f, SubY/16.0f, (SubX+1)/16.0f, (SubY+1)/16.0f);
	IGraphics::CQuadItem QuadItem(PosX - m_pConfig->m_IconSize/2, PosY - m_pConfig->m_IconSize/2, m_pConfig->m_IconSize, m_pConfig->m_IconSize);
	Graphics()->QuadsDrawTL(&QuadItem, 1);
	Graphics()->QuadsEnd();
}

void CIconButton::SetIcon(int IconId)
{
	m_IconId = IconId;
}

/* TEXT BUTTON ********************************************************/

CTextButton::CTextButton(CConfig *pConfig, const char* pText, int IconId) :
	CAbstractButton(pConfig)
{
	SetText(pText);
	SetIcon(IconId);
	
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
		
		if(m_IconId >= 0)
			CenterX += m_pConfig->m_IconSize/2;
		
		PosX = CenterX - TextWidth/2;
	}
	else
	{
		PosX = m_Rect.x + m_pConfig->m_LabelMargin;
		
		if(m_IconId >= 0)
			PosX += m_pConfig->m_IconSize;
	}
	
	int CenterY = m_Rect.y + m_Rect.h/2;
	int PosY = CenterY - m_pConfig->m_TextSize[TEXTSTYLE_NORMAL]*0.7;
	
	CTextCursor Cursor;
	TextRender()->SetCursor(&Cursor, PosX, PosY, m_pConfig->m_TextSize[TEXTSTYLE_NORMAL], TEXTFLAG_RENDER);
	TextRender()->TextColor(m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].x, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].y, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].z, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].w);
	TextRender()->TextEx(&Cursor, m_aText, -1);
	
	if(m_IconId >= 0)
	{
		float IconSize = 16.0f;
		
		PosX -= m_pConfig->m_IconSize + 4; //Icon size and space
		
		int SubX = m_IconId%16;
		int SubY = m_IconId/16;
		
		Graphics()->TextureSet(m_pConfig->m_Texture);
		Graphics()->QuadsBegin();
		Graphics()->QuadsSetSubset(SubX/16.0f, SubY/16.0f, (SubX+1)/16.0f, (SubY+1)/16.0f);
		IGraphics::CQuadItem QuadItem(PosX, CenterY - m_pConfig->m_IconSize/2, m_pConfig->m_IconSize, m_pConfig->m_IconSize);
		Graphics()->QuadsDrawTL(&QuadItem, 1);
		Graphics()->QuadsEnd();
	}
}

void CTextButton::SetText(const char* pText)
{
	if(pText)
		str_copy(m_aText, pText, sizeof(m_aText));
}

void CTextButton::SetIcon(int IconId)
{
	m_IconId = IconId;
}

/* EXTERNAL TEXT BUTTON ********************************************************/

CExternalTextButton::CExternalTextButton(class CConfig *pConfig, const char* pText, int IconId) :
	CAbstractButton(pConfig),
	m_pText(pText),
	m_Centered(true)
{
	SetIcon(IconId);
	
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
		
		if(m_IconId >= 0)
			CenterX += m_pConfig->m_IconSize/2;
		
		PosX = CenterX - TextWidth/2;
	}
	else
	{
		PosX = m_Rect.x + m_pConfig->m_LabelMargin;
		
		if(m_IconId >= 0)
			PosX += m_pConfig->m_IconSize;
	}
	
	int CenterY = m_Rect.y + m_Rect.h/2;
	int PosY = CenterY - m_pConfig->m_TextSize[TEXTSTYLE_NORMAL]*0.7;
	
	CTextCursor Cursor;
	TextRender()->SetCursor(&Cursor, PosX, PosY, m_pConfig->m_TextSize[TEXTSTYLE_NORMAL], TEXTFLAG_RENDER);
	TextRender()->TextColor(m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].x, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].y, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].z, m_pConfig->m_TextColor[TEXTSTYLE_NORMAL].w);
	TextRender()->TextEx(&Cursor, m_pText, -1);
	
	if(m_IconId >= 0)
	{
		float IconSize = 16.0f;
		
		PosX -= m_pConfig->m_IconSize + 4; //Icon size and space
		
		int SubX = m_IconId%16;
		int SubY = m_IconId/16;
		
		Graphics()->TextureSet(m_pConfig->m_Texture);
		Graphics()->QuadsBegin();
		Graphics()->QuadsSetSubset(SubX/16.0f, SubY/16.0f, (SubX+1)/16.0f, (SubY+1)/16.0f);
		IGraphics::CQuadItem QuadItem(PosX, CenterY - m_pConfig->m_IconSize/2, m_pConfig->m_IconSize, m_pConfig->m_IconSize);
		Graphics()->QuadsDrawTL(&QuadItem, 1);
		Graphics()->QuadsEnd();
	}
}

void CExternalTextButton::SetIcon(int IconId)
{
	m_IconId = IconId;
}

}

}
