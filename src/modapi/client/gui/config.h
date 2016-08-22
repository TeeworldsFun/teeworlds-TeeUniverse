#ifndef TU_CLIENT_GUI_CONFIG_H
#define TU_CLIENT_GUI_CONFIG_H

#include <engine/graphics.h>

class IGraphics;
class ITextRender;
class CRenderTools;
class IInput;

namespace tu
{
	
namespace gui
{

enum
{
	TEXTSTYLE_NORMAL = 0,
	TEXTSTYLE_HEADER,
	TEXTSTYLE_HEADER2,
	NUM_TEXTSTYLES,
};

enum
{
	BUTTONSTYLE_NONE = 0,
	BUTTONSTYLE_NORMAL,
	BUTTONSTYLE_HIGHLIGHT,
	BUTTONSTYLE_MOUSEOVER,
	NUM_BUTTONSTYLES,
};

typedef void (*CShowHintFunc)(const char*, void*);

class CConfig
{
public:
	IGraphics *m_pGraphics;
	ITextRender *m_pTextRender;
	CRenderTools *m_pRenderTools;
	IInput *m_pInput;
	
	IGraphics::CTextureHandle m_Texture;
	
	float m_TextSize[NUM_TEXTSTYLES];
	vec4 m_TextColor[NUM_TEXTSTYLES];
	vec4 m_ButtonColor[NUM_BUTTONSTYLES];
	
	int m_IconSize;
	int m_LabelMargin;
	int m_ButtonHeight;
	int m_SliderWidth;
	int m_SliderHeight;
	int m_LayoutSpacing;
	int m_LayoutMargin;
	int m_ExpandShift;
	int m_ExpandSpacing;
	
	CShowHintFunc m_fShowHint;
	void* m_pShowHintData;
	
public:
	CConfig(IGraphics *pGraphics, CRenderTools *pRenderTools, ITextRender *pTextRender, IInput *pInput, IGraphics::CTextureHandle Texture);
	void ShowHind(const char* pText);
};

}

}

#endif
