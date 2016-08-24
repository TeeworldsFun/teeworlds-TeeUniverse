#ifndef TU_CLIENT_GUI_FLOATEDIT_H
#define TU_CLIENT_GUI_FLOATEDIT_H

#include "widget.h"
#include "button.h"

namespace tu
{
	
namespace gui
{

class CAbstractFloatEdit : public CWidget
{
protected:
	bool RefreshText;
	float m_LastValue;
	char m_aFloatText[64];
	int m_CursorCharPos;
	bool m_UnderMouse;
	bool m_Clicked;
	
protected:
	void ApplyTextEntry();
	
	virtual void SetValue(float v) = 0;
	virtual float GetValue() = 0;
	virtual void ApplyFormat();
	
public:
	CAbstractFloatEdit(class CConfig *pConfig);
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnInputEvent();
};

class CFloatEdit : public CAbstractFloatEdit
{
protected:
	float m_Value;
	
	virtual void SetValue(float v);
	virtual float GetValue();
	
public:
	CFloatEdit(class CConfig *pConfig, float DefaultValue);
};

class CExternalFloatEdit : public CAbstractFloatEdit
{
protected:
	float* m_Memory;
	
	virtual void SetValue(float v);
	virtual float GetValue();
	
public:
	CExternalFloatEdit(class CConfig *pConfig, float* m_Memory);
};

}

}

#endif
