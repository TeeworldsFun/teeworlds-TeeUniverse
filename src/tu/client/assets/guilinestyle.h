#ifndef TU_CLIENT_ASSETS_GUILINESTYLE_H
#define TU_CLIENT_ASSETS_GUILINESTYLE_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_GuiLineStyle : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_GUILINESTYLE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_Flags;
		int m_BorderColor;
		int m_ImagePath;
		float m_ImageUV_X_Min;
		float m_ImageUV_X_Max;
		float m_ImageUV_Y_Min;
		float m_ImageUV_Y_Max;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:
	enum
	{
		FLAG_BORDER=1,
		FLAG_IMAGE=2,
	};

	int m_Flags;
	vec4 m_BorderColor;
	CAssetPath m_ImagePath;
	vec2 m_ImageUV_Min;
	vec2 m_ImageUV_Max;
	

/* FUNCTIONS **********************************************************/
public:
	CAsset_GuiLineStyle();
	
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
