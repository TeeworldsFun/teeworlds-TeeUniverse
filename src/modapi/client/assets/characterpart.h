#ifndef TU_CLIENT_ASSETS_CHARACTERPART_H
#define TU_CLIENT_ASSETS_CHARACTERPART_H

#include <modapi/client/assets.h>
#include <modapi/client/assets/character.h>

class CDataFileWriter;

namespace tu
{

class CAsset_CharacterPart : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_CHARACTERPART;

	struct CStorageType : public CAsset::CStorageType
	{
		int m_CharacterPath;
		int m_CharacterPart;
		int m_SkeletonSkinPath;
	};
	
	void InitFromAssetsFile(class CAssetManager* pAssetManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	void Unload(class CAssetManager* pAssetManager);

public:
	CAssetPath m_CharacterPath;
	CAsset_Character::CSubPath m_CharacterPart;
	CAssetPath m_SkeletonSkinPath;

public:
	CAsset_CharacterPart()
	{
		
	}
	
	//Getter/Setter for AssetsEdtiro
public:
	enum
	{
		CHARACTERPATH = CAsset::NUM_MEMBERS, //Path
		CHARACTERPART, //Int
		SKELETONSKINPATH, //Path
	};
	
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
