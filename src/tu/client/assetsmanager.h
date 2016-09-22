#ifndef TU_CLIENT_ASSETSMANAGER_H
#define TU_CLIENT_ASSETSMANAGER_H

	//Search Tag: TAG_NEW_ASSET
#include <engine/shared/datafile.h>
#include <tu/client/kernel.h>
#include <tu/client/assets.h>
#include <tu/client/assetshistory.h>
#include <tu/client/assetscatalog.h>
#include <tu/client/assets/image.h>
#include <tu/client/assets/sprite.h>
#include <tu/client/assets/skeleton.h>
#include <tu/client/assets/skeletonskin.h>
#include <tu/client/assets/skeletonanimation.h>
#include <tu/client/assets/character.h>
#include <tu/client/assets/characterpart.h>
#include <tu/client/assets/weapon.h>
#include <tu/client/assets/map.h>
#include <tu/client/assets/mapgroup.h>
#include <tu/client/assets/mapzonetiles.h>
#include <tu/client/assets/maplayertiles.h>
#include <tu/client/assets/maplayerquads.h>
#include <tu/client/assets/zonetype.h>
#include <tu/client/assets/guirectstyle.h>
#include <tu/client/assets/guilinestyle.h>
#include <tu/client/assets/guiboxstyle.h>
#include <tu/client/assets/guiscrollbarstyle.h>
#include <tu/client/assets/guitabsstyle.h>

namespace tu
{

class CAssetsManager : public CKernel::CComponent
{	
private:
	struct CStorageType
	{
		int m_Version;
		int m_Source;
	};

public:
	class IListener
	{
	protected:
		int m_AssetManagerListener_Token;
	
	public:
		virtual void OnAssetsListModified() = 0;
		inline void AssetManager_SetToken(int Token) { m_AssetManagerListener_Token = Token; };
		inline int AssetManager_GetToken() const { return m_AssetManagerListener_Token; };
	};
	
private:
	CAssetsHistory* m_pHistory;
	array<IListener*> m_pListeners;
	bool m_AssetsListModified;
	
private:	
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) CAssetCatalog<ClassName> CatalogName;
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE

protected:
	template<class ASSETTYPE>
	ASSETTYPE* NewAsset(const CAssetPath& AssetPath)
	{
		return GetAssetCatalog<ASSETTYPE>()->NewAsset(this, AssetPath);
	}

	template<class ASSETTYPE>
	ASSETTYPE* GetEditableAsset(CAssetPath AssetPath)
	{
		return GetAssetCatalog<ASSETTYPE>()->GetAsset(AssetPath);
	}
	
public:
	CAssetsManager(CKernel* pKernel);
	virtual ~CAssetsManager();
	void Init();
	
	void AddListener(IListener* pListener);
	void RemoveListener(int Token);
	
	CAssetPath FindSkinPart(CAssetPath CharacterPath, CAsset_Character::CSubPath CharacterPart, const char* pName);
	
	CAssetPath DuplicateAsset(const CAssetPath& Path, int Source);
	void DeleteAsset(const CAssetPath& Path);
	
	CAssetState* GetAssetState(CAssetPath AssetPath);
	void InitAssetState(int Source, const CAssetState& State);
	
	void Update();
	int SaveInAssetsFile(const char *pFileName, int Source);
	int LoadAssetsFile(const char *pFileName, int Source, unsigned Crc = 0);
	int LoadAssetsFile_Asset(CDataFileReader* pDataFile, int Source);
	int LoadAssetsFile_Map(CDataFileReader* pDataFile, int Source);
	int OnAssetsFileUnloaded(int Source);
	
	template<class ASSETTYPE>
	const CAssetCatalog<ASSETTYPE>* GetAssetCatalog() const
	{
		return 0;
	}
	
	template<class ASSETTYPE>
	CAssetCatalog<ASSETTYPE>* GetAssetCatalog()
	{
		return 0;
	}
	
	template<class ASSETTYPE>
	const ASSETTYPE* GetAsset(CAssetPath AssetPath) const
	{
		return GetAssetCatalog<ASSETTYPE>()->GetAsset(AssetPath);
	}
	
	template<class ASSETTYPE>
	void SetAsset(CAssetPath AssetPath, const ASSETTYPE* pAsset)
	{
		ASSETTYPE* pOldAsset = GetAssetCatalog<ASSETTYPE>()->GetAsset(AssetPath);
		if(pOldAsset)
			*pOldAsset = *pAsset;
	}
	
	template<class ASSETTYPE>
	ASSETTYPE* NewAsset(CAssetPath* pAssetPath, int Source, int Token)
	{
		ASSETTYPE* pNewAsset = GetAssetCatalog<ASSETTYPE>()->NewAsset(this, pAssetPath, Source);
		if(m_pHistory)
			m_pHistory->AddOperation_AddAsset(*pAssetPath, Token);
		return pNewAsset;
	}
	
	CAsset_Image* NewImage(CAssetPath* pAssetPath, int Source, int StorageType, const char* pFilename, int Token);
	
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
	
	int AddSubItem(CAssetPath AssetPath, int SubItemType, int Token = -1);
	void DeleteSubItem(CAssetPath AssetPath, int SubPath);
			
	template<typename T>
	T GetAssetValue(CAssetPath AssetPath, int FieldType, int FieldPath, T DefaultValue)
	{
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
		{\
			const ClassName* pAsset = GetAsset<ClassName>(AssetPath);\
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
	bool SetAssetValue(CAssetPath AssetPath, int SubPath, int Field, const T& Value, int Token = -1)
	{
		if(m_pHistory)
			m_pHistory->AddOperation_EditAsset(AssetPath, Token);
		
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
		{\
			ClassName* pAsset = GetEditableAsset<ClassName>(AssetPath);\
			if(pAsset)\
				return pAsset->SetValue<T>(Field, SubPath, Value);\
			break;\
		}
		
		switch(AssetPath.GetType())
		{
			#include <tu/client/assetsmacro.h>
		}
		
		#undef TU_MACRO_ASSETTYPE
		
		return false;
	}
	
	void EnableAssetsHistory();
	void Undo();
	int GenerateToken();

public:
	//System
	static void InitAssetsManager_System(CAssetsManager* pAssetsManager);
	
	//Teeworlds Universe
	static void InitAssetsManager_Universe_TeeWorlds(CAssetsManager* pAssetsManager);
	
	//Skins
	static void LoadSkins(CAssetsManager* pAssetsManager);
	static int LoadSkinAssets_BodyScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_FootScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_EyeScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_HandsScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_MarkingScan(const char *pName, int IsDir, int DirType, void *pUser);
	static int LoadSkinAssets_DecorationScan(const char *pName, int IsDir, int DirType, void *pUser);
};

}

#endif
