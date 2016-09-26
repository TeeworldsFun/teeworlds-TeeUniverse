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
	bool m_MouseOver;
	bool m_Clicked;
	CAssetPath m_ButtonStylePath;
	
protected:
	CAbstractButton(class CContext *pConfig);
	
	virtual void MouseClickAction() = 0;
	
public:
	virtual void Update();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	
	void RefreshLabelStyle();
	inline CAssetPath GetButtonStyle() const { return m_ButtonStylePath; }
	void SetButtonStyle(CAssetPath Path);
};

class CButton : public CAbstractButton
{
public:
	CButton(class CContext *pConfig, const char* pText);
	CButton(class CContext *pConfig, CAssetPath IconPath);
	CButton(class CContext *pConfig, const char* pText, CAssetPath IconPath);
};

}

}

#endif
