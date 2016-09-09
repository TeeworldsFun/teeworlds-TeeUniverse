#ifndef TU_CLIENT_GUI_TABS_H
#define TU_CLIENT_GUI_TABS_H

#include <base/tl/array.h>

#include "widget.h"

namespace tu
{
	
namespace gui
{

class CTabs : public CWidget
{
public:
	struct CTab
	{
		CAssetPath m_IconPath;
		char m_aHint[128];
		CWidget* m_pWidget;
		CRect m_Rect;
	};
	
protected:
	CRect m_ContentRect;
	array<CTab> m_Tabs;
	int m_SelectedTab;
	
public:
	CTabs(class CConfig *pConfig);
	virtual ~CTabs();
	
	virtual void Update();
	virtual void Render();
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	void AddTab(CWidget* pWidget, CAssetPath IconPath, const char* pHint);
	void Clear();
};

}

}

#endif
