#include <engine/graphics.h>
#include <engine/textrender.h>
#include <game/client/render.h>

#include "widget.h"

namespace tu
{
	
namespace gui
{

CWidget::CWidget(CContext *pContext) :
	CContext::CGuest(pContext),
	m_Disabled(false)
{
	
}

CWidget::~CWidget()
{
	
}

void CWidget::UpdatePosition(CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
}

}

}
