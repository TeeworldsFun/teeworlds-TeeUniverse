#include <base/tl/sorted_array.h>

#include <engine/shared/config.h>
#include <engine/shared/datafile.h>

#include <engine/client.h>
#include <engine/input.h>
#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/storage.h>
#include <engine/keys.h>

#include <tu/client/clientmode.h>
#include <tu/client/gui/button.h>
#include <tu/client/gui/label.h>
#include <tu/client/gui/layout.h>
#include <tu/client/gui/integer-edit.h>
#include <tu/client/gui/text-edit.h>

#include <cstddef>

#include "timeline.h"
#include "editor.h"
#include "popup.h"

namespace tu
{

namespace assetseditor
{

const char* CTimeline::s_CursorToolHints[] = {
	"Key Frame Editor: move a key frame in the timeline along the time axis",
	"Key Frame Creator: add a new key frame in the timeline",
	"Key Frame Eraser: delete a key frame in the timeline along the time axis",
	"Key Frame Painter: edit color of a key frame (only for layers)",
};

CTimeline::CTimeline(CAssetsEditor* pAssetsEditor) :
	tu::gui::CWidget(pAssetsEditor->m_pGuiConfig),
	m_pAssetsEditor(pAssetsEditor),
	m_TimeScroll(0.0f),
	m_ValueScroll(0.0f),
	m_TimeScale(1.0f),
	m_TimeMax(5.0f),
	m_ToolbarHeight(30),
	m_TimelineTop(0),
	m_TimelineBottom(0),
	m_pToolbar(0),
	m_ValueHeight(100),
	m_CursorTool(CURSORTOOL_FRAME_MOVE),
	m_Drag(0),
	m_DragedElement(CAsset_SkeletonAnimation::CSubPath::Null())
{
	m_pTimeSlider = new CTimeSlider(this);
	m_pValueSlider = new CValueSlider(this);
	
	tu::gui::CLabel* pTimeScaleLabel = new tu::gui::CLabel(m_pConfig, "Time scale:");
	pTimeScaleLabel->SetRect(tu::gui::CRect(
		0, 0, //Positions will be filled when the toolbar is updated
		90,
		pTimeScaleLabel->m_Rect.h
	));
	
	m_CursorToolButtons[CURSORTOOL_FRAME_MOVE] = new CCursorToolButton(this, CAssetPath::SpriteUniverse(SPRITE_ICON_FRAME_MOVE), CURSORTOOL_FRAME_MOVE);
	m_CursorToolButtons[CURSORTOOL_FRAME_ADD] = new CCursorToolButton(this, CAssetPath::SpriteUniverse(SPRITE_ICON_FRAME_ADD), CURSORTOOL_FRAME_ADD);
	m_CursorToolButtons[CURSORTOOL_FRAME_DELETE] = new CCursorToolButton(this, CAssetPath::SpriteUniverse(SPRITE_ICON_FRAME_DELETE), CURSORTOOL_FRAME_DELETE);
	m_CursorToolButtons[CURSORTOOL_FRAME_COLOR] = new CCursorToolButton(this, CAssetPath::SpriteUniverse(SPRITE_ICON_FRAME_COLOR), CURSORTOOL_FRAME_COLOR);
	
	m_pToolbar = new tu::gui::CHListLayout(m_pConfig, gui::CConfig::LAYOUTSTYLE_TOOLBAR);
	m_pToolbar->Add(new CFirstFrameButton(m_pAssetsEditor));
	m_pToolbar->Add(new CPrevFrameButton(m_pAssetsEditor));
	m_pToolbar->Add(new CPlayPauseButton(m_pAssetsEditor));
	m_pToolbar->Add(new CNextFrameButton(m_pAssetsEditor));
	m_pToolbar->Add(new CLastFrameButton(m_pAssetsEditor));
	m_pToolbar->AddSeparator();
	m_pToolbar->Add(pTimeScaleLabel);
	m_pToolbar->Add(new CTimeScaleSlider(this));
	m_pToolbar->AddSeparator();
	m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_FRAME_MOVE]);
	m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_FRAME_ADD]);
	m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_FRAME_DELETE]);
	m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_FRAME_COLOR]);
	
	m_pToolbar->Update();
	
	m_FrameMargin = 1.5f;
	m_FrameHeight = 30.0f - 2.0f*m_FrameMargin;
}
	
CTimeline::~CTimeline()
{
	if(m_pToolbar) delete m_pToolbar;
	if(m_pTimeSlider) delete m_pTimeSlider;
	if(m_pValueSlider) delete m_pValueSlider;
}

