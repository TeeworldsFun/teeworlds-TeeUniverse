#ifndef TU_CLIENT_GUI_POPUP_H
#define TU_CLIENT_GUI_POPUP_H

#include <base/tl/array.h>

#include "widget.h"
#include "layout.h"
#include "button.h"

namespace tu
{
	
namespace gui
{

class CPopup : public CWidget
{
public:
	enum
	{
		ALIGNMENT_LEFT,
		ALIGNMENT_RIGHT,
		ALIGNMENT_INNER,
	};
	
protected:
	int m_ChildWidth;
	int m_ChildHeight;
	CWidget* m_Child;
	bool m_IsClosed;

	CPopup(class CConfig *pConfig, const CRect& CreatorRect, int Width, int Height, int Alignment);

public:
	virtual ~CPopup();
	
	virtual void Update();
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	virtual void Add(CWidget* pWidget);
	virtual void Clear();
	
	virtual void Close();
	virtual bool IsClosed();
};

}

}

#endif
