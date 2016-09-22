#include "skeletonskin.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_SkeletonSkin::CAsset_SkeletonSkin()
{
	
}

void CAsset_SkeletonSkin::AddSprite(const CAsset_SkeletonSkin::CAsset_Sprite& Sprite)
{
	m_Sprites.add(Sprite);
}
	
CAsset_SkeletonSkin::CAsset_Sprite& CAsset_SkeletonSkin::AddSprite(
	CAssetPath SkeletonPath,
	CAsset_Skeleton::CSubPath BonePath,
	CAsset_Skeleton::CSubPath LayerPath)
{
	m_Sprites.add(CAsset_SkeletonSkin::CAsset_Sprite(SkeletonPath));
	return m_Sprites[m_Sprites.size()-1].Bone(BonePath).Layer(LayerPath);
}

/* IO *****************************************************************/

void CAsset_SkeletonSkin::InitFromAssetsFile(CDataFileReader* pFileReader, const CAsset_SkeletonSkin::CStorageType* pItem)
{
	// load name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
				
	// load info
	m_SkeletonPath = pItem->m_SkeletonPath;
	
	// load sprites
	const CStorageType::CAsset_Sprite* pSprites = static_cast<CStorageType::CAsset_Sprite*>(pFileReader->GetData(pItem->m_SpritesData));
	for(int i=0; i<pItem->m_NumSprites; i++)
	{
		m_Sprites.add(CAsset_Sprite());
		CAsset_Sprite& Sprite = m_Sprites[m_Sprites.size()-1];
		
		Sprite.m_SpritePath = pSprites[i].m_SpritePath;
		Sprite.m_BonePath = pSprites[i].m_BonePath;
		Sprite.m_LayerPath = pSprites[i].m_LayerPath;
		Sprite.m_Translation = vec2(pSprites[i].m_TranslationX, pSprites[i].m_TranslationY);
		Sprite.m_Scale = vec2(pSprites[i].m_ScaleX, pSprites[i].m_ScaleY);
		Sprite.m_Angle = pSprites[i].m_Angle;
		Sprite.m_Anchor = pSprites[i].m_Anchor;
		Sprite.m_Color = tu::IntToColor(pSprites[i].m_Color);
		Sprite.m_Alignment = pSprites[i].m_Alignment;
	}
}

