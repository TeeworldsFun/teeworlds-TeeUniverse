#include <engine/graphics.h>
#include <tu/client/graphics.h>

#include "skeletonrenderer.h"

namespace tu
{

CSkeletonRenderer::CSkeletonRenderer(CGraphics* pTUGraphics, CAssetsManager* pAssetsManager) :
	m_pTUGraphics(pTUGraphics),
	m_pGraphics(pTUGraphics->Graphics()),
	m_pAssetsManager(pAssetsManager),
	m_NumLayers(0)
{
	m_Aim = vec2(1.0f, 0.0f);
	m_Motion = vec2(1.0f, 0.0f);
	m_Hook = vec2(1.0f, 0.0f);
}

void CSkeletonRenderer::AddSkeleton(CAssetPath SkeletonPath)
{
	for(int i=0; i<m_Skeletons.size(); i++)
	{
		if(m_Skeletons[i].m_Path == SkeletonPath)
			return;
	}
	
	CAsset_Skeleton* pSkeleton = AssetsManager()->GetAsset<CAsset_Skeleton>(SkeletonPath);
	if(!pSkeleton)
		return;
	
	int SkeletonId = m_Skeletons.size();
	m_Skeletons.add(CSkeletonState());
	CSkeletonState& SkeletonState = m_Skeletons[SkeletonId];
	SkeletonState.m_Path = SkeletonPath;
	SkeletonState.m_Parent = -1;
	
	//Search parent ID
	if(!pSkeleton->m_ParentPath.IsNull())
	{
		for(int i=0; i<m_Skeletons.size()-1; i++)
		{
			if(pSkeleton->m_ParentPath == m_Skeletons[i].m_Path)
			{
				SkeletonState.m_Parent = i;
				break;
			}
		}
	}
	
	SkeletonState.m_Bones.set_size(pSkeleton->m_Bones.size());
	for(int i=0; i<pSkeleton->m_Bones.size(); i++)
	{
		CSkeletonRenderer::CBoneState& BoneState = SkeletonState.m_Bones[i];
		BoneState.m_ParentSkeleton = -1;
		BoneState.m_ParentBone = -1;
		BoneState.m_Finalized = false;
		
		if(!pSkeleton->m_Bones[i].m_ParentPath.IsNull())
		{
			if(pSkeleton->m_Bones[i].m_ParentPath.GetSource() == CAsset_Skeleton::CSubPath::SRC_PARENT)
			{
				if(SkeletonState.m_Parent >= 0)
				{
					BoneState.m_ParentSkeleton = SkeletonState.m_Parent;
					BoneState.m_ParentBone = pSkeleton->m_Bones[i].m_ParentPath.GetId();
				}
			}
			else
			{
				BoneState.m_ParentSkeleton = SkeletonId;
				BoneState.m_ParentBone = pSkeleton->m_Bones[i].m_ParentPath.GetId();
			}
		}
			
		BoneState.m_Length = pSkeleton->m_Bones[i].m_Length;
		BoneState.m_Anchor = pSkeleton->m_Bones[i].m_Anchor;
		BoneState.m_Color = pSkeleton->m_Bones[i].m_Color;
		
		BoneState.m_StartPoint = pSkeleton->m_Bones[i].m_Translation;
		BoneState.m_Transform = matrix2x2::rotation(pSkeleton->m_Bones[i].m_Angle) * matrix2x2::scaling(pSkeleton->m_Bones[i].m_Scale);
	}
	
	SkeletonState.m_Layers.set_size(pSkeleton->m_Layers.size());
	for(int i=0; i<pSkeleton->m_Layers.size(); i++)
	{
		CSkeletonRenderer::CLayerState& LayerState = SkeletonState.m_Layers[i];
		LayerState.m_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		LayerState.m_State = CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE;
	}
}

void CSkeletonRenderer::AddSkeletonWithParents(CAssetPath SkeletonPath, int AddDefaultSkin)
{
	CAsset_Skeleton* pSkeleton = AssetsManager()->GetAsset<CAsset_Skeleton>(SkeletonPath);
	if(!pSkeleton)
		return;
	
	AddSkeletonWithParents(pSkeleton->m_ParentPath, (AddDefaultSkin == ADDDEFAULTSKIN_ONLYPARENT ? ADDDEFAULTSKIN_YES : AddDefaultSkin));	
	AddSkeleton(SkeletonPath);
	if(AddDefaultSkin == ADDDEFAULTSKIN_YES)
	{
		AddSkin(pSkeleton->m_DefaultSkinPath, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void CSkeletonRenderer::ApplyAnimation(CAssetPath SkeletonAnimationPath, float Time)
{
	CAsset_SkeletonAnimation* pSkeletonAnimation = AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(SkeletonAnimationPath);
	if(!pSkeletonAnimation)
		return;
	
	int SkeletonId = -1;
	for(int s=0; s<m_Skeletons.size(); s++)
	{
		if(m_Skeletons[s].m_Path == pSkeletonAnimation->m_SkeletonPath)
		{
			SkeletonId = s;
			break;
		}
	}
	
	if(SkeletonId < 0)
		return;
	
	for(int i=0; i<pSkeletonAnimation->m_BoneAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath BonePath = pSkeletonAnimation->m_BoneAnimations[i].m_BonePath;
		int sId = SkeletonId;
		if(BonePath.GetSource() == CAsset_Skeleton::CSubPath::SRC_PARENT && m_Skeletons[SkeletonId].m_Parent >= 0)
		{
			sId = m_Skeletons[SkeletonId].m_Parent;
		}
		int bId = BonePath.GetId();
		
		if(sId < 0 || bId < 0 || sId >= m_Skeletons.size() || bId >= m_Skeletons[sId].m_Bones.size())
			continue;
		
		CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[sId].m_Bones[bId];
		
		CAsset_SkeletonAnimation::CBoneAnimation::CFrame Frame;
		if(pSkeletonAnimation->m_BoneAnimations[i].GetFrame(Time, &Frame))
		{
			BoneState.m_StartPoint += BoneState.m_Transform * Frame.m_Translation;
			BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation(Frame.m_Angle) * matrix2x2::scaling(Frame.m_Scale);
			BoneState.m_Alignment = Frame.m_Alignment;
		}
	}
	
	for(int i=0; i<pSkeletonAnimation->m_LayerAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath LayerPath = pSkeletonAnimation->m_LayerAnimations[i].m_LayerPath;
		int sId = SkeletonId;
		if(LayerPath.GetSource() == CAsset_Skeleton::CSubPath::SRC_PARENT && m_Skeletons[SkeletonId].m_Parent >= 0)
		{
			sId = m_Skeletons[SkeletonId].m_Parent;
		}
		int bId = LayerPath.GetId();
		
		if(sId < 0 || bId < 0 || sId >= m_Skeletons.size() || bId >= m_Skeletons[sId].m_Layers.size())
			continue;
		
		CSkeletonRenderer::CLayerState& LayerState = m_Skeletons[sId].m_Layers[bId];
		
		CAsset_SkeletonAnimation::CLayerAnimation::CFrame Frame;
		if(pSkeletonAnimation->m_LayerAnimations[i].GetFrame(Time, &Frame))
		{
			LayerState.m_Color = Frame.m_Color;
			LayerState.m_State = Frame.m_State;
		}
	}
}

void CSkeletonRenderer::AddSkin(CAssetPath Path, vec4 Color)
{
	m_Skins.add(CSkinState());
	m_Skins[m_Skins.size()-1].m_Path = Path;
	m_Skins[m_Skins.size()-1].m_Color = Color;
}

void CSkeletonRenderer::AddSkinWithSkeleton(CAssetPath Path, vec4 Color)
{
	CAsset_SkeletonSkin* pSkeletonSkin = AssetsManager()->GetAsset<CAsset_SkeletonSkin>(Path);
	if(!pSkeletonSkin)
		return;
	
	AddSkeleton(pSkeletonSkin->m_SkeletonPath);
	AddSkin(Path, Color);
}

void CSkeletonRenderer::FinalizeBone(int s, int b)
{
	CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[s].m_Bones[b];
	if(BoneState.m_Finalized)
		return;
	
	if(BoneState.m_ParentSkeleton >= 0 && BoneState.m_ParentBone >= 0)
	{
		FinalizeBone(BoneState.m_ParentSkeleton, BoneState.m_ParentBone);
		
		CSkeletonRenderer::CBoneState& ParentBoneState = m_Skeletons[BoneState.m_ParentSkeleton].m_Bones[BoneState.m_ParentBone];
		BoneState.m_Transform = ParentBoneState.m_Transform * BoneState.m_Transform;
		BoneState.m_StartPoint = ParentBoneState.m_StartPoint + (ParentBoneState.m_EndPoint - ParentBoneState.m_StartPoint)*BoneState.m_Anchor + ParentBoneState.m_Transform * BoneState.m_StartPoint;
	}
	
	switch(BoneState.m_Alignment)
	{
		case CAsset_SkeletonAnimation::BONEALIGN_AIM:
		{
			vec2 Orientation = BoneState.m_Transform * vec2(1.0f, 0.0f);
			BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation(-angle(Orientation));
			if(m_Aim.x < 0.0f)
				BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation((angle(m_Aim)-pi)) * matrix2x2::scaling(vec2(-1.0f, 1.0f));
			else
				BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation(angle(m_Aim));
			break;
		}
		case CAsset_SkeletonAnimation::BONEALIGN_MOTION:
		{
			vec2 Orientation = BoneState.m_Transform * vec2(1.0f, 0.0f);
			BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation(-angle(Orientation));
			if(m_Aim.x < 0.0f)
				BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation((angle(m_Motion)-pi)) * matrix2x2::scaling(vec2(-1.0f, 1.0f));
			else
				BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation(angle(m_Motion));
			break;
		}
		case CAsset_SkeletonAnimation::BONEALIGN_HOOK:
		{
			vec2 Orientation = BoneState.m_Transform * vec2(1.0f, 0.0f);
			BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation(-angle(Orientation));
			if(m_Aim.x < 0.0f)
				BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation((angle(m_Hook)-pi)) * matrix2x2::scaling(vec2(-1.0f, 1.0f));
			else
				BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation(angle(m_Hook));
			break;
		}
		case CAsset_SkeletonAnimation::BONEALIGN_WORLD:
		{
			vec2 Orientation = BoneState.m_Transform * vec2(1.0f, 0.0f);
			BoneState.m_Transform = BoneState.m_Transform * matrix2x2::rotation(angle(Orientation, vec2(1.0f, 0.0f)));
			break;
		}
	}
	
	BoneState.m_EndPoint = BoneState.m_StartPoint + BoneState.m_Transform * vec2(BoneState.m_Length, 0.0f);
	BoneState.m_Finalized = true;
}

void CSkeletonRenderer::Finalize()
{
	for(int s=0; s<m_Skeletons.size(); s++)
	{
		for(int b=0; b<m_Skeletons[s].m_Bones.size(); b++)
		{
			FinalizeBone(s, b);
		}
	}
}

void CSkeletonRenderer::RenderSkinsLayer(vec2 Position, float Size, int LayerSkeletonId, int LayerId)
{
	CLayerState& LayerState = m_Skeletons[LayerSkeletonId].m_Layers[LayerId];
	if(LayerState.m_State != CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE)
		return;
	
	for(int s=0; s<m_Skins.size(); s++)
	{
		CAsset_SkeletonSkin* pSkeletonSkin = AssetsManager()->GetAsset<CAsset_SkeletonSkin>(m_Skins[s].m_Path);
		if(!pSkeletonSkin)
			continue;
		
		int SkeletonId = -1;
		for(int sId=0; sId<m_Skeletons.size(); sId++)
		{
			if(m_Skeletons[sId].m_Path == pSkeletonSkin->m_SkeletonPath)
			{
				SkeletonId = sId;
				break;
			}
		}
		
		if(SkeletonId < 0)
			return;
		
		//Compute LayerPath
		CAsset_Skeleton::CSubPath LayerPath;
		if(pSkeletonSkin->m_SkeletonPath == m_Skeletons[LayerSkeletonId].m_Path)
		{
			LayerPath = CAsset_Skeleton::CSubPath::LocalLayer(LayerId);
		}
		else if(m_Skeletons[SkeletonId].m_Parent >= 0 && m_Skeletons[m_Skeletons[SkeletonId].m_Parent].m_Path == m_Skeletons[LayerSkeletonId].m_Path)
		{			
			LayerPath = CAsset_Skeleton::CSubPath::ParentLayer(LayerId);
		}
		else return;
		
		//Render sprites
		for(int i=0; i<pSkeletonSkin->m_Sprites.size(); i++)
		{
			if(!(pSkeletonSkin->m_Sprites[i].m_LayerPath == LayerPath))
				continue;
			
			CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(pSkeletonSkin->m_Sprites[i].m_SpritePath);
			if(!pSprite)
				continue;
			
			int sId = SkeletonId;
			if(pSkeletonSkin->m_Sprites[i].m_BonePath.GetSource() == CAsset_Skeleton::CSubPath::SRC_PARENT && m_Skeletons[SkeletonId].m_Parent >= 0)
			{
				sId = m_Skeletons[SkeletonId].m_Parent;
			}
			int bId = pSkeletonSkin->m_Sprites[i].m_BonePath.GetId();
		
			if(sId < 0 || bId < 0 || sId >= m_Skeletons.size() || bId >= m_Skeletons[sId].m_Bones.size())
				continue;
			
			float SizeX = pSprite->GetWidth();
			float SizeY = pSprite->GetHeight();
			
			if(SizeX > SizeY)
			{
				SizeY /= SizeX;
				SizeX = 1.0f;
			}
			else
			{
				SizeX /= SizeY;
				SizeY = 1.0f;
			}
			
			vec2 QuadPos = Position;
			vec2 DirX = vec2(SizeX*Size, 0.0f);
			vec2 DirY = vec2(0.0f, SizeY*Size);
			
			matrix2x2 Transform = matrix2x2::rotation(pSkeletonSkin->m_Sprites[i].m_Angle) * matrix2x2::scaling(pSkeletonSkin->m_Sprites[i].m_Scale);
			
			CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[sId].m_Bones[bId];

			vec2 LocalPos = BoneState.m_StartPoint + (BoneState.m_EndPoint - BoneState.m_StartPoint)*pSkeletonSkin->m_Sprites[i].m_Anchor + BoneState.m_Transform * pSkeletonSkin->m_Sprites[i].m_Translation;
			QuadPos += LocalPos * Size;
			Transform = BoneState.m_Transform * Transform;
			
			DirX = Transform * DirX/2.0f;
			DirY = Transform * DirY/2.0f;
			
			if(pSkeletonSkin->m_Sprites[i].m_Alignment == CAsset_SkeletonSkin::ALIGNEMENT_WORLD)
			{
				DirX = vec2(length(DirX), 0.0f);
				DirY = vec2(0.0f, length(DirY));
			}
			
			//UVs
			CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pSprite->GetImagePath());
			TUGraphics()->TextureSet(pSprite->GetImagePath());
			
			vec4 Color = m_Skins[s].m_Color * LayerState.m_Color;
			
			Graphics()->QuadsBegin();
			Graphics()->SetColor(Color.r*Color.a, Color.g*Color.a, Color.b*Color.a, Color.a);
			
			//Set Texture Coords
			if(pImage)
			{
				vec2 uvMin = vec2(pSprite->GetX()/(float)max(1, pImage->GetGridWidth()), pSprite->GetY()/(float)max(1, pImage->GetGridHeight()));
				vec2 uvMax = uvMin + vec2(pSprite->GetWidth()/(float)max(1, pImage->GetGridWidth()), pSprite->GetHeight()/(float)max(1, pImage->GetGridHeight()));
				
				Graphics()->QuadsSetSubsetFree(
					uvMin.x, uvMin.y,
					uvMax.x, uvMin.y,
					uvMin.x, uvMax.y,
					uvMax.x, uvMax.y);
			}
				
			IGraphics::CFreeformItem Freeform(
				QuadPos.x - DirX.x - DirY.x, QuadPos.y - DirX.y - DirY.y,
				QuadPos.x + DirX.x - DirY.x, QuadPos.y + DirX.y - DirY.y,
				QuadPos.x - DirX.x + DirY.x, QuadPos.y - DirX.y + DirY.y,
				QuadPos.x + DirX.x + DirY.x, QuadPos.y + DirX.y + DirY.y);
			Graphics()->QuadsDrawFreeform(&Freeform, 1);
			
			Graphics()->QuadsEnd();
		}
	}
}

void CSkeletonRenderer::RenderSkins(vec2 Position, float Size)
{
	for(int s=0; s<m_Skeletons.size(); s++)
	{
		for(int b=0; b<m_Skeletons[s].m_Layers.size(); b++)
		{
			RenderSkinsLayer(Position, Size, s, b);
		}
	}
}

void CSkeletonRenderer::RenderBone(vec2 Position, float Size, CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath)
{
	for(int s=0; s<m_Skeletons.size(); s++)
	{
		if(!(m_Skeletons[s].m_Path == SkeletonPath))
			continue;
		
		int b = BonePath.GetId();
		if(b >= 0 && b < m_Skeletons[s].m_Bones.size())
		{
			CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[s].m_Bones[b];
			
			Graphics()->TextureClear();
			Graphics()->QuadsBegin();
			Graphics()->SetColor(BoneState.m_Color.r*BoneState.m_Color.a, BoneState.m_Color.g*BoneState.m_Color.a, BoneState.m_Color.b*BoneState.m_Color.a, BoneState.m_Color.a);
			
			vec2 StartPoint = Position + BoneState.m_StartPoint*Size;
			vec2 EndPoint = Position + BoneState.m_EndPoint*Size;
			vec2 Dir = normalize(EndPoint - StartPoint);
			
			float StartPointLength = min(Size*4.0f, distance(StartPoint, EndPoint)/2.0f);
			vec2 TopPoint = StartPoint + vec2(Dir.x - Dir.y, Dir.x + Dir.y)*StartPointLength/sqrt(2.0f);
			vec2 BottomPoint = StartPoint + vec2(Dir.x + Dir.y, - Dir.x + Dir.y)*StartPointLength/sqrt(2.0f);
			
			IGraphics::CFreeformItem Freeform(
				StartPoint.x, StartPoint.y,
				BottomPoint.x, BottomPoint.y,
				TopPoint.x, TopPoint.y,
				EndPoint.x, EndPoint.y);
			Graphics()->QuadsDrawFreeform(&Freeform, 1);
			Graphics()->QuadsEnd();
		}
		
		return;
	}
}

void CSkeletonRenderer::RenderBoneOutline(vec2 Position, float Size, CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath)
{
	for(int s=0; s<m_Skeletons.size(); s++)
	{
		if(!(m_Skeletons[s].m_Path == SkeletonPath))
			continue;
		
		int b = BonePath.GetId();
		if(b >= 0 && b < m_Skeletons[s].m_Bones.size())
		{
			CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[s].m_Bones[b];
			
			Graphics()->TextureClear();
			Graphics()->LinesBegin();
			Graphics()->SetColor(BoneState.m_Color.r*BoneState.m_Color.a, BoneState.m_Color.g*BoneState.m_Color.a, BoneState.m_Color.b*BoneState.m_Color.a, BoneState.m_Color.a);
			
			vec2 StartPoint = Position + BoneState.m_StartPoint*Size;
			vec2 EndPoint = Position + BoneState.m_EndPoint*Size;
			vec2 Dir = normalize(EndPoint - StartPoint);
			
			float StartPointLength = min(Size*4.0f, distance(StartPoint, EndPoint)/2.0f);
			vec2 TopPoint = StartPoint + vec2(Dir.x - Dir.y, Dir.x + Dir.y)*StartPointLength/sqrt(2.0f);
			vec2 BottomPoint = StartPoint + vec2(Dir.x + Dir.y, - Dir.x + Dir.y)*StartPointLength/sqrt(2.0f);
			
			IGraphics::CLineItem Lines[4] = {
					IGraphics::CLineItem(StartPoint.x, StartPoint.y, TopPoint.x, TopPoint.y),
					IGraphics::CLineItem(TopPoint.x, TopPoint.y, EndPoint.x, EndPoint.y),
					IGraphics::CLineItem(EndPoint.x, EndPoint.y, BottomPoint.x, BottomPoint.y),
					IGraphics::CLineItem(BottomPoint.x, BottomPoint.y, StartPoint.x, StartPoint.y)
			};
			Graphics()->LinesDraw(Lines, 4);
			Graphics()->LinesEnd();
		}
		
		return;
	}
}

void CSkeletonRenderer::RenderBones(vec2 Position, float Size)
{
	Graphics()->TextureClear();
	Graphics()->QuadsBegin();
	
	for(int s=0; s<m_Skeletons.size(); s++)
	{
		for(int b=0; b<m_Skeletons[s].m_Bones.size(); b++)
		{
			CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[s].m_Bones[b];
			
			Graphics()->SetColor(BoneState.m_Color.r*BoneState.m_Color.a, BoneState.m_Color.g*BoneState.m_Color.a, BoneState.m_Color.b*BoneState.m_Color.a, BoneState.m_Color.a);
				
			vec2 StartPoint = Position + BoneState.m_StartPoint*Size;
			vec2 EndPoint = Position + BoneState.m_EndPoint*Size;
			vec2 Dir = normalize(EndPoint - StartPoint);
			
			float StartPointLength = min(Size*4.0f, distance(StartPoint, EndPoint)/2.0f);
			vec2 TopPoint = StartPoint + vec2(Dir.x - Dir.y, Dir.x + Dir.y)*StartPointLength/sqrt(2.0f);
			vec2 BottomPoint = StartPoint + vec2(Dir.x + Dir.y, - Dir.x + Dir.y)*StartPointLength/sqrt(2.0f);
			
			IGraphics::CFreeformItem Freeform(
				StartPoint.x, StartPoint.y,
				BottomPoint.x, BottomPoint.y,
				TopPoint.x, TopPoint.y,
				EndPoint.x, EndPoint.y);
			Graphics()->QuadsDrawFreeform(&Freeform, 1);
		}
	}
	
	Graphics()->QuadsEnd();
}

bool CSkeletonRenderer::BonePicking(vec2 Position, float Size, vec2 Point, CAssetPath& SkeletonPath, CAsset_Skeleton::CSubPath& BonePath)
{
	vec2 Vertices[4];
	
	for(int s=m_Skeletons.size()-1; s>=0; s--)
	{
		for(int b=m_Skeletons[s].m_Bones.size()-1; b>=0; b--)
		{
			CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[s].m_Bones[b];
			
			vec2 StartPoint = Position + BoneState.m_StartPoint*Size;
			vec2 EndPoint = Position + BoneState.m_EndPoint*Size;
			vec2 Dir = normalize(EndPoint - StartPoint);
			
			float StartPointLength = min(Size*4.0f, distance(StartPoint, EndPoint)/2.0f);
			vec2 TopPoint = StartPoint + vec2(Dir.x - Dir.y, Dir.x + Dir.y)*StartPointLength/sqrt(2.0f);
			vec2 BottomPoint = StartPoint + vec2(Dir.x + Dir.y, - Dir.x + Dir.y)*StartPointLength/sqrt(2.0f);
			
			Vertices[0] = StartPoint - Point;
			Vertices[1] = BottomPoint - Point;
			Vertices[2] = EndPoint - Point;
			Vertices[3] = TopPoint - Point;
			
			bool isInside = true;
			for(int i=0; i<4; i++)
			{
				vec2 Edge = Vertices[(i+1)%4] - Vertices[i];
				if(Edge.x * Vertices[i].y - Edge.y * Vertices[i].x > 0.0f)
				{
					isInside = false;
					break;
				}
			}
			
			if(isInside)
			{
				SkeletonPath = m_Skeletons[s].m_Path;
				BonePath = CAsset_Skeleton::CSubPath::LocalBone(b);
				return true;
			}
		}
	}
	
	return false;
}

bool CSkeletonRenderer::GetLocalAxis(vec2 Position, float Size, CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath, vec2& Origin, vec2& AxisX, vec2& AxisY)
{
	for(int s=0; s<m_Skeletons.size(); s++)
	{
		if(!(m_Skeletons[s].m_Path == SkeletonPath))
			continue;
		
		int b = BonePath.GetId();
		
		if(b < 0 || b >= m_Skeletons[s].m_Bones.size())
			return false;
		
		CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[s].m_Bones[b];
			
		vec2 StartPoint = Position + BoneState.m_StartPoint*Size;
		vec2 EndPoint = Position + BoneState.m_EndPoint*Size;
		vec2 Dir = normalize(EndPoint - StartPoint);
		
		Origin = StartPoint;
		AxisX = Dir/Size;
		AxisY = vec2(Dir.y, Dir.x)/Size;
			
		return true;
	}
	
	return false;
}

bool CSkeletonRenderer::GetParentAxis(vec2 Position, float Size, CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath, vec2& Origin, vec2& AxisX, vec2& AxisY)
{
	for(int s=0; s<m_Skeletons.size(); s++)
	{
		if(!(m_Skeletons[s].m_Path == SkeletonPath))
			continue;
		
		int b = BonePath.GetId();
		
		if(b < 0 || b >= m_Skeletons[s].m_Bones.size())
			return false;
		
		if(m_Skeletons[s].m_Bones[b].m_ParentSkeleton >= 0 && m_Skeletons[s].m_Bones[b].m_ParentBone >= 0)
		{
			int pS = m_Skeletons[s].m_Bones[b].m_ParentSkeleton;
			int pB = m_Skeletons[s].m_Bones[b].m_ParentBone;
			
			CSkeletonRenderer::CBoneState& BoneState = m_Skeletons[pS].m_Bones[pB];
			
			vec2 StartPoint = Position + BoneState.m_StartPoint*Size;
			vec2 EndPoint = Position + BoneState.m_EndPoint*Size;
			vec2 Dir = normalize(EndPoint - StartPoint);
			
			Origin = StartPoint;
			AxisX = Dir/Size;
			AxisY = vec2(Dir.y, Dir.x)/Size;
		}
		else
		{
			Origin = Position;
			AxisX = vec2(1.0f, 0.0f)/Size;
			AxisY = vec2(0.0f, 1.0f)/Size;
		}
		return true;
	}
	
	return false;
}

}
