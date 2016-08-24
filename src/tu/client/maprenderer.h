#ifndef TU_CLIENT_MAPRENDERER_H
#define TU_CLIENT_MAPRENDERER_H

#include <tu/client/assetsmanager.h>

namespace tu
{

class CMapRenderer
{
public:
	class IGraphics* m_pGraphics;
	CAssetsManager* m_pAssetsManager;
	
	vec2 m_GroupPos;
	vec2 m_GroupHardParallax;
	
	vec2 m_ScreenPos;
	vec2 m_CameraPos;
	vec2 m_GroupCameraPos;
	float m_CameraZoom;
	
public:	
	CMapRenderer(class IGraphics* pGraphics, CAssetsManager* pAssetsManager) :
		m_pGraphics(pGraphics),
		m_pAssetsManager(pAssetsManager)
	{
		
	}
	
	void SetCanvas(vec2 ScreenPos);
	void SetCamera(vec2 CameraPos, float CameraZoom);
	void SetGroup(CAssetPath GroupPath);
	void UnsetGroup();
	
	vec2 MapPosToScreenPos(vec2 MapPos) const;
	vec2 ScreenPosToMapPos(vec2 MapPos) const;
	
	void RenderTileLayerGrid(CAssetPath LayerPath);
	void RenderTiles(CAsset_MapLayerTiles::CTile* pTiles, int Width, int Height, vec2 Pos, CAssetPath ImagePath, vec4 Color);
	void RenderQuads(CAsset_MapLayerQuads::CQuad* pQuads, int NbQuads, vec2 Pos, CAssetPath ImagePath);
	void RenderSource(int Source);
	
	class IGraphics* Graphics() { return m_pGraphics; }
	CAssetsManager* AssetsManager() { return m_pAssetsManager; }
};

}

#endif
