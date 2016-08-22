#ifndef TU_CLIENT_GUI_WIDGET_H
#define TU_CLIENT_GUI_WIDGET_H

#include "config.h"

#include <engine/keys.h>

enum
{
	TU_INPUT_CTRL=0x1,
	TU_INPUT_SHIFT=0x2,
	TU_INPUT_ALT=0x4,
};

namespace tu
{
	
namespace gui
{

class CRect
{
public:
	int x;
	int y;
	int w;
	int h;
	
public:
	CRect();
	CRect(int X, int Y, int W, int H);
	
	void CenterIn(const CRect& Rect);
	bool IsInside(int X, int Y);
};

class CWidget
{
protected:
	static float s_LayoutOpacity;
	static float s_LayoutCornerRadius;
	
	static int s_ButtonInnerMargin;
	static float s_ButtonOpacity;
	static float s_ButtonCornerRadius;

public:
	class CConfig *m_pConfig;

public:
	CRect m_Rect;

protected:
	CWidget(class CConfig *pConfig);
	
	class IGraphics *Graphics() { return m_pConfig->m_pGraphics; };
	class ITextRender *TextRender() { return m_pConfig->m_pTextRender; };
	class CRenderTools *RenderTools() { return m_pConfig->m_pRenderTools; };
	class IInput *Input() { return m_pConfig->m_pInput; };
	
public:
	virtual ~CWidget();
	
	virtual void Update();
	virtual void Render() = 0;
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	void SetRect(const CRect& Rect);
	void SetX(int y);
	void SetY(int y);
	void SetWidth(int w);
	void SetHeight(int w);

	inline void ShowHint(const char* pText)
	{
		if(m_pConfig->m_fShowHint)
			m_pConfig->m_fShowHint(pText, m_pConfig->m_pShowHintData);
	}
};

}

}

#endif
