#include <engine/graphics.h>
#include <game/client/render.h>
#include <tu/client/assetsrenderer.h>
#include <tu/client/textrenderer.h>

#include "label.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT LABEL *****************************************************/

CAbstractLabel::CAbstractLabel(CContext *pContext) :
	CWidget(pContext),
	m_IconPath(CAssetPath::Null()),
	m_Localize(false)
{
	SetLabelStyle(Context()->GetLabelStyle());
	m_aText[0] = 0;
}

void CAbstractLabel::OnTextUpdated()
{
	if(m_Localize)
		m_TextCache.SetText(Localization()->Localize(m_aText));
	else
		m_TextCache.SetText(m_aText);
}

void CAbstractLabel::Update()
{
	if(m_Localize && Context()->LocalizationUpdated())
		m_TextCache.SetText(Localization()->Localize(m_aText));
}

void CAbstractLabel::UpdateBoundingSize()
{
	m_FontSize = Context()->ApplyGuiScale(11);
	int Spacing = 0;
	
	m_BoundingSizeRect.BSNoConstraint();
	
	const CAsset_GuiLabelStyle* pLabelStyle = AssetsManager()->GetAsset<CAsset_GuiLabelStyle>(m_LabelStylePath);
	if(pLabelStyle)
	{
		m_BoundingSizeRect.BSAddSpacing(
			Context()->ApplyGuiScale(pLabelStyle->GetMinWidth()),
			Context()->ApplyGuiScale(pLabelStyle->GetMinHeight())
		);
		
		m_FontSize = Context()->ApplyGuiScale(pLabelStyle->GetFontSize());
		Spacing = Context()->ApplyGuiScale(pLabelStyle->GetSpacing());
	}
	
	CRect ContentRect;
	ContentRect.BSNoConstraint();
	
	const CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(m_IconPath);
	if(pSprite)
	{
		CRect IconRect;
		IconRect.BSMinimum(
			Context()->ApplyGuiScale(Context()->GetIconScale()*pSprite->GetPixelWidth()),
			Context()->ApplyGuiScale(Context()->GetIconScale()*pSprite->GetPixelHeight())
		);
		ContentRect.BSHorizontalAdd(IconRect);
	}
	
	if(GetText()[0])
	{
		if(pSprite)
			ContentRect.BSAddSpacing(Spacing, 0);
		
		CRect TextRect;
		TextRect.BSMinimum(m_FontSize, m_FontSize);
		ContentRect.BSHorizontalAdd(TextRect);
	}
	
	m_BoundingSizeRect.BSInnerAdd(ContentRect);
	
	if(pLabelStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pLabelStyle->GetPadding()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pLabelStyle->GetMargin()));
	}
	
	m_TextCache.SetFontSize(m_FontSize);
}

