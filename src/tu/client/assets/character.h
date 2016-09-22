#ifndef TU_CLIENT_ASSETS_CHARACTER_H
#define TU_CLIENT_ASSETS_CHARACTER_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_Character : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_CHARACTER;

/* IO *****************************************************************/
public:
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
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);

/* SUBITEMS ***********************************************************/
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

/* MEMBERS ************************************************************/
private:
	array<CPart> m_Parts;
	
	CAssetPath m_IdlePath;
	CAssetPath m_WalkPath;
	CAssetPath m_ControlledJumpPath;
	CAssetPath m_UncontrolledJumpPath;

/* FUNCTIONS **********************************************************/
public:
	CAsset_Character();
	CPart& AddPart();
	
	inline CAssetPath GetIdlePath() const { return m_IdlePath; }
	inline CAssetPath GetWalkPath() const { return m_WalkPath; }
	inline CAssetPath GetControlledJumpPath() const { return m_ControlledJumpPath; }
	inline CAssetPath GetUncontrolledJumpPath() const { return m_UncontrolledJumpPath; }
	inline CAssetPath GetPartDefaultPath(CSubPath SubPath) const { m_Parts[SubPath.GetId()].m_DefaultPath; }
	
	inline void SetIdlePath(CAssetPath Path) { m_IdlePath = Path; }
	inline void SetWalkPath(CAssetPath Path) { m_WalkPath = Path; }
	inline void SetControlledJumpPath(CAssetPath Path) { m_ControlledJumpPath = Path; }
	inline void SetUncontrolledJumpPath(CAssetPath Path) { m_UncontrolledJumpPath = Path; }
	
/* GET/SET ************************************************************/
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
	
	TU_ASSET_ARRAY_ITERATOR(Part, m_Parts, Part)
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		for(int i=0; i<m_Parts.size(); i++)
		{
			m_Parts[i].m_DefaultPath.OnIdDeleted(Path);
		}
		m_IdlePath.OnIdDeleted(Path);
		m_WalkPath.OnIdDeleted(Path);
		m_ControlledJumpPath.OnIdDeleted(Path);
		m_UncontrolledJumpPath.OnIdDeleted(Path);
	}
	
	int AddSubItem(int SubItemType)
	{
		switch(SubItemType)
		{
			case CSubPath::TYPE_PART:
				AddPart();
				return CSubPath::Part(m_Parts.size()-1);
		}
	}
};

}

#endif
