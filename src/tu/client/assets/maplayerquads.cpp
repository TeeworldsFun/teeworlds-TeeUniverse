#include "maplayerquads.h"
#include "skeleton.h"
#include "skeletonanimation.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

void CAsset_MapLayerQuads::CQuad::GetTransform(CAssetsManager* pAssetsManager, float Time, matrix2x2* pMatrix, vec2* pPosition) const
{
	*pPosition = m_Pivot;
	
	float Angle = m_Angle;
	vec2 Scale = m_Size;
	
	if(!m_AnimationPath.IsNull())
	{
		const CAsset_SkeletonAnimation* pAnimation = pAssetsManager->GetAsset<CAsset_SkeletonAnimation>(m_AnimationPath);
		if(pAnimation)
		{
			for(int a=0; a<pAnimation->m_BoneAnimations.size(); a++)
			{
				tu::CAsset_Skeleton::CSubPath BonePath = pAnimation->m_BoneAnimations[a].m_BonePath;
				if(BonePath.GetSource() == tu::CAsset_Skeleton::CSubPath::SRC_LOCAL && BonePath.GetId() == 0)
				{
					tu::CAsset_SkeletonAnimation::CBoneAnimation::CFrame Frame;
					if(pAnimation->m_BoneAnimations[a].GetFrame(Time, &Frame))
					{
						*pPosition += Frame.m_Translation;
						Angle += Frame.m_Angle;
						Scale *= Frame.m_Scale;
					}
					break;
				}
			}
		}
	}
	
	*pMatrix = matrix2x2::rotation(Angle)*matrix2x2::scaling(Scale);
}

void CAsset_MapLayerQuads::CQuad::GetDrawState(CAssetsManager* pAssetsManager, float Time, vec4* pColor, int* pState) const
{
	*pColor = 1.0f;
	*pState = CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE;
	
	if(!m_AnimationPath.IsNull())
	{
		const CAsset_SkeletonAnimation* pAnimation = pAssetsManager->GetAsset<CAsset_SkeletonAnimation>(m_AnimationPath);
		if(pAnimation)
		{
			for(int a=0; a<pAnimation->m_LayerAnimations.size(); a++)
			{
				tu::CAsset_Skeleton::CSubPath LayerPath = pAnimation->m_LayerAnimations[a].m_LayerPath;
				if(LayerPath.GetSource() == tu::CAsset_Skeleton::CSubPath::SRC_LOCAL && LayerPath.GetId() == 0)
				{
					tu::CAsset_SkeletonAnimation::CLayerAnimation::CFrame Frame;
					if(pAnimation->m_LayerAnimations[a].GetFrame(Time, &Frame))
					{
						*pColor *= Frame.m_Color;
						*pState = Frame.m_State;
					}
					break;
				}
			}
		}
	}
}

CAsset_MapLayerQuads::CAsset_MapLayerQuads()
{
	
}

void CAsset_MapLayerQuads::GetQuadTransform(CSubPath SubPath, float Time, matrix2x2* pMatrix, vec2* pPosition)
{
	if(!IsValidQuad(SubPath))
		return;
	
	m_Quads[SubPath.GetId()].GetTransform(AssetsManager(), Time, pMatrix, pPosition);
}

void CAsset_MapLayerQuads::GetQuadDrawState(CSubPath SubPath, float Time, vec4* pColor, int* pState)
{
	if(!IsValidQuad(SubPath))
		return;
	
	m_Quads[SubPath.GetId()].GetDrawState(AssetsManager(), Time, pColor, pState);
}

/* IO *****************************************************************/

void CAsset_MapLayerQuads::InitFromAssetsFile(CDataFileReader* pFileReader, const CAsset_MapLayerQuads::CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
	
	m_ImagePath = pItem->m_ImagePath;
	
	// load quads
	const CStorageType::CQuad* pQuads = static_cast<CStorageType::CQuad*>(pFileReader->GetData(pItem->m_QuadsData));
	for(int i=0; i<pItem->m_NumQuads; i++)
	{
		m_Quads.add(CQuad());
		CQuad& Quad = m_Quads[m_Quads.size()-1];
		
		Quad.m_Pivot.x = pQuads[i].m_PivotX;
		Quad.m_Pivot.y = pQuads[i].m_PivotY;
		Quad.m_Size.x = pQuads[i].m_SizeX;
		Quad.m_Size.y = pQuads[i].m_SizeY;
		Quad.m_Angle = pQuads[i].m_Angle;
		Quad.m_AnimationPath = pQuads[i].m_AnimationPath;
		
		for(int p=0; p<4; p++)
		{
			Quad.m_Vertices[p].x = pQuads[i].m_Vertices[p*2+0];
			Quad.m_Vertices[p].y = pQuads[i].m_Vertices[p*2+1];
			Quad.m_UVs[p].x = pQuads[i].m_UVs[p*2+0];
			Quad.m_UVs[p].y = pQuads[i].m_UVs[p*2+1];
			Quad.m_Colors[p] = IntToColor(pQuads[i].m_Colors[p]);
		}
	}
}

