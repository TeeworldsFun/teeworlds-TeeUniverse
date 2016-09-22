#ifndef TU_CLIENT_GUI_PANELLAYOUT_H
#define TU_CLIENT_GUI_PANELLAYOUT_H

#include <base/tl/array.h>

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CAbstractPanelLayout : public CWidget
{
protected:
	class CChild
	{
	public:
		CWidget* m_pWidget;
		bool m_Fill;
		int m_Size;
		CRect m_SliderRect;
		bool m_SliderClicked;
	};
	
protected:
	CAssetPath m_BoxStylePath;
	array<CChild> m_Childs;
	CRect m_ClipRect;
	
public:
	CAbstractPanelLayout(class CContext *pConfig);
	virtual ~CAbstractPanelLayout();
	
	void Clear();
	void Add(CWidget* pWidget, int Size);
	
	virtual void Update();
	virtual void Render();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	inline void SetBoxStyle(CAssetPath BoxStylePath) { m_BoxStylePath = BoxStylePath; }
	inline CAssetPath GetBoxStyle() const { return m_BoxStylePath; }
};

class CHPanelLayout : public CAbstractPanelLayout
{
public:
	CHPanelLayout(class CContext *pConfig);
	
	virtual void UpdateBoundingSize();
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
};

class CVPanelLayout : public CAbstractPanelLayout
{
public:
	CVPanelLayout(class CContext *pConfig);
	
	virtual void UpdateBoundingSize();
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
};

}

}

#endif
