#include <engine/graphics.h>
#include <tu/client/graphics.h>

#include "maprenderer.h"

namespace tu
{

CMapRenderer::CMapRenderer(CGraphics* pTUGraphics, CAssetsManager* pAssetsManager) :
	m_pTUGraphics(pTUGraphics),
	m_pGraphics(pTUGraphics->Graphics()),
	m_pAssetsManager(pAssetsManager),
	m_GroupPos(0.0f, 0.0f),
	m_GroupHardParallax(1.0f, 1.0f)
{
	
}

void CMapRenderer::SetTime(float Time)
{
	m_Time = Time;
}

void CMapRenderer::SetLocalTime(float Time)
{
	m_LocalTime = Time;
}
	
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
		m_GroupPos = pGroup->GetPosition();
		m_GroupHardParallax = pGroup->GetHardParallax();
	
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

vec2 CMapRenderer::MapPosToTilePos(vec2 MapPos) const
{
	return MapPos/32.0f;
}

vec2 CMapRenderer::TilePosToMapPos(vec2 TilePos) const
{
	return TilePos*32.0f;
}

vec2 CMapRenderer::ScreenPosToTilePos(vec2 MapPos) const
{
	return MapPosToTilePos(ScreenPosToMapPos(MapPos));
}

vec2 CMapRenderer::TilePosToScreenPos(vec2 MapPos) const
{
	return MapPosToScreenPos(TilePosToMapPos(MapPos));
}

void CMapRenderer::RenderGrid(int Width, int Height, float Scale)
{
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(0.5f, 0.5f, 0.5f, 0.5f);
	
	vec2 minCorner = MapPosToScreenPos(vec2(0.0f, 0.0f));
	vec2 maxCorner = MapPosToScreenPos(vec2(Width*Scale, Height*Scale));
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
	for(int j=0; j<=Height; j++)
	{
		float y = minCorner.y+size.y*(static_cast<float>(j)/Height);
		IGraphics::CLineItem Line(minCorner.x, y, maxCorner.x, y);
		Graphics()->LinesDraw(&Line, 1);
	}
	for(int i=0; i<Width; i++)
	{
		float x = minCorner.x+size.x*(static_cast<float>(i)/Width);
		IGraphics::CLineItem Line(x, minCorner.y, x, maxCorner.y);
		Graphics()->LinesDraw(&Line, 1);
	}
	
	Graphics()->LinesEnd();
}

void CMapRenderer::RenderGrid_LayerTiles(CAssetPath LayerPath)
{
	if(LayerPath.GetType() == CAsset_MapLayerTiles::TypeId)
	{
		CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(LayerPath);
		if(!pLayer)
			return;
		
		RenderGrid(pLayer->GetWidth(), pLayer->GetHeight(), 32.0f);
	}
	else if(LayerPath.GetType() == CAsset_MapZoneTiles::TypeId)
	{
		CAsset_MapZoneTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapZoneTiles>(LayerPath);
		if(!pLayer)
			return;
		
		RenderGrid(pLayer->GetWidth(), pLayer->GetHeight(), 32.0f);
	}
}

template<class TILE>
void RenderTiles_Zone_Impl(CMapRenderer* pMapRenderer, CAssetPath ZoneTypePath, TILE* pTiles, int Width, int Height, vec2 Pos, IGraphics::CTextureHandle& TextureId)
{
	CAsset_ZoneType* pZoneType = pMapRenderer->AssetsManager()->GetAsset<CAsset_ZoneType>(ZoneTypePath);
	
	pMapRenderer->Graphics()->TextureSet(TextureId);
	pMapRenderer->Graphics()->QuadsBegin();

	float TileSize = 32.0f*pMapRenderer->m_CameraZoom;

	//Draw tile layer
	for(int j=0; j<Height; j++)
	{
		for(int i=0; i<Width; i++)
		{
			TILE* pTile = &pTiles[j*Width+i];
			
			if(pTile->m_Index < 1)
				continue;
			
			bool IndexFound = false;
			vec4 Color = 1.0f;
			if(pZoneType)
			{
				CAsset_ZoneType::CSubPath IndexPath;
				CAsset_ZoneType::CIteratorIndex Iter;
				for(Iter = pZoneType->BeginIndex(); Iter != pZoneType->EndIndex(); ++Iter)
				{
					if(pZoneType->GetIndexNumber(*Iter) == pTile->m_Index)
					{
						Color = pZoneType->GetIndexColor(*Iter);
						IndexFound = true;
						break;
					}
				}
			}
			
			if(!IndexFound)
				continue;
			
			Color.a *= 0.5f;
			pMapRenderer->Graphics()->SetColor(Color, true);
			
			vec2 TilePos = pMapRenderer->MapPosToScreenPos(Pos + vec2(i*32.0f, j*32.0f));
			
			float Shift = fmod(pMapRenderer->m_LocalTime/4.0f, 1.0f);
			int Size = 8;
			
			float x0 = ((i%Size))/static_cast<float>(Size) + Shift;
			float y0 = ((j%Size))/static_cast<float>(Size) + Shift;
			float x2 = x0 + 1.0f/static_cast<float>(Size);
			float y2 = y0 + 1.0f/static_cast<float>(Size);
			float x1 = x2;
			float y1 = y0;
			float x3 = x0;
			float y3 = y2;
			
			pMapRenderer->Graphics()->QuadsSetSubsetFree(x0, y0, x1, y1, x2, y2, x3, y3);
			IGraphics::CQuadItem QuadItem(TilePos.x, TilePos.y, TileSize, TileSize);
			pMapRenderer->Graphics()->QuadsDrawTL(&QuadItem, 1);
		}
	}
	pMapRenderer->Graphics()->QuadsEnd();
	
	pMapRenderer->Graphics()->TextureClear();
	pMapRenderer->Graphics()->QuadsBegin();
	//Draw border
	for(int j=0; j<Height; j++)
	{
		for(int i=0; i<Width; i++)
		{
			TILE* pTile = &pTiles[j*Width+i];
			
			if(pTile->m_Index < 1)
				continue;
				
			bool IndexFound = false;
			vec4 Color = 1.0f;
			if(pZoneType)
			{
				CAsset_ZoneType::CSubPath IndexPath;
				CAsset_ZoneType::CIteratorIndex Iter;
				for(Iter = pZoneType->BeginIndex(); Iter != pZoneType->EndIndex(); ++Iter)
				{
					if(pZoneType->GetIndexNumber(*Iter) == pTile->m_Index)
					{
						Color = pZoneType->GetIndexColor(*Iter);
						IndexFound = true;
						break;
					}
				}
			}
			
			if(!IndexFound)
				continue;
			
			pMapRenderer->Graphics()->SetColor(Color, true);
			
			vec2 TilePos = pMapRenderer->MapPosToScreenPos(Pos + vec2(i*32.0f, j*32.0f));
			
			bool AirL = (i<=0        || pTiles[j*Width+(i-1)].m_Index != pTile->m_Index);
			bool AirR = (i>=Width-1  || pTiles[j*Width+(i+1)].m_Index != pTile->m_Index);
			bool AirT = (j<=0        || pTiles[(j-1)*Width+i].m_Index != pTile->m_Index);
			bool AirB = (j>=Height-1 || pTiles[(j+1)*Width+i].m_Index != pTile->m_Index);
			
			bool AirTL = (i<=0       || j<=0        || pTiles[(j-1)*Width+(i-1)].m_Index != pTile->m_Index);
			bool AirTR = (i>=Width-1 || j<=0        || pTiles[(j-1)*Width+(i+1)].m_Index != pTile->m_Index);
			bool AirBL = (i<=0       || j>=Height-1 || pTiles[(j+1)*Width+(i-1)].m_Index != pTile->m_Index);
			bool AirBR = (i>=Width-1 || j>=Height-1 || pTiles[(j+1)*Width+(i+1)].m_Index != pTile->m_Index);
			
			if(AirT)
			{
				IGraphics::CFreeformItem Freeform(
					TilePos.x, TilePos.y,
					TilePos.x + TileSize, TilePos.y,
					TilePos.x + (AirL ? TileSize/4.0f : 0.0f), TilePos.y+TileSize/4.0f,
					TilePos.x + TileSize - (AirR ? TileSize/4.0f : 0.0f), TilePos.y+TileSize/4.0f
				);
				pMapRenderer->Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			if(AirB)
			{
				IGraphics::CFreeformItem Freeform(
					TilePos.x, TilePos.y+TileSize,
					TilePos.x+TileSize, TilePos.y+TileSize,
					TilePos.x+(AirL ? TileSize/4.0f : 0.0f), TilePos.y+TileSize-TileSize/4.0f,
					TilePos.x+TileSize-(AirR ? TileSize/4.0f : 0.0f), TilePos.y+TileSize-TileSize/4.0f
				);
				pMapRenderer->Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			if(AirL)
			{
				IGraphics::CFreeformItem Freeform(
					TilePos.x, TilePos.y,
					TilePos.x, TilePos.y + TileSize,
					TilePos.x+TileSize/4.0f, TilePos.y+(AirT ? TileSize/4.0f : 0.0f),
					TilePos.x+TileSize/4.0f, TilePos.y+TileSize-(AirB ? TileSize/4.0f : 0.0f)
				);
				pMapRenderer->Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			if(AirR)
			{
				IGraphics::CFreeformItem Freeform(
					TilePos.x+TileSize, TilePos.y,
					TilePos.x+TileSize, TilePos.y+TileSize,
					TilePos.x+TileSize-TileSize/4.0f, TilePos.y+(AirT ? TileSize/4.0f : 0.0f),
					TilePos.x+TileSize-TileSize/4.0f, TilePos.y+TileSize-(AirB ? TileSize/4.0f : 0.0f)
				);
				pMapRenderer->Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			
			if(!AirT && !AirL && AirTL)
			{
				IGraphics::CFreeformItem Freeform(
					TilePos.x, TilePos.y,
					TilePos.x+TileSize/4.0f, TilePos.y,
					TilePos.x, TilePos.y+TileSize/4.0f,
					TilePos.x+TileSize/4.0f, TilePos.y+TileSize/4.0f
				);
				pMapRenderer->Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			if(!AirT && !AirR && AirTR)
			{
				IGraphics::CFreeformItem Freeform(
					TilePos.x+TileSize, TilePos.y,
					TilePos.x+TileSize-TileSize/4.0f, TilePos.y,
					TilePos.x+TileSize, TilePos.y+TileSize/4.0f,
					TilePos.x+TileSize-TileSize/4.0f, TilePos.y+TileSize/4.0f
				);
				pMapRenderer->Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			if(!AirB && !AirL && AirBL)
			{
				IGraphics::CFreeformItem Freeform(
					TilePos.x, TilePos.y+TileSize,
					TilePos.x+TileSize/4.0f, TilePos.y+TileSize,
					TilePos.x, TilePos.y+TileSize-TileSize/4.0f,
					TilePos.x+TileSize/4.0f, TilePos.y+TileSize-TileSize/4.0f
				);
				pMapRenderer->Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			if(!AirB && !AirR && AirBR)
			{
				IGraphics::CFreeformItem Freeform(
					TilePos.x+TileSize, TilePos.y+TileSize,
					TilePos.x+TileSize-TileSize/4.0f, TilePos.y+TileSize,
					TilePos.x+TileSize, TilePos.y+TileSize-TileSize/4.0f,
					TilePos.x+TileSize-TileSize/4.0f, TilePos.y+TileSize-TileSize/4.0f
				);
				pMapRenderer->Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
		}
	}
	
	pMapRenderer->Graphics()->QuadsEnd();
}

void CMapRenderer::RenderTiles_Zone(CAssetPath ZoneTypePath, const CAsset_MapZoneTiles::CTile* pTiles, int Width, int Height, vec2 Pos, IGraphics::CTextureHandle& TextureId)
{
	RenderTiles_Zone_Impl(this, ZoneTypePath, pTiles, Width, Height, Pos, TextureId);
}

void CMapRenderer::RenderTiles_Zone(CAssetPath ZoneTypePath, const CAsset_MapLayerTiles::CTile* pTiles, int Width, int Height, vec2 Pos, IGraphics::CTextureHandle& TextureId)
{
	RenderTiles_Zone_Impl(this, ZoneTypePath, pTiles, Width, Height, Pos, TextureId);
}

void CMapRenderer::RenderTiles_Image(const CAsset_MapLayerTiles::CTile* pTiles, int Width, int Height, vec2 Pos, CAssetPath ImagePath, vec4 Color)
{
	TUGraphics()->TextureSet(ImagePath);
	
	Graphics()->QuadsBegin();
	Graphics()->SetColor(Color, true);

	//Draw tile layer
	for(int j=0; j<Height; j++)
	{
		for(int i=0; i<Width; i++)
		{
			const CAsset_MapLayerTiles::CTile* pTile = &pTiles[j*Width+i];
			
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
		}
	}
	
	Graphics()->QuadsEnd();
}

void CMapRenderer::RenderQuads(const CAsset_MapLayerQuads::CQuad* pQuads, int NbQuads, vec2 LayerPos, CAssetPath ImagePath, vec4 LayerColor)
{
	TUGraphics()->TextureSet(ImagePath);
	
	Graphics()->QuadsBegin();
	
	for(int i=0; i<NbQuads; i++)
	{
		const CAsset_MapLayerQuads::CQuad* pQuad = &pQuads[i];
				
		vec2 Position;
		matrix2x2 Transform;
		vec4 Color;
		int DrawState;
		pQuad->GetTransform(AssetsManager(), m_Time, &Transform, &Position);
		pQuad->GetDrawState(AssetsManager(), m_Time, &Color, &DrawState);
		
		if(DrawState == CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN)
			continue;
		
		Position += LayerPos;
		Color *= LayerColor;
		
		//Color
		Graphics()->SetColor4(pQuad->m_Colors[0]*Color, pQuad->m_Colors[1]*Color, pQuad->m_Colors[2]*Color, pQuad->m_Colors[3]*Color, true);
		
		//UVs
		bool RepeatU = false;
		bool RepeatV = false;
		for(int p = 0; p < 4; p++)
		{
			if(pQuad->m_UVs[p].x < 0.0f || pQuad->m_UVs[p].x > 1.0f)
				RepeatU = true;
			if(pQuad->m_UVs[p].y < 0.0f || pQuad->m_UVs[p].y > 1.0f)
				RepeatV = true;
		}
		Graphics()->WrapMode(
			RepeatU ? IGraphics::WRAP_REPEAT : IGraphics::WRAP_CLAMP,
			RepeatV ? IGraphics::WRAP_REPEAT : IGraphics::WRAP_CLAMP);
		
		Graphics()->QuadsSetSubsetFree(
			pQuad->m_UVs[0].x, pQuad->m_UVs[0].y,
			pQuad->m_UVs[1].x, pQuad->m_UVs[1].y,
			pQuad->m_UVs[2].x, pQuad->m_UVs[2].y,
			pQuad->m_UVs[3].x, pQuad->m_UVs[3].y
		);
		
		//Vertices
		
		vec2 Vertices[4];
		for(int p=0; p<4; p++)
		{
			vec2 TestBefore = pQuad->m_Vertices[p];
			vec2 TestAfter = Transform*pQuad->m_Vertices[p] + Position;
			Vertices[p] = MapPosToScreenPos(Transform*pQuad->m_Vertices[p] + Position);
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

void CMapRenderer::RenderQuads_Mesh(const CAsset_MapLayerQuads::CQuad* pQuads, int NbQuads)
{
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	
	for(int i=0; i<NbQuads; i++)
	{
		const CAsset_MapLayerQuads::CQuad* pQuad = &pQuads[i];
		
		vec4 Color = 1.0f;
		
		vec2 Position;
		matrix2x2 Transform;
		pQuad->GetTransform(AssetsManager(), m_Time, &Transform, &Position);
		
		//Vertices
		{
			for(int p=0; p<4; p++)
			{
				int P0;
				int P1;
				switch(p)
				{
					case 0:
						P0 = 0;
						P1 = 1;
						break;
					case 1:
						P0 = 1;
						P1 = 3;
						break;
					case 2:
						P0 = 3;
						P1 = 2;
						break;
					case 3:
						P0 = 2;
						P1 = 0;
						break;
				}
				
				vec2 Vertex0Pos = MapPosToScreenPos(Transform * pQuads[i].m_Vertices[P0] + Position);
				vec2 Vertex1Pos = MapPosToScreenPos(Transform * pQuads[i].m_Vertices[P1] + Position);
				
				vec4 Color0 = pQuads[i].m_Colors[P0];
				vec4 Color1 = pQuads[i].m_Colors[P1];
				
				Graphics()->SetColor2(Color0, Color1, true);
				
				IGraphics::CLineItem Line(Vertex0Pos.x, Vertex0Pos.y, Vertex1Pos.x, Vertex1Pos.y);
				Graphics()->LinesDraw(&Line, 1);
			}
		}
	}
	
	Graphics()->LinesEnd();
}

void CMapRenderer::RenderGroup(CAssetPath GroupPath)
{
	CAsset_MapGroup* pGroup = AssetsManager()->GetAsset<CAsset_MapGroup>(GroupPath);
	if(!pGroup)
		return;
	
	SetGroup(GroupPath);
	
	CAsset_MapGroup::CIteratorLayer IterLayer;
	for(IterLayer = pGroup->BeginLayer(); IterLayer != pGroup->EndLayer(); ++IterLayer)
	{
		CAssetPath LayerPath = pGroup->GetLayer(*IterLayer);
		if(LayerPath.GetType() == CAsset_MapLayerTiles::TypeId)
		{
			CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(LayerPath);
			if(!pLayer)
				continue;
			
			RenderTiles_Image(pLayer->GetTilesPointer(), pLayer->GetWidth(), pLayer->GetHeight(), vec2(0.0f, 0.0f), pLayer->GetImagePath(), pLayer->GetColor());
		}
		else if(LayerPath.GetType() == CAsset_MapLayerQuads::TypeId)
		{
			CAsset_MapLayerQuads* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(LayerPath);
			if(!pLayer)
				continue;
			
			RenderQuads(pLayer->GetQuadsPointer(), pLayer->GetNumQuads(), vec2(0.0f, 0.0f), pLayer->GetImagePath(), vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

void CMapRenderer::RenderMap(CAssetPath MapPath)
{
	CAsset_Map* pMap = AssetsManager()->GetAsset<CAsset_Map>(MapPath);
	if(!pMap)
		return;
	
	{
		CAsset_Map::CIteratorBgGroup Iter;
		for(Iter = pMap->BeginBgGroup(); Iter != pMap->EndBgGroup(); ++Iter)
		{
			RenderGroup(pMap->GetBgGroup(*Iter));
		}
	}
	{
		CAsset_Map::CIteratorFgGroup Iter;
		for(Iter = pMap->BeginFgGroup(); Iter != pMap->EndFgGroup(); ++Iter)
		{
			RenderGroup(pMap->GetFgGroup(*Iter));
		}
	}
		
	UnsetGroup();
}

void CMapRenderer::RenderSource(int Source)
{
	int nbMaps = AssetsManager()->GetNumAssets<CAsset_Map>(Source);
	for(int m=0; m<nbMaps; m++)
	{
		CAssetPath MapPath = CAssetPath(CAsset_Map::TypeId, Source, m);
		RenderMap(MapPath);
	}
}

void CMapRenderer::RenderMap_Zones(CAssetPath MapPath, IGraphics::CTextureHandle& TextureId)
{
	CAsset_Map* pMap = AssetsManager()->GetAsset<CAsset_Map>(MapPath);
	if(!pMap)
		return;
	
	CAsset_Map::CIteratorZoneLayer Iter;
	for(Iter = pMap->BeginZoneLayer(); Iter != pMap->EndZoneLayer(); ++Iter)
	{
		CAssetPath ZonePath = pMap->GetZoneLayer(*Iter);
		CAsset_MapZoneTiles* pZone = AssetsManager()->GetAsset<CAsset_MapZoneTiles>(ZonePath);
		if(!pZone)
			continue;
		
		RenderTiles_Zone(pZone->GetZoneTypePath(), pZone->GetTilesPointer(), pZone->GetWidth(), pZone->GetHeight(), 0.0f, TextureId);
	}
}

void CMapRenderer::RenderSource_Zones(int Source, IGraphics::CTextureHandle& TextureId)
{
	int nbMaps = AssetsManager()->GetNumAssets<CAsset_Map>(Source);
	for(int m=0; m<nbMaps; m++)
	{
		CAssetPath MapPath = CAssetPath(CAsset_Map::TypeId, Source, m);
		RenderMap_Zones(MapPath, TextureId);
	}
}

}
