#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/input.h>
#include <engine/keys.h>
#include <game/client/render.h>
#include <game/client/lineinput.h>

#include "float-edit.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT FLOAT EDIT ************************************************/

CAbstractFloatEdit::CAbstractFloatEdit(CContext *pConfig) :
	CAbstractTextEdit(pConfig)
{
	
}

void CAbstractFloatEdit::SaveFromTextBuffer()
{
	SetValue(atof(GetText()));
}

void CAbstractFloatEdit::CopyToTextBuffer()
{
	str_format(m_aText, sizeof(m_aText), "%.02f", GetValue());
	OnTextUpdated();
}

/* FLOAT EDIT *********************************************************/

CFloatEdit::CFloatEdit(CContext *pConfig, float DefaultValue) :
	CAbstractFloatEdit(pConfig),
	m_FloatValue(DefaultValue)
{
	
}

float CFloatEdit::GetValue() const
{
	return m_FloatValue;
}

void CFloatEdit::SetValue(float Value)
{
	m_FloatValue = Value;
}

/* EXTERNAL FLOAT EDIT ************************************************/

CExternalFloatEdit::CExternalFloatEdit(CContext *pConfig, float* pFloat) :
	CAbstractFloatEdit(pConfig),
	m_pFloat(pFloat)
{
	
}

float CExternalFloatEdit::GetValue() const
{
	return *m_pFloat;
}

void CExternalFloatEdit::SetValue(float Value)
{
	*m_pFloat = Value;
}

}

}