void CAsset_SkeletonSkin::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);

	Item.m_SkeletonPath = m_SkeletonPath;
	
	//save sprites
	{
		CStorageType::CAsset_Sprite* pSprites = new CStorageType::CAsset_Sprite[m_Sprites.size()];
		for(int i=0; i<m_Sprites.size(); i++)
		{
			pSprites[i].m_SpritePath = m_Sprites[i].m_SpritePath;
			pSprites[i].m_BonePath = m_Sprites[i].m_BonePath;
			pSprites[i].m_LayerPath = m_Sprites[i].m_LayerPath;
			pSprites[i].m_TranslationX = m_Sprites[i].m_Translation.x;
			pSprites[i].m_TranslationY = m_Sprites[i].m_Translation.y;
			pSprites[i].m_ScaleX = m_Sprites[i].m_Scale.x;
			pSprites[i].m_ScaleY = m_Sprites[i].m_Scale.y;
			pSprites[i].m_Angle = m_Sprites[i].m_Angle;
			pSprites[i].m_Anchor = m_Sprites[i].m_Anchor;
			pSprites[i].m_Color = ColorToInt(m_Sprites[i].m_Color);
			pSprites[i].m_Alignment = m_Sprites[i].m_Alignment;
		}
		Item.m_NumSprites = m_Sprites.size();
		Item.m_SpritesData = pFileWriter->AddData(Item.m_NumSprites * sizeof(CStorageType::CAsset_Sprite), pSprites);
		delete[] pSprites;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/
	
template<>
int CAsset_SkeletonSkin::GetValue(int ValueType, int Path, int DefaultValue) const
{
	switch(ValueType)
	{
		case SPRITE_ALIGNMENT:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_Alignment;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue(ValueType, Path, DefaultValue);
	}
}
	
template<>
bool CAsset_SkeletonSkin::SetValue(int ValueType, int Path, int Value)
{
	switch(ValueType)
	{
		case SPRITE_ALIGNMENT:
			if(Path >= 0 && Path < m_Sprites.size() && Value >= -1)
			{
				m_Sprites[Path].m_Alignment = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue(ValueType, Path, Value);
}

/* VALUE FLOAT ********************************************************/

template<>
float CAsset_SkeletonSkin::GetValue(int ValueType, int Path, float DefaultValue) const
{
	switch(ValueType)
	{
		case SPRITE_TRANSLATION_X:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_Translation.x;
			else
				return DefaultValue;
		case SPRITE_TRANSLATION_Y:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_Translation.y;
			else
				return DefaultValue;
		case SPRITE_SCALE_X:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_Scale.x;
			else
				return DefaultValue;
		case SPRITE_SCALE_Y:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_Scale.y;
			else
				return DefaultValue;
		case SPRITE_ANGLE:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_Angle;
			else
				return DefaultValue;
		case SPRITE_ANCHOR:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_Anchor;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue(ValueType, Path, DefaultValue);
	}
}
	
template<>
bool CAsset_SkeletonSkin::SetValue(int ValueType, int Path, float Value)
{
	switch(ValueType)
	{
		case SPRITE_TRANSLATION_X:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_Translation.x = Value;
				return true;
			}
			else return false;
		case SPRITE_TRANSLATION_Y:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_Translation.y = Value;
				return true;
			}
			else return false;
		case SPRITE_SCALE_X:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_Scale.x = Value;
				return true;
			}
			else return false;
		case SPRITE_SCALE_Y:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_Scale.y = Value;
				return true;
			}
			else return false;
		case SPRITE_ANGLE:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_Angle = Value;
				return true;
			}
			else return false;
		case SPRITE_ANCHOR:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_Anchor = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue(ValueType, Path, Value);
}

/* VALUE ASSETPATH ****************************************************/

template<>
CAssetPath CAsset_SkeletonSkin::GetValue(int ValueType, int Path, CAssetPath DefaultValue) const
{
	switch(ValueType)
	{
		case SKELETONPATH:
			return m_SkeletonPath;
		case SPRITE_PATH:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_SpritePath;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue(ValueType, Path, DefaultValue);
	}
}
	
template<>
bool CAsset_SkeletonSkin::SetValue(int ValueType, int Path, CAssetPath Value)
{
	switch(ValueType)
	{
		case SKELETONPATH:
			m_SkeletonPath = Value;
			return true;
		case SPRITE_PATH:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_SpritePath = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue(ValueType, Path, Value);
}

/* VALUE BONEPATH *****************************************************/
	
template<>
CAsset_Skeleton::CSubPath CAsset_SkeletonSkin::GetValue<CAsset_Skeleton::CSubPath>(int ValueType, int Path, CAsset_Skeleton::CSubPath DefaultValue) const
{
	switch(ValueType)
	{
		case SPRITE_BONE:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_BonePath;
			else
				return DefaultValue;
		case SPRITE_LAYER:
			if(Path >= 0 && Path < m_Sprites.size())
				return m_Sprites[Path].m_LayerPath;
			else
				return DefaultValue;
		default:
			return CAsset::GetValue<CAsset_Skeleton::CSubPath>(ValueType, Path, DefaultValue);
	}
}
	
template<>
bool CAsset_SkeletonSkin::SetValue<CAsset_Skeleton::CSubPath>(int ValueType, int Path, CAsset_Skeleton::CSubPath Value)
{
	switch(ValueType)
	{
		case SPRITE_BONE:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_BonePath = Value;
				return true;
			}
			else return false;
		case SPRITE_LAYER:
			if(Path >= 0 && Path < m_Sprites.size())
			{
				m_Sprites[Path].m_LayerPath = Value;
				return true;
			}
			else return false;
	}
	
	return CAsset::SetValue<CAsset_Skeleton::CSubPath>(ValueType, Path, Value);
}

}
