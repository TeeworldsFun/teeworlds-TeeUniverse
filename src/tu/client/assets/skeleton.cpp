#include "skeleton.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_Skeleton::CAsset_Skeleton() :
	m_ParentPath(CAssetPath::Null())
{
	
}
	
void CAsset_Skeleton::AddBone(const CAsset_Skeleton::CBone& Bone)
{
	m_Bones.add(Bone);
}
	
CAsset_Skeleton::CBone& CAsset_Skeleton::AddBone()
{
	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "bone%d", m_Bones.size());
	m_Bones.add(CAsset_Skeleton::CBone());
	return m_Bones[m_Bones.size()-1].Name(aBuf);
}
	
CAsset_Skeleton::CLayer& CAsset_Skeleton::AddLayer()
{
	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "layer%d", m_Layers.size());
	m_Layers.add(CAsset_Skeleton::CLayer());
	return m_Layers[m_Layers.size()-1].Name(aBuf);
}

/* IO *****************************************************************/

void CAsset_Skeleton::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// load name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
				
	// load info
	m_ParentPath = pItem->m_ParentPath;
	m_DefaultSkinPath = pItem->m_DefaultSkinPath;
	
	// load bones
	const CStorageType::CBone* pBones = static_cast<CStorageType::CBone*>(pFileReader->GetData(pItem->m_BonesData));
	for(int i=0; i<pItem->m_NumBones; i++)
	{
		m_Bones.add(CBone());
		CBone& Bone = m_Bones[m_Bones.size()-1];
		
		Bone.m_ParentPath = pBones[i].m_ParentPath;
		Bone.m_Length = pBones[i].m_Length;
		Bone.m_Anchor = pBones[i].m_Anchor;
		Bone.m_Translation = vec2(pBones[i].m_TranslationX, pBones[i].m_TranslationY);
		Bone.m_Scale = vec2(pBones[i].m_ScaleX, pBones[i].m_ScaleY);
		Bone.m_Angle = pBones[i].m_Angle;
		str_copy(Bone.m_aName, pBones[i].m_aName, sizeof(Bone.m_aName));
		Bone.m_Color = tu::IntToColor(pBones[i].m_Color);
	}
	
	// load layers
	const CStorageType::CLayer* pLayers = static_cast<CStorageType::CLayer*>(pFileReader->GetData(pItem->m_LayersData));
	for(int i=0; i<pItem->m_NumLayers; i++)
	{
		m_Layers.add(CLayer());
		CLayer& Layer = m_Layers[m_Layers.size()-1];
		
		str_copy(Layer.m_aName, pLayers[i].m_aName, sizeof(Layer.m_aName));
	}
}

void CAsset_Skeleton::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);

	Item.m_ParentPath = m_ParentPath;
	Item.m_DefaultSkinPath = m_DefaultSkinPath;
	
	{
		CStorageType::CBone* pBones = new CStorageType::CBone[m_Bones.size()];
		for(int i=0; i<m_Bones.size(); i++)
		{
			pBones[i].m_ParentPath = m_Bones[i].m_ParentPath;
			pBones[i].m_Length = m_Bones[i].m_Length;
			pBones[i].m_Anchor = m_Bones[i].m_Anchor;
			pBones[i].m_TranslationX = m_Bones[i].m_Translation.x;
			pBones[i].m_TranslationY = m_Bones[i].m_Translation.y;
			pBones[i].m_ScaleX = m_Bones[i].m_Scale.x;
			pBones[i].m_ScaleY = m_Bones[i].m_Scale.y;
			pBones[i].m_Angle = m_Bones[i].m_Angle;
			str_copy(pBones[i].m_aName, m_Bones[i].m_aName, sizeof(pBones[i].m_aName));
			pBones[i].m_Color = ColorToInt(m_Bones[i].m_Color);
		}
		Item.m_NumBones = m_Bones.size();
		Item.m_BonesData = pFileWriter->AddData(Item.m_NumBones * sizeof(CStorageType::CBone), pBones);
		delete[] pBones;
	}
	
	{
		CStorageType::CLayer* pLayers = new CStorageType::CLayer[m_Layers.size()];
		for(int i=0; i<m_Layers.size(); i++)
		{
			str_copy(pLayers[i].m_aName, m_Layers[i].m_aName, sizeof(pLayers[i].m_aName));
		}
		Item.m_NumLayers = m_Layers.size();
		Item.m_LayersData = pFileWriter->AddData(Item.m_NumLayers * sizeof(CStorageType::CLayer), pLayers);
		delete[] pLayers;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE FLOAT ********************************************************/

template<>
float CAsset_Skeleton::GetValue(int ValueType, int PathInt, float DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case BONE_LENGTH:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_Length;
			else
				return DefaultValue;
		case BONE_ANCHOR:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_Anchor;
			else
				return DefaultValue;
		case BONE_TRANSLATION_X:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_Translation.x;
			else
				return DefaultValue;
		case BONE_TRANSLATION_Y:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_Translation.y;
			else
				return DefaultValue;
		case BONE_SCALE_X:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_Scale.x;
			else
				return DefaultValue;
		case BONE_SCALE_Y:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_Scale.y;
			else
				return DefaultValue;
		case BONE_ANGLE:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_Angle;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_Skeleton::SetValue(int ValueType, int PathInt, float Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case BONE_LENGTH:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_Length = Value;
				return true;
			}
			else return false;
		case BONE_ANCHOR:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_Anchor = Value;
				return true;
			}
			else return false;
		case BONE_TRANSLATION_X:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_Translation.x = Value;
				return true;
			}
			else return false;
		case BONE_TRANSLATION_Y:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_Translation.y = Value;
				return true;
			}
			else return false;
		case BONE_SCALE_X:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_Scale.x = Value;
				return true;
			}
			else return false;
		case BONE_SCALE_Y:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_Scale.y = Value;
				return true;
			}
			else return false;
		case BONE_ANGLE:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_Angle = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue(ValueType, PathInt, Value);
}

