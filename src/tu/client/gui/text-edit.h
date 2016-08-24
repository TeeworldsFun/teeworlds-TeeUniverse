#ifndef TU_CLIENT_GUI_TEXTEDIT_H
#define TU_CLIENT_GUI_TEXTEDIT_H

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CAbstractTextEdit : public CWidget
{
protected:
	int m_TextStyle;
	int m_TextMaxSize;
	
	int m_CursorCharPos;
	
	bool m_UnderMouse;
	bool m_Clicked;

protected:
	virtual char* GetTextPtr() = 0;

public:
	CAbstractTextEdit(class CConfig *pConfig, int TextMaxChar, int Style = TEXTSTYLE_NORMAL);
	
	virtual void Render();
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnInputEvent();
};

class CExternalTextEdit : public CAbstractTextEdit
{
protected:
	char* m_pText;
	
protected:
	virtual char* GetTextPtr();

public:
	CExternalTextEdit(class CConfig *pConfig, char* pText, int TextMaxChar, int Style = TEXTSTYLE_NORMAL);
};

}

}

#endif
