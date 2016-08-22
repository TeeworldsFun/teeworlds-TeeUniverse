#include <modapi/client/graphics.h>
#include <engine/storage.h>
#include <modapi/shared/assetsfile.h>
#include <generated/client_data.h>
#include <game/client/render.h>
#include <engine/shared/datafile.h>
#include <engine/shared/config.h>

#include <game/gamecore.h>

#include <engine/external/pnglite/pnglite.h>

namespace tu
{
	
CClient_Graphics::CClient_Graphics(IGraphics* pGraphics, CAssetManager* pAssetManager)
{
	m_pGraphics = pGraphics;
	m_pAssetManager = pAssetManager;
}

bool CClient_Graphics::TextureSet(CAssetPath AssetPath)
{
	const CAsset_Image* pImage = AssetManager()->GetAsset<CAsset_Image>(AssetPath);
	if(pImage == 0)
		return false;
	
	m_pGraphics->TextureSet(pImage->m_Texture);
	
	return true;
}

void CClient_Graphics::DrawSprite(CAssetPath SpritePath, vec2 Pos, float Size, float Angle, int Flags)
{
	DrawSprite(SpritePath, Pos, Size, Angle, Flags, vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void CClient_Graphics::DrawSprite(CAssetPath SpritePath, vec2 Pos, float Size, float Angle, int Flags, vec4 Color)
{
	DrawSprite(SpritePath, Pos, vec2(Size, Size), Angle, Flags, Color);
}

void CClient_Graphics::DrawSprite(CAssetPath SpritePath, vec2 Pos, vec2 Size, float Angle, int Flags, vec4 Color)
{
	//Get sprite
	const CAsset_Sprite* pSprite = AssetManager()->GetAsset<CAsset_Sprite>(SpritePath);
	if(pSprite == 0)
		return;
	
	//Get texture
	const CAsset_Image* pImage = AssetManager()->GetAsset<CAsset_Image>(pSprite->m_ImagePath);
	if(pImage == 0)
		return;

	m_pGraphics->BlendNormal();
	m_pGraphics->TextureSet(pImage->m_Texture);
	m_pGraphics->QuadsBegin();
	m_pGraphics->SetColor(Color.r*Color.a, Color.g*Color.a, Color.b*Color.a, Color.a);
	
	//Compute texture subquad
	float texX0 = pSprite->m_X/(float)max(1, pImage->m_GridWidth);
	float texX1 = (pSprite->m_X + pSprite->m_Width - 1.0f/32.0f)/(float)max(1, pImage->m_GridWidth);
	float texY0 = pSprite->m_Y/(float)max(1, pImage->m_GridHeight);
	float texY1 = (pSprite->m_Y + pSprite->m_Height - 1.0f/32.0f)/(float)max(1, pImage->m_GridHeight);
	
	float Temp = 0;
	if(Flags&CAsset_Sprite::FLAG_FLIP_Y)
	{
		Temp = texY0;
		texY0 = texY1;
		texY1 = Temp;
	}

	if(Flags&CAsset_Sprite::FLAG_FLIP_X)
	{
		Temp = texX0;
		texX0 = texX1;
		texX1 = Temp;
	}

	m_pGraphics->QuadsSetSubset(texX0, texY0, texX1, texY1);
	
	m_pGraphics->QuadsSetRotation(Angle);

	//Compute sprite size
	float ratio = sqrtf(pSprite->m_Width * pSprite->m_Width + pSprite->m_Height * pSprite->m_Height);
	
	if(Flags & CAsset_Sprite::FLAG_SIZE_HEIGHT)
		ratio = pSprite->m_Height;
		
	vec2 QuadSize = vec2(pSprite->m_Width/ratio, pSprite->m_Height/ratio) * Size;
	
	//Draw
	IGraphics::CQuadItem QuadItem(Pos.x, Pos.y, QuadSize.x, QuadSize.y);
	m_pGraphics->QuadsDraw(&QuadItem, 1);
	
	m_pGraphics->QuadsEnd();
}

void CClient_Graphics::DrawText(ITextRender* pTextRender, const char *pText, vec2 Pos, vec4 Color, float Size, int Alignment)
{	
	float width = pTextRender->TextWidth(0, Size, pText, -1);
	float height = pTextRender->TextHeight(Size);
	
	switch(Alignment)
	{
		case tu::TEXTALIGN_CENTER:
			Pos.x -= width/2;
			Pos.y -= height/2;
			break;
		case tu::TEXTALIGN_RIGHT_BOTTOM:
			break;
		case tu::TEXTALIGN_RIGHT_CENTER:
			Pos.y -= height/2;
			break;
		case tu::TEXTALIGN_RIGHT_TOP:
			Pos.y -= height;
			break;
		case tu::TEXTALIGN_CENTER_TOP:
			Pos.x -= width/2;
			Pos.y -= height;
			break;
		case tu::TEXTALIGN_LEFT_TOP:
			Pos.x -= width;
			Pos.y -= height;
			break;
		case tu::TEXTALIGN_LEFT_CENTER:
			Pos.x -= width;
			Pos.y -= height/2;
			break;
		case tu::TEXTALIGN_LEFT_BOTTOM:
			Pos.x -= width;
			break;
		case tu::TEXTALIGN_CENTER_BOTTOM:
			Pos.y -= height/2;
			break;
	}
	
	pTextRender->TextColor(Color.r,Color.g,Color.b,Color.a);
	pTextRender->Text(0, Pos.x, Pos.y, Size, pText, -1);
	
	//reset color
	pTextRender->TextColor(255,255,255,1);
}

}
