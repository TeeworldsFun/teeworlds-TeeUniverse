#ifndef TU_CLIENT_GUI_LAYOUT_H
#define TU_CLIENT_GUI_LAYOUT_H

#include <base/tl/array.h>

#include "widget.h"
#include "slider.h"

namespace tu
{
	
namespace gui
{

enum
{
	LAYOUTSTYLE_NONE=0,
	LAYOUTSTYLE_FRAME,
};

enum
{
	LAYOUTFILLING_NONE=0,
	LAYOUTFILLING_FIRST,
	LAYOUTFILLING_LAST,
	LAYOUTFILLING_ALL,
};

class CHListLayout : public CWidget
{
protected:
	class CSeparator : public CWidget
	{
	public:
		CSeparator(CConfig* pConfig);
		virtual void Render();
	};
	
protected:
	int m_Style;
	int m_Model;
	
	array<CWidget*> m_Childs;

protected:
	void Update_FillingNone();
	void Update_FillingFirst();
	void Update_FillingLast();
	void Update_FillingAll();

public:
	CHListLayout(class CConfig *pConfig, int Style = LAYOUTSTYLE_FRAME, int Model = LAYOUTFILLING_NONE);
	virtual ~CHListLayout();
	
	virtual void Clear();
	virtual void Add(CWidget* pWidget);
	virtual void AddSeparator();
	
	virtual void Update();
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
};

class CVListLayout : public CWidget
{
protected:
	class CSlider : public CVSlider
	{
	protected:
		CVListLayout* m_pLayout;
		
	protected:
		virtual void OnNewPosition(float Pos);
		
	public:
		CSlider(CVListLayout* pLayout);
	};
	
	class CSeparator : public CWidget
	{
	public:
		CSeparator(CConfig* pConfig);
		virtual void Render();
	};
	
protected:	
	CVSlider* m_pSlider;
	array<CWidget*> m_Childs;
	int m_ChildrenHeight;
	int m_Style;
	
	bool m_ShowScrollBar;
	int m_ScrollValue;
	
public:
	CVListLayout(class CConfig *pConfig, int Style = LAYOUTSTYLE_FRAME);
	virtual ~CVListLayout();
	
	virtual void Clear();
	virtual void Add(CWidget* pWidget);
	virtual void AddSeparator();
	
	virtual void Update();
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	virtual void OnNewScrollPos(float Pos);
	virtual void SetScrollPos(float Pos);
	virtual float GetScrollPos();
};

}

}

#endif
