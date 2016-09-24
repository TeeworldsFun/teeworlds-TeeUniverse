#include <engine/storage.h>
#include <engine/shared/config.h>
#include <engine/shared/datafile.h>
#include <generated/client_data.h>
#include <game/mapitems.h>
#include <game/gamecore.h>
#include <game/client/render.h>

#include <engine/external/pnglite/pnglite.h>

#include <tu/client/assetsmanager.h>

namespace tu
{

CAssetsManager::CAssetsManager(CKernel* pKernel) :
	CKernel::CComponent(pKernel),
	m_pHistory(0)
{
	
}

CAssetsManager::~CAssetsManager()
{
	if(m_pHistory)
		delete m_pHistory;
}

void CAssetsManager::Init()
{
	m_AssetsListModified = false;
	CAssetsManager::InitAssetsManager_System(this);
	CAssetsManager::InitAssetsManager_Universe_TeeWorlds(this);
}
	
void CAssetsManager::AddListener(IListener* pListener)
{
	pListener->AssetManager_SetToken(rand());
	m_pListeners.add(pListener);
}

void CAssetsManager::RemoveListener(int Token)
{
	for(int i=0; i<m_pListeners.size(); i++)
	{
		if(m_pListeners[i]->AssetManager_GetToken() == Token)
			m_pListeners.remove_index(i);
	}
}

#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) template<>\
CAssetCatalog<ClassName>* CAssetsManager::GetAssetCatalog<ClassName>()\
{\
	return &CatalogName;\
}
#include <tu/client/assetsmacro.h>
#undef TU_MACRO_ASSETTYPE

#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) template<>\
const CAssetCatalog<ClassName>* CAssetsManager::GetAssetCatalog<ClassName>() const \
{\
	return &CatalogName;\
}
#include <tu/client/assetsmacro.h>
#undef TU_MACRO_ASSETTYPE

CAssetPath CAssetsManager::FindSkinPart(CAssetPath CharacterPath, CAsset_Character::CSubPath CharacterPart, const char* pName)
{
	char aAssetName[256];
	str_format(aAssetName, sizeof(aAssetName), "%s", pName);

	for(int i=0; i<m_CharacterPartsCatalog.m_Assets[CAssetPath::SRC_SKIN].size(); i++)
	{
		const CAsset_CharacterPart* pCharacterPart = &m_CharacterPartsCatalog.m_Assets[CAssetPath::SRC_SKIN][i].m_Asset;
		if(
			str_comp(pCharacterPart->m_aName, aAssetName) == 0 &&
			pCharacterPart->GetCharacterPath() == CharacterPath &&
			pCharacterPart->GetCharacterPart() == CharacterPart
		)
		{
			return CAssetPath::Skin(CAssetPath::TYPE_CHARACTERPART, i);
		}
	}

	return CAssetPath::Null();
}

CAsset_Image* CAssetsManager::NewImage(CAssetPath* pAssetPath, int Source, int StorageType, const char* pFilename, int Token)
{
	char aCompleteFilename[512];
	unsigned char *pBuffer;
	png_t Png; // ignore_convention

	// open file for reading
	png_init(0,0); // ignore_convention

	IOHANDLE File = Storage()->OpenFile(pFilename, IOFLAG_READ, StorageType, aCompleteFilename, sizeof(aCompleteFilename));
	if(File)
	{
		io_close(File);
	}
	else
	{
		dbg_msg("mod", "failed to open file. filename='%s'", pFilename);
		return 0;
	}

	int Error = png_open_file(&Png, aCompleteFilename); // ignore_convention
	if(Error != PNG_NO_ERROR)
	{
		dbg_msg("mod", "failed to open file. filename='%s'", aCompleteFilename);
		if(Error != PNG_FILE_ERROR)
		{
			png_close_file(&Png); // ignore_convention
		}
		return 0;
	}

	if(Png.depth != 8 || (Png.color_type != PNG_TRUECOLOR && Png.color_type != PNG_TRUECOLOR_ALPHA) || Png.width > (2<<12) || Png.height > (2<<12)) // ignore_convention
	{
		dbg_msg("mod", "invalid format. filename='%s'", aCompleteFilename);
		png_close_file(&Png); // ignore_convention
		return 0;
	}

	pBuffer = (unsigned char *) mem_alloc(Png.width * Png.height * Png.bpp, 1); // ignore_convention
	png_get_data(&Png, pBuffer); // ignore_convention
	png_close_file(&Png); // ignore_convention

	int Format;
	if(Png.color_type == PNG_TRUECOLOR)
		Format = CImageInfo::FORMAT_RGB;
	else if(Png.color_type == PNG_TRUECOLOR_ALPHA)
		Format = CImageInfo::FORMAT_RGBA;
	
	CAsset_Image* pImage = NewAsset<CAsset_Image>(pAssetPath, Source, Token);
	pImage->SetAssetsManager(this);
	pImage->SetName(pFilename);
	pImage->SetData(Png.width, Png.height, 1, 1, Format, pBuffer);
	mem_free(pBuffer);
	
	return pImage;
}

void CAssetsManager::Update()
{
	if(m_AssetsListModified)
	{
		for(int i=0; i<m_pListeners.size(); i++)
			m_pListeners[i]->OnAssetsListModified();
		m_AssetsListModified = false;
	}
	
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.Update();
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
}

int CAssetsManager::SaveInAssetsFile(const char *pFileName, int Source)
{
	CDataFileWriter df;
	if(!df.Open(Storage(), pFileName))
	{
		dbg_msg("AssetsManager", "can't create the assets file %s", pFileName);
		return 0;
	}

	{
		CStorageType Item;
		Item.m_Version = 0;
		Item.m_Source = Source;

		df.AddItem(CAsset_Image::TypeId, 0, sizeof(CStorageType), &Item);
	}
	
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.SaveInAssetsFile(&df, Source);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE

	df.Finish(DATAFILE_TYPE_ASSET);

	return 1;
}

int CAssetsManager::LoadAssetsFile_Asset(CDataFileReader* pDataFile, int Source)
{
	int Start, Num;
	pDataFile->GetType(0, &Start, &Num);
	if(Num > 0)
	{
		CStorageType* pItem = (CStorageType*) pDataFile->GetItem(Start, 0, 0);
		Source = pItem->m_Source % CAssetPath::NUM_SOURCES;
	}
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.LoadFromAssetsFile(this, pDataFile, Source);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE

	return 1;
}

int CAssetsManager::LoadAssetsFile_Map(CDataFileReader* pDataFile, int Source)
{
	CMapItemVersion *pItem = (CMapItemVersion *)pDataFile->FindItem(MAPITEMTYPE_VERSION, 0);
	if(!pItem)
		return 0;

	if(pItem->m_Version != CMapItemVersion::CURRENT_VERSION)
		return 0;

	char aBuf[128];

	CAssetPath MapPath;
	CAsset_Map* pMap = NewAsset<CAsset_Map>(&MapPath, Source, CAssetsHistory::NO_TOKEN);
	
	pMap->SetName("map");
				
	CAssetPath* pImagePath;
	//Load images
	{
		int Start, Num;
		pDataFile->GetType(MAPITEMTYPE_IMAGE, &Start, &Num);
		
		pImagePath = new CAssetPath[Num];

		for(int i = 0; i < Num; i++)
		{
			CMapItemImage *pItem = (CMapItemImage *)pDataFile->GetItem(Start+i, 0, 0);

			CAsset_Image* pImage = NewAsset<CAsset_Image>(pImagePath+i, Source, CAssetsHistory::NO_TOKEN);
			pImage->SetAssetsManager(this);
			
			//Image name
			{
				char *pName = (char *)pDataFile->GetData(pItem->m_ImageName);
				pImage->SetName(pName);
				pDataFile->UnloadData(pItem->m_ImageName);
			}
			
			//Image data
			if(pItem->m_External)
			{
				char* pFilename = (char*) pDataFile->GetData(pItem->m_ImageName);
					
				char aBuf[256];
				str_format(aBuf, sizeof(aBuf), "mapres/%s.png", pFilename);
				
				pImage->LoadData(aBuf, -1, -1);
				pDataFile->UnloadData(pItem->m_ImageName);	
			}
			else
			{
				unsigned char* pData = (unsigned char*) pDataFile->GetData(pItem->m_ImageData);
				int Format = CAsset_Image::FORMAT_RGBA;
				if(pItem->m_Version > 1)
				{
					switch(pItem->m_Format)
					{
						case CImageInfo::FORMAT_RGB:
							Format = CAsset_Image::FORMAT_RGB;
							break;
						case CImageInfo::FORMAT_AUTO:
						case CImageInfo::FORMAT_RGBA:
							Format = CAsset_Image::FORMAT_RGBA;
							break;
						case CImageInfo::FORMAT_ALPHA:
							Format = CAsset_Image::FORMAT_ALPHA;
							break;
					}
				}
				
				pImage->SetData(pItem->m_Width, pItem->m_Height, pItem->m_Width/64, pItem->m_Height/64, Format, pData);
				pDataFile->UnloadData(pItem->m_ImageData);	
			}
		}
	}
	
	bool Background = true;
	
	//Load groups
	{
		int LayersStart, LayersNum;
		pDataFile->GetType(MAPITEMTYPE_LAYER, &LayersStart, &LayersNum);

		int Start, Num;
		pDataFile->GetType(MAPITEMTYPE_GROUP, &Start, &Num);
		for(int g = 0; g < Num; g++)
		{
			CMapItemGroup *pGItem = (CMapItemGroup *)pDataFile->GetItem(Start+g, 0, 0);

			if(pGItem->m_Version < 1 || pGItem->m_Version > CMapItemGroup::CURRENT_VERSION)
				continue;

			CAssetPath MapGroupPath;
			CAsset_MapGroup* pMapGroup = NewAsset<CAsset_MapGroup>(&MapGroupPath, Source, CAssetsHistory::NO_TOKEN);

			if(Background)
				pMap->AddBgGroup(MapGroupPath);
			else
				pMap->AddFgGroup(MapGroupPath);
			
			//Don't forget to add parameters when the game group is splitted
			pMapGroup->SetPosition(vec2(pGItem->m_OffsetX, pGItem->m_OffsetY));
			pMapGroup->SetHardParallax(vec2(pGItem->m_ParallaxX/100.0f, pGItem->m_ParallaxY/100.0f));

			//~ if(pGItem->m_Version >= 2)
			//~ {
				//~ pGroup->m_UseClipping = pGItem->m_UseClipping;
				//~ pGroup->m_ClipX = pGItem->m_ClipX;
				//~ pGroup->m_ClipY = pGItem->m_ClipY;
				//~ pGroup->m_ClipW = pGItem->m_ClipW;
				//~ pGroup->m_ClipH = pGItem->m_ClipH;
			//~ }

			// load group name
			aBuf[0] = 0;

			if(pGItem->m_Version >= 3)
				IntsToStr(pGItem->m_aName, sizeof(pGItem->m_aName)/sizeof(int), aBuf);

			if(!aBuf[0])
				str_format(aBuf, sizeof(aBuf), "group%d", MapGroupPath.GetId());

			pMapGroup->SetName(aBuf);

			for(int l = 0; l < pGItem->m_NumLayers; l++)
			{
				CMapItemLayer *pLayerItem = (CMapItemLayer *)pDataFile->GetItem(LayersStart+pGItem->m_StartLayer+l, 0, 0);
				if(!pLayerItem)
					continue;

				if(pLayerItem->m_Type == LAYERTYPE_TILES)
				{
					CMapItemLayerTilemap *pTilemapItem = (CMapItemLayerTilemap *)pLayerItem;

					if(pTilemapItem->m_Flags&TILESLAYERFLAG_GAME)
					{
						Background = false;
							
						//Tiles
						CTile* pTiles = (CTile*) pDataFile->GetData(pTilemapItem->m_Data);
						int Width = pTilemapItem->m_Width;
						int Height = pTilemapItem->m_Height;
						
						CAssetPath PhysicsZonePath;
						CAsset_MapZoneTiles* pPhysicsZone = NewAsset<CAsset_MapZoneTiles>(&PhysicsZonePath, Source, CAssetsHistory::NO_TOKEN);
						
						CAssetPath DeathZonePath;
						CAsset_MapZoneTiles* pDeathZone = NewAsset<CAsset_MapZoneTiles>(&DeathZonePath, Source, CAssetsHistory::NO_TOKEN);
						
						pPhysicsZone->SetName("PhysicsZone");
						pPhysicsZone->SetZoneTypePath(CAssetPath::Universe(CAssetPath::TYPE_ZONETYPE, ZONETYPE_PHYSICS));
						pPhysicsZone->SetSize(Width, Height);
						pMap->AddZoneLayer(PhysicsZonePath);
						
						pDeathZone->SetName("DeathZone");
						pDeathZone->SetZoneTypePath(CAssetPath::Universe(CAssetPath::TYPE_ZONETYPE, ZONETYPE_DEATH));
						pDeathZone->SetSize(Width, Height);
						pMap->AddZoneLayer(DeathZonePath);
							
						for(int j=0; j<Height; j++)
						{
							for(int i=0; i<Width; i++)
							{
								switch(pTiles[j*Width+i].m_Index)
								{
									case TILE_SOLID:
										pPhysicsZone->SetTileIndex(i, j, ZONETYPEINDEX_PHYSICS_GROUND);
										break;
									case TILE_NOHOOK:
										pPhysicsZone->SetTileIndex(i, j, ZONETYPEINDEX_PHYSICS_NOHOOK);
										break;
									default:
										pPhysicsZone->SetTileIndex(i, j, ZONETYPEINDEX_PHYSICS_AIR);
										break;
								}
								switch(pTiles[j*Width+i].m_Index)
								{
									case TILE_DEATH:
										pDeathZone->SetTileIndex(i, j, ZONETYPEINDEX_DEATH_DEATH);
										break;
									default:
										pDeathZone->SetTileIndex(i, j, 0);
										break;
								}
								
								int Skip = pTiles[j*Width+i].m_Skip;
								for(int s=0; s<Skip; s++)
								{
									pPhysicsZone->SetTileIndex(i, j, 0);
									pDeathZone->SetTileIndex(i, j, 0);
									i++;
								}
							}
						}
						
						//Create a new group to separate background/foreground
						{
							CAsset_MapGroup* pNewGroup = NewAsset<CAsset_MapGroup>(&MapGroupPath, Source, CAssetsHistory::NO_TOKEN);
							pMap->AddFgGroup(MapGroupPath);
							pNewGroup->SetName(pMapGroup->m_aName);
							pNewGroup->SetPosition(pMapGroup->GetPosition());
							pNewGroup->SetHardParallax(pMapGroup->GetHardParallax());
							pMapGroup = pNewGroup;
						}
					}
					else
					{
						CAssetPath MapLayerPath;
						CAsset_MapLayerTiles* pMapLayer = NewAsset<CAsset_MapLayerTiles>(&MapLayerPath, Source, CAssetsHistory::NO_TOKEN);
						
						pMapGroup->AddLayer(MapLayerPath);
						
						//Name
						aBuf[0] = 0;
						
						if(pTilemapItem->m_Version >= 3)
							IntsToStr(pTilemapItem->m_aName, sizeof(pTilemapItem->m_aName)/sizeof(int), aBuf);
						
						if(!aBuf[0])
							str_format(aBuf, sizeof(aBuf), "tilelayer%d", MapLayerPath.GetId());
						
						pMapLayer->SetName(aBuf);
										
						//Tiles
						int Width = pTilemapItem->m_Width;
						pMapLayer->SetSize(Width, pTilemapItem->m_Height);
						CTile* pTiles = (CTile*) pDataFile->GetData(pTilemapItem->m_Data);
						
						for(int j=0; j<pMapLayer->GetHeight(); j++)
						{
							for(int i=0; i<pMapLayer->GetWidth(); i++)
							{
								int Skip = pTiles[j*Width+i].m_Skip;
								pMapLayer->SetTileIndex(i, j, pTiles[j*Width+i].m_Index);
								pMapLayer->SetTileFlags(i, j, pTiles[j*Width+i].m_Flags);
								
								for(int s=0; s<Skip; s++)
								{
									pMapLayer->SetTileIndex(i, j, 0);
									pMapLayer->SetTileFlags(i, j, 0);
									i++;
								}
							}
						}
						
						//Image
						if(pTilemapItem->m_Image >= 0)
							pMapLayer->SetImagePath(pImagePath[pTilemapItem->m_Image]);
						
						//Color
						vec4 Color;
						Color.r = static_cast<float>(pTilemapItem->m_Color.r)/255.0f;
						Color.g = static_cast<float>(pTilemapItem->m_Color.g)/255.0f;
						Color.b = static_cast<float>(pTilemapItem->m_Color.b)/255.0f;
						Color.a = static_cast<float>(pTilemapItem->m_Color.a)/255.0f;
						pMapLayer->SetColor(Color);
					}
				}
				else if(pLayerItem->m_Type == LAYERTYPE_QUADS)
				{
					CMapItemLayerQuads *pQuadsItem = (CMapItemLayerQuads *)pLayerItem;

					CAssetPath MapLayerPath;
					CAsset_MapLayerQuads* pMapLayer = NewAsset<CAsset_MapLayerQuads>(&MapLayerPath, Source, CAssetsHistory::NO_TOKEN);
					pMapGroup->AddLayer(MapLayerPath);
					
					//Name
					aBuf[0] = 0;

					if(pQuadsItem->m_Version >= 2)
						IntsToStr(pQuadsItem->m_aName, sizeof(pQuadsItem->m_aName)/sizeof(int), aBuf);

					if(!aBuf[0])
						str_format(aBuf, sizeof(aBuf), "quadlayer%d", MapLayerPath.GetId());

					pMapLayer->SetName(aBuf);

					//Quads
					CQuad *pQuads = (CQuad *)pDataFile->GetDataSwapped(pQuadsItem->m_Data);
					for(int i=0; i<pQuadsItem->m_NumQuads; i++)
					{
						CAsset_MapLayerQuads::CSubPath QuadPath = pMapLayer->NewQuad();
						vec2 Pivot;
						Pivot.x = fx2f(pQuads[i].m_aPoints[4].x);
						Pivot.y = fx2f(pQuads[i].m_aPoints[4].y);
						pMapLayer->SetQuadPivot(QuadPath, Pivot);
						
						for(int p=0; p<4; p++)
						{
							CAsset_MapLayerQuads::CSubPath VertexPath = QuadPath.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p);
							
							vec2 Position;
							Position.x = fx2f(pQuads[i].m_aPoints[p].x) - Pivot.x;
							Position.y = fx2f(pQuads[i].m_aPoints[p].y) - Pivot.y;
							pMapLayer->SetVertexPosition(VertexPath, Position);
							
							vec2 UV;
							UV.x = fx2f(pQuads[i].m_aTexcoords[p].x);
							UV.y = fx2f(pQuads[i].m_aTexcoords[p].y);
							pMapLayer->SetVertexTexture(VertexPath, UV);
							
							vec4 Color;
							Color.r = static_cast<float>(pQuads[i].m_aColors[p].r)/255.0f;
							Color.g = static_cast<float>(pQuads[i].m_aColors[p].g)/255.0f;
							Color.b = static_cast<float>(pQuads[i].m_aColors[p].b)/255.0f;
							Color.a = static_cast<float>(pQuads[i].m_aColors[p].a)/255.0f;
							pMapLayer->SetVertexColor(VertexPath, Color);
						}
					}

					//Image
					if(pQuadsItem->m_Image >= 0)
						pMapLayer->SetImagePath(pImagePath[pQuadsItem->m_Image]);
				}
			}
		}
	}

