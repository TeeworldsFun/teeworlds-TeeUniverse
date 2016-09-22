#ifndef TU_CLIENT_SKELETONRENDERER_H
#define TU_CLIENT_SKELETONRENDERER_H

#include <base/tl/array.h>
#include <tu/client/assetsmanager.h>

class IGraphics;

namespace tu
{

class CSkeletonRenderer : public CKernel::CGuest
{
public:
	enum
	{
		ADDDEFAULTSKIN_NO=0,
		ADDDEFAULTSKIN_ONLYPARENT,
		ADDDEFAULTSKIN_YES,
	};
	
protected:
	class CBoneState
	{
	public:
		int m_ParentSkeleton;
		int m_ParentBone;
		float m_Length;
		float m_Anchor;
		matrix2x2 m_Transform;
		vec2 m_StartPoint;
		vec2 m_EndPoint;
		vec4 m_Color;
		int m_Alignment;
		
		bool m_Finalized;
	};
	
	class CLayerState
	{
	public:
		int m_State;
		vec4 m_Color;
	};
	
	class CSkeletonState
	{
	public:
		CAssetPath m_Path;
		int m_Parent;
		array<CBoneState> m_Bones;
		array<CLayerState> m_Layers;
	};
	
	class CSkinState
	{
	public:
		CAssetPath m_Path;
		vec4 m_Color;
	};

protected:	
	array<CSkeletonState> m_Skeletons;
	array<CSkinState> m_Skins;
	int m_NumLayers;
	
	vec2 m_Aim;
	vec2 m_Motion;
	vec2 m_Hook;
	
public:
	CSkeletonRenderer(CKernel* pKernel);
	
	void SetAim(vec2 Aim) { m_Aim = Aim; }
	void SetMotion(vec2 Motion) { m_Motion = Motion; }
	void SetHook(vec2 Hook) { m_Hook = Hook; }
	
	void AddSkeleton(CAssetPath SkeletonPath);
	void AddSkeletonWithParents(CAssetPath SkeletonPath, int AddDefaultSkins = ADDDEFAULTSKIN_NO);
	void ApplyAnimation(CAssetPath SkeletonAnimationPath, float Time);
	void AddSkin(CAssetPath SkeletonSkinPath, vec4 Color);
	void AddSkinWithSkeleton(CAssetPath SkeletonSkinPath, vec4 Color);
	void FinalizeBone(int s, int b);
	void Finalize();
	void RenderBone(vec2 Position, float Size, CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath);
	void RenderBoneOutline(vec2 Position, float Size, CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath);
	void RenderBones(vec2 Position, float Size);
	void RenderSkinsLayer(vec2 Position, float Size, int LayerSkeletonId, int LayerId);
	void RenderSkins(vec2 Position, float Size);
	bool BonePicking(vec2 Position, float Size, vec2 Point, CAssetPath& SkeletonPath, CAsset_Skeleton::CSubPath& BonePath);
	bool GetLocalAxis(vec2 Position, float Size, CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath, vec2& Origin, vec2& AxisX, vec2& AxisY);
	bool GetParentAxis(vec2 Position, float Size, CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath, vec2& Origin, vec2& AxisX, vec2& AxisY);
};

}

#endif
