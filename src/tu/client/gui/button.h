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
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	
	void SetButtonStyle(int Style);
};

class CIconButton : public CAbstractButton
{
protected:
	CAssetPath m_IconPath;
	
protected:
	CIconButton(class CConfig *pConfig, CAssetPath IconPath);
	
public:
	virtual void Render();
	
	void SetIcon(CAssetPath IconPath);
};

class CIconToggle : public CIconButton
{
protected:
	bool m_Toggle;
	
protected:
	CIconToggle(class CConfig *pConfig, CAssetPath IconPath);
	
public:
	virtual void MouseClickAction();
	virtual void OnToggle(bool Toggle) = 0;
};

class CTextButton : public CAbstractButton
{
protected:
	char m_aText[128];
	CAssetPath m_IconPath;
	bool m_Centered;
	
protected:
	CTextButton(class CConfig *pConfig, const char* pText = 0, CAssetPath IconPath = CAssetPath::Null());
	
	virtual void MouseClickAction() = 0;
	
public:
	virtual void Render();
	
	void SetText(const char* pText);
	void SetIcon(CAssetPath IconPath);
};

class CExternalTextButton : public CAbstractButton
{
protected:
	CAssetPath m_IconPath;
	bool m_Centered;

public:
	const char* m_pText;
	
protected:
	CExternalTextButton(class CConfig *pConfig, const char* pText, CAssetPath IconPath = CAssetPath::Null());
	
	virtual void MouseClickAction() = 0;
	
public:
	virtual void Render();
	
	void SetIcon(CAssetPath IconPath);
};

}

}

#endif
