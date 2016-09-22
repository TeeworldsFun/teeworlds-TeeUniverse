#ifndef TU_CLIENT_ASSETS_MAPLAYERTILES_H
#define TU_CLIENT_ASSETS_MAPLAYERTILES_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_MapLayerTiles : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_MAPLAYERTILES;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		struct CTile
		{
			unsigned char m_Index;
			unsigned char m_Flags;
		};
		
		int m_Width;
		int m_Height;
		int m_TilesData;
		int m_ImagePath;
		int m_Color;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
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
			unsigned char m_Flags;
		
		public:
			CTile() :
				m_Index(0),
				m_Flags(0)
			{
				
			}
	};

/* MEMBERS ************************************************************/
public:	
	enum
	{
		TILEFLAG_VFLIP=1,
		TILEFLAG_HFLIP=2,
		TILEFLAG_OPAQUE=4,
		TILEFLAG_ROTATE=8,
	};

private:
	int m_Width;
	int m_Height;
	CTile* m_pTiles;
	CAssetPath m_ImagePath;
	vec4 m_Color;

/* FUNCTIONS **********************************************************/
public:
	CAsset_MapLayerTiles();	
	CAsset_MapLayerTiles(const CAsset_MapLayerTiles& Layer);
	virtual ~CAsset_MapLayerTiles();
	CAsset_MapLayerTiles& operator=(const CAsset_MapLayerTiles& Layer);
	
	void SetSize(int Width, int Height);
	void Resize(int Width, int Height);
	
	inline const CTile* GetTilesPointer() const { return m_pTiles; };
	const CTile* GetTilePointer(int x, int y) const;
	inline CAssetPath GetImagePath() const { return m_ImagePath; }
	inline vec4 GetColor() const { return m_Color; }
	inline int GetWidth() const { return max(1, m_Width); }
	inline int GetHeight() const { return max(1, m_Height); }
	inline int GetTileIndex(int x, int y) const
	{
		if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
			return m_pTiles[y*m_Width+x].m_Index;
		else
			return 0;
	}
	inline int GetTileIndex(CSubPath SubPath) const { return GetTileIndex(SubPath.GetX(), SubPath.GetY()); }
	inline int GetTileFlags(int x, int y) const
	{
		if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
			return m_pTiles[y*m_Width+x].m_Flags;
		else
			return 0;
	}
	inline int GetTileFlags(CSubPath SubPath) const { return GetTileFlags(SubPath.GetX(), SubPath.GetY()); }
	
	inline void SetColor(vec4 Value) { m_Color = Value; }
	inline void SetImagePath(CAssetPath Value) { m_ImagePath = Value; }
	inline void SetTileIndex(int x, int y, int Index)
	{
		if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
			m_pTiles[y*m_Width+x].m_Index = Index;
	}
	inline void SetTileIndex(CSubPath SubPath, int Index) { SetTileIndex(SubPath.GetX(), SubPath.GetY(), Index); }
	inline void SetTileFlags(int x, int y, int Flags)
	{
		if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
			m_pTiles[y*m_Width+x].m_Flags = Flags;
	}
	inline void SetTileFlags(CSubPath SubPath, int Flags) { SetTileFlags(SubPath.GetX(), SubPath.GetY(), Flags); }
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
	
	
/* GET/SET ************************************************************/
public:
	enum
	{
		WIDTH = CAsset::NUM_MEMBERS, //Int
		HEIGHT, //Int
		IMAGEPATH, //Path
		COLOR, //Vec4
		TILE_INDEX, //Int
		TILE_FLAGS, //Int
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_ImagePath.OnIdDeleted(Path);
	}
};

}

#endif
