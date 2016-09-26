#ifndef TU_CLIENT_ASSETS_GUIBUTTONSTYLE_H
#define TU_CLIENT_ASSETS_GUIBUTTONSTYLE_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_GuiButtonStyle : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_GUIBUTTONSTYLE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_IdleStylePath;
		int m_MouseOverStylePath;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
private:
	CAssetPath m_IdleStylePath;
	CAssetPath m_MouseOverStylePath;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_GuiButtonStyle();
	
	inline CAssetPath GetIdleStylePath() const { return m_IdleStylePath; }
	inline CAssetPath GetMouseOverStylePath() const { return m_MouseOverStylePath; }
	
	inline void SetIdleStylePath(CAssetPath Path) { m_IdleStylePath = Path; }
	inline void SetMouseOverStylePath(CAssetPath Path) { m_MouseOverStylePath = Path; }
	
/* GET/SET ************************************************************/
public:
	enum
	{
		IDLESTYLEPATH = CAsset::NUM_MEMBERS, //AssetPath
		MOUSEOVERSTYLEPATH, //AssetPath
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_IdleStylePath.OnIdDeleted(Path);
		m_MouseOverStylePath.OnIdDeleted(Path);
	}
};

}

#endif
