#include <engine/graphics.h>

#include "maprenderer.h"

namespace tu
{

void CMapRenderer::SetCanvas(vec2 ScreenPos)
{
	m_ScreenPos = ScreenPos;
}

void CMapRenderer::SetCamera(vec2 CameraPos, float CameraZoom)
{
	m_CameraPos = CameraPos;
	m_CameraZoom = CameraZoom;
	
	m_GroupCameraPos = (m_CameraPos * m_GroupHardParallax) + m_GroupPos;
}

void CMapRenderer::SetGroup(CAssetPath GroupPath)
{
	CAsset_MapGroup* pGroup = AssetsManager()->GetAsset<CAsset_MapGroup>(GroupPath);
	if(pGroup)
	{
		m_GroupPos = pGroup->m_Position;
		m_GroupHardParallax = pGroup->m_HardParallax;
	
		m_GroupCameraPos = (m_CameraPos * m_GroupHardParallax) + m_GroupPos;
	}
	else
		UnsetGroup();
}

void CMapRenderer::UnsetGroup()
{
	m_GroupPos = vec2(0.0f, 0.0f);
	m_GroupHardParallax = vec2(1.0f, 1.0f);
	
	m_GroupCameraPos = (m_CameraPos * m_GroupHardParallax) + m_GroupPos;
}

vec2 CMapRenderer::MapPosToScreenPos(vec2 MapPos) const
{
	return m_ScreenPos + (MapPos - m_GroupCameraPos)*m_CameraZoom;
}

vec2 CMapRenderer::ScreenPosToMapPos(vec2 ScreenPos) const
{
	return m_GroupCameraPos + (ScreenPos - m_ScreenPos)/m_CameraZoom;
}

void CMapRenderer::RenderTileLayerGrid(CAssetPath LayerPath)
{
	CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(LayerPath);
	if(!pLayer)
		return;
	
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(0.5f, 0.5f, 0.5f, 0.5f);
	
	vec2 minCorner = MapPosToScreenPos(vec2(0.0f, 0.0f));
	vec2 maxCorner = MapPosToScreenPos(vec2(pLayer->m_Width*32.0f, pLayer->m_Height*32.0f));
	vec2 size = maxCorner - minCorner;
	
	{
		IGraphics::CLineItem Lines[4];
		Lines[0] = IGraphics::CLineItem(minCorner.x, minCorner.y, maxCorner.x, minCorner.y);
		Lines[1] = IGraphics::CLineItem(maxCorner.x, minCorner.y, maxCorner.x, maxCorner.y);
		Lines[2] = IGraphics::CLineItem(maxCorner.x, maxCorner.y, minCorner.x, maxCorner.y);
		Lines[3] = IGraphics::CLineItem(minCorner.x, maxCorner.y, minCorner.x, minCorner.y);
		
		Graphics()->LinesDraw(Lines, 4);
	}
	
	Graphics()->SetColor(0.25f, 0.25f, 0.25f, 0.25f);
	for(int j=0; j<=pLayer->m_Height; j++)
	{
		float y = minCorner.y+size.y*(static_cast<float>(j)/pLayer->m_Height);
		IGraphics::CLineItem Line(minCorner.x, y, maxCorner.x, y);
		Graphics()->LinesDraw(&Line, 1);
	}
	for(int i=0; i<pLayer->m_Width; i++)
	{
		float x = minCorner.x+size.x*(static_cast<float>(i)/pLayer->m_Width);
		IGraphics::CLineItem Line(x, minCorner.y, x, maxCorner.y);
		Graphics()->LinesDraw(&Line, 1);
	}
	
	Graphics()->LinesEnd();
}

void CMapRenderer::RenderTiles(CAsset_MapLayerTiles::CTile* pTiles, int Width, int Height, vec2 Pos, CAssetPath ImagePath, vec4 Color)
{
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(ImagePath);
	if(pImage)
	{
		Graphics()->TextureSet(pImage->m_Texture);
	}
	else
	{
		Graphics()->TextureClear();
	}
	
	Graphics()->QuadsBegin();
	Graphics()->SetColor(Color, true);

	//Draw tile layer
	for(int j=0; j<Height; j++)
	{
		for(int i=0; i<Width; i++)
		{
			CAsset_MapLayerTiles::CTile* pTile = &pTiles[j*Width+i];
			
			int Index = pTile->m_Index;
			if(Index)
			{
				vec2 TilePos = MapPosToScreenPos(Pos + vec2(i*32.0f, j*32.0f));
				
				float x0 = 0;
				float y0 = 0;
				float x1 = 1;
				float y1 = 0;
				float x2 = 1;
				float y2 = 1;
				float x3 = 0;
				float y3 = 1;

				if(pTile->m_Flags&CAsset_MapLayerTiles::TILEFLAG_VFLIP)
				{
					x0 = x2;
					x1 = x3;
					x2 = x3;
					x3 = x0;
				}

				if(pTile->m_Flags&CAsset_MapLayerTiles::TILEFLAG_HFLIP)
				{
					y0 = y3;
					y2 = y1;
					y3 = y1;
					y1 = y0;
				}

				if(pTile->m_Flags&CAsset_MapLayerTiles::TILEFLAG_ROTATE)
				{
					float Tmp = x0;
					x0 = x3;
					x3 = x2;
					x2 = x1;
					x1 = Tmp;
					Tmp = y0;
					y0 = y3;
					y3 = y2;
					y2 = y1;
					y1 = Tmp;
				}
				
				Graphics()->QuadsSetSubsetFree(x0, y0, x1, y1, x2, y2, x3, y3, Index);
				IGraphics::CQuadItem QuadItem(TilePos.x, TilePos.y, 32.0f*m_CameraZoom, 32.0f*m_CameraZoom);
				Graphics()->QuadsDrawTL(&QuadItem, 1);
			}
			
			i += pTile->m_Skip;
		}
	}
	
	Graphics()->QuadsEnd();
}

void CMapRenderer::RenderQuads(CAsset_MapLayerQuads::CQuad* pQuads, int NbQuads, vec2 Pos, CAssetPath ImagePath)
{
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(ImagePath);
	if(pImage)
	{
		Graphics()->TextureSet(pImage->m_Texture);
	}
	else
	{
		Graphics()->TextureClear();
	}
	
	Graphics()->QuadsBegin();
	
	for(int i=0; i<NbQuads; i++)
	{
		CAsset_MapLayerQuads::CQuad* Quad = &pQuads[i];
		
		//Color
		Graphics()->SetColor4(Quad->m_Colors[0], Quad->m_Colors[1], Quad->m_Colors[2], Quad->m_Colors[3], true);
		
		//UVs
		bool RepeatU = false;
		bool RepeatV = false;
		for(int p = 0; p < 4; p++)
		{
			if(Quad->m_UVs[p].x < 0.0f || Quad->m_UVs[p].x > 1.0f)
				RepeatU = true;
			if(Quad->m_UVs[p].y < 0.0f || Quad->m_UVs[p].y > 1.0f)
				RepeatV = true;
		}
		Graphics()->WrapMode(
			RepeatU ? IGraphics::WRAP_REPEAT : IGraphics::WRAP_CLAMP,
			RepeatV ? IGraphics::WRAP_REPEAT : IGraphics::WRAP_CLAMP);
		
		Graphics()->QuadsSetSubsetFree(
			Quad->m_UVs[0].x, Quad->m_UVs[0].y,
			Quad->m_UVs[1].x, Quad->m_UVs[1].y,
			Quad->m_UVs[2].x, Quad->m_UVs[2].y,
			Quad->m_UVs[3].x, Quad->m_UVs[3].y
		);
		
		//Vertices
		vec2 Vertices[4];
		for(int p=0; p<4; p++)
		{
			vec2 MapPos = rotate(Quad->m_Vertices[p], Quad->m_Angle)*Quad->m_Size + Quad->m_Pivot;
			Vertices[p] = MapPosToScreenPos(MapPos);
		}

		IGraphics::CFreeformItem Freeform(
			Vertices[0].x, Vertices[0].y,
			Vertices[1].x, Vertices[1].y,
			Vertices[2].x, Vertices[2].y,
			Vertices[3].x, Vertices[3].y
		);
		Graphics()->QuadsDrawFreeform(&Freeform, 1);
	}
	
	Graphics()->QuadsEnd();
	Graphics()->WrapNormal();
}

void CMapRenderer::RenderSource(int Source)
{
	int nbGroups = AssetsManager()->GetNumAssets<CAsset_MapGroup>(Source);
	
	for(int g=0; g<nbGroups; g++)
	{
		CAssetPath GroupPath = CAssetPath(CAsset_MapGroup::TypeId, Source, g);
		CAsset_MapGroup* pGroup = AssetsManager()->GetAsset<CAsset_MapGroup>(GroupPath);
		if(!pGroup)
			continue;
		
		SetGroup(GroupPath);
		
		for(int l=0; l<pGroup->m_Layers.size(); l++)
		{
			CAssetPath LayerPath = pGroup->m_Layers[l];
			if(LayerPath.GetType() == CAsset_MapLayerTiles::TypeId)
			{
				CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(LayerPath);
				if(!pLayer)
					continue;
				
				RenderTiles(pLayer->m_pTiles, pLayer->m_Width, pLayer->m_Height, vec2(0.0f, 0.0f), pLayer->m_ImagePath, pLayer->m_Color);
			}
			else if(LayerPath.GetType() == CAsset_MapLayerQuads::TypeId)
			{
				CAsset_MapLayerQuads* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(LayerPath);
				if(!pLayer)
					continue;
				
				RenderQuads(&pLayer->m_Quads[0], pLayer->m_Quads.size(), vec2(0.0f, 0.0f), pLayer->m_ImagePath);
			}
		}
	}
	
	UnsetGroup();
}

}
