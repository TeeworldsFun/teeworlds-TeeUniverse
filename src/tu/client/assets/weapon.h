#ifndef TU_CLIENT_ASSETS_WEAPON_H
#define TU_CLIENT_ASSETS_WEAPON_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_Weapon : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_WEAPON;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_CharacterPath;
		int m_CursorPath;
		int m_SkinPath;
		int m_AttackAnimationPath;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:	
	CAssetPath m_CharacterPath;
	CAssetPath m_CursorPath;
	CAssetPath m_SkinPath;
	CAssetPath m_AttackAnimationPath;

/* FUNCTIONS **********************************************************/
public:
	CAsset_Weapon();
	
/* GET/SET ************************************************************/
public:
	enum
	{
		CHARACTERPATH = CAsset::NUM_MEMBERS, 
		CURSORPATH, 
		SKINPATH, 
		ATTACKANIMATIONPATH, 
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_CharacterPath.OnIdDeleted(Path);
		m_CursorPath.OnIdDeleted(Path);
		m_SkinPath.OnIdDeleted(Path);
		m_AttackAnimationPath.OnIdDeleted(Path);
	}
};

}

#endif
