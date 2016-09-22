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
	m_IconPath(CAssetPath::Null())
{
	SetBoxStyle(Context()->GetLabelStyle());
	m_aText[0] = 0;
	m_aRendererText[0] = 0;
	m_TextCache.Reset();
}

void CAbstractLabel::OnTextUpdated()
{
	if(str_comp(m_aRendererText, m_aText) != 0)
	{
		str_copy(m_aRendererText, m_aText, sizeof(m_aRendererText));
		m_TextCache.Reset();
	}
}

void CAbstractLabel::UpdateBoundingSize()
{
	m_FontSize = Context()->ApplyGuiScale(11);
	int Spacing = 0;
	
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		m_FontSize = Context()->ApplyGuiScale(pBoxStyle->GetFontSize());
		Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
	}
	
	m_BoundingSizeRect.BSNoConstraint();
	const CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(m_IconPath);
	if(pSprite)
	{
		CRect IconRect;
		IconRect.BSMinimum(
			Context()->ApplyGuiScale(pSprite->GetPixelWidth()),
			Context()->ApplyGuiScale(pSprite->GetPixelHeight())
		);
		m_BoundingSizeRect.BSHorizontalAdd(IconRect);
	}
	
	if(GetRendererText()[0])
	{
		if(pSprite)
			m_BoundingSizeRect.BSAddSpacing(Spacing, 0);
		
		CRect TextRect;
		TextRect.BSMinimum(m_FontSize, m_FontSize);
		m_BoundingSizeRect.BSHorizontalAdd(TextRect);
	}
	
	if(pBoxStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	}
}

void CAbstractLabel::Render()
{		
	CRect Rect = m_DrawRect;
	CRect ClipRect = m_DrawRect;
	vec4 FontColor = 1.0f;
	int Alignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	m_FontSize = Context()->ApplyGuiScale(11);
	int Spacing = 0;
	
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		m_FontSize = Context()->ApplyGuiScale(pBoxStyle->GetFontSize());
		FontColor = pBoxStyle->m_TextColor;
		Alignment = pBoxStyle->m_TextAlignment;
		
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
		ClipRect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
		
		ivec2 MousePos = Context()->GetMousePos();
		if(Rect.IsInside(MousePos.x, MousePos.y))
			AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetMouseOverRectPath());
		else
			AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetDefaultRectPath());
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
	}
	
	int IconWidth = 0;
	int TextWidth = 0;
	
	const CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(m_IconPath);
	
	if(pSprite)
	{
		IconWidth = Context()->ApplyGuiScale(pSprite->GetPixelWidth());
	}
	
	if(GetRendererText()[0])
	{
		if(pSprite && pBoxStyle)
			Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
		
		TextWidth = TextRenderer()->GetTextWidth(GetRendererText(), CRect(0, 0, TextWidth, Rect.h), m_FontSize, &m_TextCache);
	}
	
	int TotalWidth = IconWidth + Spacing + TextWidth;
	int PosX = 0;
	
	if(Context()->GetGuiDirection() == CContext::DIRECTION_RTL)
	{
		if(Alignment == CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT)
			Alignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_RIGHT;
		else if(Alignment == CAsset_GuiBoxStyle::TEXTALIGNMENT_RIGHT)
			Alignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	}
	
	if(Alignment == CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT)
		PosX = Rect.x;
	else if(Alignment == CAsset_GuiBoxStyle::TEXTALIGNMENT_RIGHT)
		PosX = Rect.x + Rect.w - TotalWidth;
	else
		PosX = Rect.x + Rect.w/2 - TotalWidth/2;
	
	Graphics()->ClipPush(ClipRect.x, ClipRect.y, ClipRect.w, ClipRect.h);
	if(Context()->GetGuiDirection() == CContext::DIRECTION_RTL)
	{
		if(GetRendererText()[0])
		{
			m_TextRect = CRect(PosX, Rect.y, TextWidth, Rect.h);
			TextRenderer()->DrawText(GetRendererText(), m_TextRect, m_FontSize, FontColor, &m_TextCache);
			PosX += TextWidth;
		}
		
		PosX += Spacing;
		
		if(IconWidth > 0)
		{		
			AssetsRenderer()->DrawSprite(
				m_IconPath,
				vec2(PosX + IconWidth/2, Rect.y + Rect.h/2),
				Context()->GetGuiScale(), 0.0f, 0x0, 1.0f
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
				Context()->GetGuiScale(), 0.0f, 0x0, 1.0f
			);
			
			PosX += IconWidth;
		}
		
		PosX += Spacing;
		
		if(GetRendererText()[0])
		{
			m_TextRect = CRect(PosX, Rect.y, TextWidth, Rect.h);
			TextRenderer()->DrawText(GetRendererText(), m_TextRect, m_FontSize, FontColor, &m_TextCache);
		}
	}
	Graphics()->ClipPop();
}

void CAbstractLabel::SetText(const char* pText)
{
	if(pText)
		str_copy(m_aText, pText, sizeof(m_aText));
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
	SetBoxStyle(Context()->GetLabelHeaderStyle());
	SetIcon(IconPath);
	SetText(pText);
}

}

}
