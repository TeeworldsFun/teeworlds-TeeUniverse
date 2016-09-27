#include <tu/client/assetsrenderer.h>
#include <engine/storage.h>
#include <generated/client_data.h>
#include <game/client/render.h>
#include <engine/shared/datafile.h>
#include <engine/shared/config.h>

#include <game/gamecore.h>

#include <engine/external/pnglite/pnglite.h>

namespace tu
{
	
CAssetsRenderer::CAssetsRenderer(CKernel* pKernel) :
	CKernel::CComponent(pKernel)
{
	
}

void CAssetsRenderer::TextureSet(CAssetPath AssetPath)
{
	const CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(AssetPath);
	if(pImage)
		Graphics()->TextureSet(pImage->GetTexture());
	else
		Graphics()->TextureClear();
}

void CAssetsRenderer::DrawSprite(CAssetPath SpritePath, vec2 Pos, vec2 Size, float Angle, int Flags, vec4 Color)
{
	//Get sprite
	const CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(SpritePath);
	if(pSprite == 0)
		return;
	
	//Get texture
	int ImageWidth = 1;
	int ImageHeight = 1;
	int GridWidth = 1;
	int GridHeight = 1;
	const CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pSprite->GetImagePath());
	if(pImage)
	{
		ImageWidth = pImage->GetWidth();
		ImageHeight = pImage->GetHeight();
		GridWidth = pImage->GetGridWidth();
		GridHeight = pImage->GetGridHeight();
		Graphics()->TextureSet(pImage->GetTexture());
	}
	else
	{
		Graphics()->TextureClear();
	}
	
	Graphics()->BlendNormal();
	Graphics()->QuadsBegin();
	Graphics()->SetColor(Color, true);
	
	//Compute texture subquad
	float texStepX = 1.0f/GridWidth;
	float texStepY = 1.0f/GridHeight;
	float texX0 = pSprite->GetX()*texStepX;
	float texX1 = texX0 + pSprite->GetWidth()*texStepX;
	float texY0 = pSprite->GetY()*texStepY;
	float texY1 = texY0 + pSprite->GetHeight()*texStepY;

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

	Graphics()->QuadsSetSubset(texX0, texY0, texX1, texY1);
	Graphics()->QuadsSetRotation(Angle);

	//Compute sprite size
	vec2 QuadSize = vec2(
		pSprite->GetWidth() * static_cast<float>(ImageWidth)/GridWidth,
		pSprite->GetHeight() * static_cast<float>(ImageHeight)/GridHeight
	) * Size;
	
	//Draw
	IGraphics::CQuadItem QuadItem(Pos.x, Pos.y, QuadSize.x, QuadSize.y);
	Graphics()->QuadsDraw(&QuadItem, 1);
	
	Graphics()->QuadsEnd();
}

void CAssetsRenderer::DrawText(const char *pText, vec2 Pos, vec4 Color, float Size, int Alignment)
{	
	float width = TextRender()->TextWidth(0, Size, pText, -1);
	float height = TextRender()->TextHeight(Size);
	
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
	
	TextRender()->TextColor(Color.r,Color.g,Color.b,Color.a);
	TextRender()->Text(0, Pos.x, Pos.y, Size, pText, -1);
	
	//reset color
	TextRender()->TextColor(255,255,255,1);
}

