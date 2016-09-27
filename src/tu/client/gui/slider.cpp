#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "slider.h"

namespace tu
{
	
namespace gui
{

/* ABSTRACT SLIDER ****************************************************/

CAbstractSlider::CAbstractSlider(CContext *pContext, int Min, int Max) :
	CWidget(pContext),
	m_Clicked(false),
	m_Min(Min),
	m_Max(Max)
{
	SetSliderStyle(Context()->GetSliderStyle());
}

void CAbstractSlider::UpdateBoundingSize()
{
	m_BoundingSizeRect.BSNoConstraint();
	
	const CAsset_GuiSliderStyle* pSliderStyle = AssetsManager()->GetAsset<CAsset_GuiSliderStyle>(m_SliderStylePath);
	if(pSliderStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pSliderStyle->m_Padding));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pSliderStyle->m_Margin));
	}
}

void CAbstractSlider::OnButtonRelease(int Button)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	m_Clicked = false;
}

/* H SCROLLBAR *************************************************/

CHSlider::CHSlider(CContext *pContext, int Min, int Max) :
	CAbstractSlider(pContext, Min, Max)
{
	
}

void CHSlider::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	int Padding = 0;
	m_ClipRect = m_DrawRect;
	const CAsset_GuiSliderStyle* pSliderStyle = AssetsManager()->GetAsset<CAsset_GuiSliderStyle>(m_SliderStylePath);
	if(pSliderStyle)
	{
		Padding = Context()->ApplyGuiScale(pSliderStyle->m_Padding);
		m_ClipRect.RemoveMargin(Context()->ApplyGuiScale(pSliderStyle->m_Margin));
		m_ClipRect.RemoveMargin(Padding);
		
		const CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(pSliderStyle->m_CursorPath);
		if(pSprite)
		{
			int PosX = static_cast<float>(m_ClipRect.w * (GetValue() - m_Min))/static_cast<float>(m_Max - m_Min);
			int Width = Context()->GetImageScale()*Context()->GetGuiScale()*pSprite->GetPixelWidth();
			int Height = Context()->GetImageScale()*Context()->GetGuiScale()*pSprite->GetPixelHeight();
			m_CursorRect = CRect(
				m_ClipRect.x + PosX - Width/2,
				m_ClipRect.y - Height/2,
				Width, 
				Height
			);
		}
	}
}
	
void CHSlider::Render()
{
	const CAsset_GuiSliderStyle* pSliderStyle = AssetsManager()->GetAsset<CAsset_GuiSliderStyle>(m_SliderStylePath);
	if(pSliderStyle)
	{
		CRect Rect = m_DrawRect;
		Rect.RemoveMargin(Context()->ApplyGuiScale(pSliderStyle->m_Margin));
	
		AssetsRenderer()->DrawGuiRect(&Rect, pSliderStyle->m_RectPath);
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pSliderStyle->m_Padding));
	
		ivec2 MousePos = Context()->GetMousePos();
		
		float Scale = Context()->GetImageScale()*Context()->GetGuiScale();
		
		ivec2 RailPos0(m_ClipRect.x, m_ClipRect.y + m_ClipRect.h/2);
		ivec2 RailPos1(m_ClipRect.x + m_ClipRect.w, m_ClipRect.y + m_ClipRect.h/2);
		AssetsRenderer()->DrawGuiLine(RailPos0, RailPos1, pSliderStyle->m_DefaultRailPath, Scale);
		
		const CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(pSliderStyle->m_CursorPath);
		if(pSprite)
		{	
			AssetsRenderer()->DrawSprite(
				pSliderStyle->m_CursorPath,
				vec2(m_CursorRect.x + m_CursorRect.w/2, m_CursorRect.y + m_CursorRect.h/2),
				Context()->GetImageScale()*Context()->GetGuiScale(), 0.0f, 0x0, 1.0f
			);
		}
	}
}

void CHSlider::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(m_ClipRect.IsInside(X, Y))
	{
		if(Button == KEY_MOUSE_WHEEL_UP)
			SetValue(clamp(GetValue()+1, m_Min, m_Max));
		else if(Button == KEY_MOUSE_WHEEL_DOWN)
			SetValue(clamp(GetValue()-1, m_Min, m_Max));
	}
	else if(m_CursorRect.IsInside(X, Y))
	{
		if(Button == KEY_MOUSE_1)
		{
			m_Clicked = true;
			m_ClickShift = X - (m_CursorRect.x + m_CursorRect.w/2);
		}
	}
}

void CHSlider::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_Clicked)
	{
		int PosX = static_cast<float>(m_ClipRect.w * (GetValue() - m_Min))/static_cast<float>(m_Max - m_Min);
		
		int CursorPos = (X - m_ClickShift) - m_ClipRect.x;
		int Value = m_Min + static_cast<float>(CursorPos * (m_Max - m_Min)) / m_ClipRect.w;
		SetValue(clamp(Value, m_Min, m_Max));
	}
}

}

}
