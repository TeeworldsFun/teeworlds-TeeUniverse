#ifndef TU_CLIENT_GUI_TOGGLE_H
#define TU_CLIENT_GUI_TOGGLE_H

#include "label.h"

namespace tu
{
	
namespace gui
{

class CAbstractToggle : public CAbstractLabel
{
protected:
	bool m_MouseOver;
	bool m_Clicked;
	CAssetPath m_ToggleStylePath;
	
protected:
	CAbstractToggle(class CContext *pConfig);
	
	void RefreshLabelStyle();
	
public:
	virtual void Update();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);

	virtual bool GetValue() const = 0;
	virtual void SetValue(bool Value) = 0;
	
	void SetToggleStyle(CAssetPath StylePath);
	inline CAssetPath GetToggleStyle() const { return m_ToggleStylePath; }
};

class CToggle : public CAbstractToggle
{
public:
	CToggle(class CContext *pConfig, const char* pText);
	CToggle(class CContext *pConfig, CAssetPath IconPath);
	CToggle(class CContext *pConfig, const CLocalizableString& LocalizableString);
	CToggle(class CContext *pConfig, const char* pText, CAssetPath IconPath);
	CToggle(class CContext *pConfig, const CLocalizableString& LocalizableString, CAssetPath IconPath);
};

}

}

#endif
