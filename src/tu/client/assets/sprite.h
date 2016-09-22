#ifndef TU_CLIENT_ASSETS_SPRITE_H
#define TU_CLIENT_ASSETS_SPRITE_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_Sprite : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_SPRITE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_ImagePath;
		int m_X;
		int m_Y;
		int m_Width;
		int m_Height;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:	
	enum
	{
		FLAG_FLIP_Y = (0x1 << 0),
		FLAG_FLIP_X = (0x1 << 1),
		FLAG_FLIP_ANIM_Y = (0x1 << 2),
		FLAG_FLIP_ANIM_X = (0x1 << 3),
		FLAG_SIZE_HEIGHT = (0x1 << 4),
	};

private:
	CAssetPath m_ImagePath;
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

/* FUNCTIONS **********************************************************/
public:
	CAsset_Sprite();

	inline void Init(CAssetPath ImagePath, int X, int Y, int W, int H)
	{
		m_ImagePath = ImagePath;
		m_X = X;
		m_Y = Y;
		m_Width = W;
		m_Height = H;
	}
	
	inline int GetImagePath() const { return m_ImagePath; }
	inline int GetX() const { return m_X; }
	inline int GetY() const { return m_Y; }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	int GetPixelWidth() const;
	int GetPixelHeight() const;
	
/* GET/SET ************************************************************/
public:
	enum
	{
		IMAGEPATH = CAsset::NUM_MEMBERS, //Path
		X, //Int
		Y, //Int
		WIDTH, //Int
		HEIGHT, //Int
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_ImagePath.OnIdDeleted(Path);
	}
};

}

#endif
