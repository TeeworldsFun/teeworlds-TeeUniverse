#ifndef TU_CLIENT_ASSETS_SPRITE_H
#define TU_CLIENT_ASSETS_SPRITE_H

#include <modapi/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_Sprite : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_SPRITE;

	struct CStorageType : public CAsset::CStorageType
	{
		int m_ImagePath;
		int m_X;
		int m_Y;
		int m_Width;
		int m_Height;
	};
	
	void InitFromAssetsFile(class CAssetManager* pAssetManager, class IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	void Unload(class CAssetManager* pAssetManager);
	
public:
	enum
	{
		FLAG_FLIP_Y = (0x1 << 0),
		FLAG_FLIP_X = (0x1 << 1),
		FLAG_FLIP_ANIM_Y = (0x1 << 2),
		FLAG_FLIP_ANIM_X = (0x1 << 3),
		FLAG_SIZE_HEIGHT = (0x1 << 4),
	};

public:
	CAssetPath m_ImagePath;
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

public:
	CAsset_Sprite() :
		m_X(0),
		m_Y(0),
		m_Width(1),
		m_Height(1)
	{
		
	}

	inline void Init(CAssetPath ImagePath, int X, int Y, int W, int H)
	{
		m_ImagePath = ImagePath;
		m_X = X;
		m_Y = Y;
		m_Width = W;
		m_Height = H;
	}
	
	//Getter/Setter for AssetsEditor
public:
	enum
	{
		IMAGEPATH = CAsset::NUM_MEMBERS, //Path
		X, //Int
		Y, //Int
		WIDTH, //Int
		HEIGHT, //Int
	};
	
	template<typename T>
	T GetValue(int ValueType, int Path, T DefaultValue)
	{
		return CAsset::GetValue<T>(ValueType, Path, DefaultValue);
	}
	
	template<typename T>
	bool SetValue(int ValueType, int Path, T Value)
	{
		return CAsset::SetValue<T>(ValueType, Path, Value);
	}
	
	inline void OnAssetDeleted(const CAssetPath& Path)
	{
		m_ImagePath.OnIdDeleted(Path);
	}
	
	inline int AddSubItem(int SubItemType) { }
	inline bool DeleteSubItem(int SubItemPath) { return false; }
	inline void OnSubItemDeleted(const CAssetPath& Path, int SubItemPath) { }
};

}

#endif
