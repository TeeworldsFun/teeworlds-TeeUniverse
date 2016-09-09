#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/input.h>
#include <engine/keys.h>
#include <game/client/render.h>
#include <game/client/lineinput.h>

#include "integer-edit.h"

namespace tu
{
	
namespace gui
{

CAbstractIntegerEdit::CIncreaseButton::CIncreaseButton(CAbstractIntegerEdit *pIntegerEdit) :
	CIconButton(pIntegerEdit->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_INCREASE)),
	m_pIntegerEdit(pIntegerEdit)
{
	
}

void CAbstractIntegerEdit::CIncreaseButton::MouseClickAction()
{
	m_pIntegerEdit->SetValue(m_pIntegerEdit->GetValue()+1);
}

CAbstractIntegerEdit::CDecreaseButton::CDecreaseButton(CAbstractIntegerEdit *pIntegerEdit) :
	CIconButton(pIntegerEdit->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_DECREASE)),
	m_pIntegerEdit(pIntegerEdit)
{
	
}

void CAbstractIntegerEdit::CDecreaseButton::MouseClickAction()
{
	m_pIntegerEdit->SetValue(m_pIntegerEdit->GetValue()-1);
}

CAbstractIntegerEdit::CAbstractIntegerEdit(CConfig *pConfig) :
	CWidget(pConfig),
	m_pIncreaseButton(0),
	m_pDecreaseButton(0),
	m_CursorCharPos(-1),
	m_UnderMouse(false),
	m_Clicked(false)
{
	m_Rect.h = m_pConfig->m_ButtonHeight;
	
	m_pIncreaseButton = new CAbstractIntegerEdit::CIncreaseButton(this);
	m_pDecreaseButton = new CAbstractIntegerEdit::CDecreaseButton(this);
	
	m_IntegerRect.h = m_Rect.h;
}

CAbstractIntegerEdit::~CAbstractIntegerEdit()
{
	delete m_pIncreaseButton;
	delete m_pDecreaseButton;
}

void CAbstractIntegerEdit::Update()
{
	m_pIncreaseButton->SetRect(CRect(
		m_Rect.x + m_Rect.w - m_pIncreaseButton->m_Rect.w,
		m_Rect.y,
		m_pIncreaseButton->m_Rect.w,
		m_pIncreaseButton->m_Rect.h
	));
	
	m_pDecreaseButton->SetRect(CRect(
		m_Rect.x + m_Rect.w - m_pDecreaseButton->m_Rect.w - m_pIncreaseButton->m_Rect.w - 4.0f,
		m_Rect.y,
		m_pDecreaseButton->m_Rect.w,
		m_pDecreaseButton->m_Rect.h
	));
	
	m_IntegerRect.x = m_Rect.x;
	m_IntegerRect.y = m_Rect.y;
	m_IntegerRect.w = (m_pDecreaseButton->m_Rect.x - 4.0f) - m_Rect.x;
	m_IntegerRect.h = m_Rect.h;
}
	
void CAbstractIntegerEdit::Render()
{
	//Integer
	{
		if(m_UnderMouse)
			TUGraphics()->DrawGuiRect(&m_IntegerRect, CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_ENTRY_DEFAULT_MOUSEOVER));
		else
			TUGraphics()->DrawGuiRect(&m_IntegerRect, CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_ENTRY_DEFAULT));
		
		int Value = GetValue();
		if(m_LastValue != Value)
		{
			str_format(m_aIntegerText, sizeof(m_aIntegerText), "%d", GetValue());
			m_LastValue = Value;
		}
		
		int TextSize = m_pConfig->m_TextSize[TEXTSTYLE_NORMAL];
		
		int ContentX = m_IntegerRect.x + m_IntegerRect.w/2;
		int ContentY = m_IntegerRect.y + m_IntegerRect.h/2;
	
		int TextWidth = TextRender()->TextWidth(0, m_pConfig->m_TextSize[TEXTSTYLE_NORMAL], m_aIntegerText, -1);
		int PosX = ContentX - TextWidth/2;
		int PosY = ContentY - m_pConfig->m_TextSize[TEXTSTYLE_NORMAL]*0.7;
		
		CTextCursor Cursor;
		TextRender()->SetCursor(&Cursor, PosX, PosY, TextSize, TEXTFLAG_RENDER);
		TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
		TextRender()->TextEx(&Cursor, m_aIntegerText, -1);
	
		// render the cursor
				// cursor position
		if(m_CursorCharPos >= 0)
		{
			float CursorPos = PosX;
			CursorPos += TextRender()->TextWidth(0, TextSize, m_aIntegerText, m_CursorCharPos);
			
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
	
	m_pIncreaseButton->Render();
	m_pDecreaseButton->Render();
}