void CAsset_MapLayerQuads::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CAsset_MapLayerQuads::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_ImagePath = m_ImagePath;
	
	//save quads
	{
		CStorageType::CQuad* pQuads = new CStorageType::CQuad[m_Quads.size()];
		for(int i=0; i<m_Quads.size(); i++)
		{
			pQuads[i].m_PivotX = m_Quads[i].m_Pivot.x;
			pQuads[i].m_PivotY = m_Quads[i].m_Pivot.y;
			pQuads[i].m_SizeX = m_Quads[i].m_Size.x;
			pQuads[i].m_SizeY = m_Quads[i].m_Size.y;
			pQuads[i].m_Angle = m_Quads[i].m_Angle;
			pQuads[i].m_AnimationPath = m_Quads[i].m_AnimationPath;
			
			for(int p=0; p<4; p++)
			{
				pQuads[i].m_Vertices[p*2+0] = m_Quads[i].m_Vertices[p].x;
				pQuads[i].m_Vertices[p*2+1] = m_Quads[i].m_Vertices[p].y;
				pQuads[i].m_UVs[p*2+0] = m_Quads[i].m_UVs[p].x;
				pQuads[i].m_UVs[p*2+1] = m_Quads[i].m_UVs[p].y;
				pQuads[i].m_Colors[p] = ColorToInt(m_Quads[i].m_Colors[p]);
			}
		}
		Item.m_NumQuads = m_Quads.size();
		Item.m_QuadsData = pFileWriter->AddData(Item.m_NumQuads * sizeof(CStorageType::CQuad), pQuads);
		delete[] pQuads;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE ASSETPATH ****************************************************/
	
template<>
CAssetPath CAsset_MapLayerQuads::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, IMAGEPATH, GetImagePath());
		TU_ASSET_GET_FUNC_IMPL_ARRAY_VARIABLE(CAssetPath, QUAD_ANIMATIONPATH, TYPE_QUAD, m_Quads, m_AnimationPath);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_MapLayerQuads::SetValue(int ValueType, int PathInt, CAssetPath Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, IMAGEPATH, m_ImagePath);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_VARIABLE(CAssetPath, QUAD_ANIMATIONPATH, TYPE_QUAD, m_Quads, m_AnimationPath);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

/* VALUE FLOAT ********************************************************/

template<>
float CAsset_MapLayerQuads::GetValue(int ValueType, int PathInt, float DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_POSITION_X, TYPE_QUAD, m_Quads, m_Pivot.x);
		TU_ASSET_GET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_POSITION_Y, TYPE_QUAD, m_Quads, m_Pivot.y);
		TU_ASSET_GET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_SIZE_X, TYPE_QUAD, m_Quads, m_Size.x);
		TU_ASSET_GET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_SIZE_Y, TYPE_QUAD, m_Quads, m_Size.y);
		TU_ASSET_GET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_ANGLE, TYPE_QUAD, m_Quads, m_Angle);
		TU_ASSET_GET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(float, QUAD_POINT_POSITION_X, TYPE_QUAD, m_Quads, m_Vertices, CSubPath::POINT_VERTEX0, 4, x);
		TU_ASSET_GET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(float, QUAD_POINT_POSITION_Y, TYPE_QUAD, m_Quads, m_Vertices, CSubPath::POINT_VERTEX0, 4, y);
		TU_ASSET_GET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(float, QUAD_POINT_UV_X, TYPE_QUAD, m_Quads, m_UVs, CSubPath::POINT_VERTEX0, 4, x);
		TU_ASSET_GET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(float, QUAD_POINT_UV_Y, TYPE_QUAD, m_Quads, m_UVs, CSubPath::POINT_VERTEX0, 4, y);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(float)
}
	
template<>
bool CAsset_MapLayerQuads::SetValue(int ValueType, int PathInt, float Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_POSITION_X, TYPE_QUAD, m_Quads, m_Pivot.x);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_POSITION_Y, TYPE_QUAD, m_Quads, m_Pivot.y);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_SIZE_X, TYPE_QUAD, m_Quads, m_Size.x);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_SIZE_Y, TYPE_QUAD, m_Quads, m_Size.y);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_VARIABLE(float, QUAD_ANGLE, TYPE_QUAD, m_Quads, m_Angle);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(float, QUAD_POINT_POSITION_X, TYPE_QUAD, m_Quads, m_Vertices, CSubPath::POINT_VERTEX0, 4, x);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(float, QUAD_POINT_POSITION_Y, TYPE_QUAD, m_Quads, m_Vertices, CSubPath::POINT_VERTEX0, 4, y);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(float, QUAD_POINT_UV_X, TYPE_QUAD, m_Quads, m_UVs, CSubPath::POINT_VERTEX0, 4, x);
		TU_ASSET_SET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(float, QUAD_POINT_UV_Y, TYPE_QUAD, m_Quads, m_UVs, CSubPath::POINT_VERTEX0, 4, y);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(float)
}

/* VALUE VEC4 *********************************************************/
	
template<>
vec4 CAsset_MapLayerQuads::GetValue<vec4>(int ValueType, int PathInt, vec4 DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_ARRAY_RARRAY(vec4, QUAD_POINT_COLOR, TYPE_QUAD, m_Quads, m_Colors, CSubPath::POINT_VERTEX0, 4);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(vec4)
}
	
template<>
bool CAsset_MapLayerQuads::SetValue<vec4>(int ValueType, int PathInt, vec4 Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_ARRAY_RARRAY(vec4, QUAD_POINT_COLOR, TYPE_QUAD, m_Quads, m_Colors, CSubPath::POINT_VERTEX0, 4);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(vec4)
}

}
