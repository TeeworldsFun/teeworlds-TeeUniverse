#ifndef TU_CLIENT_ASSETS_GUITOGGLESTYLE_H
#define TU_CLIENT_ASSETS_GUITOGGLESTYLE_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_GuiToggleStyle : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_GUITOGGLESTYLE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_SwitchIcon;
		
		int m_IdleTrueStylePath;
		int m_MouseOverTrueStylePath;
		int m_IconTruePath;
		
		int m_IdleFalseStylePath;
		int m_MouseOverFalseStylePath;
		int m_IconFalsePath;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
private:
	bool m_SwitchIcon;

	CAssetPath m_IdleTrueStylePath;
	CAssetPath m_MouseOverTrueStylePath;
	CAssetPath m_IconTruePath;
	
	CAssetPath m_IdleFalseStylePath;
	CAssetPath m_MouseOverFalseStylePath;
	CAssetPath m_IconFalsePath;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_GuiToggleStyle();
	
	inline bool GetSwitchIcon() const { return m_SwitchIcon; }
	inline CAssetPath GetIdleTrueStylePath() const { return m_IdleTrueStylePath; }
	inline CAssetPath GetIdleFalseStylePath() const { return m_IdleFalseStylePath; }
	inline CAssetPath GetMouseOverTrueStylePath() const { return m_MouseOverTrueStylePath; }
	inline CAssetPath GetMouseOverFalseStylePath() const { return m_MouseOverFalseStylePath; }
	inline CAssetPath GetIconTruePath() const { return m_IconTruePath; }
	inline CAssetPath GetIconFalsePath() const { return m_IconFalsePath; }
	
	inline void SetSwitchIcon(bool Value) { m_SwitchIcon = Value; }
	inline void SetIdleTrueStylePath(CAssetPath Path) { m_IdleTrueStylePath = Path; }
	inline void SetIdleFalseStylePath(CAssetPath Path) { m_IdleFalseStylePath = Path; }
	inline void SetMouseOverTrueStylePath(CAssetPath Path) { m_MouseOverTrueStylePath = Path; }
	inline void SetMouseOverFalseStylePath(CAssetPath Path) { m_MouseOverFalseStylePath = Path; }
	inline void SetIconTruePath(CAssetPath Path) { m_IconTruePath = Path; }
	inline void SetIconFalsePath(CAssetPath Path) { m_IconFalsePath = Path; }
	
/* GET/SET ************************************************************/
public:
	enum
	{
		SWITCHICON = CAsset::NUM_MEMBERS, //bool
		IDLETRUESTYLEPATH, //AssetPath
		IDLEFALSESTYLEPATH, //AssetPath
		MOUSEOVERTRUESTYLEPATH, //AssetPath
		MOUSEOVERFALSESTYLEPATH, //AssetPath
		ICONTRUEPATH, //AssetPath
		ICONFALSEPATH, //AssetPath
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_IdleTrueStylePath.OnIdDeleted(Path);
		m_IdleFalseStylePath.OnIdDeleted(Path);
		m_MouseOverTrueStylePath.OnIdDeleted(Path);
		m_MouseOverFalseStylePath.OnIdDeleted(Path);
		m_IconTruePath.OnIdDeleted(Path);
		m_IconFalsePath.OnIdDeleted(Path);
	}
};

}

#endif
