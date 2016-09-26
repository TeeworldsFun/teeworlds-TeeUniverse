#ifndef TU_CLIENT_ASSETSEDITOR_H
#define TU_CLIENT_ASSETSEDITOR_H

#include <tu/client/gui/context.h>
#include <tu/client/assetsmanager.h>

namespace tu
{

namespace assetseditor
{

class CAssetsEditor : public gui::CContext, CAssetsManager::IListener
{
private:
	CAssetPath m_EditedAssetPath;
	int m_EditedSubPath;
	
	class CAssetsOrganizer* m_pAssetsOrganizer;
	
public:
	CAssetsEditor(CKernel* pKernel);
	virtual ~CAssetsEditor();
	
	virtual void CreateMainWidget();
	virtual void DoShortcuts();
	virtual void RenderBackground();
	virtual void OnAssetsListModified();
	void Close();
	
	CAssetPath GetItemIcon(CAssetPath AssetPath, int SubPath);
	const char* GetItemName(CAssetPath AssetPath, int SubPath);
	
	inline void SetEditedAsset(CAssetPath Path, int SubPath)
	{
		m_EditedAssetPath = Path;
		m_EditedSubPath = SubPath;
	}
	inline CAssetPath GetEditedAssetPath() const { return m_EditedAssetPath; }
	inline int GetEditedSubPath() const { return m_EditedSubPath; }
};

}

}

#endif
