#ifndef MODAPI_CLIENT_ASSETSMANAGER_H
#define MODAPI_CLIENT_ASSETSMANAGER_H

	//Search Tag: TAG_NEW_ASSET
#include <modapi/client/assets.h>
#include <modapi/client/assetscatalog.h>
#include <modapi/client/assets/image.h>
#include <modapi/client/assets/sprite.h>
#include <modapi/client/assets/skeleton.h>
#include <modapi/client/assets/skeletonskin.h>
#include <modapi/client/assets/skeletonanimation.h>
#include <modapi/client/assets/character.h>
#include <modapi/client/assets/characterpart.h>
#include <modapi/client/assets/weapon.h>

class IModAPI_AssetsFile;

class CModAPI_AssetManager
{
private:
	struct CStorageType
	{
		int m_Version;
		int m_Source;
	};
	
private:
	IGraphics* m_pGraphics;
	class IStorage* m_pStorage;
	
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) CModAPI_AssetCatalog<ClassName> CatalogName;
	#include <modapi/client/assetsmacro.h>
	#undef MODAPI_MACRO_ASSETTYPE

public:
	CModAPI_AssetManager(IGraphics* pGraphics, class IStorage* pStorage);
	void Init(IStorage* pStorage);
	void LoadInteralAssets();
	void LoadSkinAssets(class IStorage* pStorage);
	
	static int LoadSkinAssets_BodyScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_FootScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_EyeScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_HandsScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_MarkingScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_DecorationScan(const char *pName, int IsDir, int DirType, void *pUser);
	
	CModAPI_AssetPath FindSkinPart(CModAPI_AssetPath CharacterPath, CModAPI_Asset_Character::CSubPath CharacterPart, const char* pName);
	
	class IGraphics *Graphics() { return m_pGraphics; };
	class IStorage *Storage() { return m_pStorage; };
	
	CModAPI_AssetPath AddImage(int StorageType, const char* pFilename, int Source);
	void DeleteAsset(int Type, CModAPI_AssetPath Path);
	
	int SaveInAssetsFile(const char *pFileName, int Source);
	int OnAssetsFileLoaded(IModAPI_AssetsFile* pAssetsFile);
	int OnAssetsFileUnloaded();
	
	template<class ASSETTYPE>
	CModAPI_AssetCatalog<ASSETTYPE>* GetAssetCatalog()
	{
		return 0;
	}
	
	template<class ASSETTYPE>
	ASSETTYPE* GetAsset(CModAPI_AssetPath AssetPath)
	{
		return GetAssetCatalog<ASSETTYPE>()->GetAsset(AssetPath);
	}
	
	template<class ASSETTYPE>
	int GetNumAssets(int Source)
	{
		return GetAssetCatalog<ASSETTYPE>()->m_Assets[Source].size();
	}
	
	template<class ASSETTYPE>
	int GetNumLists(int Source)
	{
		return GetAssetCatalog<ASSETTYPE>()->m_Lists[Source].size();
	}
	
	int AddSubItem(CModAPI_AssetPath AssetPath, int SubItemType);
	void DeleteSubItem(CModAPI_AssetPath AssetPath, int SubPath);
			
	template<typename T>
	T GetAssetValue(CModAPI_AssetPath AssetPath, int FieldType, int FieldPath, T DefaultValue)
	{
		#define GET_ASSET_VALUE(TypeName) case TypeName::TypeId:\
		{\
			TypeName* pAsset = GetAsset<TypeName>(AssetPath);\
			if(pAsset)\
				return pAsset->GetValue<T>(FieldType, FieldPath, DefaultValue);\
			else\
				return DefaultValue;\
		}
	
		switch(AssetPath.GetType())
		{
			//Search Tag: TAG_NEW_ASSET
			GET_ASSET_VALUE(CModAPI_Asset_Image);
			GET_ASSET_VALUE(CModAPI_Asset_Sprite);
			GET_ASSET_VALUE(CModAPI_Asset_Skeleton);
			GET_ASSET_VALUE(CModAPI_Asset_SkeletonSkin);
			GET_ASSET_VALUE(CModAPI_Asset_SkeletonAnimation);
			GET_ASSET_VALUE(CModAPI_Asset_Character);
			GET_ASSET_VALUE(CModAPI_Asset_CharacterPart);
			GET_ASSET_VALUE(CModAPI_Asset_Weapon);
		}
		
		return DefaultValue;
	}
	
	template<typename T>
	bool SetAssetValue(CModAPI_AssetPath AssetPath, int FieldType, int FieldPath, const T& Value)
	{
		#define SET_ASSET_VALUE(TypeName) case TypeName::TypeId:\
		{\
			TypeName* pAsset = GetAsset<TypeName>(AssetPath);\
			if(pAsset)\
				return pAsset->SetValue<T>(FieldType, FieldPath, Value);\
			break;\
		}
		
		switch(AssetPath.GetType())
		{
			//Search Tag: TAG_NEW_ASSET
			SET_ASSET_VALUE(CModAPI_Asset_Image);
			SET_ASSET_VALUE(CModAPI_Asset_Sprite);
			SET_ASSET_VALUE(CModAPI_Asset_Skeleton);
			SET_ASSET_VALUE(CModAPI_Asset_SkeletonSkin);
			SET_ASSET_VALUE(CModAPI_Asset_SkeletonAnimation);
			SET_ASSET_VALUE(CModAPI_Asset_Character);
			SET_ASSET_VALUE(CModAPI_Asset_CharacterPart);
			SET_ASSET_VALUE(CModAPI_Asset_Weapon);
		}
		
		return false;
	}
	
	void DeleteAsset(const CModAPI_AssetPath& Path);
};

#endif
