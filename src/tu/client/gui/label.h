#ifndef TU_CLIENT_GUI_LABEL_H
#define TU_CLIENT_GUI_LABEL_H

#include "widget.h"
#include <tu/client/textrenderer.h>
#include <tu/client/localization.h>

namespace tu
{
	
namespace gui
{

class CAbstractLabel : public CWidget
{
private:
	CAssetPath m_LabelStylePath;
	CAssetPath m_IconPath;
	int m_FontSize;
	CRect m_TextRect;
	bool m_Localize;

protected:
	char m_aText[128];
	CTextRenderer::CTextCache m_TextCache;
	
public:
	CAbstractLabel(CContext *pContext);
	
	virtual void Update();
	virtual void UpdateBoundingSize();
	virtual void Render();
	
	inline void SetIcon(CAssetPath IconPath) { m_IconPath = IconPath; }
	inline CAssetPath GetIcon() const { return m_IconPath; }
	
	inline void SetLabelStyle(CAssetPath Path) { m_LabelStylePath = Path; }
	inline CAssetPath GetLabelStyle() const { return m_LabelStylePath; }
	
	inline ivec2 GetTextPosition() const { return ivec2(m_TextRect.x, m_TextRect.y); }
	inline float GetFontSize() const { return m_FontSize; }
	
	void SetText(const char* pText, bool Localize = false);
	inline const char* GetText() const { return m_aText; }
	
	inline const CRect& GetTextRect() const { return m_TextRect; }
	
	void OnTextUpdated();
};

class CLabel : public CAbstractLabel
{
public:
	CLabel(class CContext *pConfig, const char* pText, CAssetPath IconPath = CAssetPath::Null());
};

class CLabelHeader : public CAbstractLabel
{
public:
	CLabelHeader(class CContext *pConfig, const char* pText, CAssetPath IconPath = CAssetPath::Null());
	CLabelHeader(class CContext *pConfig, const CLocalizableString& LocalizableString, CAssetPath IconPath = CAssetPath::Null());
};

}

}

#endif