CAssetPath CTimeline::GetSkeletonPath()
{
	switch(m_pAssetsEditor->m_EditedAssetPath.GetType())
	{
		case CAssetPath::TYPE_SKELETON:
			return m_pAssetsEditor->m_EditedAssetPath;
		case CAssetPath::TYPE_SKELETONANIMATION:
		{
			return m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
				m_pAssetsEditor->m_EditedAssetPath,
				CAsset_SkeletonAnimation::SKELETONPATH,
				-1,
				CAssetPath::Null()
			);
		}
		case CAssetPath::TYPE_SKELETONSKIN:
		{
			return m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
				m_pAssetsEditor->m_EditedAssetPath,
				CAsset_SkeletonSkin::SKELETONPATH,
				-1,
				CAssetPath::Null()
			);
		}
		case CAssetPath::TYPE_MAPLAYERQUADS:
		{
			CAssetPath AnimationPath = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
				m_pAssetsEditor->m_EditedAssetSubPath,
				CAsset_MapLayerQuads::QUAD_ANIMATIONPATH,
				m_pAssetsEditor->m_EditedAssetSubPath,
				CAssetPath::Null()
			);
			if(AnimationPath.IsNull())
				return CAssetPath::Universe(CAssetPath::TYPE_SKELETON, SKELETON_PLACEHOLDER);
			else
			{
				return m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
					AnimationPath,
					CAsset_SkeletonAnimation::SKELETONPATH,
					-1,
					CAssetPath::Null()
				);
			}
		}
		default:
			return CAssetPath::Null();
	}
}

CAssetPath CTimeline::GetSkeletonAnimationPath()
{
	switch(m_pAssetsEditor->m_EditedAssetPath.GetType())
	{
		case CAssetPath::TYPE_SKELETONANIMATION:
			return m_pAssetsEditor->m_EditedAssetPath;
		case CAssetPath::TYPE_MAPLAYERQUADS:
		{
			return m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
				m_pAssetsEditor->m_EditedAssetSubPath,
				CAsset_MapLayerQuads::QUAD_ANIMATIONPATH,
				m_pAssetsEditor->m_EditedAssetSubPath,
				CAssetPath::Null()
			);
		}
		default:
			return CAssetPath::Null();
	}
}

void CTimeline::CreateAnimationIfNeeded()
{
	switch(m_pAssetsEditor->m_EditedAssetPath.GetType())
	{
		case CAssetPath::TYPE_MAPLAYERQUADS:
		{
			CAsset_MapLayerQuads* pLayer = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapLayerQuads>(m_pAssetsEditor->m_EditedAssetPath);
			if(!pLayer)
				return;
			
			CAsset_MapLayerQuads::CSubPath QuadPath = CAsset_MapLayerQuads::CSubPath(m_pAssetsEditor->m_EditedAssetSubPath);			
			CAssetPath ExistingAnimationPath = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
				m_pAssetsEditor->m_EditedAssetPath,
				CAsset_MapLayerQuads::QUAD_ANIMATIONPATH,
				QuadPath,
				CAssetPath::Null()
			);
			
			if(!ExistingAnimationPath.IsNull())
				return;
			
			//Create animation
			int Token = m_pAssetsEditor->AssetsManager()->GenerateToken();
			
			CAssetPath NewAnimationPath;
			char aBuf[128];
			
			CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->NewAsset<CAsset_SkeletonAnimation>(
				&NewAnimationPath,
				m_pAssetsEditor->m_EditedAssetPath.GetSource(),
				Token
			);
			
			str_format(aBuf, sizeof(aBuf), "quadAnimation%d", NewAnimationPath.GetId());
			pSkeletonAnimation->SetName(aBuf);
			pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, SKELETON_PLACEHOLDER);
			
			m_pAssetsEditor->AssetsManager()->SetAssetValue<CAssetPath>(
				m_pAssetsEditor->m_EditedAssetPath,
				QuadPath,
				CAsset_MapLayerQuads::QUAD_ANIMATIONPATH,
				NewAnimationPath,
				Token
			);
			
			m_pAssetsEditor->RefreshAssetsList();
			m_pAssetsEditor->RefreshAssetsEditor();
			
			return;
		}
	}
}

void CTimeline::TimeScaleCallback(float Pos)
{
	float MinScale = static_cast<float>(m_Rect.w) / (m_TimeMax);
	float MaxScale = static_cast<float>(m_Rect.w) * 10.0f;
	float ScaleWidth = MaxScale - MinScale;
	m_TimeScale = MinScale + Pos * Pos * ScaleWidth;
}

void CTimeline::ValueScrollCallback(float Pos)
{
	float VisibleValue = static_cast<float>(m_TimelineRect.h);
	float NeededScroll = max(m_ValueHeight - VisibleValue, 0.0f);
	m_ValueScroll = NeededScroll * Pos;
}

void CTimeline::TimeScrollCallback(float Pos)
{
	float VisibleTime = static_cast<float>(m_TimelineRect.w) / m_TimeScale;
	float NeededScroll = max(m_TimeMax - VisibleTime, 0.0f);
	m_TimeScroll = NeededScroll * Pos;
}

int CTimeline::TimeToTimeline(float Time)
{		
	return m_TimelineRect.x + m_TimeScale * (Time - m_TimeScroll);
}

