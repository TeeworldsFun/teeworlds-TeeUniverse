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
	
	m_CursorToolButtons[CURSORTOOL_FRAME_MOVE] = new CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_FRAME_MOVE, CURSORTOOL_FRAME_MOVE);
	m_CursorToolButtons[CURSORTOOL_FRAME_ADD] = new CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_FRAME_ADD, CURSORTOOL_FRAME_ADD);
	m_CursorToolButtons[CURSORTOOL_FRAME_DELETE] = new CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_FRAME_DELETE, CURSORTOOL_FRAME_DELETE);
	m_CursorToolButtons[CURSORTOOL_FRAME_COLOR] = new CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_FRAME_COLOR, CURSORTOOL_FRAME_COLOR);
	
	m_pToolbar = new tu::gui::CHListLayout(m_pConfig);
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
	
	m_TimelineRect.x = m_ListRect.x + m_ListRect.w + m_pConfig->m_LayoutMargin;
	m_TimelineRect.y = m_ListRect.y;
	m_TimelineRect.w = m_Rect.w - m_TimelineRect.x + m_Rect.x - m_pConfig->m_LayoutMargin;
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
	
	{
		CUIRect rect;
		rect.x = m_TimelineRect.x;
		rect.y = m_TimelineRect.y;
		rect.w = m_TimelineRect.w;
		rect.h = m_TimelineRect.h;
		RenderTools()->DrawRoundRect(&rect, vec4(0.0f, 0.0f, 0.0f, s_LayoutOpacity), s_LayoutCornerRadius);
	}
	{
		CUIRect rect;
		rect.x = m_ListRect.x;
		rect.y = m_ListRect.y;
		rect.w = m_ListRect.w;
		rect.h = m_ListRect.h;
		RenderTools()->DrawRoundRect(&rect, vec4(0.0f, 0.0f, 0.0f, s_LayoutOpacity), s_LayoutCornerRadius);
	}
	
	Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);
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
	
	if(m_pAssetsEditor->m_EditedAssetPath.GetType() != CAssetPath::TYPE_SKELETONANIMATION)
		return;
	
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonAnimation)
		return;
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return;

	m_ValueHeight = m_FrameMargin;
	float FrameY = m_TimelineTop-m_ValueScroll+m_FrameMargin;
	
	//Bones
	for(int s=0; s<2; s++)
	{
		CAsset_Skeleton* pCurrentSkeleton;
		CAssetPath CurrentSkeletonPath;
		
		if(s==0)
		{
			pCurrentSkeleton = pSkeleton;
			CurrentSkeletonPath = pSkeletonAnimation->m_SkeletonPath;
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
			Graphics()->ClipEnable(m_ListRect.x, m_ListRect.y, m_ListRect.w, m_ListRect.h);
			
			char* pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
				CurrentSkeletonPath,
				CAsset_Skeleton::BONE_NAME,
				CAsset_Skeleton::CSubPath::Bone(i).ConvertToInteger(),
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
				
				PosX -= m_pConfig->m_IconSize + m_pConfig->m_LabelMargin; //Icon size and space
				
				int SubX = CAssetsEditor::ICON_BONE%16;
				int SubY = CAssetsEditor::ICON_BONE/16;
				
				Graphics()->TextureSet(m_pConfig->m_Texture);
				Graphics()->QuadsBegin();
				Graphics()->QuadsSetSubset(SubX/16.0f, SubY/16.0f, (SubX+1)/16.0f, (SubY+1)/16.0f);
				IGraphics::CQuadItem QuadItem(PosX, CenterY - m_pConfig->m_IconSize/2, m_pConfig->m_IconSize, m_pConfig->m_IconSize);
				Graphics()->QuadsDrawTL(&QuadItem, 1);
				Graphics()->QuadsEnd();
			}
			
			Graphics()->ClipDisable();
			
			Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);
			Graphics()->TextureClear();
			Graphics()->QuadsBegin();
	
			CAsset_Skeleton::CBonePath BonePath;
			if(s == 0) BonePath = CAsset_Skeleton::CBonePath::Local(i);
			else BonePath = CAsset_Skeleton::CBonePath::Parent(i);
			
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
					
					if(SubPath == m_DragedElement)
						Graphics()->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
					else
						Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
					
					float Time = pAnimation->m_KeyFrames[j].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
					float TimePos = TimeToTimeline(Time);

					IGraphics::CFreeformItem Freeform1(
						TimePos, FrameY,
						TimePos, FrameY+m_FrameHeight,
						TimePos+4.0f, FrameY+4.0f,
						TimePos+4.0f, FrameY+m_FrameHeight-4.0f);
					IGraphics::CFreeformItem Freeform2(
						TimePos, FrameY,
						TimePos, FrameY+m_FrameHeight,
						TimePos-4.0f, FrameY+4.0f,
						TimePos-4.0f, FrameY+m_FrameHeight-4.0f);
					Graphics()->QuadsDrawFreeform(&Freeform1, 1);
					Graphics()->QuadsDrawFreeform(&Freeform2, 1);
				}
			}
			
			Graphics()->QuadsEnd();
			
			Graphics()->LinesBegin();
			Graphics()->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
			IGraphics::CLineItem Separator(m_TimelineRect.x, FrameY-1, m_TimelineRect.x+m_TimelineRect.w, FrameY-1);
			Graphics()->LinesDraw(&Separator, 1);
			Graphics()->LinesEnd();
			
			Graphics()->ClipDisable();
				
			FrameY += m_FrameHeight + 2.0f*m_FrameMargin;
			m_ValueHeight += m_FrameHeight + 2.0f*m_FrameMargin;
		}
	}

	//Layers
	for(int s=0; s<2; s++)
	{
		CAsset_Skeleton* pCurrentSkeleton;
		CAssetPath CurrentSkeletonPath;
		
		if(s==0)
		{
			pCurrentSkeleton = pSkeleton;
			CurrentSkeletonPath = pSkeletonAnimation->m_SkeletonPath;
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
			Graphics()->ClipEnable(m_ListRect.x, m_ListRect.y, m_ListRect.w, m_ListRect.h);
			
			char* pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
				CurrentSkeletonPath,
				CAsset_Skeleton::LAYER_NAME,
				CAsset_Skeleton::CSubPath::Layer(i).ConvertToInteger(),
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
				
				PosX -= m_pConfig->m_IconSize + m_pConfig->m_LabelMargin; //Icon size and space
				
				int SubX = CAssetsEditor::ICON_LAYERS%16;
				int SubY = CAssetsEditor::ICON_LAYERS/16;
				
				Graphics()->TextureSet(m_pConfig->m_Texture);
				Graphics()->QuadsBegin();
				Graphics()->QuadsSetSubset(SubX/16.0f, SubY/16.0f, (SubX+1)/16.0f, (SubY+1)/16.0f);
				IGraphics::CQuadItem QuadItem(PosX, CenterY - m_pConfig->m_IconSize/2, m_pConfig->m_IconSize, m_pConfig->m_IconSize);
				Graphics()->QuadsDrawTL(&QuadItem, 1);
				Graphics()->QuadsEnd();
			}
			
			Graphics()->ClipDisable();
			
			Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);
			Graphics()->TextureClear();

			CAsset_Skeleton::CBonePath LayerPath;
			if(s == 0) LayerPath = CAsset_Skeleton::CBonePath::Local(i);
			else LayerPath = CAsset_Skeleton::CBonePath::Parent(i);
			
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
						Graphics()->QuadsBegin();
						
						if(SubPath == m_DragedElement)
							Graphics()->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
						else
						{
							vec4 LayerColor = pAnimation->m_KeyFrames[j].m_Color;
							Graphics()->SetColor(LayerColor.r, LayerColor.g, LayerColor.b, 1.0f);
						}

						IGraphics::CFreeformItem Freeform1(
							TimePos, FrameY,
							TimePos, FrameY+m_FrameHeight,
							TimePos+4.0f, FrameY+4.0f,
							TimePos+4.0f, FrameY+m_FrameHeight-4.0f);
						Graphics()->QuadsDrawFreeform(&Freeform1, 1);
						
						if(SubPath == m_DragedElement)
							Graphics()->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
						else
						{
							vec4 LayerColor = pAnimation->m_KeyFrames[j].m_Color;
							Graphics()->SetColor(LayerColor.r*LayerColor.a, LayerColor.g*LayerColor.a, LayerColor.b*LayerColor.a, LayerColor.a);
						}
						
						IGraphics::CFreeformItem Freeform2(
							TimePos, FrameY,
							TimePos, FrameY+m_FrameHeight,
							TimePos-4.0f, FrameY+4.0f,
							TimePos-4.0f, FrameY+m_FrameHeight-4.0f);
						Graphics()->QuadsDrawFreeform(&Freeform2, 1);
						
						Graphics()->QuadsEnd();
					}
					else
					{
						Graphics()->LinesBegin();
						if(SubPath == m_DragedElement)
							Graphics()->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
						else
						{
							vec4 LayerColor = pAnimation->m_KeyFrames[j].m_Color;
							Graphics()->SetColor(LayerColor.r, LayerColor.g, LayerColor.b, 1.0f);
						}
						
						IGraphics::CLineItem FrameBorder[6] = {
							IGraphics::CLineItem(TimePos, FrameY, TimePos-4.0f, FrameY+4.0f),
							IGraphics::CLineItem(TimePos-4.0f, FrameY+4.0f, TimePos-4.0f, FrameY+m_FrameHeight-4.0f),
							IGraphics::CLineItem(TimePos-4.0f, FrameY+m_FrameHeight-4.0f, TimePos, FrameY+m_FrameHeight),
							IGraphics::CLineItem(TimePos, FrameY+m_FrameHeight, TimePos+4.0f, FrameY+m_FrameHeight-4.0f),
							IGraphics::CLineItem(TimePos+4.0f, FrameY+m_FrameHeight-4.0f, TimePos+4.0f, FrameY+4.0f),
							IGraphics::CLineItem(TimePos+4.0f, FrameY+4.0f, TimePos, FrameY),
						};
						Graphics()->LinesDraw(FrameBorder, 6);
						
						Graphics()->LinesEnd();
					}
				}
			}
			
			Graphics()->ClipEnable(m_TimelineRect.x, m_TimelineRect.y, m_TimelineRect.w, m_TimelineRect.h);
			
			Graphics()->LinesBegin();
			Graphics()->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
			IGraphics::CLineItem Separator(m_TimelineRect.x, FrameY-1, m_TimelineRect.x+m_TimelineRect.w, FrameY-1);
			Graphics()->LinesDraw(&Separator, 1);
			Graphics()->LinesEnd();
		
			Graphics()->ClipDisable();
				
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
				
			bool DrawOutline = false;
			CAsset_Skeleton::CBonePath BonePath = BonePicking(m_CursorX, m_CursorY);
			if(!BonePath.IsNull())
			{
				DrawOutline = true;
			}
			else
			{
				CAsset_Skeleton::CBonePath LayerPath = LayerPicking(m_CursorX, m_CursorY);
				if(!LayerPath.IsNull())
				{
					DrawOutline = true;
				}
			}
			
			if(DrawOutline)
			{
				int ListId = (m_CursorY - m_TimelineTop + m_ValueScroll)/(m_FrameHeight + 2*m_FrameMargin);
				float TimePos = TimeToTimeline(Frame/static_cast<float>(TU_SKELETONANIMATION_TIMESTEP));
				float FrameY = m_TimelineTop-m_ValueScroll+m_FrameMargin;
				FrameY += (m_FrameHeight + 2.0f*m_FrameMargin)*ListId;
				Graphics()->LinesBegin();
				Graphics()->SetColor(0.0f, 1.0f, 0.0f, 1.0f);
				
				IGraphics::CLineItem FrameBorder[6] = {
					IGraphics::CLineItem(TimePos, FrameY, TimePos-4.0f, FrameY+4.0f),
					IGraphics::CLineItem(TimePos-4.0f, FrameY+4.0f, TimePos-4.0f, FrameY+m_FrameHeight-4.0f),
					IGraphics::CLineItem(TimePos-4.0f, FrameY+m_FrameHeight-4.0f, TimePos, FrameY+m_FrameHeight),
					IGraphics::CLineItem(TimePos, FrameY+m_FrameHeight, TimePos+4.0f, FrameY+m_FrameHeight-4.0f),
					IGraphics::CLineItem(TimePos+4.0f, FrameY+m_FrameHeight-4.0f, TimePos+4.0f, FrameY+4.0f),
					IGraphics::CLineItem(TimePos+4.0f, FrameY+4.0f, TimePos, FrameY),
				};
				Graphics()->LinesDraw(FrameBorder, 6);
				
				Graphics()->LinesEnd();
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
			Graphics()->LinesBegin();
			Graphics()->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			
			IGraphics::CLineItem FrameBorder[6] = {
				IGraphics::CLineItem(TimePos, FrameY, TimePos-4.0f, FrameY+4.0f),
				IGraphics::CLineItem(TimePos-4.0f, FrameY+4.0f, TimePos-4.0f, FrameY+m_FrameHeight-4.0f),
				IGraphics::CLineItem(TimePos-4.0f, FrameY+m_FrameHeight-4.0f, TimePos, FrameY+m_FrameHeight),
				IGraphics::CLineItem(TimePos, FrameY+m_FrameHeight, TimePos+4.0f, FrameY+m_FrameHeight-4.0f),
				IGraphics::CLineItem(TimePos+4.0f, FrameY+m_FrameHeight-4.0f, TimePos+4.0f, FrameY+4.0f),
				IGraphics::CLineItem(TimePos+4.0f, FrameY+4.0f, TimePos, FrameY),
			};
			Graphics()->LinesDraw(FrameBorder, 6);
			
			Graphics()->LinesEnd();
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
				Graphics()->LinesBegin();
				Graphics()->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
				
				IGraphics::CLineItem FrameBorder[6] = {
					IGraphics::CLineItem(TimePos, FrameY, TimePos-4.0f, FrameY+4.0f),
					IGraphics::CLineItem(TimePos-4.0f, FrameY+4.0f, TimePos-4.0f, FrameY+m_FrameHeight-4.0f),
					IGraphics::CLineItem(TimePos-4.0f, FrameY+m_FrameHeight-4.0f, TimePos, FrameY+m_FrameHeight),
					IGraphics::CLineItem(TimePos, FrameY+m_FrameHeight, TimePos+4.0f, FrameY+m_FrameHeight-4.0f),
					IGraphics::CLineItem(TimePos+4.0f, FrameY+m_FrameHeight-4.0f, TimePos+4.0f, FrameY+4.0f),
					IGraphics::CLineItem(TimePos+4.0f, FrameY+4.0f, TimePos, FrameY),
				};
				Graphics()->LinesDraw(FrameBorder, 6);
				
				Graphics()->LinesEnd();
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
	
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonAnimation)
		return CAsset_SkeletonAnimation::CSubPath::Null();
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return CAsset_SkeletonAnimation::CSubPath::Null();
	
	//Bones
	if(ListId < pSkeleton->m_Bones.size())
		return pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Local(ListId), Frame);
	else
		ListId -= pSkeleton->m_Bones.size();
	
	//Parent bones
	CAsset_Skeleton* pSkeletonParent = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Bones.size())
			return pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Parent(ListId), Frame);
		else
			ListId -= pSkeletonParent->m_Bones.size();
	}
	
	//Layers
	if(ListId < pSkeleton->m_Layers.size())
		return pSkeletonAnimation->GetLayerKeyFramePath(CAsset_Skeleton::CBonePath::Local(ListId), Frame);
	else
		ListId -= pSkeleton->m_Layers.size();
	
	//Parent layers
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Layers.size())
			return pSkeletonAnimation->GetLayerKeyFramePath(CAsset_Skeleton::CBonePath::Parent(ListId), Frame);
		else
			ListId -= pSkeletonParent->m_Layers.size();
	}
	
	return CAsset_SkeletonAnimation::CSubPath::Null();
}

