#ifndef TU_CLIENT_GUI_BUTTON_H
#define TU_CLIENT_GUI_BUTTON_H

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CAbstractButton : public CWidget
{
protected:
	bool m_UnderMouse;
	bool m_Clicked;
	int m_ButtonStyle;
	
protected:
	CAbstractButton(class CConfig *pConfig);
	
	virtual void MouseClickAction() = 0;
	
public:
	virtual void Render();
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button);
	virtual void OnButtonRelease(int Button);
	
	void SetButtonStyle(int Style);
};

class CIconButton : public CAbstractButton
{
protected:
	int m_IconId;
	
protected:
	CIconButton(class CConfig *pConfig, int IconId);
	
public:
	virtual void Render();
	
	void SetIcon(int IconId);
};

class CTextButton : public CAbstractButton
{
protected:
	char m_aText[128];
	int m_IconId;
	bool m_Centered;
	
protected:
	CTextButton(class CConfig *pConfig, const char* pText = 0, int IconId = -1);
	
	virtual void MouseClickAction() = 0;
	
public:
	virtual void Render();
	
	void SetText(const char* pText);
	void SetIcon(int IconId);
};

class CExternalTextButton : public CAbstractButton
{
protected:
	const char* m_pText;
	int m_IconId;
	bool m_Centered;
	
protected:
	CExternalTextButton(class CConfig *pConfig, const char* pText, int IconId = -1);
	
	virtual void MouseClickAction() = 0;
	
public:
	virtual void Render();
	
	void SetIcon(int IconId);
};

}

}

#endif
