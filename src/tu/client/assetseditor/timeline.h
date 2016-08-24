#ifndef TU_ASSETSEDITOR_TIMELINE_H
#define TU_ASSETSEDITOR_TIMELINE_H

#include <base/vmath.h>
#include <engine/kernel.h>
#include <game/client/render.h>
#include <tu/client/gui/layout.h>
#include <tu/client/gui/button.h>

#include "assetseditor.h"

namespace tu
{
	
namespace assetseditor
{

class CTimeline : public gui::CWidget
{
	class CCursorToolButton : public gui::CIconButton
	{
	protected:
		CTimeline* m_pTimeline;
		int m_CursorTool;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pTimeline->SetCursorTool(m_CursorTool);
		}
		
	public:
		CCursorToolButton(CTimeline* pTimeline, int Icon, int CursorTool) :
			gui::CIconButton(pTimeline->m_pConfig, Icon),
			m_pTimeline(pTimeline),
			m_CursorTool(CursorTool)
		{
			
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
			{
				ShowHint(CTimeline::s_CursorToolHints[m_CursorTool]);
			}
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
	class CFirstFrameButton : public gui::CIconButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->EditAssetFirstFrame();
		}
		
	public:
		CFirstFrameButton(CAssetsEditor* pAssetsEditor) :
			gui::CIconButton(pAssetsEditor->m_pGuiConfig, CAssetsEditor::ICON_FIRST_FRAME),
			m_pAssetsEditor(pAssetsEditor)
		{
			
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
				ShowHint("Move time to zero");
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
	class CLastFrameButton : public gui::CIconButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->EditAssetLastFrame();
		}
		
	public:
		CLastFrameButton(CAssetsEditor* pAssetsEditor) :
			gui::CIconButton(pAssetsEditor->m_pGuiConfig, CAssetsEditor::ICON_LAST_FRAME),
			m_pAssetsEditor(pAssetsEditor)
		{
			
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
				ShowHint("Move time to the last keyframe");
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
	class CPrevFrameButton : public gui::CIconButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->EditAssetPrevFrame();
		}
		
	public:
		CPrevFrameButton(CAssetsEditor* pAssetsEditor) :
			gui::CIconButton(pAssetsEditor->m_pGuiConfig, CAssetsEditor::ICON_PREV_FRAME),
			m_pAssetsEditor(pAssetsEditor)
		{
			
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
				ShowHint("Move time to the previous keyframe");
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
	class CNextFrameButton : public gui::CIconButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->EditAssetNextFrame();
		}
		
	public:
		CNextFrameButton(CAssetsEditor* pAssetsEditor) :
			gui::CIconButton(pAssetsEditor->m_pGuiConfig, CAssetsEditor::ICON_NEXT_FRAME),
			m_pAssetsEditor(pAssetsEditor)
		{
			
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
				ShowHint("Move time to the next keyframe");
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
	class CPlayPauseButton : public gui::CIconButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->SetPause(!m_pAssetsEditor->IsPaused());
		}
		
	public:
		CPlayPauseButton(CAssetsEditor* pAssetsEditor) :
			gui::CIconButton(pAssetsEditor->m_pGuiConfig, (pAssetsEditor->IsPaused() ? CAssetsEditor::ICON_PLAY : CAssetsEditor::ICON_PAUSE)),
			m_pAssetsEditor(pAssetsEditor)
		{
			
		}
		
		virtual void Render()
		{
			if(m_pAssetsEditor->IsPaused())
				SetIcon(CAssetsEditor::ICON_PLAY);
			else
				SetIcon(CAssetsEditor::ICON_PAUSE);
			
			gui::CIconButton::Render();
		}
		
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
				ShowHint("Play/Pause");
			
			gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
		}
	};
	
	class CTimeScaleSlider : public gui::CHSlider
	{
	protected:
		CTimeline* m_pTimeline;
		
	protected:
		virtual void OnNewPosition(float Pos)
		{
			m_pTimeline->TimeScaleCallback(Pos);
		}
		
	public:
		CTimeScaleSlider(CTimeline* pTimeline) :
			gui::CHSlider(pTimeline->m_pConfig),
			m_pTimeline(pTimeline)
		{
			m_Rect.w = 150;
			SetSliderSize(50);
		}
	};
	
	class CTimeSlider : public gui::CHSlider
	{
	protected:
		CTimeline* m_pTimeline;
		
	protected:
		virtual void OnNewPosition(float Pos)
		{
			m_pTimeline->TimeScrollCallback(Pos);
		}
		
	public:
		CTimeSlider(CTimeline* pTimeline) :
			gui::CHSlider(pTimeline->m_pConfig),
			m_pTimeline(pTimeline)
		{
			
		}
	};
	
	class CValueSlider : public gui::CVSlider
	{
	protected:
		CTimeline* m_pTimeline;
		
	protected:
		virtual void OnNewPosition(float Pos)
		{
			m_pTimeline->ValueScrollCallback(Pos);
		}
		
	public:
		CValueSlider(CTimeline* pTimeline) :
			gui::CVSlider(pTimeline->m_pConfig),
			m_pTimeline(pTimeline)
		{
			m_Pos = 0.5;
		}
	};
	
protected:
	enum
	{
		CURSORTOOL_FRAME_MOVE=0,
		CURSORTOOL_FRAME_ADD,
		CURSORTOOL_FRAME_DELETE,
		CURSORTOOL_FRAME_COLOR,
		NUM_CURSORTOOLS
	};
	
public:
	static const char* s_CursorToolHints[];

protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Margin;
	int m_ToolbarHeight;
	int m_TimelineTop;
	int m_TimelineBottom;
	int m_ValueHeight;
	
	float m_ValueScroll;
	float m_TimeScroll;
	float m_TimeScale;
	float m_TimeMax;
	
	
	float m_FrameMargin;
	float m_FrameHeight;
	
	int m_CursorTool;
	int m_CursorX;
	int m_CursorY;
	int m_Drag;
	CAsset_SkeletonAnimation::CSubPath m_DragedElement;
	CCursorToolButton* m_CursorToolButtons[NUM_CURSORTOOLS];
	
	gui::CRect m_TimelineRect;
	gui::CRect m_ListRect;
	CTimeSlider* m_pTimeSlider;
	CValueSlider* m_pValueSlider;
	gui::CHListLayout* m_pToolbar;
	
protected:
	void SetPause(bool Paused);
	void TimeScaleCallback(float Pos);
	void ValueScrollCallback(float Pos);
	void TimeScrollCallback(float Pos);
	
	int TimeToTimeline(float Time);
	float TimelineToTime(int Time);
	
public:
	CTimeline(CAssetsEditor* pAssetsEditor);
	virtual ~CTimeline();
	
	void OnEditedAssetChange();
	void OnEditedAssetFrameChange();
	
	int GetCursorTool() { return m_CursorTool; }
	void SetCursorTool(int CursorTool) { m_CursorTool = CursorTool; }
	
	CAsset_SkeletonAnimation::CSubPath KeyFramePicking(int X, int Y);
	CAsset_Skeleton::CBonePath BonePicking(int X, int Y);
	CAsset_Skeleton::CBonePath LayerPicking(int X, int Y);
	
	virtual void Update();
	virtual void Render();
	
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
};

}

}

#endif
