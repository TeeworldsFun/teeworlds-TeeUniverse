#ifndef TU_CLIENT_GUI_TABS_H
#define TU_CLIENT_GUI_TABS_H

#include <base/tl/array.h>

#include "listlayout.h"
#include "button.h"

namespace tu
{
	
namespace gui
{

class CTabs : public CWidget
{
private:
	class CTabButton : public CButton
	{
	protected:
		CTabs* m_pTabs;
		int m_Id;
		virtual void MouseClickAction();
		
	public:
		CTabButton(CContext *pContext, CTabs *m_pTabs, int Id, const char* pName, CAssetPath IconPath, bool Localized);
	};
	
	struct CTab
	{
		CAssetPath m_IconPath;
		CWidget* m_pWidget;
		CTabButton* m_pTabButton;
		char m_aName[128];
		bool m_Localized;
		bool m_Disabled;
	};
	
protected:
	CAssetPath m_TabsStylePath;
	CRect m_ClipRect;
	array<CTab> m_Tabs;
	int m_SelectedTab;
	CHListLayout* m_pButtonList;
	
protected:
	void RegenerateButtons();
	void AddTab(CWidget* pWidget, const char* pName, CAssetPath IconPath, bool Localized);
	
public:
	CTabs(class CContext *pConfig);
	virtual ~CTabs();
	
	virtual void UpdateBoundingSize();
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void Update();
	virtual void Render();
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	void AddTab(CWidget* pWidget, const char* pName, CAssetPath IconPath);
	void AddTab(CWidget* pWidget, const CLocalizableString& LocalizableString, CAssetPath IconPath);
	void Clear();
	void OpenTab(int Id);
	
	void SetTabsStyle(CAssetPath TabsStylePath) { m_TabsStylePath = TabsStylePath; }
	CAssetPath GetTabsStyle() const { return m_TabsStylePath; }
};

}

}

#endif
