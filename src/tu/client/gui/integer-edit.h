#ifndef TU_CLIENT_GUI_INTEGEREDIT_H
#define TU_CLIENT_GUI_INTEGEREDIT_H

#include "widget.h"
#include "button.h"

namespace tu
{
	
namespace gui
{

class CAbstractIntegerEdit : public CWidget
{
	class CIncreaseButton : public CIconButton
	{
	protected:
		CAbstractIntegerEdit* m_pIntegerEdit;
		
		virtual void MouseClickAction();
		
	public:
		CIncreaseButton(CAbstractIntegerEdit *pIntegerEdit);
	};
	
	class CDecreaseButton : public CIconButton
	{
	protected:
		CAbstractIntegerEdit* m_pIntegerEdit;
		
		virtual void MouseClickAction();
		
	public:
		CDecreaseButton(CAbstractIntegerEdit *pIntegerEdit);
	};
	
protected:	
	CRect m_IntegerRect;
	
	int m_LastValue;
	char m_aIntegerText[64];
	int m_CursorCharPos;
	bool m_UnderMouse;
	bool m_Clicked;
	
	CIncreaseButton* m_pIncreaseButton;
	CDecreaseButton* m_pDecreaseButton;
	
protected:
	void ApplyTextEntry();
	
	virtual void SetValue(int v) = 0;
	virtual int GetValue() = 0;
	
public:
	CAbstractIntegerEdit(class CConfig *pConfig);
	virtual ~CAbstractIntegerEdit();
	virtual void Update();
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
};

class CIntegerEdit : public CAbstractIntegerEdit
{
protected:
	int m_Value;
	
	virtual void SetValue(int v);
	virtual int GetValue();
	
public:
	CIntegerEdit(class CConfig *pConfig, int DefaultValue);
};

class CExternalIntegerEdit : public CAbstractIntegerEdit
{
protected:
	int* m_Memory;
	
	virtual void SetValue(int v);
	virtual int GetValue();
	
public:
	CExternalIntegerEdit(class CConfig *pConfig, int* m_Memory);
};

}

}

#endif
