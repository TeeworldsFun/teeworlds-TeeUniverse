#include <tu/client/assetseditor/view_gui.h>
#include <tu/client/assetseditor/assetseditor.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/assetsrenderer.h>
#include <engine/graphics.h>

namespace tu
{

namespace assetseditor
{

/* VIEW GUI ***********************************************************/

CViewGui::CViewGui(CAssetsEditor* pAssetsEditor) :
	CViewManager::CView(pAssetsEditor)
{

}

void CViewGui::RenderView()
{
	switch(AssetsEditor()->GetEditedAssetPath().GetType())
	{
		case CAssetPath::TYPE_GUIRECTSTYLE:
		{
			gui::CRect Rect(
				m_ViewRect.x + m_ViewRect.w/2 - 150,
				m_ViewRect.y + m_ViewRect.h/2 - 100,
				300,
				200
			);
			AssetsRenderer()->DrawGuiRect(&Rect, AssetsEditor()->GetEditedAssetPath());
			
			Rect.AddMargin(25);
			AssetsRenderer()->DrawGuiRect(&Rect, CAssetPath::GuiRectStyleSystem(GUIRECTSTYLE_STAMP));
			break;
		}
	}
}

}

}