void CAbstractLabel::Render()
{		
	CRect Rect = m_DrawRect;
	CRect ClipRect = m_DrawRect;
	vec4 FontColor = 1.0f;
	int Alignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_LEFT;
	int Spacing = 0;
	
	const CAsset_GuiLabelStyle* pLabelStyle = AssetsManager()->GetAsset<CAsset_GuiLabelStyle>(m_LabelStylePath);
	if(pLabelStyle)
	{
		FontColor = pLabelStyle->m_TextColor;
		Alignment = pLabelStyle->m_TextAlignment;
		
		Rect.RemoveMargin(Context()->ApplyGuiScale(pLabelStyle->GetMargin()));
		ClipRect.RemoveMargin(Context()->ApplyGuiScale(pLabelStyle->GetMargin()));
		
		AssetsRenderer()->DrawGuiRect(&Rect, pLabelStyle->GetRectPath());
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pLabelStyle->GetPadding()));
	}
	
	int IconWidth = 0;
	int TextWidth = 0;
	
	const CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(m_IconPath);
	
	if(pSprite)
	{
		IconWidth = Context()->ApplyGuiScale(Context()->GetIconScale()*pSprite->GetPixelWidth());
	}
	
	m_TextCache.SetBoxSize(ivec2(-1, Rect.h));
	
	if(GetText()[0])
	{
		if(pSprite && pLabelStyle)
			Spacing = Context()->ApplyGuiScale(pLabelStyle->GetSpacing());
		
		TextWidth = TextRenderer()->GetTextWidth(&m_TextCache);
	}
	
	int TotalWidth = IconWidth + Spacing + TextWidth;
	int PosX = 0;
	
	if(Localization()->GetWritingDirection() == CLocalization::DIRECTION_RTL)
	{
		if(Alignment == CAsset_GuiLabelStyle::TEXTALIGNMENT_LEFT)
			Alignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_RIGHT;
		else if(Alignment == CAsset_GuiLabelStyle::TEXTALIGNMENT_RIGHT)
			Alignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_LEFT;
	}
	
	if(Alignment == CAsset_GuiLabelStyle::TEXTALIGNMENT_LEFT)
		PosX = Rect.x;
	else if(Alignment == CAsset_GuiLabelStyle::TEXTALIGNMENT_RIGHT)
		PosX = Rect.x + Rect.w - TotalWidth;
	else
		PosX = Rect.x + Rect.w/2 - TotalWidth/2;
	
	Graphics()->ClipPush(ClipRect.x, ClipRect.y, ClipRect.w, ClipRect.h);
	if(Localization()->GetWritingDirection() == CLocalization::DIRECTION_RTL)
	{
		if(GetText()[0])
		{
			m_TextRect.x = PosX;
			m_TextRect.y = Rect.y;
			m_TextRect.w = TextRenderer()->GetTextWidth(&m_TextCache);
			m_TextRect.h = Rect.h;
			
			m_TextCache.SetBoxSize(ivec2(-1, m_TextRect.h));
			
			TextRenderer()->DrawText(&m_TextCache, ivec2(m_TextRect.x, m_TextRect.y), FontColor);
			
			PosX += TextWidth;
		}
		
		PosX += Spacing;
		
		if(IconWidth > 0)
		{		
			AssetsRenderer()->DrawSprite(
				m_IconPath,
				vec2(PosX + IconWidth/2, Rect.y + Rect.h/2),
				Context()->GetIconScale()*Context()->GetGuiScale(), 0.0f, 0x0, 1.0f
			);
		}
	}
	else
	{
		if(IconWidth > 0)
		{		
			AssetsRenderer()->DrawSprite(
				m_IconPath,
				vec2(PosX + IconWidth/2, Rect.y + Rect.h/2),
				Context()->GetIconScale()*Context()->GetGuiScale(), 0.0f, 0x0, 1.0f
			);
			
			PosX += IconWidth;
		}
		
		PosX += Spacing;
		
		if(GetText()[0])
		{
			m_TextRect.x = PosX;
			m_TextRect.y = Rect.y;
			m_TextRect.w = TextRenderer()->GetTextWidth(&m_TextCache);
			m_TextRect.h = Rect.h;
			
			m_TextCache.SetBoxSize(ivec2(-1, m_TextRect.h));
			
			TextRenderer()->DrawText(&m_TextCache, ivec2(m_TextRect.x, m_TextRect.y), FontColor);
		}
	}
	Graphics()->ClipPop();
}

void CAbstractLabel::SetText(const char* pText, bool Localize)
{
	if(pText)
	{
		str_copy(m_aText, pText, sizeof(m_aText));
		m_Localize = Localize;
	}
	else
		m_aText[0] = 0;
	
	OnTextUpdated();
}

/* LABEL **************************************************************/

CLabel::CLabel(CContext *pConfig, const char* pText, CAssetPath IconPath) :
	CAbstractLabel(pConfig)
{	
	SetIcon(IconPath);
	SetText(pText);
}

/* LABEL HEADER *******************************************************/

CLabelHeader::CLabelHeader(CContext *pConfig, const char* pText, CAssetPath IconPath) :
	CAbstractLabel(pConfig)
{
	SetLabelStyle(Context()->GetLabelHeaderStyle());
	SetIcon(IconPath);
	SetText(pText);
}

CLabelHeader::CLabelHeader(CContext *pConfig, const CLocalizableString& LocalizableString, CAssetPath IconPath) :
	CAbstractLabel(pConfig)
{
	SetLabelStyle(Context()->GetLabelHeaderStyle());
	SetIcon(IconPath);
	SetText(LocalizableString.m_pText, true);
}

}

}
