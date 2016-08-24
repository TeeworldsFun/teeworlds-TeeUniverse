#ifndef TU_CLIENT_GUI_SLIDER_H
#define TU_CLIENT_GUI_SLIDER_H

#include <base/tl/array.h>

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CAbstractSlider : public CWidget
{
protected:
	float m_Pos;

	CRect m_RailRect;
	CRect m_SliderRect;
	bool m_UnderMouse;
	bool m_ButtonDown;
	
	virtual void OnNewPosition(float Pos) = 0;

public:
	CAbstractSlider(class CConfig *pConfig);
	
	virtual void Render();
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonRelease(int Button);
	
	virtual void SetSliderSize(int Size) = 0;
	virtual float GetSliderPos();
	virtual void SetSliderPos(float Pos, bool Trigger = true);
};

class CHSlider : public CAbstractSlider
{
public:
	CHSlider(class CConfig *pConfig);
	
	virtual void Update();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	
	virtual void SetSliderSize(int Size);
};

class CVSlider : public CAbstractSlider
{
public:
	CVSlider(class CConfig *pConfig);
	
	virtual void Update();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	
	virtual void SetSliderSize(int Size);
};

}

}

#endif