float CTimeline::TimelineToTime(int X)
{
	return m_TimeScroll + (X - m_TimelineRect.x)/m_TimeScale;
}

void CTimeline::Update()
{
	m_ListRect.x = m_Rect.x + 2*m_pConfig->m_LayoutMargin + m_pValueSlider->m_Rect.w;
	m_ListRect.y = m_Rect.y + m_pConfig->m_LayoutMargin;
	m_ListRect.w = 150;
	m_ListRect.h = m_Rect.h - 4*m_pConfig->m_LayoutMargin - m_ToolbarHeight - m_pTimeSlider->m_Rect.h;
	
	m_TimelineRect.x = m_ListRect.x + m_ListRect.w + 1;
	m_TimelineRect.y = m_ListRect.y;
	m_TimelineRect.w = m_Rect.w - m_TimelineRect.x + m_Rect.x - 1;
	m_TimelineRect.h = m_ListRect.h;
	
	m_TimelineTop = m_TimelineRect.y;
	m_TimelineBottom = m_TimelineTop + m_TimelineRect.h;
	
	m_TimeScale = static_cast<float>(m_Rect.w) / (m_TimeMax/2.0f);
	
	m_pToolbar->SetRect(tu::gui::CRect(
		m_Rect.x,
		m_Rect.y + m_Rect.h - m_ToolbarHeight,
		m_Rect.w,
		m_ToolbarHeight
	));
	
	m_pTimeSlider->SetRect(tu::gui::CRect(
		m_TimelineRect.x,
		m_TimelineRect.y + m_TimelineRect.h + m_pConfig->m_LayoutMargin,
		m_TimelineRect.w,
		m_pTimeSlider->m_Rect.h
	));
	m_pTimeSlider->Update();
	
	m_pValueSlider->SetRect(tu::gui::CRect(
		m_ListRect.x - m_pValueSlider->m_Rect.w - m_pConfig->m_LayoutMargin,
		m_ListRect.y,
		m_pValueSlider->m_Rect.w,
		m_ListRect.h
	));
	m_pValueSlider->Update();
	
	m_pToolbar->Update();
	
	tu::gui::CWidget::Update();
}

