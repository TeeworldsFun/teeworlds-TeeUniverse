#ifndef TU_CLIENT_GUI_INTEGEREDIT_H
#define TU_CLIENT_GUI_INTEGEREDIT_H

#include "listlayout.h"
#include "text-edit.h"
#include "button.h"

namespace tu
{
	
namespace gui
{

class CAbstractIntegerEdit : public CHListLayout
{
	class CEntry : public CAbstractTextEdit
	{
	protected:
		CAbstractIntegerEdit* m_pIntegerEdit;
		virtual void SaveFromTextBuffer();
		virtual void CopyToTextBuffer();
		
	public:
		CEntry(CAbstractIntegerEdit *pIntegerEdit);
	};
	
	class CDecreaseButton : public CButton
	{
	protected:
		CAbstractIntegerEdit* m_pIntegerEdit;
		virtual void MouseClickAction();
		
	public:
		CDecreaseButton(CAbstractIntegerEdit *pIntegerEdit);
	};
	
	class CIncreaseButton : public CButton
	{
	protected:
		CAbstractIntegerEdit* m_pIntegerEdit;
		virtual void MouseClickAction();
		
	public:
		CIncreaseButton(CAbstractIntegerEdit *pIntegerEdit);
	};
	
protected:
	CEntry* m_pEntry;

protected:
	virtual int GetValue() const = 0;
	virtual void SetValue(int Value) = 0;
	
public:
	CAbstractIntegerEdit(class CContext *pConfig);
};

class CIntegerEdit : public CAbstractIntegerEdit
{
protected:
	int m_Value;

protected:
	virtual int GetValue() const;
	virtual void SetValue(int Value);
	
public:
	CIntegerEdit(class CContext *pConfig, int DefaultValue);
};

class CExternalIntegerEdit : public CAbstractIntegerEdit
{
protected:
	int* m_Memory;

protected:
	virtual int GetValue() const;
	virtual void SetValue(int Value);
	
public:
	CExternalIntegerEdit(class CContext *pConfig, int* m_Memory);
};

}

}

#endif
