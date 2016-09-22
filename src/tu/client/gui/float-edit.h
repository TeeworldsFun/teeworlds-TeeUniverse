#ifndef TU_CLIENT_GUI_FLOATEDIT_H
#define TU_CLIENT_GUI_FLOATEDIT_H

#include "text-edit.h"

namespace tu
{
	
namespace gui
{

class CAbstractFloatEdit : public CAbstractTextEdit
{
protected:
	float m_FloatValue;
	
	virtual void SaveFromTextBuffer();
	virtual void CopyToTextBuffer();
	
	virtual float GetValue() const = 0;
	virtual void SetValue(float Value) = 0;
	
public:
	CAbstractFloatEdit(class CContext *pConfig);
};

class CFloatEdit : public CAbstractFloatEdit
{
protected:
	float m_FloatValue;
	
	virtual float GetValue() const;
	virtual void SetValue(float Value);
	
public:
	CFloatEdit(class CContext *pConfig, float DefaultValue);
};

class CExternalFloatEdit : public CAbstractFloatEdit
{
protected:
	float* m_pFloat;
	
	virtual float GetValue() const;
	virtual void SetValue(float Value);
	
public:
	CExternalFloatEdit(class CContext *pConfig, float* m_pFloat);
};

}

}

#endif
