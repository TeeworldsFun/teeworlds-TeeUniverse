#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/input.h>
#include <engine/keys.h>
#include <game/client/render.h>
#include <game/client/lineinput.h>

#include "integer-edit.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT INTEGER EDIT **********************************************/

	//Entry
CAbstractIntegerEdit::CEntry::CEntry(CAbstractIntegerEdit *pIntegerEdit) :
	CAbstractTextEdit(pIntegerEdit->Context()),
	m_pIntegerEdit(pIntegerEdit)
{
	
}

void CAbstractIntegerEdit::CEntry::SaveFromTextBuffer()
{
	m_pIntegerEdit->SetValue(atoi(GetText()));
}

void CAbstractIntegerEdit::CEntry::CopyToTextBuffer()
{
	str_format(m_aText, sizeof(m_aText), "%d", m_pIntegerEdit->GetValue());
	OnTextUpdated();
}

	//Decrease Button
CAbstractIntegerEdit::CDecreaseButton::CDecreaseButton(CAbstractIntegerEdit *pIntegerEdit) :
	CButton(pIntegerEdit->Context(), CAssetPath::SpriteSystem(SPRITE_ICON_DECREASE)),
	m_pIntegerEdit(pIntegerEdit)
{
	
}

void CAbstractIntegerEdit::CDecreaseButton::MouseClickAction()
{
	m_pIntegerEdit->SetValue(m_pIntegerEdit->GetValue()-1);
	m_pIntegerEdit->m_pEntry->RemoveChanges();
}

	//Increase Button
CAbstractIntegerEdit::CIncreaseButton::CIncreaseButton(CAbstractIntegerEdit *pIntegerEdit) :
	CButton(pIntegerEdit->Context(), CAssetPath::SpriteSystem(SPRITE_ICON_INCREASE)),
	m_pIntegerEdit(pIntegerEdit)
{
	
}

void CAbstractIntegerEdit::CIncreaseButton::MouseClickAction()
{
	m_pIntegerEdit->SetValue(m_pIntegerEdit->GetValue()+1);
	m_pIntegerEdit->m_pEntry->RemoveChanges();
}

	//Integer Edit
CAbstractIntegerEdit::CAbstractIntegerEdit(CContext *pContext) :
	CHListLayout(pContext),
	m_pEntry(0)
{
	m_pEntry = new CAbstractIntegerEdit::CEntry(this);
	Add(m_pEntry, true);
	Add(new CAbstractIntegerEdit::CDecreaseButton(this), false);
	Add(new CAbstractIntegerEdit::CIncreaseButton(this), false);
}

/* INTEGER EDIT *******************************************************/

CIntegerEdit::CIntegerEdit(CContext *pConfig, int DefaultValue) :
	CAbstractIntegerEdit(pConfig),
	m_Value(DefaultValue)
{
	
}
	
void CIntegerEdit::SetValue(int Value)
{
	m_Value = Value;
}
	
int CIntegerEdit::GetValue() const
{
	return m_Value;
}

/* EXTERNAL INTEGER EDIT **********************************************/

CExternalIntegerEdit::CExternalIntegerEdit(CContext *pContext, int* Memory) :
	CAbstractIntegerEdit(pContext),
	m_Memory(Memory)
{
	
}
	
void CExternalIntegerEdit::SetValue(int v)
{
	*m_Memory = v;
}
	
int CExternalIntegerEdit::GetValue() const
{
	return *m_Memory;
}

}

}