	delete[] pImagePath;

	return 1;
}

int CAssetsManager::LoadAssetsFile(const char* pFilename, int Source, unsigned Crc)
{
	CDataFileReader df;
	
	if(!df.Open(Storage(), pFilename, IStorage::TYPE_ALL))
	{
		dbg_msg("AssetsManager", "can't open the file %s", pFilename);
		return 0;
	}
	
	if(Crc != 0 && df.Crc() != Crc)
	{
		dbg_msg("AssetsManager", "wrong crc for the file %s (%d != %d)", pFilename, Crc, df.Crc());
		return 0;
	}
	
	//Remove previous assets
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.Unload(Source);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
	
	//Load
	int Res;
	if(df.GetDataFileType() == DATAFILE_TYPE_ASSET)
		Res = LoadAssetsFile_Asset(&df, Source);
	else
		Res = LoadAssetsFile_Map(&df, Source);

	m_AssetsListModified = true;
	
	return Res;
}

int CAssetsManager::OnAssetsFileUnloaded(int Source)
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.Unload(Source);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE

	m_AssetsListModified = true;
}
	
CAssetPath CAssetsManager::DuplicateAsset(const CAssetPath& Path, int Source)
{
	CAssetPath NewAssetPath;
	char aBuf[128];
	
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
	{\
		if(!GetAsset<ClassName>(Path))\
			return CAssetPath::Null();\
		ClassName* pNewAsset = NewAsset<ClassName>(&NewAssetPath, Source, -1);\
		if(!pNewAsset)\
			return CAssetPath::Null();\
		const ClassName* pOldAsset = GetAsset<ClassName>(Path);\
		*pNewAsset = *pOldAsset;\
		\
		int DuplicateNum = ((Source == Path.GetSource()) ? 1 : 0);\
		bool NameFound;\
		do\
		{\
			NameFound = false;\
			DuplicateNum++;\
			if(DuplicateNum < 2)\
				str_copy(aBuf, pOldAsset->m_aName, sizeof(aBuf));\
			else\
				str_format(aBuf, sizeof(aBuf), "%s (%d)", pOldAsset->m_aName, DuplicateNum);\
			for(int i=0; i<CatalogName.m_Assets[Source].size(); i++)\
			{\
				if(str_comp(CatalogName.m_Assets[Source][i].m_Asset.m_aName, aBuf) == 0)\
				{\
					NameFound = true;\
					break;\
				}\
			}\
		}\
		while(NameFound);\
		pNewAsset->SetName(aBuf);\
		break;\
	}
	
	switch(Path.GetType())
	{
		//~ #include <tu/client/assetsmacro.h>
		TU_MACRO_ASSETTYPE(tu::CAsset_Character, m_CharactersCatalog, "Character", "character")
	}
	
	#undef TU_MACRO_ASSETTYPE
	
	return NewAssetPath;
}

