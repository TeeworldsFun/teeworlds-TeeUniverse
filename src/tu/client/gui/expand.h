#ifndef TU_CLIENT_GUI_EXPAND_H
#define TU_CLIENT_GUI_EXPAND_H

#include <base/tl/array.h>

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CExpand : public CWidget
{	
protected:
	CAssetPath m_BoxStylePath;
	CWidget* m_pTitle;
	array<CWidget*> m_Childs;
	bool m_Expanded;

	inline int GetShift() { return Context()->ApplyGuiScale(20); }

public:
	CExpand(class CContext *pConfig);
	virtual ~CExpand();
	
	void Clear();
	void Add(CWidget* pWidget);
	void SetTitle(CWidget* pWidget);
	inline void ShowContent() { m_Expanded = true; }
	inline void HideContent() { m_Expanded = false; }
	
	virtual void UpdateBoundingSize();
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void Update();
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	void SetBoxStyle(CAssetPath BoxStylePath) { m_BoxStylePath = BoxStylePath; }
	CAssetPath GetBoxStyle() const { return m_BoxStylePath; }
};

}

}

#endif
