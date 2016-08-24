#ifndef TU_CLIENT_GRAPHICS_H
#define TU_CLIENT_GRAPHICS_H

#include <base/tl/array.h>
#include <engine/graphics.h>
#include <engine/textrender.h>

#include <tu/graphics.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/gui/widget.h>

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
	CAssetsManager* m_pAssetsManager;

public:
	CClient_Graphics(IGraphics* pGraphics, CAssetsManager* pAssetsManager);
	void Init();
	
	IGraphics *Graphics() { return m_pGraphics; };
	CAssetsManager *AssetsManager() { return m_pAssetsManager; };
	
	bool TextureSet(CAssetPath AssetPath);
	
	void DrawSprite(CAssetPath AssetPath, vec2 Pos, vec2 Size, float Angle, int FlipFlag, vec4 Color);
	void DrawSprite(CAssetPath AssetPath, vec2 Pos, float Size, float Angle, int FlipFlag, vec4 Color);
	void DrawSprite(CAssetPath AssetPath, vec2 Pos, float Size, float Angle, int FlipFlag);
	void DrawText(ITextRender* pTextRender, const char *pText, vec2 Pos, vec4 Color, float Size, int Alignment);
	
	void GetTeeAlignAxis(int Align, vec2 Dir, vec2 Aim, vec2& PartDirX, vec2& PartDirY);
	
	void Draw_RoundRect_Background(float x, float y, float w, float h, float r, int Corners);
	void Draw_RoundRect_Border(float x, float y, float w, float h, float r, int Borders, int Corners);
	
	void Draw_GuiRect(const gui::CRect* pRect, CAssetPath StylePath);
};

}

#endif