void CTimeline::Render()
{
	TUGraphics()->DrawGuiRect(&m_Rect, CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_LAYOUT_TOOLBAR));
	
	m_pTimeSlider->Render();
	m_pValueSlider->Render();
	m_pToolbar->Render();
	
	for(int i=0; i<NUM_CURSORTOOLS; i++)
	{
		if(m_CursorTool == i)
			m_CursorToolButtons[i]->SetButtonStyle(gui::CConfig::BUTTONSTYLE_DEFAULT_HIGHLIGHT);
		else
			m_CursorToolButtons[i]->SetButtonStyle(gui::CConfig::BUTTONSTYLE_DEFAULT);
	}
	
	TUGraphics()->DrawGuiRect(&m_TimelineRect, CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_LAYOUT_VIEW));
	TUGraphics()->DrawGuiRect(&m_ListRect, CAssetPath::Universe(CAsset_GuiRectStyle::TypeId, GUIRECTSTYLE_LAYOUT_VIEW));
	
	Graphics()->ClipEnable(m_TimelineRect.x+1, m_TimelineRect.y+1, m_TimelineRect.w-2, m_TimelineRect.h-2);
	{
		float TimeIter = 0.0f;
		int RectIter = 0;
		
		while(TimeIter < m_TimeMax)
		{
			if(RectIter%2 == 0)
			{
				CUIRect timerect;
				timerect.x = TimeToTimeline(TimeIter);
				timerect.y = m_TimelineRect.y;
				timerect.w = TimeToTimeline(TimeIter + 1.0f/TU_SKELETONANIMATION_TIMESTEP) - timerect.x;
				timerect.h = m_TimelineRect.h;
			
				RenderTools()->DrawUIRect(&timerect, vec4(0.0f, 0.0f, 0.0f, s_LayoutOpacity), 0, s_LayoutCornerRadius);
			}
			RectIter++;
			TimeIter += 1.0f/TU_SKELETONANIMATION_TIMESTEP;
		}
	}
	
	{
		int TimePos = TimeToTimeline(m_pAssetsEditor->GetTime());		
			
		IGraphics::CLineItem Line(TimePos, m_TimelineTop, TimePos, m_TimelineBottom);
		Graphics()->TextureClear();
		Graphics()->LinesBegin();
		Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		Graphics()->LinesDraw(&Line, 1);
		Graphics()->LinesEnd();
	}
	Graphics()->ClipDisable();
	
	CAssetPath SkeletonPath = GetSkeletonPath();
	CAssetPath SkeletonAnimationPath = GetSkeletonAnimationPath();
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(SkeletonPath);
	if(!pSkeleton)
		return;

	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(SkeletonAnimationPath);
	
	m_ValueHeight = m_FrameMargin;
	float FrameY = m_TimelineTop-m_ValueScroll+m_FrameMargin;
	
	//Bones
	//Pass 1: Local bones
	//Pass 2: Parent bones
	for(int s=0; s<2; s++)
	{
		CAsset_Skeleton* pCurrentSkeleton;
		CAssetPath CurrentSkeletonPath;
		
		if(s==0)
		{
			pCurrentSkeleton = pSkeleton;
			CurrentSkeletonPath = SkeletonPath;
		}
		else
		{
			CAsset_Skeleton* pParentSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
			if(!pParentSkeleton)
				continue;
			pCurrentSkeleton = pParentSkeleton;
			CurrentSkeletonPath = pSkeleton->m_ParentPath;
		}
		
		for(int i=0; i<pCurrentSkeleton->m_Bones.size(); i++)
		{
			//Draw List
			Graphics()->ClipEnable(m_ListRect.x, m_ListRect.y, m_ListRect.w, m_ListRect.h);
			
			char* pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
				CurrentSkeletonPath,
				CAsset_Skeleton::BONE_NAME,
				CAsset_Skeleton::CSubPath::LocalBone(i),
				0);
			
			if(pText)
			{
				int TextWidth = TextRender()->TextWidth(0, m_pConfig->m_TextSize[gui::TEXTSTYLE_NORMAL], pText, -1);
				
				int PosX = m_ListRect.x + 2*m_pConfig->m_LabelMargin + m_pConfig->m_IconSize;					
				int CenterY = FrameY + m_FrameHeight/2;
				int PosY = CenterY - m_pConfig->m_TextSize[gui::TEXTSTYLE_NORMAL]*0.7;
				
				CTextCursor Cursor;
				TextRender()->SetCursor(&Cursor, PosX, PosY, m_pConfig->m_TextSize[gui::TEXTSTYLE_NORMAL], TEXTFLAG_RENDER);
				TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
				TextRender()->TextEx(&Cursor, pText, -1);
				
				PosX -= m_pConfig->m_IconSize/2 + m_pConfig->m_LabelMargin; //Icon size and space
				
				TUGraphics()->DrawSprite(
					CAssetPath::Universe(CAssetPath::TYPE_SPRITE, SPRITE_ICON_BONE),
					vec2(PosX, CenterY),
					1.0f, 0.0f, 0x0, 1.0f
				);
			}
			
			Graphics()->ClipDisable();
			
			//Draw Frames
			if(pSkeletonAnimation)
			{
				Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);
				
				CAsset_Skeleton::CSubPath BonePath;
				if(s == 0) BonePath = CAsset_Skeleton::CSubPath::LocalBone(i);
				else BonePath = CAsset_Skeleton::CSubPath::ParentBone(i);
				
				CAsset_SkeletonAnimation::CBoneAnimation* pAnimation = 0;
				
				int AnimId = -1;
				for(int j=0; j<pSkeletonAnimation->m_BoneAnimations.size(); j++)
				{
					if(pSkeletonAnimation->m_BoneAnimations[j].m_BonePath == BonePath)
					{
						AnimId = j;
						pAnimation = &pSkeletonAnimation->m_BoneAnimations[j];
						break;
					}
				}
				
				if(pAnimation)
				{
					for(int j=0; j<pAnimation->m_KeyFrames.size(); j++)
					{
						CAsset_SkeletonAnimation::CSubPath SubPath = CAsset_SkeletonAnimation::CSubPath::BoneKeyFrame(AnimId, j);
						
						float Time = pAnimation->m_KeyFrames[j].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
						float TimePos = TimeToTimeline(Time);

						TUGraphics()->DrawSprite(
							CAssetPath::SpriteUniverse(SPRITE_GIZMOFRAME),
							vec2(TimePos, FrameY+m_FrameHeight/2.0f),
							1.0f, 0.0f, 0x0, 1.0f
						);
					}
				}
				
				Graphics()->TextureClear();
				Graphics()->LinesBegin();
				Graphics()->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
				IGraphics::CLineItem Separator(m_TimelineRect.x, FrameY-1, m_TimelineRect.x+m_TimelineRect.w, FrameY-1);
				Graphics()->LinesDraw(&Separator, 1);
				Graphics()->LinesEnd();
				
				Graphics()->ClipDisable();
			}
			
			FrameY += m_FrameHeight + 2.0f*m_FrameMargin;
			m_ValueHeight += m_FrameHeight + 2.0f*m_FrameMargin;
		}
	}

	//Layers
	//Pass 1: Local layers
	//Pass 2: Parent layers
	for(int s=0; s<2; s++)
	{
		CAsset_Skeleton* pCurrentSkeleton;
		CAssetPath CurrentSkeletonPath;
		
		if(s==0)
		{
			pCurrentSkeleton = pSkeleton;
			CurrentSkeletonPath = SkeletonPath;
		}
		else
		{
			CAsset_Skeleton* pParentSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
			if(!pParentSkeleton)
				continue;
			pCurrentSkeleton = pParentSkeleton;
			CurrentSkeletonPath = pSkeleton->m_ParentPath;
		}
		
		for(int i=0; i<pCurrentSkeleton->m_Layers.size(); i++)
		{
			//Draw List
			Graphics()->ClipEnable(m_ListRect.x, m_ListRect.y, m_ListRect.w, m_ListRect.h);
			
			char* pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
				CurrentSkeletonPath,
				CAsset_Skeleton::LAYER_NAME,
				CAsset_Skeleton::CSubPath::LocalLayer(i),
				0);
			
			if(pText)
			{
				int TextWidth = TextRender()->TextWidth(0, m_pConfig->m_TextSize[gui::TEXTSTYLE_NORMAL], pText, -1);
				
				int PosX = m_ListRect.x + 2*m_pConfig->m_LabelMargin + m_pConfig->m_IconSize;					
				int CenterY = FrameY + m_FrameHeight/2;
				int PosY = CenterY - m_pConfig->m_TextSize[gui::TEXTSTYLE_NORMAL]*0.7;
				
				CTextCursor Cursor;
				TextRender()->SetCursor(&Cursor, PosX, PosY, m_pConfig->m_TextSize[gui::TEXTSTYLE_NORMAL], TEXTFLAG_RENDER);
				TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
				TextRender()->TextEx(&Cursor, pText, -1);
				
				PosX -= m_pConfig->m_IconSize/2 + m_pConfig->m_LabelMargin; //Icon size and space
				
				TUGraphics()->DrawSprite(
					CAssetPath::SpriteUniverse(SPRITE_ICON_LAYERS),
					vec2(PosX, CenterY),
					1.0f, 0.0f, 0x0, 1.0f
				);
			}
			
			Graphics()->ClipDisable();
			
			//Draw Frames
			if(pSkeletonAnimation)
			{
				Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);

				CAsset_Skeleton::CSubPath LayerPath;
				if(s == 0) LayerPath = CAsset_Skeleton::CSubPath::LocalLayer(i);
				else LayerPath = CAsset_Skeleton::CSubPath::ParentLayer(i);
				
				CAsset_SkeletonAnimation::CLayerAnimation* pAnimation = 0;
				
				int AnimId = -1;
				for(int j=0; j<pSkeletonAnimation->m_LayerAnimations.size(); j++)
				{
					if(pSkeletonAnimation->m_LayerAnimations[j].m_LayerPath == LayerPath)
					{
						AnimId = j;
						pAnimation = &pSkeletonAnimation->m_LayerAnimations[j];
						break;
					}
				}
				
				if(pAnimation)
				{
					for(int j=0; j<pAnimation->m_KeyFrames.size(); j++)
					{
						CAsset_SkeletonAnimation::CSubPath SubPath = CAsset_SkeletonAnimation::CSubPath::LayerKeyFrame(AnimId, j);
						
						float Time = pAnimation->m_KeyFrames[j].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
						float TimePos = TimeToTimeline(Time);
						
						if(pAnimation->m_KeyFrames[j].m_State == CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE)
						{
							vec4 Color = pAnimation->m_KeyFrames[j].m_Color;
							TUGraphics()->DrawSprite(
								CAssetPath::SpriteUniverse(SPRITE_GIZMOFRAME_COLORED_BG),
								vec2(TimePos, FrameY+m_FrameHeight/2.0f),
								1.0f, 0.0f, 0x0, Color
							);
							Color.a = 1.0;
							TUGraphics()->DrawSprite(
								CAssetPath::SpriteUniverse(SPRITE_GIZMOFRAME_COLORED_FG),
								vec2(TimePos, FrameY+m_FrameHeight/2.0f),
								1.0f, 0.0f, 0x0, Color
							);
						}
						else
						{
							TUGraphics()->DrawSprite(
								CAssetPath::SpriteUniverse(SPRITE_GIZMOFRAME_HIDDEN),
								vec2(TimePos, FrameY+m_FrameHeight/2.0f),
								1.0f, 0.0f, 0x0, 1.0f
							);
						}
					}
				}
				
				Graphics()->TextureClear();
				Graphics()->LinesBegin();
				Graphics()->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
				IGraphics::CLineItem Separator(m_TimelineRect.x, FrameY-1, m_TimelineRect.x+m_TimelineRect.w, FrameY-1);
				Graphics()->LinesDraw(&Separator, 1);
				Graphics()->LinesEnd();
			
				Graphics()->ClipDisable();
			}
				
			FrameY += m_FrameHeight + 2.0f*m_FrameMargin;
			m_ValueHeight += m_FrameHeight + 2.0f*m_FrameMargin;
		}
	}
	
	if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_ADD)
	{
		CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(m_CursorX, m_CursorY);
		if(KeyFramePath.IsNull())
		{
			float Time = TimelineToTime(m_CursorX);
			int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
				
			bool DrawGizmo = false;
			CAsset_Skeleton::CSubPath SubPath = AnimationPicking(m_CursorX, m_CursorY);
			if(!SubPath.IsNull())
				DrawGizmo = true;
			
			if(DrawGizmo)
			{
				int ListId = (m_CursorY - m_TimelineTop + m_ValueScroll)/(m_FrameHeight + 2*m_FrameMargin);
				float TimePos = TimeToTimeline(Frame/static_cast<float>(TU_SKELETONANIMATION_TIMESTEP));
				float FrameY = m_TimelineTop-m_ValueScroll+m_FrameMargin;
				FrameY += (m_FrameHeight + 2.0f*m_FrameMargin)*ListId;
				
				Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);
				
				TUGraphics()->DrawSprite(
					CAssetPath::SpriteUniverse(SPRITE_GIZMOFRAME_ADD),
					vec2(TimePos, FrameY+m_FrameHeight/2.0f),
					1.0f, 0.0f, 0x0, 1.0f
				);
				
				Graphics()->ClipDisable();
			}
		}
	}
	else if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_DELETE)
	{
		CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(m_CursorX, m_CursorY);
		if(!KeyFramePath.IsNull())
		{
			float Time = TimelineToTime(m_CursorX);
			int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
			
			int ListId = (m_CursorY - m_TimelineTop + m_ValueScroll)/(m_FrameHeight + 2*m_FrameMargin);
			float TimePos = TimeToTimeline(Frame/static_cast<float>(TU_SKELETONANIMATION_TIMESTEP));
			float FrameY = m_TimelineTop-m_ValueScroll+m_FrameMargin;
			FrameY += (m_FrameHeight + 2.0f*m_FrameMargin)*ListId;
			
			Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);
			
			TUGraphics()->DrawSprite(
				CAssetPath::SpriteUniverse(SPRITE_GIZMOFRAME_DELETE),
				vec2(TimePos, FrameY+m_FrameHeight/2.0f),
				1.0f, 0.0f, 0x0, 1.0f
			);
			
			Graphics()->ClipDisable();
		}
	}
	else if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_MOVE || m_CursorTool == CTimeline::CURSORTOOL_FRAME_COLOR)
	{
		CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(m_CursorX, m_CursorY);
		if(!KeyFramePath.IsNull())
		{
			if(
				m_CursorTool == CTimeline::CURSORTOOL_FRAME_MOVE ||
				(m_CursorTool == CTimeline::CURSORTOOL_FRAME_COLOR && KeyFramePath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			)
			{
				float Time = TimelineToTime(m_CursorX);
				int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
				
				int ListId = (m_CursorY - m_TimelineTop + m_ValueScroll)/(m_FrameHeight + 2*m_FrameMargin);
				float TimePos = TimeToTimeline(Frame/static_cast<float>(TU_SKELETONANIMATION_TIMESTEP));
				float FrameY = m_TimelineTop-m_ValueScroll+m_FrameMargin;
				FrameY += (m_FrameHeight + 2.0f*m_FrameMargin)*ListId;
				
				Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);
				
				if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_COLOR)
				{
					TUGraphics()->DrawSprite(
						CAssetPath::SpriteUniverse(SPRITE_GIZMOFRAME_COLOR),
						vec2(TimePos, FrameY+m_FrameHeight/2.0f),
						1.0f, 0.0f, 0x0, 1.0f
					);
				}
				else
				{
					TUGraphics()->DrawSprite(
						CAssetPath::SpriteUniverse(SPRITE_GIZMOFRAME_SELECT),
						vec2(TimePos, FrameY+m_FrameHeight/2.0f),
						1.0f, 0.0f, 0x0, 1.0f
					);
				}
				
				Graphics()->ClipDisable();
			}
		}
	}
}