void CAssetsRenderer::Draw_RoundRect_Background(float x, float y, float w, float h, float r, int Corners)
{
	IGraphics::CFreeformItem ArrayF[32];
	int NumItems = 0;
	int Num = 8;
	for(int i = 0; i < Num; i+=2)
	{
		float a1 = i/(float)Num * pi/2;
		float a2 = (i+1)/(float)Num * pi/2;
		float a3 = (i+2)/(float)Num * pi/2;
		float Ca1 = cosf(a1);
		float Ca2 = cosf(a2);
		float Ca3 = cosf(a3);
		float Sa1 = sinf(a1);
		float Sa2 = sinf(a2);
		float Sa3 = sinf(a3);

		if(Corners&1) // TL
			ArrayF[NumItems++] = IGraphics::CFreeformItem(
			x+r, y+r,
			x+(1-Ca1)*r, y+(1-Sa1)*r,
			x+(1-Ca3)*r, y+(1-Sa3)*r,
			x+(1-Ca2)*r, y+(1-Sa2)*r);

		if(Corners&2) // TR
		ArrayF[NumItems++] = IGraphics::CFreeformItem(
			x+w-r, y+r,
			x+w-r+Ca1*r, y+(1-Sa1)*r,
			x+w-r+Ca3*r, y+(1-Sa3)*r,
			x+w-r+Ca2*r, y+(1-Sa2)*r);

		if(Corners&4) // BL
		ArrayF[NumItems++] = IGraphics::CFreeformItem(
			x+r, y+h-r,
			x+(1-Ca1)*r, y+h-r+Sa1*r,
			x+(1-Ca3)*r, y+h-r+Sa3*r,
			x+(1-Ca2)*r, y+h-r+Sa2*r);

		if(Corners&8) // BR
		ArrayF[NumItems++] = IGraphics::CFreeformItem(
			x+w-r, y+h-r,
			x+w-r+Ca1*r, y+h-r+Sa1*r,
			x+w-r+Ca3*r, y+h-r+Sa3*r,
			x+w-r+Ca2*r, y+h-r+Sa2*r);
	}
	Graphics()->QuadsDrawFreeform(ArrayF, NumItems);

	IGraphics::CQuadItem ArrayQ[9];
	NumItems = 0;
	ArrayQ[NumItems++] = IGraphics::CQuadItem(x+r, y+r, w-r*2, h-r*2); // center
	ArrayQ[NumItems++] = IGraphics::CQuadItem(x+r, y, w-r*2, r); // top
	ArrayQ[NumItems++] = IGraphics::CQuadItem(x+r, y+h-r, w-r*2, r); // bottom
	ArrayQ[NumItems++] = IGraphics::CQuadItem(x, y+r, r, h-r*2); // left
	ArrayQ[NumItems++] = IGraphics::CQuadItem(x+w-r, y+r, r, h-r*2); // right

	if(!(Corners&1)) ArrayQ[NumItems++] = IGraphics::CQuadItem(x, y, r, r); // TL
	if(!(Corners&2)) ArrayQ[NumItems++] = IGraphics::CQuadItem(x+w, y, -r, r); // TR
	if(!(Corners&4)) ArrayQ[NumItems++] = IGraphics::CQuadItem(x, y+h, r, -r); // BL
	if(!(Corners&8)) ArrayQ[NumItems++] = IGraphics::CQuadItem(x+w, y+h, -r, -r); // BR

	Graphics()->QuadsDrawTL(ArrayQ, NumItems);
}

