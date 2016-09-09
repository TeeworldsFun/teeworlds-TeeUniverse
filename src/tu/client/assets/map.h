#ifndef TU_CLIENT_ASSETS_MAP_H
#define TU_CLIENT_ASSETS_MAP_H

#include <tu/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_Map : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_MAP;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_NumBgGroups;
		int m_NumFgGroups;
		int m_NumZoneLayers;
		int m_BgGroupsData;
		int m_FgGroupsData;
		int m_ZoneLayersData;
	};
	
	void InitFromAssetsFile(class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* SUBITEMS ***********************************************************/
public:
	class CSubPath : public CGenericPath<3, 0, 0>
	{
	public:
		enum
		{
			TYPE_BGGROUP=0,
			TYPE_FGGROUP,
			TYPE_ZONELAYER,
			NUM_TYPES,
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id) { }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath BgGroup(int Id) { return CSubPath(TYPE_BGGROUP, Id); }
		static inline CSubPath FgGroup(int Id) { return CSubPath(TYPE_FGGROUP, Id); }
		static inline CSubPath ZoneLayer(int Id) { return CSubPath(TYPE_ZONELAYER, Id); }
	};

/* MEMBERS ************************************************************/
private:
	array<CAssetPath> m_BgGroups;
	array<CAssetPath> m_FgGroups;
	array<CAssetPath> m_ZoneLayers;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_Map();
	
	inline CAssetPath GetBgGroup(CSubPath SubPath) const { return m_BgGroups[SubPath.GetId()]; }
	inline CAssetPath GetFgGroup(CSubPath SubPath) const { return m_FgGroups[SubPath.GetId()]; }
	inline CAssetPath GetZoneLayer(CSubPath SubPath) const { return m_ZoneLayers[SubPath.GetId()]; }
	
	inline void AddBgGroup(CAssetPath Path) { m_BgGroups.add(Path); }
	inline void AddFgGroup(CAssetPath Path) { m_FgGroups.add(Path); }
	inline void AddZoneLayer(CAssetPath Path) { m_ZoneLayers.add(Path); }
	
/* GET/SET ************************************************************/
public:
	enum
	{
		BGGROUP = CAsset::NUM_MEMBERS, //Path
		FGGROUP, //Path
		ZONELAYER, //Path
	};
	
	TU_ASSET_ARRAY_ITERATOR(BgGroup, m_BgGroups, BgGroup)
	TU_ASSET_ARRAY_ITERATOR(FgGroup, m_FgGroups, FgGroup)
	TU_ASSET_ARRAY_ITERATOR(ZoneLayer, m_ZoneLayers, ZoneLayer)
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		for(int i=0; i<m_BgGroups.size(); i++)
			m_BgGroups[i].OnIdDeleted(Path);
		for(int i=0; i<m_FgGroups.size(); i++)
			m_FgGroups[i].OnIdDeleted(Path);
		for(int i=0; i<m_ZoneLayers.size(); i++)
			m_ZoneLayers[i].OnIdDeleted(Path);
	}
};

}

#endif
