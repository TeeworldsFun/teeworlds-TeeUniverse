#ifndef TU_CLIENT_ASSETSEDITOR_VIEWMAP_H
#define TU_CLIENT_ASSETSEDITOR_VIEWMAP_H

#include <tu/client/gui/listlayout.h>
#include <tu/client/assetseditor/view.h>

namespace tu
{

namespace assetseditor
{

class CViewMap : public CViewManager::CView
{
public:
	class CCursorTool : public CViewManager::CView::CCursorTool
	{
	protected:
		CViewMap* m_pViewMap;
	
	public:
		CCursorTool(CViewMap* pView, CAssetPath IconPath);
		
		inline CViewMap* ViewMap() { return m_pViewMap; }
	};
	
protected:
	CAssetPath GetMapPath();
	float m_CameraZoom;
	vec2 m_CameraPos;
	bool m_CameraDraged;

public:
	CViewMap(CAssetsEditor* pAssetsEditor);
	virtual void RenderView();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
};

}

}

#endif