CAsset_SkeletonAnimation::CSubPath CTimeline::KeyFramePicking(int X, int Y)
{
	if(!m_TimelineRect.IsInside(X, Y))
		return CAsset_SkeletonAnimation::CSubPath::Null();
		
	float Time = TimelineToTime(X);
	int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
	
	int ListId = (Y - m_TimelineTop + m_ValueScroll)/(m_FrameHeight + 2*m_FrameMargin);
	if(ListId < 0)
		return CAsset_SkeletonAnimation::CSubPath::Null();
	
	CAssetPath SkeletonAnimationPath = GetSkeletonAnimationPath();
	CAssetPath SkeletonPath = GetSkeletonPath();
	
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(SkeletonAnimationPath);
	if(!pSkeletonAnimation)
		return CAsset_SkeletonAnimation::CSubPath::Null();
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(SkeletonPath);
	if(!pSkeleton)
		return CAsset_SkeletonAnimation::CSubPath::Null();
	
	//Bones
	if(ListId < pSkeleton->m_Bones.size())
		return pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CSubPath::LocalBone(ListId), Frame);
	else
		ListId -= pSkeleton->m_Bones.size();
	
	//Parent bones
	CAsset_Skeleton* pSkeletonParent = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Bones.size())
			return pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CSubPath::ParentBone(ListId), Frame);
		else
			ListId -= pSkeletonParent->m_Bones.size();
	}
	
	//Layers
	if(ListId < pSkeleton->m_Layers.size())
		return pSkeletonAnimation->GetLayerKeyFramePath(CAsset_Skeleton::CSubPath::LocalLayer(ListId), Frame);
	else
		ListId -= pSkeleton->m_Layers.size();
	
	//Parent layers
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Layers.size())
			return pSkeletonAnimation->GetLayerKeyFramePath(CAsset_Skeleton::CSubPath::ParentLayer(ListId), Frame);
		else
			ListId -= pSkeletonParent->m_Layers.size();
	}
	
	return CAsset_SkeletonAnimation::CSubPath::Null();
}

