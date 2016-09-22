#ifndef TU_CLIENT_ASSETSEDITOR_TIMELINE_H
#define TU_CLIENT_ASSETSEDITOR_TIMELINE_H

#include <tu/client/gui/widget.h>

namespace tu
{

namespace assetseditor
{

class CTimeLine : public gui::CWidget
{
private:
	class CAssetsEditor* m_pAssetsEditor;
	
public:
	CTimeLine(CAssetsEditor* pAssetsEditor);
	
	virtual void UpdateBoundingSize();
	virtual void Render();
	
	class CAssetsEditor* AssetsEditor() { return m_pAssetsEditor; }
};

}

}

#endif