void CAbstractIntegerEdit::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1)
	{
		if(m_IntegerRect.IsInside(X, Y))
		{
			m_Clicked = true;
			
			float FontSize = m_pConfig->m_TextSize[TEXTSTYLE_NORMAL];

			int TextLength = str_length(m_aIntegerText);
			int CursorCharPos = 0;
			float CursorPos = m_Rect.x+m_pConfig->m_LabelMargin;
			float TextWidth = 0.0f;
			for(CursorCharPos = 0; CursorCharPos <= TextLength; CursorCharPos++)
			{
				float NewTextWidth = TextRender()->TextWidth(0, FontSize, m_aIntegerText, CursorCharPos);
				float CharWidth = NewTextWidth - TextWidth;
				TextWidth = NewTextWidth;
				
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
		else if(m_Clicked)
		{
			ApplyTextEntry();
		}
	}
		
	m_pIncreaseButton->OnButtonClick(X, Y, Button, Count);
	m_pDecreaseButton->OnButtonClick(X, Y, Button, Count);
}

void CAbstractIntegerEdit::OnButtonRelease(int Button)
{
	m_pIncreaseButton->OnButtonRelease(Button);
	m_pDecreaseButton->OnButtonRelease(Button);
}

void CAbstractIntegerEdit::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_IntegerRect.IsInside(X, Y))
	{
		m_UnderMouse = true;
	}
	else
	{
		m_UnderMouse = false;
	}
		
	m_pIncreaseButton->OnMouseOver(X, Y, RelX, RelY, KeyState);
	m_pDecreaseButton->OnMouseOver(X, Y, RelX, RelY, KeyState);
}

void CAbstractIntegerEdit::ApplyTextEntry()
{
	m_Clicked = false;
	m_CursorCharPos = -1;
	int Value = atoi(m_aIntegerText);
	SetValue(Value);
	
	m_LastValue = Value;
	str_format(m_aIntegerText, sizeof(m_aIntegerText), "%d", Value);
}

void CAbstractIntegerEdit::OnInputEvent()
{
	if(Input()->KeyIsPressed(KEY_KP_ENTER))
	{
		ApplyTextEntry();
	}
	else
	{
		if(m_CursorCharPos >= 0)
		{
			for(int i = 0; i < Input()->NumEvents(); i++)
			{
				int Len = str_length(m_aIntegerText);
				int NumChars = Len;
				int ReturnValue = CLineInput::Manipulate(Input()->GetEvent(i), m_aIntegerText, 64, 64, &Len, &m_CursorCharPos, &NumChars);
			}
		}
	}
}

CIntegerEdit::CIntegerEdit(CConfig *pConfig, int DefaultValue) :
	CAbstractIntegerEdit(pConfig),
	m_Value(DefaultValue)
{
	
}
	
void CIntegerEdit::SetValue(int v)
{
	m_Value = v;
}
	
int CIntegerEdit::GetValue()
{
	return m_Value;
}

CExternalIntegerEdit::CExternalIntegerEdit(CConfig *pConfig, int* Memory) :
	CAbstractIntegerEdit(pConfig),
	m_Memory(Memory)
{
	
}
	
void CExternalIntegerEdit::SetValue(int v)
{
	*m_Memory = v;
}
	
int CExternalIntegerEdit::GetValue()
{
	return *m_Memory;
}

}

}
