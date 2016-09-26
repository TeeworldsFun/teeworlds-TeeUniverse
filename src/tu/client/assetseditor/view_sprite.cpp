#include <tu/client/assetseditor/view_sprite.h>
#include <tu/client/assetseditor/assetseditor.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/assetsrenderer.h>
#include <engine/graphics.h>

namespace tu
{

namespace assetseditor
{

/* VIEW SPRITE ********************************************************/

CViewSprite::CViewSprite(CAssetsEditor* pAssetsEditor) :
	CViewManager::CView(pAssetsEditor)
{

}

void CViewSprite::RenderView()
{
	const CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(AssetsEditor()->GetEditedAssetPath());
	if(pSprite)
		return;
	
	vec2 Pos;
	Pos.x = m_ViewRect.x + m_ViewRect.w/2;
	Pos.y = m_ViewRect.y + m_ViewRect.h/2;
	
	AssetsRenderer()->DrawSprite(AssetsEditor()->GetEditedAssetPath(), Pos, 1.0f, 0.0f, 0x0, 1.0f);
}

}

}
