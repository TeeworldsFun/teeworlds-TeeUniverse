#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/input.h>
#include <engine/keys.h>
#include <game/client/render.h>
#include <game/client/lineinput.h>

#include "float-edit.h"

namespace tu
{
	
namespace gui
{

CAbstractFloatEdit::CAbstractFloatEdit(CConfig *pConfig) :
	CWidget(pConfig),
	m_CursorCharPos(-1),
	m_UnderMouse(false),
	m_Clicked(false),
	RefreshText(true)
{
	m_Rect.h = m_pConfig->m_ButtonHeight;
}
	
void CAbstractFloatEdit::Render()
{
	//Float
	{
		if(m_UnderMouse)
			TUGraphics()->DrawGuiRect(&m_Rect, CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_ENTRY_DEFAULT_MOUSEOVER));
		else
			TUGraphics()->DrawGuiRect(&m_Rect, CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_ENTRY_DEFAULT));
		
		float Value = GetValue();
		if(RefreshText || m_LastValue != Value)
		{
			ApplyFormat();
			m_LastValue = Value;
			RefreshText = false;
		}
		
		int TextSize = m_pConfig->m_TextSize[TEXTSTYLE_NORMAL];
		
		int ContentX = m_Rect.x + m_Rect.w/2;
		int ContentY = m_Rect.y + m_Rect.h/2;
	
		int TextWidth = TextRender()->TextWidth(0, m_pConfig->m_TextSize[TEXTSTYLE_NORMAL], m_aFloatText, -1);
		int PosX = ContentX - TextWidth/2;
		int PosY = ContentY - m_pConfig->m_TextSize[TEXTSTYLE_NORMAL]*0.7;
		
		CTextCursor Cursor;
		TextRender()->SetCursor(&Cursor, PosX, PosY, TextSize, TEXTFLAG_RENDER);
		TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
		TextRender()->TextEx(&Cursor, m_aFloatText, -1);
	
		// render the cursor
				// cursor position
		if(m_CursorCharPos >= 0)
		{
			float CursorPos = PosX;
			CursorPos += TextRender()->TextWidth(0, TextSize, m_aFloatText, m_CursorCharPos);
			
			if((2*time_get()/time_freq()) % 2)
			{
				Graphics()->TextureClear();
				Graphics()->LinesBegin();
				Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				
				float x = CursorPos;
				float y0 = m_Rect.y + m_Rect.h/2 - TextSize*0.6;
				float y1 = m_Rect.y + m_Rect.h/2 + TextSize*0.6;
				IGraphics::CLineItem Line(x, y0, x, y1);
				Graphics()->LinesDraw(&Line, 1);
				
				Graphics()->LinesEnd();
			}
		}
	}
}

void CAbstractFloatEdit::ApplyFormat()
{
	str_format(m_aFloatText, sizeof(m_aFloatText), "%.02f", GetValue());
}

void CAbstractFloatEdit::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_Rect.IsInside(X, Y))
	{
		m_Clicked = true;
		
		float FontSize = m_pConfig->m_TextSize[TEXTSTYLE_NORMAL];

		int TextLength = str_length(m_aFloatText);
		int TextWidth = TextRender()->TextWidth(0, FontSize, m_aFloatText, -1);
		int CursorCharPos = 0;
		float CursorPos = m_Rect.x + m_Rect.w/2 - TextWidth/2;
		float TextWidthIter = 0.0f;
		for(CursorCharPos = 0; CursorCharPos <= TextLength; CursorCharPos++)
		{
			float NewTextWidth = TextRender()->TextWidth(0, FontSize, m_aFloatText, CursorCharPos);
			float CharWidth = NewTextWidth - TextWidthIter;
			TextWidthIter = NewTextWidth;
			
			if(X < CursorPos + CharWidth)
			{
				if(X > CursorPos + CharWidth/2)
				{
					CursorCharPos++;
					CursorPos += CharWidth;
				}
				break;
			}
			CursorPos += CharWidth;
		}
		
		m_CursorCharPos = CursorCharPos;
	}
	else if(m_CursorCharPos >= 0)
	{
		ApplyTextEntry();
	}
}

void CAbstractFloatEdit::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
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

void CAbstractFloatEdit::ApplyTextEntry()
{
	m_Clicked = false;
	m_CursorCharPos = -1;
	float Value = atof(m_aFloatText);
	SetValue(Value);
	
	RefreshText = true;
}

void CAbstractFloatEdit::OnInputEvent()
{
	if(m_CursorCharPos >= 0)
	{
		if(Input()->KeyIsPressed(KEY_RETURN) || Input()->KeyIsPressed(KEY_KP_ENTER))
		{
			ApplyTextEntry();
		}
		else
		{
			for(int i = 0; i < Input()->NumEvents(); i++)
			{
				int Len = str_length(m_aFloatText);
				int NumChars = Len;
				int ReturnValue = CLineInput::Manipulate(Input()->GetEvent(i), m_aFloatText, 64, 64, &Len, &m_CursorCharPos, &NumChars);
			}
		}
	}
}

CFloatEdit::CFloatEdit(CConfig *pConfig, float DefaultValue) :
	CAbstractFloatEdit(pConfig),
	m_Value(DefaultValue)
{
	
}
	
void CFloatEdit::SetValue(float v)
{
	m_Value = v;
}
	
float CFloatEdit::GetValue()
{
	return m_Value;
}

CExternalFloatEdit::CExternalFloatEdit(CConfig *pConfig, float* Memory) :
	CAbstractFloatEdit(pConfig),
	m_Memory(Memory)
{
	
}
	
void CExternalFloatEdit::SetValue(float v)
{
	*m_Memory = v;
}
	
float CExternalFloatEdit::GetValue()
{
	return *m_Memory;
}

}

}
