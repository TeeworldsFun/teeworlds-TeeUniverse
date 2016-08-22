#ifndef TU_CLIENT_GRAPHICS_H
#define TU_CLIENT_GRAPHICS_H

#include <base/tl/array.h>
#include <engine/graphics.h>
#include <engine/textrender.h>

#include <modapi/graphics.h>
#include <modapi/client/assetsmanager.h>

class CRenderTools;

namespace tu
{
	
class IAssetsFile;

class CSkeletonState
{
	class CBoneState
	{
		
	};
};

class CClient_Graphics
{
private:
	IGraphics* m_pGraphics;
	CAssetManager* m_pAssetManager;

public:
	CClient_Graphics(IGraphics* pGraphics, CAssetManager* pAssetManager);
	void Init();
	
	IGraphics *Graphics() { return m_pGraphics; };
	CAssetManager *AssetManager() { return m_pAssetManager; };
	
	bool TextureSet(CAssetPath AssetPath);
	
	void DrawSprite(CAssetPath AssetPath, vec2 Pos, vec2 Size, float Angle, int FlipFlag, vec4 Color);
	void DrawSprite(CAssetPath AssetPath, vec2 Pos, float Size, float Angle, int FlipFlag, vec4 Color);
	void DrawSprite(CAssetPath AssetPath, vec2 Pos, float Size, float Angle, int FlipFlag);
	void DrawText(ITextRender* pTextRender, const char *pText, vec2 Pos, vec4 Color, float Size, int Alignment);
	
	void GetTeeAlignAxis(int Align, vec2 Dir, vec2 Aim, vec2& PartDirX, vec2& PartDirY);
};

}

#endif
