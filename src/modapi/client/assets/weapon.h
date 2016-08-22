#ifndef TU_CLIENT_ASSETS_WEAPON_H
#define TU_CLIENT_ASSETS_WEAPON_H

#include <modapi/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_Weapon : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_WEAPON;

	struct CStorageType : public CAsset::CStorageType
	{
		int m_CharacterPath;
		int m_CursorPath;
		int m_SkinPath;
		int m_AttackAnimationPath;
	};
	
	void InitFromAssetsFile(class CAssetManager* pAssetManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	void Unload(class CAssetManager* pAssetManager);
	
public:
	CAssetPath m_CharacterPath;
	CAssetPath m_CursorPath;
	CAssetPath m_SkinPath;
	CAssetPath m_AttackAnimationPath;

public:
	CAsset_Weapon()
	{
		
	}
	
	//Getter/Setter for AssetsEditor
public:
	enum
	{
		CHARACTERPATH = CAsset::NUM_MEMBERS, 
		CURSORPATH, 
		SKINPATH, 
		ATTACKANIMATIONPATH, 
	};

public:
	template<typename T>
	T GetValue(int ValueType, int Path, T DefaultValue)
	{
		return CAsset::GetValue<T>(ValueType, Path, DefaultValue);
	}
	
	template<typename T>
	bool SetValue(int ValueType, int Path, T Value)
	{
		return CAsset::SetValue<T>(ValueType, Path, Value);
	}
	
	inline void OnAssetDeleted(const CAssetPath& Path) { }
	inline int AddSubItem(int SubItemType) { }
	inline bool DeleteSubItem(int SubItemPath) { return false; }
	inline void OnSubItemDeleted(const CAssetPath& Path, int SubItemPath) { }
};

}

#endif
