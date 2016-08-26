#ifndef TU_CLIENT_ASSETS_SKELETON_H
#define TU_CLIENT_ASSETS_SKELETON_H

#include <tu/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_Skeleton : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_SKELETON;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		struct CBone
		{
			int m_ParentPath;
			float m_Length;
			float m_Anchor;
			float m_TranslationX;
			float m_TranslationY;
			float m_ScaleX;
			float m_ScaleY;
			float m_Angle;
			
			char m_aName[64];
			int m_Color;
		};
		
		struct CLayer
		{
			char m_aName[64];
		};
			
		int m_ParentPath;
		int m_DefaultSkinPath;
		
		int m_NumBones;
		int m_BonesData;
		
		int m_NumLayers;
		int m_LayersData;
	};
	
	void InitFromAssetsFile(class CAssetsManager* pAssetsManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);

/* SUBITEMS ***********************************************************/
public:
	class CSubPath : public CGenericPath<2, 0, 0>
	{
	public:
		enum
		{
			TYPE_BONE=0, //Bone from the current skeleton
			TYPE_LAYER,   //Bone from the parent skeleton
			NUM_SOURCES,       //Asset provided by the skin
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id) { }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath Bone(int Id) { return CSubPath(TYPE_BONE, Id); }
		static inline CSubPath Layer(int Id) { return CSubPath(TYPE_LAYER, Id); }
	};
	
	class CBonePath : public CGenericPath<0, 2, 0>
	{
	public:
		enum
		{
			SRC_LOCAL=0, //Bone from the current skeleton
			SRC_PARENT,   //Bone from the parent skeleton
			NUM_SOURCES,       //Asset provided by the skin
		};

	public:
		CBonePath() : CGenericPath() { }
		CBonePath(int PathInt) : CGenericPath(PathInt) { }
		CBonePath(int Source, int Id) : CGenericPath(0, Source, 0x0, Id) { }
		
		static inline CBonePath Null() { return CBonePath(CGenericPath::UNDEFINED); }
		static inline CBonePath Local(int Id) { return CBonePath(SRC_LOCAL, Id); }
		static inline CBonePath Parent(int Id) { return CBonePath(SRC_PARENT, Id); }
	};

	class CBone
	{
	public:
		//Hierarchy
		CAsset_Skeleton::CBonePath m_ParentPath;
		
		//Physics
		float m_Length;
		float m_Anchor;
		vec2 m_Translation;
		vec2 m_Scale;
		float m_Angle;
		
		//Gui
		char m_aName[64];
		vec4 m_Color;
		
	public:
		CBone() :
			m_ParentPath(CAsset_Skeleton::CBonePath::Null()),
			m_Length(64.0f),
			m_Anchor(1.0f),
			m_Translation(vec2(0.0f, 0.0f)),
			m_Scale(vec2(1.0f, 1.0f)),
			m_Angle(0.0f),
			m_Color(1.0f, 0.0f, 0.0f, 1.0f)
		{
			
		}
		
		inline CBone& Parent(CAsset_Skeleton::CBonePath v)
		{
			m_ParentPath = v;
			return *this;
		}
		
		inline CBone& Length(float v)
		{
			m_Length = v;
			return *this;
		}
		
		inline CBone& Anchor(float v)
		{
			m_Anchor = v;
			return *this;
		}
		
		inline CBone& Translation(vec2 v)
		{
			m_Translation = v;
			return *this;
		}
		
		inline CBone& Scale(vec2 v)
		{
			m_Scale = v;
			return *this;
		}
		
		inline CBone& Angle(float v)
		{
			m_Angle = v;
			return *this;
		}
		
		inline CBone& Name(const char* pText)
		{
			str_copy(m_aName, pText, sizeof(m_aName));
			return *this;
		}
		
		inline CBone& Color(vec4 v)
		{
			m_Color = v;
			return *this;
		}
	};
	
	class CLayer
	{
	public:
		char m_aName[64];
		
		inline CLayer& Name(const char* pText)
		{
			str_copy(m_aName, pText, sizeof(m_aName));
			return *this;
		}
	};
	
	inline int GetBonePath(int i)
	{
		return CSubPath::Bone(i).ConvertToInteger();
	}
	
	inline int GetLayerPath(int i)
	{
		return CSubPath::Layer(i).ConvertToInteger();
	}

/* MEMBERS ************************************************************/
public:	
	enum
	{
		SUBITEM_BONE=0,
		SUBITEM_LAYER,
	};
	
	CAssetPath m_ParentPath;
	CAssetPath m_DefaultSkinPath;
	array<CBone> m_Bones;
	array<CLayer> m_Layers;

/* FUNCTIONS **********************************************************/
public:
	CAsset_Skeleton();
	
	void AddBone(const CBone& Bone);
	CBone& AddBone();
	CLayer& AddLayer();

/* GET/SET ************************************************************/
public:
	enum
	{
		PARENTPATH = CAsset::NUM_MEMBERS, //Path
		SKINPATH, //Path
		BONE_PARENT, //Int
		BONE_LENGTH, //Float
		BONE_TRANSLATION_X, //Float
		BONE_TRANSLATION_Y, //Float
		BONE_SCALE_X, //Float
		BONE_SCALE_Y, //Float
		BONE_ANGLE, //Float
		BONE_NAME, //String
		BONE_COLOR, //vec4
		BONE_ANCHOR, //Float
		LAYER_NAME, //String
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_ParentPath.OnIdDeleted(Path);
		m_DefaultSkinPath.OnIdDeleted(Path);
	}
	
	int AddSubItem(int SubItemType)
	{
		switch(SubItemType)
		{
			case SUBITEM_BONE:
				AddBone();
				return CSubPath::Bone(m_Bones.size()-1).ConvertToInteger();
			case SUBITEM_LAYER:
				AddLayer();
				return CSubPath::Layer(m_Layers.size()-1).ConvertToInteger();
		}
	}
	
	bool DeleteSubItem(CSubPath SubItemPath)
	{
		switch(SubItemPath.GetType())
		{
			case SUBITEM_BONE:
				m_Bones.remove_index(SubItemPath.GetId());
				for(int i=0; i<m_Bones.size(); i++)
				{
					if(m_Bones[i].m_ParentPath.GetSource() == CBonePath::SRC_LOCAL)
					{
						CSubPath TmpPath = CSubPath::Bone(m_Bones[i].m_ParentPath.GetId());
						TmpPath.OnIdDeleted(SubItemPath);
						m_Bones[i].m_ParentPath.SetId(TmpPath.GetId());
					}
				}
				return true;
			case SUBITEM_LAYER:
				m_Layers.remove_index(SubItemPath.GetId());
				return true;
		}
		
		return false;
	}
	
	void OnSubItemDeleted(const CAssetPath& Path, int SubPathInt)
	{
		if(Path.GetType() == CAssetPath::TYPE_SKELETON)
		{
			if(!(Path == m_ParentPath))
				return;
			
			CSubPath SubPath(SubPathInt);
			if(SubPath.GetType() != SUBITEM_BONE)
				return;
			
			for(int i=0; i<m_Bones.size(); i++)
			{
				if(m_Bones[i].m_ParentPath.GetSource() == CBonePath::SRC_PARENT)
				{
					CSubPath TmpPath = CSubPath::Bone(m_Bones[i].m_ParentPath.GetId());
					TmpPath.OnIdDeleted(SubPath);
					m_Bones[i].m_ParentPath.SetId(TmpPath.GetId());
				}
			}
		}
	}
};

}

#endif
