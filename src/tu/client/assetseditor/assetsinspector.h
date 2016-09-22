#ifndef TU_CLIENT_ASSETSINSPECTOR_H
#define TU_CLIENT_ASSETSINSPECTOR_H

#include <tu/client/gui/tabs.h>
#include <tu/client/gui/listlayout.h>

namespace tu
{

namespace assetseditor
{

class CAssetsInspector : public gui::CTabs
{
	//Search Tag: TAG_NEW_ASSET
	enum
	{
		TAB_GENERIC_ASSET=0,
		TAB_IMAGE_ASSET,
		TAB_SPRITE_ASSET,
		TAB_GUIRECTSTYLE_ASSET,
		TAB_GUILINESTYLE_ASSET,
		TAB_GUIBOXSTYLE_ASSET,
		TAB_GUISCROLLBARSTYLE_ASSET,
		TAB_GUITABSSTYLE_ASSET,
		NUM_TABS
	};

private:
	class CAssetsEditor* m_pAssetsEditor;
	gui::CVListLayout* m_pTabs[NUM_TABS];
	
private:
	void AddField(gui::CVListLayout* pList, gui::CWidget* pWidget, const char* pLabelText);
	void AddField_Text(gui::CVListLayout* pList, int Member, int TextSize, const char* pLabelText);
	void AddField_Integer(gui::CVListLayout* pList, int Member, const char* pLabelText);
	void AddField_Asset(gui::CVListLayout* pList, int Member, int AssetType, const char* pLabelText);
	void AddField_AssetProperties(gui::CVListLayout* pList);

	gui::CVListLayout* CreateTab_Generic_Asset();
	gui::CVListLayout* CreateTab_Image_Asset();
	gui::CVListLayout* CreateTab_Sprite_Asset();
	gui::CVListLayout* CreateTab_GuiRectStyle_Asset();
	gui::CVListLayout* CreateTab_GuiLineStyle_Asset();
	gui::CVListLayout* CreateTab_GuiBoxStyle_Asset();
	gui::CVListLayout* CreateTab_GuiScrollbarStyle_Asset();
	gui::CVListLayout* CreateTab_GuiTabsStyle_Asset();

public:
	CAssetsInspector(CAssetsEditor* pAssetsEditor);
	
	virtual void Update();
	
	class CAssetsEditor* AssetsEditor() { return m_pAssetsEditor; }
};

}

}

#endif
