#ifndef TU_CLIENT_ASSETS_ZONETYPE_H
#define TU_CLIENT_ASSETS_ZONETYPE_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_ZoneType : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_ZONETYPE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		struct CIndex
		{
			char m_aName[128];
			int m_Number;
			int m_Color;
		};
		
		int m_NumIndices;
		int m_IndicesData;
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
			TYPE_INDEX=0,
			NUM_TYPES,
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id) { }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath Index(int Id) { return CSubPath(TYPE_INDEX, Id); }
	};
	
	class CIndex
	{
	public:
		char m_aName[128];
		int m_Number;
		vec4 m_Color;
	};
	
/* MEMBERS ************************************************************/
private:
	array<CIndex> m_Indices;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_ZoneType();
	
	inline bool IsValidIndex(CSubPath SubPath) const { return (SubPath.GetType() == CSubPath::TYPE_INDEX && SubPath.GetId() >= 0 && SubPath.GetId() < m_Indices.size()); }
	
	inline const char* GetIndexName(CSubPath SubPath) const { return (IsValidIndex(SubPath) ? m_Indices[SubPath.GetId()].m_aName : 0); };
	inline char* GetIndexName(CSubPath SubPath) { return (IsValidIndex(SubPath) ? m_Indices[SubPath.GetId()].m_aName : 0); };
	inline int GetIndexNumber(CSubPath SubPath) const { return (IsValidIndex(SubPath) ? m_Indices[SubPath.GetId()].m_Number : -1); };
	inline vec4 GetIndexColor(CSubPath SubPath) const { return (IsValidIndex(SubPath) ? m_Indices[SubPath.GetId()].m_Color : 1.0f); };
	
	inline void SetIndexName(CSubPath SubPath, const char* pValue) { if(IsValidIndex(SubPath)) str_copy(m_Indices[SubPath.GetId()].m_aName, pValue, sizeof(CIndex::m_aName)); };
	inline void SetIndexNumber(CSubPath SubPath, int Value) { if(IsValidIndex(SubPath)) m_Indices[SubPath.GetId()].m_Number = Value; };
	inline void SetIndexColor(CSubPath SubPath, vec4 Value) { if(IsValidIndex(SubPath)) m_Indices[SubPath.GetId()].m_Color = Value; };
	
	inline CSubPath NewIndex() { m_Indices.add(CIndex()); return CSubPath::Index(m_Indices.size()-1); }
	
/* GET/SET ************************************************************/
public:
	enum
	{
		INDEX_NAME = CAsset::NUM_MEMBERS, //String
		INDEX_NUMBER, //Int
		INDEX_COLOR, //vec4
	};
	
	TU_ASSET_ARRAY_ITERATOR(Index, m_Indices, Index)
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	int AddSubItem(int SubItemType)
	{
		switch(SubItemType)
		{
			case CSubPath::TYPE_INDEX:
				return NewIndex();
		}
		
		return CSubPath::Null();
	}
};

}

#endif
