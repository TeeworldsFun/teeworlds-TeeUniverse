#ifndef TU_CLIENT_GUI_LISTLAYOUT_H
#define TU_CLIENT_GUI_LISTLAYOUT_H

#include <base/tl/array.h>

#include "widget.h"
#include "scrollbar.h"

namespace tu
{
	
namespace gui
{

enum
{
	LAYOUTFILLING_NONE=0,
	LAYOUTFILLING_FIRST,
	LAYOUTFILLING_LAST,
	LAYOUTFILLING_ALL,
};

class CAbstractListLayout : public CWidget
{
protected:
	class CChild
	{
	public:
		CWidget* m_pWidget;
		bool m_Fill;
	};
	
protected:
	CAssetPath m_BoxStylePath;
	array<CChild> m_Childs;
	CRect m_ClipRect;

public:
	CAbstractListLayout(class CContext *pConfig);
	virtual ~CAbstractListLayout();
	
	virtual void Update();
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	void Clear();
	void Add(CWidget* pWidget, bool Fill = false);
	
	void SetBoxStyle(CAssetPath BoxStylePath) { m_BoxStylePath = BoxStylePath; }
	CAssetPath GetBoxStyle() const { return m_BoxStylePath; }
};

class CHListLayout : public CAbstractListLayout
{
protected:
	class CSeparator : public CWidget
	{
	public:
		CSeparator(CContext* pConfig);
		virtual void UpdateBoundingSize();
		virtual void Render();
	};
	
public:
	CHListLayout(class CContext *pConfig);
	
	virtual void UpdateBoundingSize();
	virtual void UpdatePosition(CRect BoundingRect);
	
	void AddSeparator();
};

class CVListLayout : public CAbstractListLayout
{
protected:	
	class CSeparator : public CWidget
	{
	public:
		CSeparator(CContext* pConfig);
		virtual void UpdateBoundingSize();
		virtual void UpdatePosition(CRect BoundingRect);
		virtual void Render();
	};
	
protected:	
	bool m_ShowScrollBar;
	CVScrollBar* m_pScrollBar;
	int m_ChildrenLength;
	
public:
	CVListLayout(class CContext *pConfig);
	virtual ~CVListLayout();
	
	virtual void AddSeparator();
	
	virtual void UpdateBoundingSize();
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void Update();
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
};

}

}

#endif
