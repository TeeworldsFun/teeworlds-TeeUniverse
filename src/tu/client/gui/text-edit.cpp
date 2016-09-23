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
	SetBoxStyle(Context()->GetTextEntryStyle());
}

void CAbstractTextEdit::Update()
{
	if(!m_Changes)
		CopyToTextBuffer();
}
	
void CAbstractTextEdit::Render()
{
	CAbstractLabel::Render();
	
	if(m_Composing)
	{
		int FontSize = GetFontSize();
		vec4 FontColor = 1.0f;
		
		CRect ComposingRect = CRect(
			GetTextRect().x + GetTextRect().w,
			GetTextRect().y,
			TextRenderer()->GetTextWidth(&m_ComposingTextCache),
			GetTextRect().h
		);
		
		const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_COMPOSING));
		if(pBoxStyle)
		{
			FontSize = Context()->ApplyGuiScale(pBoxStyle->GetFontSize());
			FontColor = pBoxStyle->m_TextColor;
			
			CRect Rect = ComposingRect;
			int Padding = Context()->ApplyGuiScale(pBoxStyle->GetPadding());
			Rect.AddMargin(Padding);
			ComposingRect.x += Padding;
			
			ivec2 MousePos = Context()->GetMousePos();
			if(Rect.IsInside(MousePos.x, MousePos.y))
				AssetsRenderer()->DrawGuiRect(&ComposingRect, pBoxStyle->GetMouseOverRectPath());
			else
				AssetsRenderer()->DrawGuiRect(&ComposingRect, pBoxStyle->GetDefaultRectPath());
		}
		m_ComposingTextCache.SetFontSize(GetFontSize());
		m_ComposingTextCache.SetBoxSize(ivec2(-1, ComposingRect.h));
		TextRenderer()->DrawText(&m_ComposingTextCache, ivec2(ComposingRect.x, ComposingRect.y), FontColor);
	}
	
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
		if(!m_Focus)
		{
			OnFocusIn();
			m_Focus = true;
		}
		
		m_TextCursor = TextRenderer()->GetTextCursorFromPosition(&m_TextCache, GetTextPosition(), ivec2(X, Y));
	}
	else if(m_Focus)
	{
		SaveFromTextBuffer();
		m_Changes = false;
		
		m_TextCursor.m_TextIter = -1;
		m_Focus = false;
		OnFocusOut();
	}
}

void CAbstractTextEdit::OnInputEvent()
{
	m_Composing = false;
	if(m_Focus)
	{
		const char* m_pEditedText = Input()->GetEditedText();
		if(m_pEditedText && str_length(m_pEditedText))
		{
			m_ComposingTextCache.SetText(m_pEditedText);
			m_Composing = true;
		}
	}
	
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
			m_TextCursor = TextRenderer()->GetTextCursorFromTextIter(&m_TextCache, GetTextPosition(), TextIter);
			m_Changes = true;
		}
	}
}

void CAbstractTextEdit::RemoveChanges()
{
	m_Changes = false;
}

void CAbstractTextEdit::OnFocusIn()
{
	Input()->StartTextEditing(GetTextRect().x, GetTextRect().y, GetTextRect().w, GetTextRect().h);
}

void CAbstractTextEdit::OnFocusOut()
{
	Input()->StopTextEditing();
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
