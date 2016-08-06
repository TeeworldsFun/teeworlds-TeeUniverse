#ifndef MODAPI_CLIENT_ASSETS_WEAPON_H
#define MODAPI_CLIENT_ASSETS_WEAPON_H

#include <modapi/client/assets.h>

class CModAPI_Asset_Weapon : public CModAPI_Asset
{
public:
	static const int TypeId = CModAPI_AssetPath::TYPE_WEAPON;

	struct CStorageType : public CModAPI_Asset::CStorageType
	{
		int m_CharacterPath;
		int m_CursorPath;
		int m_SkinPath;
		int m_AttackAnimationPath;
	};
	
	void InitFromAssetsFile(class CModAPI_AssetManager* pAssetManager, class IModAPI_AssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(class CDataFileWriter* pFileWriter, int Position);
	void Unload(class CModAPI_AssetManager* pAssetManager);
	
public:
	CModAPI_AssetPath m_CharacterPath;
	CModAPI_AssetPath m_CursorPath;
	CModAPI_AssetPath m_SkinPath;
	CModAPI_AssetPath m_AttackAnimationPath;

public:
	CModAPI_Asset_Weapon()
	{
		
	}
	
	//Getter/Setter for AssetsEditor
public:
	enum
	{
		CHARACTERPATH = CModAPI_Asset::NUM_MEMBERS, 
		CURSORPATH, 
		SKINPATH, 
		ATTACKANIMATIONPATH, 
	};

public:
	template<typename T>
	T GetValue(int ValueType, int Path, T DefaultValue)
	{
		return CModAPI_Asset::GetValue<T>(ValueType, Path, DefaultValue);
	}
	
	template<typename T>
	bool SetValue(int ValueType, int Path, T Value)
	{
		return CModAPI_Asset::SetValue<T>(ValueType, Path, Value);
	}
	
	inline void OnAssetDeleted(const CModAPI_AssetPath& Path) { }
	inline int AddSubItem(int SubItemType) { }
	inline bool DeleteSubItem(int SubItemPath) { return false; }
	inline void OnSubItemDeleted(const CModAPI_AssetPath& Path, int SubItemPath) { }
};

#endif
