#ifndef TU_CLIENT_ASSETSMANAGER_H
#define TU_CLIENT_ASSETSMANAGER_H

	//Search Tag: TAG_NEW_ASSET
#include <tu/client/assets.h>
#include <tu/client/assetscatalog.h>
#include <tu/client/assets/image.h>
#include <tu/client/assets/sprite.h>
#include <tu/client/assets/skeleton.h>
#include <tu/client/assets/skeletonskin.h>
#include <tu/client/assets/skeletonanimation.h>
#include <tu/client/assets/character.h>
#include <tu/client/assets/characterpart.h>
#include <tu/client/assets/weapon.h>
#include <tu/client/assets/mapgroup.h>
#include <tu/client/assets/maplayertiles.h>
#include <tu/client/assets/maplayerquads.h>
#include <tu/client/assets/guirectstyle.h>

namespace tu
{

class IAssetsFile;

class CAssetsManager
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

	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) CAssetCatalog<ClassName> CatalogName;
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE

public:
	CAssetsManager(IGraphics* pGraphics, class IStorage* pStorage);
	void Init(IStorage* pStorage);
	void LoadInteralAssets();
	void LoadSkinAssets(class IStorage* pStorage);

	static int LoadSkinAssets_BodyScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_FootScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_EyeScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_HandsScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_MarkingScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_DecorationScan(const char *pName, int IsDir, int DirType, void *pUser);

	CAssetPath FindSkinPart(CAssetPath CharacterPath, CAsset_Character::CSubPath CharacterPart, const char* pName);

	class IGraphics *Graphics() { return m_pGraphics; };
	class IStorage *Storage() { return m_pStorage; };

	CAssetPath AddImage(int StorageType, const char* pFilename, int Source);
	void DeleteAsset(int Type, CAssetPath Path);

	void UpdateAssets();
	int SaveInAssetsFile(const char *pFileName, int Source);
	int OnAssetsFileLoaded_Asset(tu::IAssetsFile* pAssetsFile);
	int OnAssetsFileLoaded_Map(tu::IAssetsFile* pAssetsFile);
	int OnAssetsFileLoaded(tu::IAssetsFile* pAssetsFile);
	int OnAssetsFileUnloaded();

	template<class ASSETTYPE>
	CAssetCatalog<ASSETTYPE>* GetAssetCatalog()
	{
		return 0;
	}

	template<class ASSETTYPE>
	ASSETTYPE* GetAsset(CAssetPath AssetPath)
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

	int AddSubItem(CAssetPath AssetPath, int SubItemType);
	void DeleteSubItem(CAssetPath AssetPath, int SubPath);

	template<typename T>
	T GetAssetValue(CAssetPath AssetPath, int FieldType, int FieldPath, T DefaultValue)
	{
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) case ClassName::TypeId:\
		{\
			ClassName* pAsset = GetAsset<ClassName>(AssetPath);\
			if(pAsset)\
				return pAsset->GetValue<T>(FieldType, FieldPath, DefaultValue);\
			else\
				return DefaultValue;\
		}

		switch(AssetPath.GetType())
		{
			#include <tu/client/assetsmacro.h>
		}

		#undef TU_MACRO_ASSETTYPE

		return DefaultValue;
	}

	template<typename T>
	bool SetAssetValue(CAssetPath AssetPath, int FieldType, int FieldPath, const T& Value)
	{
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) case ClassName::TypeId:\
		{\
			ClassName* pAsset = GetAsset<ClassName>(AssetPath);\
			if(pAsset)\
				return pAsset->SetValue<T>(FieldType, FieldPath, Value);\
			break;\
		}

		switch(AssetPath.GetType())
		{
			#include <tu/client/assetsmacro.h>
		}

		#undef TU_MACRO_ASSETTYPE

		return false;
	}

	void DeleteAsset(const CAssetPath& Path);
};

// http://stackoverflow.com/questions/59331/visibility-of-template-specialization-of-c-function
// http://stackoverflow.com/questions/2498435/declaration-of-template-class-member-specialization

// make sure all calls to GetAssetCatalog are aware of template-specializations

#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) template<>\
CAssetCatalog<ClassName>* CAssetsManager::GetAssetCatalog<ClassName>();

#include <tu/client/assetsmacro.h>

#undef TU_MACRO_ASSETTYPE

}

#endif
