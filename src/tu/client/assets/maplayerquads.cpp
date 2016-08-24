#include "maplayerquads.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_MapLayerQuads::CAsset_MapLayerQuads()
{
	
}

/* IO *****************************************************************/

void CAsset_MapLayerQuads::InitFromAssetsFile(CAssetsManager* pAssetsManager, tu::IAssetsFile* pAssetsFile, const CAsset_MapLayerQuads::CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	// load quads
	const CStorageType::CQuad* pQuads = static_cast<CStorageType::CQuad*>(pAssetsFile->GetData(pItem->m_QuadsData));
	for(int i=0; i<pItem->m_NumQuads; i++)
	{
		m_Quads.add(CQuad());
		CQuad& Quad = m_Quads[m_Quads.size()-1];
		
		Quad.m_Pivot.x = pQuads[i].m_PivotX;
		Quad.m_Pivot.y = pQuads[i].m_PivotY;
		Quad.m_Size.x = pQuads[i].m_SizeX;
		Quad.m_Size.y = pQuads[i].m_SizeY;
		Quad.m_Angle = pQuads[i].m_Angle;
		
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
CAssetPath CAsset_MapLayerQuads::GetValue<CAssetPath>(int ValueType, int PathInt, CAssetPath DefaultValue)
{
	switch(ValueType)
	{
		case IMAGEPATH:
			return m_ImagePath;
		default:
			return CAsset::GetValue<CAssetPath>(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_MapLayerQuads::SetValue<CAssetPath>(int ValueType, int Path, CAssetPath Value)
{
	switch(ValueType)
	{
		case IMAGEPATH:
			m_ImagePath = Value;
			return true;
	}
	
	return CAsset::SetValue<CAssetPath>(ValueType, Path, Value);
}

/* VALUE FLOAT ********************************************************/

template<>
float CAsset_MapLayerQuads::GetValue<float>(int ValueType, int PathInt, float DefaultValue)
{
	CSubPath Path(PathInt);
	
	switch(ValueType)
	{
		case QUAD_PIVOT_POSITION_X:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_Pivot.x;
			else
				return DefaultValue;
		case QUAD_PIVOT_POSITION_Y:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_Pivot.y;
			else
				return DefaultValue;
		case QUAD_SIZE_X:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_Size.x;
			else
				return DefaultValue;
		case QUAD_SIZE_Y:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_Size.y;
			else
				return DefaultValue;
		case QUAD_ANGLE:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_Angle;
			else
				return DefaultValue;
		case QUAD_POINT0_POSITION_X:
		case QUAD_POINT1_POSITION_X:
		case QUAD_POINT2_POSITION_X:
		case QUAD_POINT3_POSITION_X:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_Vertices[ValueType-QUAD_POINT0_POSITION_X].x;
			else
				return DefaultValue;
		case QUAD_POINT0_POSITION_Y:
		case QUAD_POINT1_POSITION_Y:
		case QUAD_POINT2_POSITION_Y:
		case QUAD_POINT3_POSITION_Y:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_Vertices[ValueType-QUAD_POINT0_POSITION_Y].y;
			else
				return DefaultValue;
		case QUAD_POINT0_UV_X:
		case QUAD_POINT1_UV_X:
		case QUAD_POINT2_UV_X:
		case QUAD_POINT3_UV_X:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_UVs[ValueType-QUAD_POINT0_UV_X].x;
			else
				return DefaultValue;
		case QUAD_POINT0_UV_Y:
		case QUAD_POINT1_UV_Y:
		case QUAD_POINT2_UV_Y:
		case QUAD_POINT3_UV_Y:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_UVs[ValueType-QUAD_POINT0_UV_Y].y;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue<float>(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_MapLayerQuads::SetValue<float>(int ValueType, int PathInt, float Value)
{
	CSubPath Path(PathInt);
	
	switch(ValueType)
	{
		case QUAD_PIVOT_POSITION_X:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Pivot.x = Value;
				return true;
			}
			else return false;
		case QUAD_PIVOT_POSITION_Y:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Pivot.y = Value;
				return true;
			}
			else return false;
		case QUAD_SIZE_X:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Size.x = Value;
				return true;
			}
			else return false;
		case QUAD_SIZE_Y:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Size.y = Value;
				return true;
			}
			else return false;
		case QUAD_ANGLE:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Angle = Value;
				return true;
			}
			else return false;
		case QUAD_POINT0_POSITION_X:
		case QUAD_POINT1_POSITION_X:
		case QUAD_POINT2_POSITION_X:
		case QUAD_POINT3_POSITION_X:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Vertices[ValueType-QUAD_POINT0_POSITION_X].x = Value;
				return true;
			}
			else return false;
		case QUAD_POINT0_POSITION_Y:
		case QUAD_POINT1_POSITION_Y:
		case QUAD_POINT2_POSITION_Y:
		case QUAD_POINT3_POSITION_Y:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Vertices[ValueType-QUAD_POINT0_POSITION_X].y = Value;
				return true;
			}
			else return false;
		case QUAD_POINT0_UV_X:
		case QUAD_POINT1_UV_X:
		case QUAD_POINT2_UV_X:
		case QUAD_POINT3_UV_X:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Vertices[ValueType-QUAD_POINT0_UV_X].x = Value;
				return true;
			}
			else return false;
		case QUAD_POINT0_UV_Y:
		case QUAD_POINT1_UV_Y:
		case QUAD_POINT2_UV_Y:
		case QUAD_POINT3_UV_Y:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Vertices[ValueType-QUAD_POINT0_UV_Y].y = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue<float>(ValueType, PathInt, Value);
}

/* VALUE VEC4 *********************************************************/
	
template<>
vec4 CAsset_MapLayerQuads::GetValue<vec4>(int ValueType, int PathInt, vec4 DefaultValue)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case QUAD_POINT0_COLOR:
		case QUAD_POINT1_COLOR:
		case QUAD_POINT2_COLOR:
		case QUAD_POINT3_COLOR:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
				return m_Quads[Path.GetId()].m_Colors[ValueType-QUAD_POINT0_COLOR];
			else
				return DefaultValue;
		default:
			return CAsset::GetValue<vec4>(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_MapLayerQuads::SetValue<vec4>(int ValueType, int PathInt, vec4 Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case QUAD_POINT0_COLOR:
		case QUAD_POINT1_COLOR:
		case QUAD_POINT2_COLOR:
		case QUAD_POINT3_COLOR:
			if(Path.GetType() == CSubPath::TYPE_QUAD && Path.GetId() >= 0 && Path.GetId() < m_Quads.size())
			{
				m_Quads[Path.GetId()].m_Colors[ValueType-QUAD_POINT0_COLOR] = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue<vec4>(ValueType, PathInt, Value);
}

}
