#ifndef TU_ASSETSEDITOR_VIEW_H
#define TU_ASSETSEDITOR_VIEW_H

#include <base/vmath.h>
#include <engine/kernel.h>
#include <game/client/render.h>
#include <tu/client/gui/button.h>
#include <tu/client/gui/layout.h>
#include <tu/client/assetseditor/popup.h>
#include <tu/client/maprenderer.h>

#include "assetseditor.h"
#include "view_cursortool.h"

namespace tu
{

namespace assetseditor
{

class CView : public gui::CWidget
{
public:
	class CZoomSlider : public gui::CHSlider
	{
	protected:
		CView* m_pView;
		
	protected:
		virtual void OnNewPosition(float Pos)
		{
			m_pView->ZoomCallback(Pos);
		}
		
	public:
		CZoomSlider(CView* pView) :
			gui::CHSlider(pView->m_pConfig),
			m_pView(pView)
		{
			m_Rect.w = 150;
			SetSliderSize(50);
		}
		
		static float ZoomSliderConversion(int ViewType, float Val)
		{
			switch(ViewType)
			{
				case CView::VIEWTYPE_MAP:
				{
					float LogVal = log10(Val);
					float MinVal = -1.0f;
					float MaxVal = 0.3f;
					float ValWidth = MaxVal - MinVal;
					
					float Slider = (LogVal - MinVal)/ValWidth;
					return Slider;
				}
				default:
				{
					float LogVal = log10(Val);
					float MinVal = -1.0f;
					float MaxVal = 2.0f;
					float ValWidth = MaxVal - MinVal;
					
					float Slider = (LogVal - MinVal)/ValWidth;
					return Slider;
				}
			}
		}
		
		static float SliderZoomConversion(int ViewType, float Slider)
		{
			switch(ViewType)
			{
				case CView::VIEWTYPE_MAP:
				{
					float MinVal = -1.0f;
					float MaxVal = 0.3f;
					float ValWidth = MaxVal - MinVal;
					float LogVal = MinVal + Slider * ValWidth;
					return pow(10.0f, LogVal);
				}
				default:
				{
					float MinVal = -1.0f;
					float MaxVal = 2.0f;
					float ValWidth = MaxVal - MinVal;
					float LogVal = MinVal + Slider * ValWidth;
					return pow(10.0f, LogVal);
				}
			}
		}
	};
	
	class CViewSwitch : public gui::CIconButton
	{
	protected:
		CView* m_pView;
		bool* m_pBoolean;
		const char* m_pHint;
		
	protected:
		virtual void MouseClickAction()
		{
			*m_pBoolean = !(*m_pBoolean);
		}
		
	public:
		CViewSwitch(CView* pView, int Icon, bool* pBoolean, const char* pHint) :
			gui::CIconButton(pView->m_pConfig, Icon),
			m_pView(pView),
			m_pBoolean(pBoolean),
			m_pHint(pHint)
		{
			
		}
		
		virtual void Render()
		{
			if(*m_pBoolean)
				SetButtonStyle(gui::CConfig::BUTTONSTYLE_DEFAULT_HIGHLIGHT);
			else
				SetButtonStyle(gui::CConfig::BUTTONSTYLE_DEFAULT);
			
			gui::CIconButton::Render();
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
				ShowHint(m_pHint);
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
public:	
	enum
	{
		VIEWTYPE_OBJECT,
		VIEWTYPE_MAP,
	};
	
	enum
	{
		DRAGTYPE_NONE,
		DRAGTYPE_VIEW,
		DRAGTYPE_CURSORTOOL,
		DRAGTYPE_GIZMO,
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
		HINT_SHOW_ZONE,
		HINT_SHOW_GRID,
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	vec2 m_AimDir;
	vec2 m_MotionDir;
	vec2 m_StartPointPos;
	vec2 m_EndPointPos;
	bool m_ShowSkeleton;
	bool m_ShowZones;
	bool m_ShowGrid;
	
	vec2 m_CursorPivot;
	vec2 m_CursorToolPosition;
	vec2 m_CursorToolLastPosition;
	
	vec2 m_GizmoPos[NUM_GIZMOS];
	int m_GizmoEnabled[NUM_GIZMOS];
	
	int m_ViewType;
	int m_DragType;
	CAssetPath m_SelectedAsset;
	CAsset_Skeleton::CSubPath m_SelectedBone;
	int m_SelectedGizmo;
	CZoomSlider* m_pZoomSlider;
	
	int m_ToolbarHeight;
	gui::CHListLayout* m_pToolbar;
	gui::CRect m_ViewRect;
	
	int m_LastEditedAssetType;
	
	//Cursor Tools
	CView_CursorTool* m_pCursorTool;
	CView_CursorTool::CButtonCore* m_pDefaultCursorToolButton;
	bool m_CursorToolFound;
	
	//Object View
	float m_ObjectZoom;
	
	//Map View
	vec2 m_MapPos;
	float m_MapZoom;
	
	
public:
	static const char* s_CursorToolHints[];
	static const char* s_GizmoHints[];
	static const char* s_HintText[];
	
public:
	vec2 GetViewCenter();
	vec2 GetViewPos();
	float GetViewZoom();
	vec2 GetAimPos();
	vec2 GetMotionPos();
	
private:	
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
	CView(CAssetsEditor* pAssetsEditor);
	virtual ~CView();
	virtual void Render();
	virtual void Update();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	
	CGraphics* TUGraphics(int Type = ASSETS_GAME) { return m_pAssetsEditor->TUGraphics(Type); }
	CAssetsEditor* AssetsEditor() { return m_pAssetsEditor; }
	CAssetsManager* AssetsManager() { return m_pAssetsEditor->AssetsManager(); }
	
	void RefreshToolBar();
	
	void ZoomCallback(float Pos);
	
	//Getter
	inline gui::CHListLayout* Toolbar() { return m_pToolbar; }
	inline gui::CRect GetViewRect() { return m_ViewRect; }
	
	//MapRenderer
	void InitMapRenderer(CMapRenderer* pMapRenderer);
	void InitMapRenderer_Layer(CMapRenderer* pMapRenderer, CAssetPath LayerPath);
	
	//Cursor Tools
	inline CView_CursorTool* GetCursorTool() { return m_pCursorTool; }
	inline void SetCursorTool(CView_CursorTool* pCursorTool)
	{
		if(m_pCursorTool)
			delete m_pCursorTool;
		m_pCursorTool = pCursorTool;
		RefreshToolBar();
	}
	inline void SetDefaultCursorToolButton(CView_CursorTool::CButtonCore* pCursorToolButton)
	{
		m_pDefaultCursorToolButton = pCursorToolButton;
	}
	inline void CursorToolFound() { m_CursorToolFound = true; }
	
	inline void SetDragType(int Type) { m_DragType = Type; }
};

}

}

#endif