CAsset_Skeleton::CSubPath CTimeline::AnimationPicking(int X, int Y)
{
	if(!m_TimelineRect.IsInside(X, Y))
		return CAsset_Skeleton::CSubPath::Null();
		
	float Time = TimelineToTime(X);
	int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
	
	int ListId = (Y - m_TimelineTop + m_ValueScroll)/(m_FrameHeight + 2*m_FrameMargin);
	if(ListId < 0)
		return CAsset_Skeleton::CSubPath::Null();
	
	CAssetPath SkeletonPath = GetSkeletonPath();
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(SkeletonPath);
	if(!pSkeleton)
		return CAsset_Skeleton::CSubPath::Null();
	
	//Local bones
	if(ListId < pSkeleton->m_Bones.size())
		return CAsset_Skeleton::CSubPath::LocalBone(ListId);
	else
		ListId -= pSkeleton->m_Bones.size();
	
	//Parent bones
	CAsset_Skeleton* pSkeletonParent = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Bones.size())
			return CAsset_Skeleton::CSubPath::ParentBone(ListId);
		else
			ListId -= pSkeletonParent->m_Bones.size();
	}
	
	//Local layers
	if(ListId < pSkeleton->m_Layers.size())
		return CAsset_Skeleton::CSubPath::LocalLayer(ListId);
	else
		ListId -= pSkeleton->m_Layers.size();
	
	//Parent layers
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Layers.size())
			return CAsset_Skeleton::CSubPath::ParentLayer(ListId);
		else
			ListId -= pSkeletonParent->m_Layers.size();
	}
	
	return CAsset_Skeleton::CSubPath::Null();
}
	
