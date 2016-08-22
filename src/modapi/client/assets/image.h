#ifndef TU_CLIENT_ASSETS_IMAGE_H
#define TU_CLIENT_ASSETS_IMAGE_H

#include <modapi/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_Image : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_IMAGE;

	struct CStorageType : public CAsset::CStorageType
	{
		int m_Width;
		int m_Height;
		int m_Format;
		int m_ImageData;
		int m_GridWidth;
		int m_GridHeight;
	};
	
	void InitFromAssetsFile(class CAssetManager* pAssetManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	void SetData(int Width, int Height, int Format, unsigned char* pData);
	int GetDataSize() const;
	void Unload(class CAssetManager* pAssetManager);
	
public:
	enum
	{
		FORMAT_RGB=0,
		FORMAT_RGBA=1,
		FORMAT_ALPHA=2,
	};

	int m_Width;
	int m_Height;
	int m_Format;
	unsigned char *m_pData;
	
	IGraphics::CTextureHandle m_Texture;
	
	int m_GridWidth;
	int m_GridHeight;
	
	//Getter/Setter for AssetsEditor
public:
	enum
	{
		GRIDWIDTH = CAsset::NUM_MEMBERS, //Int
		GRIDHEIGHT, //Int
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
	
	inline void OnAssetDeleted(const CAssetPath& Path) { }
	inline int AddSubItem(int SubItemType) { }
	inline bool DeleteSubItem(int SubItemPath) { return false; }
	inline void OnSubItemDeleted(const CAssetPath& Path, int SubItemPath) { }
};

}

#endif
