#ifndef TU_CLIENT_ASSETS_CHARACTER_H
#define TU_CLIENT_ASSETS_CHARACTER_H

#include <modapi/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_Character : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_CHARACTER;

	struct CStorageType : public CAsset::CStorageType
	{
		struct CPart
		{
			char m_aName[128];
			int m_DefaultPath;
		};
		
		int m_NumParts;
		int m_PartsData;
		
		int m_IdlePath;
		int m_WalkPath;
		int m_ControlledJumpPath;
		int m_UncontrolledJumpPath;
	};
	
	void InitFromAssetsFile(class CAssetManager* pAssetManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	void Unload(class CAssetManager* pAssetManager);

public:
	class CSubPath : public CGenericPath<2, 0, 0>
	{
	public:
		enum
		{
			TYPE_PART=0,
			NUM_TYPES,
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id) { }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath Part(int Id) { return CSubPath(TYPE_PART, Id); }
	};
	
	class CPart
	{
	public:
		char m_aName[128];
		CAssetPath m_DefaultPath;
	
	public:
		inline CPart& Name(const char* pText)
		{
			str_copy(m_aName, pText, sizeof(m_aName));
			return *this;
		}
		
		inline CPart& Default(const CAssetPath& Path)
		{
			m_DefaultPath = Path;
			return *this;
		}
	};
	
	array<CPart> m_Parts;
	
	CAssetPath m_IdlePath;
	CAssetPath m_WalkPath;
	CAssetPath m_ControlledJumpPath;
	CAssetPath m_UncontrolledJumpPath;

public:
	CAsset_Character()
	{
		
	}

	CPart& AddPart();
	
	//Getter/Setter for AssetsEdtiro
public:
	enum
	{
		PART_NAME = CAsset::NUM_MEMBERS, //Int
		PART_DEFAULTPATH, //Path
		IDLEPATH, //Path
		WALKPATH, //Path
		CONTROLLEDJUMPPATH, //Path
		UNCONTROLLEDJUMPPATH, //Path
	};
	
	template<typename T>
	T GetValue(int ValueType, int Path, T DefaultValue)
	{
		return CAsset::GetValue<T>(ValueType, Path, DefaultValue);
	}
	
	template<typename T>
	bool SetValue(int ValueType, int Path, T Value)
	{
		return CAsset::SetValue<T>(ValueType, Path, Value);
	}
	
	inline void OnAssetDeleted(const CAssetPath& Path) { }
	
	inline int AddSubItem(int SubItemType)
	{
		switch(SubItemType)
		{
			case CSubPath::TYPE_PART:
				AddPart();
				return CSubPath::Part(m_Parts.size()-1).ConvertToInteger();
		}
	}
	
	inline bool DeleteSubItem(int SubItemPath) { return false; }
	
	inline void OnSubItemDeleted(const CAssetPath& Path, int SubItemPath) { }
};

}

#endif