void CAssetsRenderer::Draw_RoundRect_Border(float x, float y, float w, float h, float r, int Borders, int Corners)
{
	IGraphics::CLineItem ArrayL[64+12];
	int NumItems = 0;
	int Num = 8;
	for(int i = 0; i < Num; i+=2)
	{
		float a1 = i/(float)Num * pi/2;
		float a2 = (i+1)/(float)Num * pi/2;
		float a3 = (i+2)/(float)Num * pi/2;
		float Ca1 = cosf(a1);
		float Ca2 = cosf(a2);
		float Ca3 = cosf(a3);
		float Sa1 = sinf(a1);
		float Sa2 = sinf(a2);
		float Sa3 = sinf(a3);

		if((Corners&1) && (Borders&(1|4))) // TL
		{
			ArrayL[NumItems++] = IGraphics::CLineItem(x+(1-Ca1)*r, y+(1-Sa1)*r, x+(1-Ca2)*r, y+(1-Sa2)*r);
			ArrayL[NumItems++] = IGraphics::CLineItem(x+(1-Ca2)*r, y+(1-Sa2)*r, x+(1-Ca3)*r, y+(1-Sa3)*r);
		}
		
		if((Corners&2) && (Borders&(1|8))) // TR
		{
			ArrayL[NumItems++] = IGraphics::CLineItem(x+w-r+Ca1*r, y+(1-Sa1)*r, x+w-r+Ca2*r, y+(1-Sa2)*r);
			ArrayL[NumItems++] = IGraphics::CLineItem(x+w-r+Ca2*r, y+(1-Sa2)*r, x+w-r+Ca3*r, y+(1-Sa3)*r);
		}
		
		if((Corners&4) && (Borders&(2|4))) // BL
		{
			ArrayL[NumItems++] = IGraphics::CLineItem(x+(1-Ca1)*r, y+h-r+Sa1*r, x+(1-Ca2)*r, y+h-r+Sa2*r);
			ArrayL[NumItems++] = IGraphics::CLineItem(x+(1-Ca2)*r, y+h-r+Sa2*r, x+(1-Ca3)*r, y+h-r+Sa3*r);
		}
		
		if((Corners&8) && (Borders&(2|8))) // BR
		{
			ArrayL[NumItems++] = IGraphics::CLineItem(x+w-r+Ca1*r, y+h-r+Sa1*r, x+w-r+Ca2*r, y+h-r+Sa2*r);
			ArrayL[NumItems++] = IGraphics::CLineItem(x+w-r+Ca2*r, y+h-r+Sa2*r, x+w-r+Ca3*r, y+h-r+Sa3*r);
		}
	}

	if(Borders&1)
		ArrayL[NumItems++] = IGraphics::CLineItem(x+r, y, x+w-r, y); // top
	if(Borders&2)
		ArrayL[NumItems++] = IGraphics::CLineItem(x+r, y+h, x+w-r, y+h); // bottom
	if(Borders&4)
		ArrayL[NumItems++] = IGraphics::CLineItem(x, y+r, x, y+h-r); // left
	if(Borders&8)
		ArrayL[NumItems++] = IGraphics::CLineItem(x+w, y+r, x+w, y+h-r); // right

	if(!(Corners&1)) // TL
	{
		if(Borders&1)
			ArrayL[NumItems++] = IGraphics::CLineItem(x, y, x+r, y);
		if(Borders&4)
			ArrayL[NumItems++] = IGraphics::CLineItem(x, y, x, y+r);
	}
	if(!(Corners&2)) // TR
	{
		if(Borders&1)
			ArrayL[NumItems++] = IGraphics::CLineItem(x+w, y, x+w-r, y);
		if(Borders&8)
			ArrayL[NumItems++] = IGraphics::CLineItem(x+w, y, x+w, y+r);
	}
	if(!(Corners&4)) // BL
	{
		if(Borders&2)
			ArrayL[NumItems++] = IGraphics::CLineItem(x, y+h, x+r, y+h);
		if(Borders&4)
			ArrayL[NumItems++] = IGraphics::CLineItem(x, y+h, x, y+h-r);
	}
	if(!(Corners&8)) // BR
	{
		if(Borders&2)
			ArrayL[NumItems++] = IGraphics::CLineItem(x+w, y+h, x+w-r, y+h);
		if(Borders&8)
			ArrayL[NumItems++] = IGraphics::CLineItem(x+w, y+h, x+w, y+h-r);
	}

	Graphics()->LinesDraw(ArrayL, NumItems);
}

