#ifndef TU_CLIENT_ASSETSRENDERER_H
#define TU_CLIENT_ASSETSRENDERER_H

#include <base/tl/array.h>
#include <engine/graphics.h>
#include <engine/textrender.h>

#include <tu/client/kernel.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/gui/rect.h>

namespace tu
{

class CAssetsRenderer : public CKernel::CComponent
{
protected:
	void Draw_RoundRect_Background(float x, float y, float w, float h, float r, int Corners);
	void Draw_RoundRect_Border(float x, float y, float w, float h, float r, int Borders, int Corners);
	
public:
	CAssetsRenderer(CKernel* pKernel);
	void Init() {};
	
	void TextureSet(CAssetPath AssetPath);
	
	void DrawSprite(CAssetPath AssetPath, vec2 Pos, vec2 Size, float Angle, int FlipFlag, vec4 Color);
	void DrawText(const char *pText, vec2 Pos, vec4 Color, float Size, int Alignment);
	void DrawGuiRect(const gui::CRect* pRect, CAssetPath StylePath);
	void DrawGuiLine(ivec2 Point0, ivec2 Point1, CAssetPath StylePath, float Scale);
};

}

#endif
