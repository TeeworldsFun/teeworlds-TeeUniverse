#ifndef TU_CLIENT_ASSETPATH_H
#define TU_CLIENT_ASSETPATH_H

namespace tu
{

template<int NBTYPES, int NBSRCS, int NBFLAGS, int ID2SIZE=0>
class CGenericPath
{
public:
	int m_Path;
	
	enum
	{
		TYPESIZE = Log2<(NearestPowerOfTwo<NBTYPES>::result)>::result,
		SRCSIZE = Log2<(NearestPowerOfTwo<NBSRCS>::result)>::result,
		FLAGSIZE = Log2<(NearestPowerOfTwo<NBFLAGS>::result)>::result,
		IDSIZE = 32 - FLAGSIZE - SRCSIZE - TYPESIZE - ID2SIZE,
		
		MASK_TYPE = ((0x1 << TYPESIZE)-1),
		MASK_SRC = ((0x1 << SRCSIZE)-1)<<TYPESIZE,
		MASK_FLAGS = ((0x1 << FLAGSIZE)-1)<<(TYPESIZE+SRCSIZE),
		MASK_ID = ((0x1 << IDSIZE)-1)<<(TYPESIZE+SRCSIZE+FLAGSIZE),
		MASK_ID2 = (ID2SIZE > 0 ? ((0x1 << ID2SIZE)-1)<<(TYPESIZE+SRCSIZE+FLAGSIZE+IDSIZE) : 0x0),
		
		TYPE_SHIFT = 0,
		SRC_SHIFT = TYPESIZE,
		FLAGS_SHIFT = TYPESIZE+SRCSIZE,
		ID_SHIFT = TYPESIZE+SRCSIZE+FLAGSIZE,
		ID2_SHIFT = TYPESIZE+SRCSIZE+FLAGSIZE+IDSIZE,
		
		UNDEFINED = -1,
	};
	
public:
	CGenericPath() : m_Path(UNDEFINED) { }
	CGenericPath(int PathInt) : m_Path(PathInt) { }
	CGenericPath(int Type, int Source, int Flags, int Id) : m_Path((Type << TYPE_SHIFT) + (Source << SRC_SHIFT) + (Flags << FLAGS_SHIFT) + (Id << ID_SHIFT)) { }
	CGenericPath(int Type, int Source, int Flags, int Id, int Id2) : m_Path((Type << TYPE_SHIFT) + (Source << SRC_SHIFT) + (Flags << FLAGS_SHIFT) + (Id << ID_SHIFT) + (Id2 << ID2_SHIFT)) { }
	
	inline int ConvertToInteger() const { return m_Path; }
	inline int GetId() const { return (m_Path & MASK_ID) >> ID_SHIFT; }
	inline int GetId2() const { return (m_Path & MASK_ID2) >> ID2_SHIFT; }
	inline int GetType() const { return (m_Path & MASK_TYPE) >> TYPE_SHIFT; }
	inline int GetSource() const { return (m_Path & MASK_SRC) >> SRC_SHIFT; }
	inline int GetFlags() const { return (m_Path & MASK_FLAGS) >> FLAGS_SHIFT; }
	
	inline void SetId(int Id) { m_Path = (m_Path & ~MASK_ID) + (Id << ID_SHIFT); }
	inline void SetId2(int Id) { m_Path = (m_Path & ~MASK_ID2) + (Id << ID2_SHIFT); }
	
	inline bool IsNull() const { return m_Path == UNDEFINED; }
	inline bool operator==(const CGenericPath& path) const { return path.m_Path == m_Path; }
	
	inline void OnIdDeleted(const CGenericPath& Path)
	{
		if(IsNull() || Path.IsNull())
			return;
		
		if((m_Path & ~MASK_ID) != (Path.m_Path & ~MASK_ID))
			return;
		
		int DeletedId = Path.GetId();
		int CurrentId = GetId();
		
		if(CurrentId == DeletedId)
			m_Path = UNDEFINED;
		else if(CurrentId > DeletedId)
			SetId(CurrentId-1);
	}
};

//TAG_NEW_ASSET
class CAssetPath : public CGenericPath<12, 4, 0>
{
public:
	enum
	{
		SRC_UNIVERSE=0, //Asset provided by the client
		SRC_WORLD,   //Asset provided by the server
		SRC_LAND,        //Asset provided by the map
		SRC_SKIN,       //Asset provided by the skin
		NUM_SOURCES
	};

	enum
	{
		TYPE_IMAGE = 0,
		TYPE_SPRITE,
		TYPE_SKELETON,
		TYPE_SKELETONANIMATION,
		TYPE_SKELETONSKIN,
		TYPE_CHARACTER,
		TYPE_CHARACTERPART,
		TYPE_WEAPON,
		TYPE_MAPGROUP,
		TYPE_MAPLAYERTILES,
		TYPE_MAPLAYERQUADS,
		TYPE_GUIRECTSTYLE,
		NUM_ASSETTYPES,
	};

public:
	CAssetPath() : CGenericPath() { }
	CAssetPath(int PathInt) : CGenericPath(PathInt) { }
	CAssetPath(int Type, int Source, int Id) : CGenericPath(Type, Source, 0x0, Id) { }
	
	//Static constructors	
	static inline CAssetPath Null() { return CAssetPath(CGenericPath::UNDEFINED); }
	
	static inline CAssetPath Asset(int Type, int Source, int Id) { return CAssetPath(Type, Source, Id); }
	
	static inline CAssetPath Universe(int Type, int Id) { return Asset(Type, SRC_UNIVERSE, Id); }
	static inline CAssetPath World(int Type, int Id) { return Asset(Type, SRC_WORLD, Id); }
	static inline CAssetPath Skin(int Type, int Id) { return Asset(Type, SRC_SKIN, Id); }
	static inline CAssetPath Land(int Type, int Id) { return Asset(Type, SRC_LAND, Id); }
	
	static inline int TypeToStoredType(int Type) { return Type+1; }
	static inline int StoredTypeToType(int StoredType) { return StoredType-1; }
};

}

#endif