void CAssetsRenderer::DrawGuiRect(const gui::CRect* pRect, CAssetPath StylePath)
{
	const CAsset_GuiRectStyle* pStyle = AssetsManager()->GetAsset<CAsset_GuiRectStyle>(StylePath);
	if(!pStyle)
		return;
	
	Graphics()->BlendNormal();
	Graphics()->TextureClear();
	
	int BorderEnabled = 0;
	if(pStyle->m_BorderFlags&CAsset_GuiRectStyle::BORDERFLAG_T)
		BorderEnabled |= 1;
	if(pStyle->m_BorderFlags&CAsset_GuiRectStyle::BORDERFLAG_B)
		BorderEnabled |= 2;
	if(pStyle->m_BorderFlags&CAsset_GuiRectStyle::BORDERFLAG_L)
		BorderEnabled |= 4;
	if(pStyle->m_BorderFlags&CAsset_GuiRectStyle::BORDERFLAG_R)
		BorderEnabled |= 8;
	
	int CornerEnabled = 0;
	if(pStyle->m_CornerFlags&CAsset_GuiRectStyle::CORNERFLAG_TL)
		CornerEnabled |= 1;
	if(pStyle->m_CornerFlags&CAsset_GuiRectStyle::CORNERFLAG_TR)
		CornerEnabled |= 2;
	if(pStyle->m_CornerFlags&CAsset_GuiRectStyle::CORNERFLAG_BL)
		CornerEnabled |= 4;
	if(pStyle->m_CornerFlags&CAsset_GuiRectStyle::CORNERFLAG_BR)
		CornerEnabled |= 8;
	
	//Background
	if(pStyle->m_Flags&CAsset_GuiRectStyle::FLAG_BACKGROUND)
	{
		Graphics()->QuadsBegin();
		
		Graphics()->SetColor(pStyle->m_BackgroundColor, true);
		
		gui::CRect BGRect;
		BGRect.x = pRect->x + pStyle->m_BackgroundPadding;
		BGRect.y = pRect->y + pStyle->m_BackgroundPadding;
		BGRect.w = pRect->w - 2.0f*pStyle->m_BackgroundPadding;
		BGRect.h = pRect->h - 2.0f*pStyle->m_BackgroundPadding;
		if(pStyle->m_Flags&CAsset_GuiRectStyle::FLAG_ROUNDCORNER)
		{
			Draw_RoundRect_Background(BGRect.x, BGRect.y, BGRect.w, BGRect.h, pStyle->m_CornerRadius, CornerEnabled);
		}
		else
		{
			IGraphics::CQuadItem QuadItem(BGRect.x+BGRect.w/2, BGRect.y+BGRect.h/2, BGRect.w, BGRect.h);
			Graphics()->QuadsDraw(&QuadItem, 1);
		}
		
		Graphics()->QuadsEnd();
	}
	
	//Image
	if(pStyle->m_Flags&CAsset_GuiRectStyle::FLAG_IMAGE)
	{
		const CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pStyle->m_ImagePath);
		if(pImage)
		{
			vec2 UVSize = (pStyle->m_ImageUV_Max - pStyle->m_ImageUV_Min)/3.0f;
			TextureSet(pStyle->m_ImagePath);
			
			Graphics()->QuadsBegin();
			
			Graphics()->SetColor(1.0f, true);
			
			vec2 U0V0 = vec2(pStyle->m_ImageUV_Min.x + 0*UVSize.x, pStyle->m_ImageUV_Min.y + 0*UVSize.y);
			vec2 U1V0 = vec2(pStyle->m_ImageUV_Min.x + 1*UVSize.x, pStyle->m_ImageUV_Min.y + 0*UVSize.y);
			vec2 U2V0 = vec2(pStyle->m_ImageUV_Min.x + 2*UVSize.x, pStyle->m_ImageUV_Min.y + 0*UVSize.y);
			vec2 U3V0 = vec2(pStyle->m_ImageUV_Min.x + 3*UVSize.x, pStyle->m_ImageUV_Min.y + 0*UVSize.y);
			vec2 U0V1 = vec2(pStyle->m_ImageUV_Min.x + 0*UVSize.x, pStyle->m_ImageUV_Min.y + 1*UVSize.y);
			vec2 U1V1 = vec2(pStyle->m_ImageUV_Min.x + 1*UVSize.x, pStyle->m_ImageUV_Min.y + 1*UVSize.y);
			vec2 U2V1 = vec2(pStyle->m_ImageUV_Min.x + 2*UVSize.x, pStyle->m_ImageUV_Min.y + 1*UVSize.y);
			vec2 U3V1 = vec2(pStyle->m_ImageUV_Min.x + 3*UVSize.x, pStyle->m_ImageUV_Min.y + 1*UVSize.y);
			vec2 U0V2 = vec2(pStyle->m_ImageUV_Min.x + 0*UVSize.x, pStyle->m_ImageUV_Min.y + 2*UVSize.y);
			vec2 U1V2 = vec2(pStyle->m_ImageUV_Min.x + 1*UVSize.x, pStyle->m_ImageUV_Min.y + 2*UVSize.y);
			vec2 U2V2 = vec2(pStyle->m_ImageUV_Min.x + 2*UVSize.x, pStyle->m_ImageUV_Min.y + 2*UVSize.y);
			vec2 U3V2 = vec2(pStyle->m_ImageUV_Min.x + 3*UVSize.x, pStyle->m_ImageUV_Min.y + 2*UVSize.y);
			vec2 U0V3 = vec2(pStyle->m_ImageUV_Min.x + 0*UVSize.x, pStyle->m_ImageUV_Min.y + 3*UVSize.y);
			vec2 U1V3 = vec2(pStyle->m_ImageUV_Min.x + 1*UVSize.x, pStyle->m_ImageUV_Min.y + 3*UVSize.y);
			vec2 U2V3 = vec2(pStyle->m_ImageUV_Min.x + 2*UVSize.x, pStyle->m_ImageUV_Min.y + 3*UVSize.y);
			vec2 U3V3 = vec2(pStyle->m_ImageUV_Min.x + 3*UVSize.x, pStyle->m_ImageUV_Min.y + 3*UVSize.y);
			
			int PartSizeX = UVSize.x * pImage->GetWidth();
			int PartSizeY = UVSize.y * pImage->GetHeight();
			
			//TL Corner
			{
				Graphics()->QuadsSetSubset(U0V0.x, U0V0.y, U1V1.x, U1V1.y);
				IGraphics::CQuadItem QuadItem(pRect->x, pRect->y, PartSizeX, PartSizeY);
				Graphics()->QuadsDraw(&QuadItem, 1);
			}
			//TR Corner
			{
				Graphics()->QuadsSetSubset(U2V0.x, U2V0.y, U3V1.x, U3V1.y);
				IGraphics::CQuadItem QuadItem(pRect->x+pRect->w, pRect->y, PartSizeX, PartSizeY);
				Graphics()->QuadsDraw(&QuadItem, 1);
			}
			//BL Corner
			{
				Graphics()->QuadsSetSubset(U0V2.x, U0V2.y, U1V3.x, U1V3.y);
				IGraphics::CQuadItem QuadItem(pRect->x, pRect->y+pRect->h, PartSizeX, PartSizeY);
				Graphics()->QuadsDraw(&QuadItem, 1);
			}
			//BR Corner
			{
				Graphics()->QuadsSetSubset(U2V2.x, U2V2.y, U3V3.x, U3V3.y);
				IGraphics::CQuadItem QuadItem(pRect->x+pRect->w, pRect->y+pRect->h, PartSizeX, PartSizeY);
				Graphics()->QuadsDraw(&QuadItem, 1);
			}
			
			//Top
			{
				int TotalLength = pRect->w - PartSizeX;
				if(TotalLength > 0)
				{
					int Iter = 0;
					while(Iter < TotalLength)
					{
						int Length = ((Iter + PartSizeX > TotalLength) ? (TotalLength - Iter) : PartSizeX);
						float Tex = UVSize.x * static_cast<float>(Length)/PartSizeX;
						Graphics()->QuadsSetSubset(U1V0.x, U1V0.y, U1V1.x + Tex, U1V1.y);
						IGraphics::CQuadItem QuadItem(pRect->x+PartSizeX/2+Iter+Length/2, pRect->y, Length, PartSizeY);
						Graphics()->QuadsDraw(&QuadItem, 1);
						
						Iter += PartSizeX;
					}
				}
			}
			//Bottom
			{
				int TotalLength = pRect->w - PartSizeX;
				if(TotalLength > 0)
				{
					int Iter = 0;
					while(Iter < TotalLength)
					{
						int Length = ((Iter + PartSizeX > TotalLength) ? (TotalLength - Iter) : PartSizeX);
						float Tex = UVSize.x * static_cast<float>(Length)/PartSizeX;
						Graphics()->QuadsSetSubset(U1V2.x, U1V2.y, U1V3.x + Tex, U1V3.y);
						IGraphics::CQuadItem QuadItem(pRect->x+PartSizeX/2+Iter+Length/2, pRect->y + pRect->h, Length, PartSizeY);
						Graphics()->QuadsDraw(&QuadItem, 1);
						
						Iter += PartSizeX;
					}
				}
			}
			//Left
			{
				int TotalLength = pRect->h - PartSizeX;
				if(TotalLength > 0)
				{
					int Iter = 0;
					while(Iter < TotalLength)
					{
						int Length = ((Iter + PartSizeY > TotalLength) ? (TotalLength - Iter) : PartSizeY);
						float Tex = UVSize.y * static_cast<float>(Length)/PartSizeY;
						Graphics()->QuadsSetSubset(U0V1.x, U0V1.y, U1V1.x, U1V1.y + Tex);
						IGraphics::CQuadItem QuadItem(pRect->x, pRect->y+PartSizeY/2+Iter+Length/2, PartSizeX, Length);
						Graphics()->QuadsDraw(&QuadItem, 1);
						
						Iter += PartSizeY;
					}
				}
			}
			//Right
			{
				int TotalLength = pRect->h - PartSizeX;
				if(TotalLength > 0)
				{
					int Iter = 0;
					while(Iter < TotalLength)
					{
						int Length = ((Iter + PartSizeY > TotalLength) ? (TotalLength - Iter) : PartSizeY);
						float Tex = UVSize.y * static_cast<float>(Length)/PartSizeY;
						Graphics()->QuadsSetSubset(U2V1.x, U2V1.y, U3V1.x, U3V1.y + Tex);
						IGraphics::CQuadItem QuadItem(pRect->x+pRect->w, pRect->y+PartSizeY/2+Iter+Length/2, PartSizeX, Length);
						Graphics()->QuadsDraw(&QuadItem, 1);
						
						Iter += PartSizeY;
					}
				}
			}
			
			Graphics()->QuadsEnd();
		}
	}
	
	//Border
	if(pStyle->m_Flags&CAsset_GuiRectStyle::FLAG_BORDER)
	{
		Graphics()->LinesBegin();
		
		Graphics()->SetColor(pStyle->m_BorderColor, true);
	
		if(pStyle->m_Flags&CAsset_GuiRectStyle::FLAG_ROUNDCORNER)
		{
			Draw_RoundRect_Border(pRect->x+0.5f, pRect->y+0.5f, pRect->w-1.0f, pRect->h-1.0f, pStyle->m_CornerRadius-0.5f, BorderEnabled, CornerEnabled);
		}
		else
		{
			IGraphics::CLineItem Lines[4];
			int NumLines = 0;
			
			if(BorderEnabled&1)
				Lines[NumLines++] = IGraphics::CLineItem(pRect->x+0.5f         , pRect->y+0.5f         , pRect->x+pRect->w-0.5f, pRect->y+0.5f         );
			if(BorderEnabled&2)
				Lines[NumLines++] = IGraphics::CLineItem(pRect->x+pRect->w-0.5f, pRect->y+pRect->h-0.5f, pRect->x+0.5f         , pRect->y+pRect->h-0.5f);
			if(BorderEnabled&4)
				Lines[NumLines++] = IGraphics::CLineItem(pRect->x+0.5f         , pRect->y+pRect->h-0.5f, pRect->x+0.5f         , pRect->y+0.5f         );
			if(BorderEnabled&8)
				Lines[NumLines++] = IGraphics::CLineItem(pRect->x+pRect->w-0.5f, pRect->y+0.5f         , pRect->x+pRect->w-0.5f, pRect->y+pRect->h-0.5f);
		
			Graphics()->LinesDraw(Lines, NumLines);
		}
		Graphics()->LinesEnd();
	}
}

