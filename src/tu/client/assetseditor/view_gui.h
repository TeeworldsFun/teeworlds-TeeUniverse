#ifndef TU_CLIENT_ASSETSEDITOR_VIEWGUI_H
#define TU_CLIENT_ASSETSEDITOR_VIEWGUI_H

#include <tu/client/assetseditor/view.h>

namespace tu
{

namespace assetseditor
{

class CViewGui : public CViewManager::CView
{
public:
	CViewGui(CAssetsEditor* pAssetsEditor);
	virtual void RenderView();
};

}

}

#endif
