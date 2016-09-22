#ifndef TU_CLIENT_GUI_POPUP_H
#define TU_CLIENT_GUI_POPUP_H

#include <base/tl/array.h>

#include "widget.h"
#include "listlayout.h"
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
		ALIGNMENT_SIDE,
		ALIGNMENT_INNER,
	};
	
protected:
	int m_ChildWidth;
	int m_ChildHeight;
	CWidget* m_pChild;
	bool m_IsClosed;
	CRect m_CreatorRect;
	CAssetPath m_BoxStylePath;
	int m_Width;
	int m_Height;
	int m_Alignment;

	CPopup(class CContext *pConfig, const CRect& CreatorRect, int Width, int Height, int Alignment);

public:
	virtual ~CPopup();
	
	virtual void UpdateBoundingSize();
	virtual void UpdatePosition(CRect BoundingRect);
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
	
	inline void SetBoxStyle(CAssetPath BoxStylePath) { m_BoxStylePath = BoxStylePath; }
	inline CAssetPath GetBoxStyle() const { return m_BoxStylePath; }
};

}

}

#endif
