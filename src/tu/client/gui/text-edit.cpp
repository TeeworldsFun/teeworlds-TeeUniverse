#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/input.h>
#include <game/client/render.h>
#include <game/client/lineinput.h>

#include "text-edit.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT TEXT EDIT *************************************************/

CAbstractTextEdit::CAbstractTextEdit(CContext *pConfig) :
	CAbstractLabel(pConfig),
	m_Focus(false),
	m_Changes(false)
{
	m_TextCursor.m_TextIter = -1;
	SetBoxStyle(Context()->GetEntryStyle());
}

void CAbstractTextEdit::Update()
{
	if(!m_Changes)
		CopyToTextBuffer();
}
	
void CAbstractTextEdit::Render()
{
	CAbstractLabel::Render();
	
	// render the cursor
			// cursor position
	if(m_TextCursor.m_TextIter >= 0)
	{
		if((2*time_get()/time_freq()) % 2)
		{
			Graphics()->TextureClear();
			Graphics()->LinesBegin();
			Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			
			float x = m_TextCursor.m_Position.x + 0.5f;
			float y0 = GetTextRect().y;
			float y1 = GetTextRect().y + GetTextRect().h;
			IGraphics::CLineItem Line(x, y0, x, y1);
			Graphics()->LinesDraw(&Line, 1);
			
			Graphics()->LinesEnd();
		}
	}
}

void CAbstractTextEdit::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(GetTextRect().IsInside(X, Y))
		Context()->SetCursorSprite(CAssetPath::SpriteSystem(SPRITE_CURSOR_TEXT));
}

void CAbstractTextEdit::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_DrawRect.IsInside(X, Y))
	{
		m_Focus = true;
		
		m_TextCursor = TextRenderer()->GetTextCursorFromPosition(GetRendererText(), ivec2(X, Y), GetTextRect(), GetFontSize(), &m_TextCache);
	}
	else if(m_Focus)
	{
		SaveFromTextBuffer();
		m_Changes = false;
		
		m_TextCursor.m_TextIter = -1;
		m_Focus = false;
	}
}

void CAbstractTextEdit::OnInputEvent()
{
	if(Input()->KeyIsPressed(KEY_RETURN) || Input()->KeyIsPressed(KEY_KP_ENTER))
	{
		if(m_Changes)
		{
			SaveFromTextBuffer();
			m_Changes = false;
		}
	}
	else if(m_TextCursor.m_TextIter >= 0)
	{
		int TextIter = m_TextCursor.m_TextIter;
		bool Changes = false;
		for(int i = 0; i < Input()->NumEvents(); i++)
		{
			int Len = str_length(GetText());
			int NumChars = Len;
			if(CLineInput::Manipulate(Input()->GetEvent(i), m_aText, sizeof(m_aText), sizeof(m_aText), &Len, &TextIter, &NumChars))
				Changes = true;
		}
		
		if(Changes)
		{
			OnTextUpdated();
			m_TextCursor = TextRenderer()->GetTextCursorFromTextIter(GetRendererText(), TextIter, GetTextRect(), GetFontSize(), &m_TextCache);
			m_Changes = true;
		}
	}
}

void CAbstractTextEdit::RemoveChanges()
{
	m_Changes = false;
}

/* EXTERNAL TEXT EDIT *************************************************/

CExternalTextEdit::CExternalTextEdit(CContext *pConfig, char* pText, int TextSize) :
	CAbstractTextEdit(pConfig),
	m_pText(pText),
	m_TextSize(TextSize)
{
	
}

void CExternalTextEdit::SaveFromTextBuffer()
{
	str_copy(m_pText, GetText(), m_TextSize);
}

void CExternalTextEdit::CopyToTextBuffer()
{
	SetText(m_pText);
}

}

}
