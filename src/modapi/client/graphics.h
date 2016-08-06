#ifndef MODAPI_CLIENT_GRAPHICS_H
#define MODAPI_CLIENT_GRAPHICS_H

#include <base/tl/array.h>
#include <engine/graphics.h>
#include <engine/textrender.h>

#include <modapi/graphics.h>
#include <modapi/client/assetsmanager.h>

class IModAPI_AssetsFile;
class CRenderTools;

class CModAPI_SkeletonState
{
	class CBoneState
	{
		
	};
};

class CModAPI_Client_Graphics
{
private:
	IGraphics* m_pGraphics;
	CModAPI_AssetManager* m_pAssetManager;

public:
	CModAPI_Client_Graphics(IGraphics* pGraphics, CModAPI_AssetManager* pAssetManager);
	void Init();
	
	IGraphics *Graphics() { return m_pGraphics; };
	CModAPI_AssetManager *AssetManager() { return m_pAssetManager; };
	
	bool TextureSet(CModAPI_AssetPath AssetPath);
	
	void DrawSprite(CModAPI_AssetPath AssetPath, vec2 Pos, vec2 Size, float Angle, int FlipFlag, vec4 Color);
	void DrawSprite(CModAPI_AssetPath AssetPath, vec2 Pos, float Size, float Angle, int FlipFlag, vec4 Color);
	void DrawSprite(CModAPI_AssetPath AssetPath, vec2 Pos, float Size, float Angle, int FlipFlag);
	void DrawText(ITextRender* pTextRender, const char *pText, vec2 Pos, vec4 Color, float Size, int Alignment);
	
	void GetTeeAlignAxis(int Align, vec2 Dir, vec2 Aim, vec2& PartDirX, vec2& PartDirY);
};

#endif
