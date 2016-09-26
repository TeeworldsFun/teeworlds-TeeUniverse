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
		TAB_GUILABELSTYLE_ASSET,
		TAB_GUIBUTTONSTYLE_ASSET,
		TAB_GUITOGGLESTYLE_ASSET,
		TAB_GUISCROLLBARSTYLE_ASSET,
		TAB_GUITABSSTYLE_ASSET,
		NUM_TABS
	};

private:
	class CAssetsEditor* m_pAssetsEditor;
	gui::CVScrollLayout* m_pTabs[NUM_TABS];
	
private:
	void AddField(gui::CVScrollLayout* pList, gui::CWidget* pWidget, const char* pLabelText);
	void AddField_Text(gui::CVScrollLayout* pList, int Member, int TextSize, const char* pLabelText);
	void AddField_Integer(gui::CVScrollLayout* pList, int Member, const char* pLabelText);
	void AddField_Asset(gui::CVScrollLayout* pList, int Member, int AssetType, const char* pLabelText);
	void AddField_AssetProperties(gui::CVScrollLayout* pList);

	gui::CVScrollLayout* CreateTab_Generic_Asset();
	gui::CVScrollLayout* CreateTab_Image_Asset();
	gui::CVScrollLayout* CreateTab_Sprite_Asset();
	gui::CVScrollLayout* CreateTab_GuiRectStyle_Asset();
	gui::CVScrollLayout* CreateTab_GuiLineStyle_Asset();
	gui::CVScrollLayout* CreateTab_GuiBoxStyle_Asset();
	gui::CVScrollLayout* CreateTab_GuiLabelStyle_Asset();
	gui::CVScrollLayout* CreateTab_GuiButtonStyle_Asset();
	gui::CVScrollLayout* CreateTab_GuiToggleStyle_Asset();
	gui::CVScrollLayout* CreateTab_GuiScrollbarStyle_Asset();
	gui::CVScrollLayout* CreateTab_GuiTabsStyle_Asset();

public:
	CAssetsInspector(CAssetsEditor* pAssetsEditor);
	
	virtual void Update();
	
	class CAssetsEditor* AssetsEditor() { return m_pAssetsEditor; }
};

}

}

#endif
