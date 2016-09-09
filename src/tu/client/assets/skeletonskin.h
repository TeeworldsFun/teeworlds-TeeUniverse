#ifndef TU_CLIENT_ASSETS_SKELETONSKIN_H
#define TU_CLIENT_ASSETS_SKELETONSKIN_H

#include <tu/client/assets.h>
#include <tu/client/assets/skeleton.h>

namespace tu
{

class CAsset_SkeletonSkin : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_SKELETONSKIN;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		struct CAsset_Sprite
		{
			int m_SpritePath;
			int m_BonePath;
			int m_LayerPath;
			float m_TranslationX;
			float m_TranslationY;
			float m_ScaleX;
			float m_ScaleY;
			float m_Angle;
			float m_Anchor;
			int m_Color;
			int m_Alignment;
		};
		
		int m_SkeletonPath;
		
		int m_NumSprites;
		int m_SpritesData;
	};
	
	void InitFromAssetsFile(class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(class CDataFileWriter* pFileWriter, int Position);

/* SUBITEMS ***********************************************************/
public:
	class CSubPath : public CGenericPath<2, 0, 0>
	{
	public:
		enum
		{
			TYPE_SPRITE=0,
			NUM_TYPES,
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id) { }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath Sprite(int Id) { return CSubPath(TYPE_SPRITE, Id); }
	};
	
	enum
	{
		ALIGNEMENT_WORLD=0,
		ALIGNEMENT_BONE,
	};

	class CAsset_Sprite
	{
	public:
		CAssetPath m_SpritePath;
		CAsset_Skeleton::CSubPath m_BonePath;
		CAsset_Skeleton::CSubPath m_LayerPath;
		vec2 m_Translation;
		vec2 m_Scale;
		float m_Angle;
		vec4 m_Color;
		float m_Anchor;
		int m_Alignment;
		
	public:
		CAsset_Sprite() :
			m_BonePath(CAsset_Skeleton::CSubPath::Null()),
			m_Translation(vec2(0.0f, 0.0f)),
			m_Scale(vec2(64.0f, 64.0f)),
			m_Angle(0.0f),
			m_Color(1.0f, 1.0f, 1.0f, 1.0f),
			m_Anchor(1.0f),
			m_Alignment(ALIGNEMENT_BONE)
		{
			
		}
		
		CAsset_Sprite(CAssetPath Path) :
			m_SpritePath(Path),
			m_BonePath(CAsset_Skeleton::CSubPath::Null()),
			m_Translation(vec2(0.0f, 0.0f)),
			m_Scale(vec2(64.0f, 64.0f)),
			m_Angle(0.0f),
			m_Color(1.0f, 1.0f, 1.0f, 1.0f),
			m_Anchor(1.0f),
			m_Alignment(ALIGNEMENT_BONE)
		{
			
		}
		
		inline CAsset_Sprite& Bone(CAsset_Skeleton::CSubPath v)
		{
			m_BonePath = v;
			return *this;
		}
		
		inline CAsset_Sprite& Layer(CAsset_Skeleton::CSubPath v)
		{
			m_LayerPath = v;
			return *this;
		}
		
		inline CAsset_Sprite& Translation(vec2 v)
		{
			m_Translation = v;
			return *this;
		}
		
		inline CAsset_Sprite& Scale(vec2 v)
		{
			m_Scale = v;
			return *this;
		}
		
		inline CAsset_Sprite& Angle(float v)
		{
			m_Angle = v;
			return *this;
		}
		
		inline CAsset_Sprite& Anchor(float v)
		{
			m_Anchor = v;
			return *this;
		}
		
		inline CAsset_Sprite& Color(vec4 v)
		{
			m_Color = v;
			return *this;
		}
		
		inline CAsset_Sprite& Alignment(int v)
		{
			m_Alignment = v;
			return *this;
		}
	};
	
	inline int GetSpritePath(int i)
	{
		return i;
	}

/* MEMBERS ************************************************************/
public:	
	CAssetPath m_SkeletonPath;
	array<CAsset_Sprite> m_Sprites;

/* FUNCTIONS **********************************************************/
public:
	CAsset_SkeletonSkin();
	
	void AddSprite(const CAsset_Sprite& Sprite);
	CAsset_Sprite& AddSprite(CAssetPath SkeletonPath, CAsset_Skeleton::CSubPath BonePath, CAsset_Skeleton::CSubPath LayerPath);

/* GET/SET ************************************************************/
public:
	enum
	{
		SUBITEM_SPRITE=0
	};

	enum
	{
		SKELETONPATH = CAsset::NUM_MEMBERS, //AssetPath
		SPRITE_PATH, //AssetPath
		SPRITE_BONE, //Int
		SPRITE_LAYER, //Int
		SPRITE_TRANSLATION_X, //Float
		SPRITE_TRANSLATION_Y, //Float
		SPRITE_SCALE_X, //Float
		SPRITE_SCALE_Y, //Float
		SPRITE_ANGLE, //Float
		SPRITE_ANCHOR, //Float
		SPRITE_ALIGNMENT, //Int
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_SkeletonPath.OnIdDeleted(Path);
		for(int i=0; i<m_Sprites.size(); i++)
		{
			m_Sprites[i].m_SpritePath.OnIdDeleted(Path);
		}
	}
	
	int AddSubItem(int SubItemType)
	{
		switch(SubItemType)
		{
			case CSubPath::TYPE_SPRITE:
			{
				m_Sprites.add(CAsset_Sprite());
				CAsset_Sprite& Sprite = m_Sprites[m_Sprites.size()-1];
				Sprite.m_SpritePath = CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_WHITESQUARE);
				return CSubPath::Sprite(m_Sprites.size()-1);
			}
		}
	}
	
	bool DeleteSubItem(CSubPath SubItemPath)
	{
		if(SubItemPath.GetType() == CSubPath::TYPE_SPRITE)
		{
			int Id = SubItemPath.GetId();
			if(Id >= 0 && Id < m_Sprites.size())
			{
				m_Sprites.remove_index(Id);
				return true;
			}
		}
		
		return false;
	}
	
	void OnSubItemDeleted(const CAssetPath& Path, int SubPathInt)
	{
		if(Path.GetType() == CAssetPath::TYPE_SKELETON)
		{
			if(!(Path == m_SkeletonPath))
				return;
			
			CAsset_Skeleton::CSubPath SubPath(SubPathInt);
			
			if(SubPath.GetType() == CAsset_Skeleton::CSubPath::TYPE_BONE)
			{
				for(int i=0; i<m_Sprites.size(); i++)
				{
					if(m_Sprites[i].m_BonePath.GetSource() == CAsset_Skeleton::CSubPath::SRC_LOCAL)
						m_Sprites[i].m_BonePath.OnIdDeleted(SubPath);
				}
			}
			else if(SubPath.GetType() == CAsset_Skeleton::CSubPath::TYPE_LAYER)
			{
				for(int i=0; i<m_Sprites.size(); i++)
				{
					if(m_Sprites[i].m_LayerPath.GetSource() == CAsset_Skeleton::CSubPath::SRC_LOCAL)
						m_Sprites[i].m_LayerPath.OnIdDeleted(SubPath);
				}
			}
		}
	}
};

}

#endif
