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
		int m_External;
		
		int m_FilenameData;
		
		int m_Width;
		int m_Height;
		int m_Format;
		int m_ImageData;
		
		int m_GridWidth;
		int m_GridHeight;
	};
	
	void InitFromAssetsFile(class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:	
	enum
	{
		FORMAT_RGB=0,
		FORMAT_RGBA=1,
		FORMAT_ALPHA=2,
	};

private:
	bool m_NeedUpdate;
	int64 m_LastUpdate;
	IGraphics::CTextureHandle m_Texture;

	bool m_External;
	
	char m_aFilename[256];
	
	int m_Width;
	int m_Height;
	int m_Format;
	unsigned char *m_pData;
	
	int m_GridWidth;
	int m_GridHeight;

public:
	inline IGraphics::CTextureHandle GetTexture() const { return m_Texture; }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetGridWidth() const { return max(m_GridWidth, 1); }
	inline int GetGridHeight() const { return max(m_GridHeight, 1); }

/* FUNCTIONS **********************************************************/
public:
	CAsset_Image();	
	CAsset_Image(const CAsset_Image& Image);
	virtual ~CAsset_Image();
	CAsset_Image& operator=(const CAsset_Image& Image);
	
	void Update();
	void Unload();

	int GetDataSize() const;
	void SetData(int Width, int Height, int GridWidth, int GridHeight, int Format, unsigned char* pData);
	void SetGridSize(int GridWidth, int GridHeight);
	void LoadData(const char* pFilename, int GridWidth, int GridHeight);
	vec4 GetColor(int x, int y);
	vec4 Sample(vec2 uv);
	
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
