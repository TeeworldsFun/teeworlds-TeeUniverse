#ifndef TU_CLIENT_ASSETS_GUIRECTSTYLE_H
#define TU_CLIENT_ASSETS_GUIRECTSTYLE_H

#include <tu/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_GuiRectStyle : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_GUIRECTSTYLE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_Flags;
		int m_BackgroundColor;
		float m_BackgroundPadding;
		int m_BorderColor;
		int m_BorderFlags;
		float m_CornerRadius;
		int m_CornerFlags;
		int m_ImagePath;
		float m_ImageUV_X_Min;
		float m_ImageUV_X_Max;
		float m_ImageUV_Y_Min;
		float m_ImageUV_Y_Max;
	};
	
	void InitFromAssetsFile(class IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:
	enum
	{
		FLAG_BACKGROUND=1,
		FLAG_BORDER=2,
		FLAG_ROUNDCORNER=4,
		FLAG_IMAGE=8,
	};
	
	enum
	{
		BORDERFLAG_T=1,
		BORDERFLAG_B=2,
		BORDERFLAG_L=4,
		BORDERFLAG_R=8,
		
		BORDERFLAG_ALL=BORDERFLAG_T|BORDERFLAG_B|BORDERFLAG_L|BORDERFLAG_R,
	};
	
	enum
	{
		CORNERFLAG_TL=1,
		CORNERFLAG_TR=2,
		CORNERFLAG_BL=4,
		CORNERFLAG_BR=8,
		
		CORNERFLAG_T=CORNERFLAG_TL|CORNERFLAG_TR,
		CORNERFLAG_B=CORNERFLAG_BL|CORNERFLAG_BR,
		CORNERFLAG_L=CORNERFLAG_TL|CORNERFLAG_BL,
		CORNERFLAG_R=CORNERFLAG_TR|CORNERFLAG_BR,
		
		CORNERFLAG_ALL=CORNERFLAG_B|CORNERFLAG_T,
	};

	int m_Flags;
	vec4 m_BackgroundColor;
	float m_BackgroundPadding;
	vec4 m_BorderColor;
	int m_BorderFlags;
	float m_CornerRadius;
	int m_CornerFlags;
	CAssetPath m_ImagePath;
	vec2 m_ImageUV_Min;
	vec2 m_ImageUV_Max;
	

/* FUNCTIONS **********************************************************/
public:
	CAsset_GuiRectStyle();
	
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
