#ifndef TU_CLIENT_ASSETS_MAPLAYERTILES_H
#define TU_CLIENT_ASSETS_MAPLAYERTILES_H

#include <tu/client/assets.h>

class CDataFileWriter;

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
			unsigned char m_Skip;
			unsigned char m_Reserved;
		};
		
		int m_Width;
		int m_Height;
		int m_TilesData;
		int m_ImagePath;
		int m_Color;
	};
	
	void InitFromAssetsFile(class CAssetsManager* pAssetsManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);

/* SUBITEMS ***********************************************************/
public:	
	class CTile
	{
		public:
			unsigned char m_Index;
			unsigned char m_Flags;
			unsigned char m_Skip;
			unsigned char m_Reserved;
		
		public:
			CTile() :
				m_Index(0),
				m_Flags(0),
				m_Skip(0),
				m_Reserved(0)
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
	CTile* GetTile(int x, int y);
	
/* GET/SET ************************************************************/
public:
	enum
	{
		WIDTH = CAsset::NUM_MEMBERS, //Int
		HEIGHT, //Int
		IMAGEPATH, //Path
		COLOR, //Vec4
	};
	
	TU_ASSET_GETSET_FUNC()
};

}

#endif