void CTimeline::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1 && m_TimelineRect.IsInside(X, Y))
	{
		if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_ADD)
		{			
			CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(X, Y);
			if(!KeyFramePath.IsNull())
				return;
					
			CAsset_Skeleton::CSubPath SubPath = AnimationPicking(X, Y);
			if(!SubPath.IsNull())
			{
				CreateAnimationIfNeeded();
				
				CAssetPath SkeletonAnimationPath = GetSkeletonAnimationPath();
				CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(SkeletonAnimationPath);
				if(!pSkeletonAnimation)
					return;
				
				float Time = TimelineToTime(X);
				int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
				
				if(SubPath.GetType() == CAsset_Skeleton::CSubPath::TYPE_BONE)
					pSkeletonAnimation->AddBoneKeyFrame(SubPath, Frame);
				else if(SubPath.GetType() == CAsset_Skeleton::CSubPath::TYPE_LAYER)
					pSkeletonAnimation->AddLayerKeyFrame(SubPath, Frame);
				
				m_pAssetsEditor->RefreshAssetsEditor();
			}
		}
		else if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_DELETE)
		{
			CAssetPath SkeletonAnimationPath = GetSkeletonAnimationPath();
			
			CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(X, Y);
			if(!KeyFramePath.IsNull())
			{
				m_pAssetsEditor->AssetsManager()->DeleteSubItem(SkeletonAnimationPath, KeyFramePath);
				m_pAssetsEditor->RefreshAssetsEditor();
			}
		}
		else if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_COLOR)
		{
			CAssetPath SkeletonAnimationPath = GetSkeletonAnimationPath();
			
			CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(X, Y);
			if(!KeyFramePath.IsNull() && KeyFramePath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			{
				m_pAssetsEditor->EditAssetSubItem(SkeletonAnimationPath, m_DragedElement, CEditor::TAB_SKELETONANIMATION_KEYFRAMES);
				m_pAssetsEditor->DisplayPopup(new CPopup_ColorEdit(
					m_pAssetsEditor, tu::gui::CRect(X-15, Y-15, 30, 30), tu::gui::CPopup::ALIGNMENT_LEFT,
					SkeletonAnimationPath, CAsset_SkeletonAnimation::LAYERKEYFRAME_COLOR, KeyFramePath
				));
			}
			else
			{
				float Time = TimelineToTime(X);
				m_pAssetsEditor->SetTime(Time);
				m_Drag = 1;
				m_DragedElement = CAsset_SkeletonAnimation::CSubPath::Null();
			}
		}
		else if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_MOVE)
		{
			CAssetPath SkeletonAnimationPath = GetSkeletonAnimationPath();
			
			CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(X, Y);
			if(!KeyFramePath.IsNull())
			{
				if(KeyFramePath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
				{
					m_Drag = 2;
					m_DragedElement = KeyFramePath;
					m_pAssetsEditor->EditAssetSubItem(SkeletonAnimationPath, m_DragedElement, CEditor::TAB_SKELETONANIMATION_KEYFRAMES);
				}
				else if(KeyFramePath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
				{
					m_Drag = 3;
					m_DragedElement = KeyFramePath;
					m_pAssetsEditor->EditAssetSubItem(SkeletonAnimationPath, m_DragedElement, CEditor::TAB_SKELETONANIMATION_KEYFRAMES);
				}
			}
			else
			{
				float Time = TimelineToTime(X);
				m_pAssetsEditor->SetTime(Time);
				m_Drag = 1;
				m_DragedElement = CAsset_SkeletonAnimation::CSubPath::Null();
			}
		}
		else
		{
			float Time = TimelineToTime(X);
			m_pAssetsEditor->SetTime(Time);
			m_Drag = 1;
			m_DragedElement = CAsset_SkeletonAnimation::CSubPath::Null();
		}
	}
	else
	{
		m_pTimeSlider->OnButtonClick(X, Y, Button, Count);
		m_pValueSlider->OnButtonClick(X, Y, Button, Count);
		m_pToolbar->OnButtonClick(X, Y, Button, Count);
	}
}

