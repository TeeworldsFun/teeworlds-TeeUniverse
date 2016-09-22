#ifndef TU_CLIENT_GUI_BUTTON_H
#define TU_CLIENT_GUI_BUTTON_H

#include "label.h"

namespace tu
{
	
namespace gui
{

class CAbstractButton : public CAbstractLabel
{
protected:
	bool m_Clicked;
	
protected:
	CAbstractButton(class CContext *pConfig);
	
	virtual void MouseClickAction() = 0;
	
public:
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
};

class CAbstractToggle : public CAbstractButton
{
protected:
	bool m_Toggle;
	
protected:
	CAbstractToggle(class CContext *pConfig);
	
	virtual void MouseClickAction();
	virtual void OnToggle(bool Toggle) = 0;
	
public:
};

class CButton : public CAbstractButton
{
public:
	CButton(class CContext *pConfig, const char* pText);
	CButton(class CContext *pConfig, CAssetPath IconPath);
	CButton(class CContext *pConfig, const char* pText, CAssetPath IconPath);
};

class CToggle : public CAbstractToggle
{
public:
	CToggle(class CContext *pConfig, const char* pText);
	CToggle(class CContext *pConfig, CAssetPath IconPath);
	CToggle(class CContext *pConfig, const char* pText, CAssetPath IconPath);
};

}

}

#endif
