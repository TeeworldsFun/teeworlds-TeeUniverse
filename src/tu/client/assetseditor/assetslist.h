#ifndef TU_CLIENT_ASSETSLIST_H
#define TU_CLIENT_ASSETSLIST_H

#include <tu/client/gui/tabs.h>
#include <tu/client/gui/listlayout.h>

namespace tu
{

namespace assetseditor
{

class CAssetsSource : public gui::CVScrollLayout
{
private:
	class CAssetsEditor* m_pAssetsEditor;
	gui::CVScrollLayout* m_pAssetsList;
	int m_Source;
	
public:
	CAssetsSource(CAssetsEditor* pAssetsEditor, int Source);
	
	void RefreshAssetsList();
	void OnAssetsListModified();
	
	class CAssetsEditor* AssetsEditor() { return m_pAssetsEditor; }
};

class CAssetsOrganizer : public gui::CTabs
{
private:
	class CAssetsEditor* m_pAssetsEditor;
	CAssetsSource* m_apAssetsSources[CAssetPath::NUM_SOURCES];
	
public:
	CAssetsOrganizer(CAssetsEditor* pAssetsEditor);
	
	void OnAssetsListModified();
	
	class CAssetsEditor* AssetsEditor() { return m_pAssetsEditor; }
};

}

}

#endif
