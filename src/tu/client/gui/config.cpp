#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/input.h>
#include <game/client/render.h>

#include "config.h"

namespace tu
{
	
namespace gui
{

CConfig::CConfig(CClient_Graphics *pTUGraphics, CRenderTools *pRenderTools, ITextRender *pTextRender, IInput *pInput, IGraphics::CTextureHandle Texture) :
	m_pGraphics(pTUGraphics->Graphics()),
	m_pTUGraphics(pTUGraphics),
	m_pRenderTools(pRenderTools),
	m_pTextRender(pTextRender),
	m_pInput(pInput),
	m_Texture(Texture)
{
	m_TextSize[TEXTSTYLE_NORMAL] = 11.0f;
	m_TextColor[TEXTSTYLE_NORMAL] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	m_TextSize[TEXTSTYLE_HEADER] = 14.0f;
	m_TextColor[TEXTSTYLE_HEADER] = vec4(1.0f, 1.0f, 0.5f, 1.0f);
	
	m_TextSize[TEXTSTYLE_HEADER2] = 12.0f;
	m_TextColor[TEXTSTYLE_HEADER2] = vec4(1.0f, 1.0f, 0.5f, 1.0f);
	
	m_IconSize = 16;
	m_LabelMargin = 4;
	m_ButtonHeight = 20;
	m_SliderWidth = 11;
	m_SliderHeight = 100;
	m_LayoutSpacing = 4;
	m_LayoutMargin = 4;
	m_ExpandShift = 16;
	m_ExpandSpacing = 4;
	
	m_fShowHint = 0;
	m_pShowHintData = 0;
	
	//Button Styles
	m_ButtonStyles[BUTTONSTYLE_DEFAULT].m_StylePath_Normal = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_BUTTON_DEFAULT);
	m_ButtonStyles[BUTTONSTYLE_DEFAULT].m_StylePath_UnderMouse = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_BUTTON_DEFAULT_MOUSEOVER);
	
	m_ButtonStyles[BUTTONSTYLE_DEFAULT_HIGHLIGHT].m_StylePath_Normal = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_BUTTON_DEFAULT_HIGHLIGHT);
	m_ButtonStyles[BUTTONSTYLE_DEFAULT_HIGHLIGHT].m_StylePath_UnderMouse = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_BUTTON_DEFAULT_MOUSEOVER);
	
	m_ButtonStyles[BUTTONSTYLE_LINK].m_StylePath_Normal = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_BUTTON_LINK);
	m_ButtonStyles[BUTTONSTYLE_LINK].m_StylePath_UnderMouse = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_BUTTON_LINK_MOUSEOVER);
	
	m_ButtonStyles[BUTTONSTYLE_LINK_HIGHLIGHT].m_StylePath_Normal = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_BUTTON_LINK_HIGHLIGHT);
	m_ButtonStyles[BUTTONSTYLE_LINK_HIGHLIGHT].m_StylePath_UnderMouse = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_BUTTON_LINK_MOUSEOVER);
	
	//Layout Styles
	m_LayoutStyles[LAYOUTSTYLE_DEFAULT].m_StylePath = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_LAYOUT_DEFAULT);
	m_LayoutStyles[LAYOUTSTYLE_DEFAULT].m_Padding = 4;
	m_LayoutStyles[LAYOUTSTYLE_DEFAULT].m_Spacing = 4;
	
	m_LayoutStyles[LAYOUTSTYLE_INVISIBLE].m_StylePath = CAssetPath::Null();
	m_LayoutStyles[LAYOUTSTYLE_INVISIBLE].m_Padding = 4;
	m_LayoutStyles[LAYOUTSTYLE_INVISIBLE].m_Spacing = 4;
	
	m_LayoutStyles[LAYOUTSTYLE_NONE].m_StylePath = CAssetPath::Null();
	m_LayoutStyles[LAYOUTSTYLE_NONE].m_Padding = 0;
	m_LayoutStyles[LAYOUTSTYLE_NONE].m_Spacing = 4;
	
	//Tab Styles
	m_TabStyles[TABSTYLE_DEFAULT].m_StylePath_Content = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_TAB_CONTENT);
	m_TabStyles[TABSTYLE_DEFAULT].m_StylePath_Button = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_TAB_BUTTON);
	m_TabStyles[TABSTYLE_DEFAULT].m_StylePath_ButtonHighlight = CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_TAB_BUTTON_HIGHLIGHT);
}

}

}
