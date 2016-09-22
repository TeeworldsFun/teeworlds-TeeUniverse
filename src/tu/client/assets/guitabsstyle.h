#ifndef TU_CLIENT_ASSETS_GUITABSSTYLE_H
#define TU_CLIENT_ASSETS_GUITABSSTYLE_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_GuiTabsStyle : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_GUITABSSTYLE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_LayoutPath;
		int m_ContentPath;
		int m_ButtonListPath;
		int m_InactiveButtonPath;
		int m_ActiveButtonPath;
		int m_ButtonListFill;
		int m_ButtonListText;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:
	CAssetPath m_LayoutPath;
	CAssetPath m_ContentPath;
	CAssetPath m_ButtonListPath;
	CAssetPath m_InactiveButtonPath;
	CAssetPath m_ActiveButtonPath;
	bool m_ButtonListFill;
	bool m_ButtonListText;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_GuiTabsStyle();
	
/* GET/SET ************************************************************/
public:
	enum
	{
		//~ IMAGEPATH = CAsset::NUM_MEMBERS, //Path
	};
	
	TU_ASSET_GETSET_FUNC()
};

}

#endif
