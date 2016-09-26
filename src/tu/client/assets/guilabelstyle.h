#ifndef TU_CLIENT_ASSETS_GUILABELSTYLE_H
#define TU_CLIENT_ASSETS_GUILABELSTYLE_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

namespace tu
{

class CAsset_GuiLabelStyle : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_GUILABELSTYLE;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_RectPath;
		int m_MinWidth;
		int m_MinHeight;
		int m_Margin;
		int m_Padding;
		int m_Spacing;
		int m_FontSize;
		int m_TextColor;
		int m_TextAlignment;
	};
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* MEMBERS ************************************************************/
public:
	enum
	{
		TEXTALIGNMENT_LEFT=0,
		TEXTALIGNMENT_CENTER,
		TEXTALIGNMENT_RIGHT,
	};

private:
	int m_MinWidth;
	int m_MinHeight;
	int m_Margin;
	int m_Padding;
	int m_Spacing;
	int m_FontSize;
	CAssetPath m_RectPath;

public:
	vec4 m_TextColor;
	int m_TextAlignment;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_GuiLabelStyle();
	
	inline int GetMinWidth() const { return max(m_MinWidth, -1); }
	inline int GetMinHeight() const { return max(m_MinHeight, -1); }
	inline int GetMargin() const { return m_Margin; }
	inline int GetPadding() const { return m_Padding; }
	inline int GetSpacing() const { return m_Spacing; }
	inline int GetFontSize() const { return m_FontSize; }
	inline CAssetPath GetRectPath() const { return m_RectPath; }
	
	inline void SetMinWidth(int Value) { m_MinWidth = max(Value, -1); }
	inline void SetMinHeight(int Value) { m_MinHeight = max(Value, -1); }
	inline void SetMargin(int Value) { m_Margin = Value; }
	inline void SetPadding(int Value) { m_Padding = Value; }
	inline void SetSpacing(int Value) { m_Spacing = Value; }
	inline void SetFontSize(int Value) { m_FontSize = Value; }
	inline void SetRectPath(CAssetPath Path) { m_RectPath = Path; }
	
/* GET/SET ************************************************************/
public:
	enum
	{
		MINWIDTH = CAsset::NUM_MEMBERS, //Int
		MINHEIGHT, //Int
		MARGIN, //Int
		PADDING, //Int
		SPACING, //Int
		FONTSIZE, //Int
		RECTPATH, //AssetPath
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_RectPath.OnIdDeleted(Path);
	}
};

}

#endif
