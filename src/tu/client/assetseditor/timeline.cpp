#include <tu/client/assetseditor/timeline.h>
#include <tu/client/assetseditor/assetseditor.h>
#include <tu/client/assetsmanager.h>
#include <game/client/localization.h>

namespace tu
{

namespace assetseditor
{

/* TIMELINE ***********************************************************/

CTimeLine::CTimeLine(CAssetsEditor* pAssetsEditor) :
	gui::CWidget(pAssetsEditor),
	m_pAssetsEditor(pAssetsEditor)
{
	
}

void CTimeLine::UpdateBoundingSize()
{
	m_BoundingSizeRect.BSNoConstraint();
}

void CTimeLine::Render()
{
	
}

}

}
