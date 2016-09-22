#ifndef TU_CLIENT_GUI_WIDGET_H
#define TU_CLIENT_GUI_WIDGET_H

#include "rect.h"
#include "context.h"

#include <engine/keys.h>

enum
{
	TU_INPUT_CTRL=0x1,
	TU_INPUT_SHIFT=0x2,
	TU_INPUT_ALT=0x4,
};

namespace tu
{
	
namespace gui
{

class CWidget : public CContext::CGuest
{
public:
	CRect m_BoundingSizeRect;
	CRect m_DrawRect;
	bool m_Disabled;

protected:
	CWidget(class CContext *pContext);
	
public:
	virtual ~CWidget();
	
	virtual void UpdateBoundingSize() = 0;
	virtual void UpdatePosition(CRect BoundingRect);
	virtual void Update() {};
	virtual void Render() = 0;
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState) {};
	virtual void OnButtonClick(int X, int Y, int Button, int Count) {};
	virtual void OnButtonRelease(int Button) {};
	virtual void OnInputEvent() {};
	
	virtual void OnEnableDisable() {};
		
	inline const CRect& GetBS() const { return m_BoundingSizeRect; }
	
	inline void Disable() { m_Disabled = true; OnEnableDisable(); }
	inline void Enable() { m_Disabled = false; OnEnableDisable(); }
	inline bool IsDisabled() const { return m_Disabled; }
};

}

}

#endif
