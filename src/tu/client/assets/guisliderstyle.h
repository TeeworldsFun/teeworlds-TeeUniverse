#ifndef TU_CLIENT_ASSETS_GUISLIDERSTYLE_H
#define TU_CLIENT_ASSETS_GUISLIDERSTYLE_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_GuiSliderStyle : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_GUISLIDERSTYLE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_RectPath;
		int m_DefaultRailPath;
		int m_CursorPath;
		int m_Margin;
		int m_Padding;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:
	CAssetPath m_RectPath;
	CAssetPath m_DefaultRailPath;
	CAssetPath m_CursorPath;
	int m_Margin;
	int m_Padding;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_GuiSliderStyle();
	
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