void CAssetsRenderer::DrawGuiLine(ivec2 Point0, ivec2 Point1, CAssetPath StylePath, float Scale)
{
	const CAsset_GuiLineStyle* pStyle = AssetsManager()->GetAsset<CAsset_GuiLineStyle>(StylePath);
	if(!pStyle)
		return;
	
	Graphics()->BlendNormal();
	Graphics()->TextureClear();
	
	vec2 p0(Point0.x, Point0.y);
	vec2 p1(Point1.x, Point1.y);
	float LineLength = length(p1-p0);
	float LineAngle = angle(p1-p0);
	
	//Image
	if(pStyle->m_Flags&CAsset_GuiLineStyle::FLAG_IMAGE)
	{
		const CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pStyle->m_ImagePath);
		if(pImage)
		{
			vec2 UVSize(pStyle->m_ImageUV_Max - pStyle->m_ImageUV_Min);
			UVSize.x /= 3.0f;
			
			int PartSizeX = Scale * UVSize.x * pImage->GetWidth();
			int PartSizeY = Scale * UVSize.y * pImage->GetHeight();
			
			TextureSet(pStyle->m_ImagePath);
			Graphics()->QuadsBegin();
			Graphics()->SetColor(1.0f, true);
			
			vec2 U0V0 = vec2(pStyle->m_ImageUV_Min.x + 0*UVSize.x, pStyle->m_ImageUV_Min.y);
			vec2 U1V0 = vec2(pStyle->m_ImageUV_Min.x + 1*UVSize.x, pStyle->m_ImageUV_Min.y);
			vec2 U2V0 = vec2(pStyle->m_ImageUV_Min.x + 2*UVSize.x, pStyle->m_ImageUV_Min.y);
			vec2 U3V0 = vec2(pStyle->m_ImageUV_Min.x + 3*UVSize.x, pStyle->m_ImageUV_Min.y);
			vec2 U0V1 = vec2(U0V0.x, U0V0.y + UVSize.y);
			vec2 U1V1 = vec2(U1V0.x, U1V0.y + UVSize.y);
			vec2 U2V1 = vec2(U2V0.x, U2V0.y + UVSize.y);
			vec2 U3V1 = vec2(U3V0.x, U3V0.y + UVSize.y);
			
			//Left
			{
				Graphics()->QuadsSetSubset(U0V0.x, U0V0.y, U1V1.x, U1V1.y);
				Graphics()->QuadsSetRotation(LineAngle);
				IGraphics::CQuadItem QuadItem(p0.x, p0.y, PartSizeX, PartSizeY);
				Graphics()->QuadsDraw(&QuadItem, 1);
			}
			//Right
			{
				Graphics()->QuadsSetSubset(U2V0.x, U2V0.y, U3V1.x, U3V1.y);
				Graphics()->QuadsSetRotation(LineAngle);
				IGraphics::CQuadItem QuadItem(p1.x, p1.y, PartSizeX, PartSizeY);
				Graphics()->QuadsDraw(&QuadItem, 1);
			}
			
			//Center
			{
				float TotalLength = LineLength - PartSizeX/2;
				if(TotalLength > 0)
				{
					float Iter = PartSizeX/2;
					while(Iter < TotalLength)
					{
						float Length = ((Iter + PartSizeX > TotalLength) ? (TotalLength - Iter) : PartSizeX);
						float Tex = UVSize.x * static_cast<float>(Length)/PartSizeX;
						Graphics()->QuadsSetSubset(U1V0.x, U1V0.y, U1V1.x + Tex, U1V1.y);
						
						vec2 Pos = p0 + rotate(vec2(Iter+Length/2, 0.0f), LineAngle);
						
						IGraphics::CQuadItem QuadItem(Pos.x, Pos.y, Length, PartSizeY);
						Graphics()->QuadsSetRotation(LineAngle);
						Graphics()->QuadsDraw(&QuadItem, 1);
						
						Iter += PartSizeX;
					}
				}
			}
			
			Graphics()->QuadsEnd();
		}
	}
	
	//Border
	if(pStyle->m_Flags&CAsset_GuiLineStyle::FLAG_BORDER)
	{
		Graphics()->LinesBegin();
		Graphics()->SetColor(pStyle->m_BorderColor, true);
		
		IGraphics::CLineItem Line(p0.x, p0.y, p1.x, p1.y);
		
		Graphics()->LinesDraw(&Line, 1);
		Graphics()->LinesEnd();
	}
}

}
