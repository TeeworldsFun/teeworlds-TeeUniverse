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
}

void CAbstractLabel::OnTextUpdated()
{
	m_TextCache.SetText(m_aText);
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
	
	if(GetText()[0])
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
	
	m_TextCache.SetFontSize(m_FontSize);
}

void CAbstractLabel::Render()
{		
	CRect Rect = m_DrawRect;
	CRect ClipRect = m_DrawRect;
	vec4 FontColor = 1.0f;
	int Alignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	int Spacing = 0;
	
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
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
	
	m_TextCache.SetBoxSize(ivec2(-1, Rect.h));
	
	if(GetText()[0])
	{
		if(pSprite && pBoxStyle)
			Spacing = Context()->ApplyGuiScale(pBoxStyle->GetSpacing());
		
		TextWidth = TextRenderer()->GetTextWidth(&m_TextCache);
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
		if(GetText()[0])
		{
			m_RendererTextPosition = ivec2(PosX, Rect.y);
			TextRenderer()->DrawText(&m_TextCache, m_RendererTextPosition, FontColor);
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
		
		if(GetText()[0])
		{
			m_RendererTextPosition = ivec2(PosX, Rect.y);
			m_TextCache.SetBoxSize(ivec2(-1, Rect.h));
			TextRenderer()->DrawText(&m_TextCache, m_RendererTextPosition, FontColor);
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
