#ifndef TU_CLIENT_ASSETSEDITOR_VIEWSPRITE_H
#define TU_CLIENT_ASSETSEDITOR_VIEWSPRITE_H

#include <tu/client/gui/listlayout.h>
#include <tu/client/assetseditor/view.h>

namespace tu
{

namespace assetseditor
{

class CViewSprite : public CViewManager::CView
{
public:
	CViewSprite(CAssetsEditor* pAssetsEditor);
	virtual void RenderView();
};

}

}

#endif
