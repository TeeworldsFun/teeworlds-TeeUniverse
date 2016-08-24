#ifndef TU_CLIENT_ASSETS_IMAGE_H
#define TU_CLIENT_ASSETS_IMAGE_H

#include <tu/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_Image : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_IMAGE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_Width;
		int m_Height;
		int m_Format;
		int m_ImageData;
		int m_GridWidth;
		int m_GridHeight;
	};
	
	void InitFromAssetsFile(class CAssetsManager* pAssetsManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:	
	enum
	{
		FORMAT_RGB=0,
		FORMAT_RGBA=1,
		FORMAT_ALPHA=2,
	};
	
	bool m_NeedUpdate;
	int64 m_LastUpdate;
	class CAssetsManager* m_pAssetsManager;
	IGraphics::CTextureHandle m_Texture;

	int m_Width;
	int m_Height;
	int m_GridWidth;
	int m_GridHeight;
	int m_Format;
	unsigned char *m_pData;

/* FUNCTIONS **********************************************************/
public:
	CAsset_Image();
	CAsset_Image(class CAssetsManager* pAssetsManager);
	
	void Update();
	void Unload();

	int GetDataSize() const;
	void SetData(int Width, int Height, int GridWidth, int GridHeight, int Format, unsigned char* pData);
	
	void SetAssetsManager(class CAssetsManager* pAssetsManager) { m_pAssetsManager = pAssetsManager; }
	
/* GET/SET ************************************************************/
public:
	enum
	{
		GRIDWIDTH = CAsset::NUM_MEMBERS, //Int
		GRIDHEIGHT, //Int
		WIDTH, //Int
		HEIGHT, //Int
	};
	
	TU_ASSET_GETSET_FUNC()
};

}

#endif
