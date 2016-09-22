#ifndef TU_CLIENT_ASSETS_CHARACTERPART_H
#define TU_CLIENT_ASSETS_CHARACTERPART_H

#include <tu/client/assets.h>
#include <tu/client/assets/character.h>
#include <engine/shared/datafile.h>

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
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);

/* MEMBERS ************************************************************/
private:
	CAssetPath m_CharacterPath;
	CAsset_Character::CSubPath m_CharacterPart;
	CAssetPath m_SkeletonSkinPath;

/* FUNCTIONS **********************************************************/
public:
	CAsset_CharacterPart();
	
	inline CAssetPath GetCharacterPath() const { return m_CharacterPath; }
	inline CAsset_Character::CSubPath GetCharacterPart() const { return m_CharacterPart; }
	inline CAssetPath GetSkeletonSkinPath() const { return m_SkeletonSkinPath; }
	
	inline void SetCharacterPath(CAssetPath Path) { m_CharacterPath = Path; }
	inline void SetCharacterPart(CAsset_Character::CSubPath SubPath) { m_CharacterPart = SubPath; }
	inline void SetSkeletonSkinPath(CAssetPath Path) { m_SkeletonSkinPath = Path; }
	
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
