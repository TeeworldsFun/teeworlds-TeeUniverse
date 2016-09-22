#ifndef TU_CLIENT_ASSETSEDITOR_VIEWIMAGE_H
#define TU_CLIENT_ASSETSEDITOR_VIEWIMAGE_H

#include <tu/client/gui/listlayout.h>
#include <tu/client/assetseditor/view.h>

namespace tu
{

namespace assetseditor
{

class CViewImage : public CViewManager::CView
{
public:
	CViewImage(CAssetsEditor* pAssetsEditor);
	virtual void RenderView();
};

}

}

#endif
