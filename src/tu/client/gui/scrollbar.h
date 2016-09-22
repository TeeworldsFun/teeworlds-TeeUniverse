#ifndef TU_CLIENT_GUI_SCROLLBAR_H
#define TU_CLIENT_GUI_SCROLLBAR_H

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CAbstractScrollBar : public CWidget
{
protected:
	int m_ViewSize;
	int m_ContentSize;
	float m_ContentPos;
	
	bool m_Clicked;
	int m_ClickShift;
	CAssetPath m_ScrollbarStylePath;
	CRect m_SliderRect;
	CRect m_ClipRect;

public:
	CAbstractScrollBar(class CContext *pContext);
	
	virtual void UpdateBoundingSize();
	virtual void SetContentInformations(int ContentSize, int ViewSize);
	
	virtual void OnButtonRelease(int Button);
	
	void OnWheelUp();
	void OnWheelDown();
	int GetContentPos() const;
	
	void SetScrollbarStyle(CAssetPath ScrollbarStylePath) { m_ScrollbarStylePath = ScrollbarStylePath; }
	CAssetPath GetScrollbarStyle() const { return m_ScrollbarStylePath; }
};

class CHScrollBar : public CAbstractScrollBar
{
public:
	CHScrollBar(class CContext *pContext);
	
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void Render();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
};

class CVScrollBar : public CAbstractScrollBar
{
public:
	CVScrollBar(class CContext *pContext);
	
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void Render();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
};

}

}

#endif