void CAssetsManager::DeleteAsset(const CAssetPath& Path)
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.DeleteAsset(Path);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
	
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.OnAssetDeleted(Path);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
}
	
int CAssetsManager::AddSubItem(CAssetPath AssetPath, int SubItemType, int Token)
{
	if(m_pHistory)
			m_pHistory->AddOperation_EditAsset(AssetPath, Token);
	
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
	{\
		ClassName* pAsset = GetEditableAsset<ClassName>(AssetPath);\
		if(pAsset)\
			return pAsset->AddSubItem(SubItemType);\
		else\
			return -1;\
	}

	switch(AssetPath.GetType())
	{
		#include <tu/client/assetsmacro.h>
	}

	#undef TU_MACRO_ASSETTYPE

	return -1;
}

void CAssetsManager::DeleteSubItem(CAssetPath AssetPath, int SubPath)
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
	{\
		ClassName* pAsset = GetEditableAsset<ClassName>(AssetPath);\
		if(pAsset)\
			pAsset->DeleteSubItem(SubPath);\
		break;\
	}

	switch(AssetPath.GetType())
	{
		#include <tu/client/assetsmacro.h>
	}
	#undef TU_MACRO_ASSETTYPE
	
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.OnSubItemDeleted(AssetPath, SubPath);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
}

CAssetState* CAssetsManager::GetAssetState(CAssetPath AssetPath)
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
	{\
		return CatalogName.GetAssetState(AssetPath);\
	}

	switch(AssetPath.GetType())
	{
		#include <tu/client/assetsmacro.h>
	}
	#undef TU_MACRO_ASSETTYPE
	
	return 0;
}

void CAssetsManager::InitAssetState(int Source, const CAssetState& State)
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CatalogName.InitAssetState(Source, State);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
}

void CAssetsManager::EnableAssetsHistory()
{
	if(m_pHistory)
		delete m_pHistory;
	
	m_pHistory = new CAssetsHistory(this);
}

void CAssetsManager::Undo()
{
	if(m_pHistory)
		m_pHistory->Undo();
}

int CAssetsManager::GenerateToken()
{
	if(m_pHistory)
		return m_pHistory->GenerateToken();
	else
		return CAssetsHistory::NO_TOKEN;
}

}
