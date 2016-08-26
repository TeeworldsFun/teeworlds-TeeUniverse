#ifndef TU_CLIENT_ASSETS_CHARACTERPART_H
#define TU_CLIENT_ASSETS_CHARACTERPART_H

#include <tu/client/assets.h>
#include <tu/client/assets/character.h>

class CDataFileWriter;

namespace tu
{

class CAsset_CharacterPart : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_CHARACTERPART;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_CharacterPath;
		int m_CharacterPart;
		int m_SkeletonSkinPath;
	};
	
	void InitFromAssetsFile(class CAssetsManager* pAssetsManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);

/* MEMBERS ************************************************************/
public:
	CAssetPath m_CharacterPath;
	CAsset_Character::CSubPath m_CharacterPart;
	CAssetPath m_SkeletonSkinPath;

/* FUNCTIONS **********************************************************/
public:
	CAsset_CharacterPart();
	
/* GET/SET ************************************************************/
public:
	enum
	{
		CHARACTERPATH = CAsset::NUM_MEMBERS, //Path
		CHARACTERPART, //Int
		SKELETONSKINPATH, //Path
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_CharacterPath.OnIdDeleted(Path);
		m_SkeletonSkinPath.OnIdDeleted(Path);
	}
	
	void OnSubItemDeleted(const CAssetPath& Path, int SubPathInt)
	{
		if(Path.GetId() == CAsset_Character::TypeId)
			m_CharacterPart.OnIdDeleted(SubPathInt);
	}
};

}

#endif
