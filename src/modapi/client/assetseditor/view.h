#ifndef TU_ASSETSEDITOR_VIEW_H
#define TU_ASSETSEDITOR_VIEW_H

#include <base/vmath.h>
#include <engine/kernel.h>
#include <game/client/render.h>
#include <modapi/client/gui/button.h>
#include <modapi/client/gui/layout.h>

#include "assetseditor.h"

namespace tu
{

class CAssetsEditorGui_View : public gui::CWidget
{
public:
	class CZoomSlider : public gui::CHSlider
	{
	protected:
		CAssetsEditorGui_View* m_pView;
		
	protected:
		virtual void OnNewPosition(float Pos)
		{
			m_pView->ZoomCallback(Pos);
		}
		
	public:
		CZoomSlider(CAssetsEditorGui_View* pView) :
			gui::CHSlider(pView->m_pConfig),
			m_pView(pView)
		{
			m_Rect.w = 150;
			SetSliderSize(50);
		}
	};
	
	class CCursorToolButton : public gui::CIconButton
	{
	protected:
		CAssetsEditorGui_View* m_pView;
		int m_CursorTool;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pView->SetCursorTool(m_CursorTool);
		}
		
	public:
		CCursorToolButton(CAssetsEditorGui_View* pView, int Icon, int CursorTool) :
			gui::CIconButton(pView->m_pConfig, Icon),
			m_pView(pView),
			m_CursorTool(CursorTool)
		{
			
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
				ShowHint(CAssetsEditorGui_View::s_CursorToolHints[m_CursorTool]);
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
	class CViewSwitch : public gui::CIconButton
	{
	protected:
		CAssetsEditorGui_View* m_pView;
		bool* m_pBoolean;
		const char* m_pHint;
		
	protected:
		virtual void MouseClickAction()
		{
			*m_pBoolean = !(*m_pBoolean);
		}
		
	public:
		CViewSwitch(CAssetsEditorGui_View* pView, int Icon, bool* pBoolean, const char* pHint) :
			gui::CIconButton(pView->m_pConfig, Icon),
			m_pView(pView),
			m_pBoolean(pBoolean),
			m_pHint(pHint)
		{
			
		}
		
		virtual void Render()
		{
			if(*m_pBoolean)
				SetButtonStyle(gui::BUTTONSTYLE_HIGHLIGHT);
			else
				SetButtonStyle(gui::BUTTONSTYLE_NORMAL);
			
			gui::CIconButton::Render();
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
				ShowHint(m_pHint);
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
protected:
	enum
	{
		CURSORTOOL_MOVE=0,
		CURSORTOOL_TRANSLATE,
		CURSORTOOL_TRANSLATE_X,
		CURSORTOOL_TRANSLATE_Y,
		CURSORTOOL_ROTATE,
		CURSORTOOL_SCALE,
		CURSORTOOL_SCALE_X,
		CURSORTOOL_SCALE_Y,
		CURSORTOOL_BONE_LENGTH,
		CURSORTOOL_BONE_ADD,
		CURSORTOOL_BONE_DELETE,
		CURSORTOOL_SPRITE_CREATOR,
		NUM_CURSORTOOLS
	};
	
	enum
	{
		DRAGTYPE_NONE,
		DRAGTYPE_CURSORTOOL,
		DRAGTYPE_GIZMO,
		DRAGTYPE_TILT,
	};
	
	enum
	{
		GIZMO_AIM=0,
		GIZMO_MOTION,
		GIZMO_HOOK,
		NUM_GIZMOS,
	};
	
	enum
	{
		HINT_SHOW_SKELETON=0,
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	vec2 m_AimDir;
	vec2 m_MotionDir;
	vec2 m_StartPointPos;
	vec2 m_EndPointPos;
	float m_Zoom;
	bool m_ShowSkeleton;
	
	vec2 m_TiltView;
	
	vec2 m_CursorPivos;
	vec2 m_CursorToolPosition;
	vec2 m_CursorToolLastPosition;
	
	vec2 m_GizmoPos[NUM_GIZMOS];
	int m_GizmoEnabled[NUM_GIZMOS];
	
	int m_DragType;
	int m_CursorTool;
	CAssetPath m_SelectedAsset;
	CAsset_Skeleton::CSubPath m_SelectedBone;
	int m_SelectedGizmo;
	CCursorToolButton* m_CursorToolButtons[NUM_CURSORTOOLS];
	
	int m_ToolbarHeight;
	gui::CHListLayout* m_pToolbar;
	gui::CRect m_ViewRect;
	
	int m_LastEditedAssetType;
	
public:
	static const char* s_CursorToolHints[];
	static const char* s_GizmoHints[];
	static const char* s_HintText[];
	
protected:
	vec2 GetMapPosition();
	vec2 GetTeePosition();
	vec2 GetAimPosition();
	vec2 GetMotionPosition();
	
	void RefreshToolBar();
	
	bool IsOnGizmo(int GizmoId, int X, int Y);
	
	//Search Tag: TAG_NEW_ASSET
	void RenderImage();
	void RenderSprite();
	void RenderSkeleton();
	void RenderSkeletonSkin();
	void RenderSkeletonAnimation();
	void RenderCharacter();
	void RenderCharacterPart();
	void RenderWeapon();
	void RenderMap();
	void RenderGizmos();
	
public:
	CAssetsEditorGui_View(CAssetsEditor* pAssetsEditor);
	virtual ~CAssetsEditorGui_View();
	virtual void Render();
	virtual void Update();
	virtual void OnButtonClick(int X, int Y, int Button);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	
	void ZoomCallback(float Pos);
	
	inline void SetCursorTool(int CursorTool) { m_CursorTool = CursorTool; };
};

}

#endif
