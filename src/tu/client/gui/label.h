#ifndef TU_CLIENT_GUI_LABEL_H
#define TU_CLIENT_GUI_LABEL_H

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CLabel : public CWidget
{
protected:
	int m_TextStyle;
	char m_aText[128];
	
	int m_TextWidth;
	int m_TextHeight;
	
public:
	CLabel(class CConfig *pConfig, const char* pText, int Style = TEXTSTYLE_NORMAL);
	virtual void Render();
	
	void SetText(const char* pText);
};

}

}

#endif
