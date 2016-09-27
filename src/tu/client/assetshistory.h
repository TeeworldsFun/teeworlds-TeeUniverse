#ifndef TU_CLIENT_ASSETSHISTORY_H
#define TU_CLIENT_ASSETSHISTORY_H

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
#include <tu/client/assets/map.h>
#include <tu/client/assets/mapgroup.h>
#include <tu/client/assets/mapzonetiles.h>
#include <tu/client/assets/maplayertiles.h>
#include <tu/client/assets/maplayerquads.h>
#include <tu/client/assets/zonetype.h>
#include <tu/client/assets/guirectstyle.h>
#include <tu/client/assets/guilinestyle.h>
#include <tu/client/assets/guiboxstyle.h>
#include <tu/client/assets/guilabelstyle.h>
#include <tu/client/assets/guibuttonstyle.h>
#include <tu/client/assets/guitogglestyle.h>
#include <tu/client/assets/guisliderstyle.h>
#include <tu/client/assets/guiscrollbarstyle.h>
#include <tu/client/assets/guitabsstyle.h>

namespace tu
{

class CAssetsHistory
{
private:
	enum
	{
		MAXHISTORYSIZE = 128,
	};
	
	enum
	{
		OPERATION_EDITASSET=0,
		OPERATION_ADDASSET,
	};
	
	struct COperation
	{
		int m_Operation;
		CAssetPath m_AssetPath;
		void* m_pAsset;
	
		COperation() : m_pAsset(0) { }
	};
	
	struct CEntry
	{
		int m_Token;
		array<COperation> m_Operations;
		
		CEntry() : m_Token(NO_TOKEN) { }
		void Reset();
	};
	
	class CAssetsManager* m_pAssetsManager;
	CEntry m_Entries[MAXHISTORYSIZE];
	int m_LastEntry;
	int m_Size;
	int m_LastToken;

public:
	enum
	{
		NO_TOKEN=-2,
		NEW_TOKEN=-1,
	};

private:
	void NewEntry(int Token);

public:
	CAssetsHistory(class CAssetsManager* pAssetsManager);
	virtual ~CAssetsHistory();
	
	inline class CAssetsManager* AssetsManager() { return m_pAssetsManager; }
	
	void AddOperation_EditAsset(CAssetPath AssetPath, int Token);
	void AddOperation_AddAsset(CAssetPath AssetPath, int Token);
	
	void Undo();
	int GenerateToken();
};

}

#endif
