#ifndef TU_CLIENT_ASSETS_MAPZONETILES_H
#define TU_CLIENT_ASSETS_MAPZONETILES_H

#include <tu/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_MapZoneTiles : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_MAPZONETILES;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		struct CTile
		{
			unsigned char m_Index;
		};
		
		int m_Width;
		int m_Height;
		int m_TilesData;
	};
	
	void InitFromAssetsFile(class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);

/* SUBITEMS ***********************************************************/
public:	
	class CSubPath : public CGenericPath<2, 0, 0, 15>
	{
	public:
		enum
		{
			TYPE_TILE=0,
			NUM_TYPES,
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id) { }
		CSubPath(int Type, int Id, int Id2) : CGenericPath(Type, 0, 0x0, Id, Id2) { }
		
		int GetX() const { return GetId(); }
		int GetY() const { return GetId2(); }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath Tile(int x, int y) { return CSubPath(TYPE_TILE, x, y); }
	};
	
	class CTile
	{
		public:
			unsigned char m_Index;
		
		public:
			CTile() :
				m_Index(ZONEFLAG_AIR)
			{ }
	};

/* MEMBERS ************************************************************/
public:
	enum
	{
		ZONEFLAG_AIR=0,
		ZONEFLAG_SOLID,
		ZONEFLAG_NOHOOK,
	};
	
private:
	int m_Width;
	int m_Height;
	CTile* m_pTiles;
	CAssetPath m_ZoneTypePath;

/* FUNCTIONS **********************************************************/
public:
	CAsset_MapZoneTiles();	
	CAsset_MapZoneTiles(const CAsset_MapZoneTiles& Layer);
	virtual ~CAsset_MapZoneTiles();
	CAsset_MapZoneTiles& operator=(const CAsset_MapZoneTiles& Layer);
	
	void SetSize(int Width, int Height);
	void Resize(int Width, int Height);
	
	inline const CTile* GetTilesPointer() const { return m_pTiles; };
	const CTile* GetTilePointer(int x, int y) const;
	inline CAssetPath GetZoneTypePath() const { return m_ZoneTypePath; }
	inline int GetWidth() const { return max(1, m_Width); }
	inline int GetHeight() const { return max(1, m_Height); }
	inline int GetTileIndex(int x, int y) const
	{
		if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
			return m_pTiles[y*m_Width+x].m_Index;
		else
			return 0;
	}
	inline int GetTileIndex(CSubPath SubPath) { return GetTileIndex(SubPath.GetX(), SubPath.GetY()); }
	
	inline void SetZoneTypePath(CAssetPath Value) { m_ZoneTypePath = Value; }
	inline void SetTileIndex(int x, int y, int Index)
	{
		if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
			m_pTiles[y*m_Width+x].m_Index = Index;
	}
	inline void SetTileIndex(CSubPath SubPath, int Index) { SetTileIndex(SubPath.GetX(), SubPath.GetY(), Index); }
	inline void SetWidth(int Width)
	{
		if(m_Width != Width)
			Resize(Width, m_Height);
	}
	inline void SetHeight(int Height)
	{
		if(m_Height != Height)
			Resize(m_Width, Height);
	}
	
	inline int GetArea()
	{
		int Counter = 0;
		for(int j=0; j<m_Height; j++)
		{
			for(int i=0; i<m_Width; i++)
			{
				if(m_pTiles[j*m_Width+i].m_Index > 0)
					Counter++;
			}	
		}
		return Counter;
	}
	
/* GET/SET ************************************************************/
public:
	enum
	{
		ZONETYPEPATH = CAsset::NUM_MEMBERS, //Path
		WIDTH, //Int
		HEIGHT, //Int
		TILE_INDEX, //Int
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_ZoneTypePath.OnIdDeleted(Path);
	}
};

}

#endif
