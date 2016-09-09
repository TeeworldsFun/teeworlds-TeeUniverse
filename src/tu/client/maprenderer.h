#ifndef TU_CLIENT_MAPRENDERER_H
#define TU_CLIENT_MAPRENDERER_H

#include <tu/client/assetsmanager.h>

namespace tu
{

class CMapRenderer
{
public:
	class IGraphics* m_pGraphics;
	class CGraphics* m_pTUGraphics;
	CAssetsManager* m_pAssetsManager;
	
	vec2 m_GroupPos;
	vec2 m_GroupHardParallax;
	
	vec2 m_ScreenPos;
	vec2 m_CameraPos;
	vec2 m_GroupCameraPos;
	float m_CameraZoom;
	float m_Time;
	float m_LocalTime;
	
public:	
	CMapRenderer(class CGraphics* pTUGraphics, CAssetsManager* pAssetsManager);
	
	void SetTime(float Time);
	void SetLocalTime(float Time);
	void SetCanvas(vec2 ScreenPos);
	void SetCamera(vec2 CameraPos, float CameraZoom);
	void SetGroup(CAssetPath GroupPath);
	void UnsetGroup();
	
	vec2 MapPosToScreenPos(vec2 MapPos) const;
	vec2 MapPosToTilePos(vec2 MapPos) const;
	vec2 ScreenPosToMapPos(vec2 MapPos) const;
	vec2 ScreenPosToTilePos(vec2 MapPos) const;
	vec2 TilePosToMapPos(vec2 MapPos) const;
	vec2 TilePosToScreenPos(vec2 MapPos) const;
	
	void RenderGrid(int Width, int Height, float Scale);
	void RenderGrid_LayerTiles(CAssetPath LayerPath);
	void RenderTiles_Image(const CAsset_MapLayerTiles::CTile* pTiles, int Width, int Height, vec2 Pos, CAssetPath ImagePath, vec4 Color);
	void RenderTiles_Zone(CAssetPath ZoneTypePath, const CAsset_MapZoneTiles::CTile* pTiles, int Width, int Height, vec2 Pos, IGraphics::CTextureHandle& TextureId);
	void RenderTiles_Zone(CAssetPath ZoneTypePath, const CAsset_MapLayerTiles::CTile* pTiles, int Width, int Height, vec2 Pos, IGraphics::CTextureHandle& TextureId);
	void RenderQuads(const CAsset_MapLayerQuads::CQuad* pQuads, int NbQuads, vec2 Pos, CAssetPath ImagePath, vec4 Color);
	void RenderQuads_Mesh(const CAsset_MapLayerQuads::CQuad* pQuads, int NbQuads);
	
	void RenderGroup(CAssetPath GroupPath);
	void RenderMap(CAssetPath MapPath);
	void RenderSource(int Source);
	
	void RenderMap_Zones(CAssetPath MapPath, IGraphics::CTextureHandle& TextureId);
	void RenderSource_Zones(int Source, IGraphics::CTextureHandle& TextureId);
	
	class IGraphics* Graphics() { return m_pGraphics; }
	class CGraphics* TUGraphics() { return m_pTUGraphics; }
	CAssetsManager* AssetsManager() { return m_pAssetsManager; }
};

}

#endif