CAsset_Skeleton::CBonePath CTimeline::BonePicking(int X, int Y)
{
	if(!m_TimelineRect.IsInside(X, Y))
		return CAsset_Skeleton::CBonePath::Null();
	
	int ListId = (Y - m_TimelineTop + m_ValueScroll)/(m_FrameHeight + 2*m_FrameMargin);
	if(ListId < 0)
		return CAsset_Skeleton::CBonePath::Null();
	
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonAnimation)
		return CAsset_Skeleton::CBonePath::Null();
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return CAsset_Skeleton::CBonePath::Null();
	
	//Bones
	if(ListId < pSkeleton->m_Bones.size())
		return CAsset_Skeleton::CBonePath::Local(ListId);
	else
		ListId -= pSkeleton->m_Bones.size();
	
	//Parent bones
	CAsset_Skeleton* pSkeletonParent = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Bones.size())
			return CAsset_Skeleton::CBonePath::Parent(ListId);
	}
	
	return CAsset_Skeleton::CBonePath::Null();
}

CAsset_Skeleton::CBonePath CTimeline::LayerPicking(int X, int Y)
{
	if(!m_TimelineRect.IsInside(X, Y))
		return CAsset_Skeleton::CBonePath::Null();
		
	float Time = TimelineToTime(X);
	int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
	
	int ListId = (Y - m_TimelineTop + m_ValueScroll)/(m_FrameHeight + 2*m_FrameMargin);
	if(ListId < 0)
		return CAsset_Skeleton::CBonePath::Null();
	
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonAnimation)
		return CAsset_Skeleton::CBonePath::Null();
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return CAsset_Skeleton::CBonePath::Null();
	
	//Bones
	if(ListId < pSkeleton->m_Bones.size())
		return CAsset_Skeleton::CBonePath::Null();
	else
		ListId -= pSkeleton->m_Bones.size();
	
	//Parent bones
	CAsset_Skeleton* pSkeletonParent = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Bones.size())
			return CAsset_Skeleton::CBonePath::Null();
		else
			ListId -= pSkeletonParent->m_Bones.size();
	}
	
	//Layers
	if(ListId < pSkeleton->m_Layers.size())
		return CAsset_Skeleton::CBonePath::Local(ListId);
	else
		ListId -= pSkeleton->m_Layers.size();
	
	//Parent bones
	if(pSkeletonParent)
	{
		if(ListId < pSkeletonParent->m_Layers.size())
			return CAsset_Skeleton::CBonePath::Parent(ListId);
		else
			ListId -= pSkeletonParent->m_Layers.size();
	}
	
	return CAsset_Skeleton::CBonePath::Null();
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
				
			CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
			if(!pSkeletonAnimation)
				return;
			
			CAsset_Skeleton::CBonePath BonePath = BonePicking(X, Y);
			if(!BonePath.IsNull())
			{
				float Time = TimelineToTime(X);
				int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
				
				pSkeletonAnimation->AddBoneKeyFrame(BonePath, Frame);
				m_pAssetsEditor->RefreshAssetsEditor();
			}
			else
			{
				CAsset_Skeleton::CBonePath LayerPath = LayerPicking(X, Y);
				if(!LayerPath.IsNull())
				{
					float Time = TimelineToTime(X);
					int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
					
					pSkeletonAnimation->AddLayerKeyFrame(LayerPath, Frame);
					m_pAssetsEditor->RefreshAssetsEditor();
				}
			}
		}
		else if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_DELETE)
		{
			CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(X, Y);
			if(!KeyFramePath.IsNull())
			{
				m_pAssetsEditor->AssetsManager()->DeleteSubItem(m_pAssetsEditor->m_EditedAssetPath, KeyFramePath.ConvertToInteger());
				m_pAssetsEditor->RefreshAssetsEditor();
			}
		}
		else if(m_CursorTool == CTimeline::CURSORTOOL_FRAME_COLOR)
		{
			CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(X, Y);
			if(!KeyFramePath.IsNull() && KeyFramePath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			{
				m_pAssetsEditor->EditAssetSubItem(m_pAssetsEditor->m_EditedAssetPath, m_DragedElement.ConvertToInteger(), CEditor::TAB_SKELETONANIMATION_KEYFRAMES);
				m_pAssetsEditor->DisplayPopup(new CPopup_ColorEdit(
					m_pAssetsEditor, tu::gui::CRect(X-15, Y-15, 30, 30), tu::gui::CPopup::ALIGNMENT_LEFT,
					m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::LAYERKEYFRAME_COLOR, KeyFramePath.ConvertToInteger()
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
			CAsset_SkeletonAnimation::CSubPath KeyFramePath = KeyFramePicking(X, Y);
			if(!KeyFramePath.IsNull())
			{
				if(KeyFramePath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
				{
					m_Drag = 2;
					m_DragedElement = KeyFramePath;
					m_pAssetsEditor->EditAssetSubItem(m_pAssetsEditor->m_EditedAssetPath, m_DragedElement.ConvertToInteger(), CEditor::TAB_SKELETONANIMATION_KEYFRAMES);
				}
				else if(KeyFramePath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
				{
					m_Drag = 3;
					m_DragedElement = KeyFramePath;
					m_pAssetsEditor->EditAssetSubItem(m_pAssetsEditor->m_EditedAssetPath, m_DragedElement.ConvertToInteger(), CEditor::TAB_SKELETONANIMATION_KEYFRAMES);
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
				
				if(m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
					m_pAssetsEditor->m_EditedAssetPath,
					CAsset_SkeletonAnimation::BONEKEYFRAME_TIME,
					m_DragedElement.ConvertToInteger(),
					Frame
				))
				{
					//Get the new Id of the frame
					CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
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
				
				if(m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
					m_pAssetsEditor->m_EditedAssetPath,
					CAsset_SkeletonAnimation::LAYERKEYFRAME_TIME,
					m_DragedElement.ConvertToInteger(),
					Frame
				))
				{
					//Get the new Id of the frame
					CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
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