/* VALUE VEC4 *********************************************************/
	
template<>
vec4 CAsset_Skeleton::GetValue<vec4>(int ValueType, int PathInt, vec4 DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case BONE_COLOR:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_Color;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue<vec4>(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_Skeleton::SetValue<vec4>(int ValueType, int PathInt, vec4 Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case BONE_COLOR:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_Color = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue<vec4>(ValueType, PathInt, Value);
}

/* VALUE STRING *******************************************************/

template<>
const char* CAsset_Skeleton::GetValue(int ValueType, int PathInt, const char* DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case BONE_NAME:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_aName;
			else
				return DefaultValue;
		case LAYER_NAME:
			if(Path.GetType() == CSubPath::TYPE_LAYER && Path.GetId() >= 0 && Path.GetId() < m_Layers.size())
				return m_Layers[Path.GetId()].m_aName;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_Skeleton::SetValue(int ValueType, int PathInt, const char* pText)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case BONE_NAME:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				str_copy(m_Bones[Path.GetId()].m_aName, pText, sizeof(m_Bones[Path.GetId()].m_aName));
				return true;
			}
			else return false;
		case LAYER_NAME:
			if(Path.GetType() == CSubPath::TYPE_LAYER && Path.GetId() >= 0 && Path.GetId() < m_Layers.size())
			{
				str_copy(m_Layers[Path.GetId()].m_aName, pText, sizeof(m_Layers[Path.GetId()].m_aName));
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue(ValueType, PathInt, pText);
}

/* VALUE ASSETPATH ****************************************************/
	
template<>
CAssetPath CAsset_Skeleton::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue) const
{
	switch(ValueType)
	{
		case PARENTPATH:
			return m_ParentPath;
		case SKINPATH:
			return m_DefaultSkinPath;
		default:
			return CAsset::GetValue(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_Skeleton::SetValue(int ValueType, int Path, CAssetPath Value)
{
	switch(ValueType)
	{
		case PARENTPATH:
			m_ParentPath = Value;
			return true;
		case SKINPATH:
			m_DefaultSkinPath = Value;
			return true;
	}
	
	return CAsset::SetValue(ValueType, Path, Value);
}

/* VALUE BONEPATH *****************************************************/
	
template<>
CAsset_Skeleton::CSubPath CAsset_Skeleton::GetValue<CAsset_Skeleton::CSubPath>(int ValueType, int PathInt, CAsset_Skeleton::CSubPath DefaultValue) const
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case BONE_PARENT:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
				return m_Bones[Path.GetId()].m_ParentPath;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue<CAsset_Skeleton::CSubPath>(ValueType, PathInt, DefaultValue);
	}
}

template<>
bool CAsset_Skeleton::SetValue<CAsset_Skeleton::CSubPath>(int ValueType, int PathInt, CAsset_Skeleton::CSubPath Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		case BONE_PARENT:
			if(Path.GetType() == CSubPath::TYPE_BONE && Path.GetId() >= 0 && Path.GetId() < m_Bones.size())
			{
				m_Bones[Path.GetId()].m_ParentPath = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue<CAsset_Skeleton::CSubPath>(ValueType, PathInt, Value);
}

}