void CTimeline::OnButtonRelease(int Button)
{
	if(Button == KEY_MOUSE_1 && m_Drag > 0)
	{
		if(m_DragedElement.IsNull())
		{
			float Time = m_pAssetsEditor->GetTime();
			int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
			m_pAssetsEditor->SetTime(Frame / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP));
		}
		
		m_Drag = 0;
		m_DragedElement = CAsset_SkeletonAnimation::CSubPath::Null();
	}
	
	m_pTimeSlider->OnButtonRelease(Button);
	m_pValueSlider->OnButtonRelease(Button);
	m_pToolbar->OnButtonRelease(Button);
}

void CTimeline::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	m_CursorX = X;
	m_CursorY = Y;
	
	switch(m_Drag)
	{
		case 1:
		{
			float Time = TimelineToTime(X);
			m_pAssetsEditor->SetTime(Time);
			break;
		}
		case 2:
			if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_MOVE)
			{
				float Time = TimelineToTime(X);
				int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
				
				CAssetPath SkeletonAnimationPath = GetSkeletonAnimationPath();
				
				if(m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
					SkeletonAnimationPath,
					m_DragedElement,
					CAsset_SkeletonAnimation::BONEKEYFRAME_TIME,
					Frame
				))
				{
					//Get the new Id of the frame
					CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(SkeletonAnimationPath);
					if(pSkeletonAnimation)
					{
						m_DragedElement = pSkeletonAnimation->GetBoneKeyFramePath(CAsset_SkeletonAnimation::CSubPath::BoneAnimation(m_DragedElement.GetId()), Frame);
						if(m_DragedElement.IsNull())
							m_Drag = 0;
						else
							m_pAssetsEditor->RefreshAssetsEditor();
					}
					else
					{
						m_Drag = 0;
						m_DragedElement = CAsset_SkeletonAnimation::CSubPath::Null();
					}
				}
			}
			break;	
		case 3:
			if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_MOVE)
			{
				float Time = TimelineToTime(X);
				int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
				
				CAssetPath SkeletonAnimationPath = GetSkeletonAnimationPath();
				
				if(m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
					SkeletonAnimationPath,
					m_DragedElement,
					CAsset_SkeletonAnimation::LAYERKEYFRAME_TIME,
					Frame
				))
				{
					//Get the new Id of the frame
					CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(SkeletonAnimationPath);
					if(pSkeletonAnimation)
					{
						m_DragedElement = pSkeletonAnimation->GetLayerKeyFramePath(CAsset_SkeletonAnimation::CSubPath::LayerAnimation(m_DragedElement.GetId()), Frame);
						if(m_DragedElement.IsNull())
							m_Drag = 0;
						else
							m_pAssetsEditor->RefreshAssetsEditor();
					}
					else
					{
						m_Drag = 0;
						m_DragedElement = CAsset_SkeletonAnimation::CSubPath::Null();
					}
				}
			}
			break;			
	}
	
	m_pTimeSlider->OnMouseOver(X, Y, RelX, RelY, KeyState);
	m_pValueSlider->OnMouseOver(X, Y, RelX, RelY, KeyState);
	m_pToolbar->OnMouseOver(X, Y, RelX, RelY, KeyState);
}

}

}
