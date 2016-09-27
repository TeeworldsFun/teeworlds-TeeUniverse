#ifndef TU_CLIENT_GUI_SLIDER_H
#define TU_CLIENT_GUI_SLIDER_H

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CAbstractSlider : public CWidget
{
protected:	
	bool m_Clicked;
	int m_ClickShift;
	CAssetPath m_SliderStylePath;
	CRect m_ClipRect;
	CRect m_CursorRect;
	
	int m_Min;
	int m_Max;

	virtual int GetValue() const = 0;
	virtual void SetValue(int Value) = 0;
	
public:
	CAbstractSlider(class CContext *pContext, int Min, int Max);
	
	virtual void UpdateBoundingSize();
	
	virtual void OnButtonRelease(int Button);
	
	int GetContentPos() const;
	
	void SetSliderStyle(CAssetPath StylePath) { m_SliderStylePath = StylePath; }
	CAssetPath GetSliderStyle() const { return m_SliderStylePath; }
};

class CHSlider : public CAbstractSlider
{
public:
	CHSlider(class CContext *pContext, int Min, int Max);
	
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void Render();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
};

}

}

#endif
