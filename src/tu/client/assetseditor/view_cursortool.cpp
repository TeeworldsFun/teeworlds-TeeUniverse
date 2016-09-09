#include <engine/client.h>
#include <tu/client/assetseditor/view.h>
#include <tu/client/skeletonrenderer.h>
#include <tu/shared/geometry.h>

#include "view_cursortool.h"
#include "editor.h"
	
const char* g_CursorToolHints[] = {
	"Bone Translation: move a bone",
	"Bone Horizontal Translation: move a bone along his local X axis",
	"Bone Vertical Translation: move an object along his local Y axis",
	"Bone Rotation: rotate a bone",
	"Bone Scale: scale a bone",
	"Bone Horizontal Scale: scale a bone along his local X axis",
	"Bone Vertical Scale: scale a bone along his local Y axis",
	"Bone Length: edit the length of a bone",
	"Bone Creator: create a new bone from an existing one",
	"Bone Eraser: delete a bone",
	"Sprite Creator: create sprite from an image",
	"Tile Stamp: create, copy and past tiles",
	"Vertex Editor: select and edit the position of quad verticies",
	"Quad Transform: select, translate, rotate and scale quads",
	"Quad Stamp: create, copy and past quads"
};

namespace tu
{

namespace assetseditor
{
	
CView_CursorTool::CButtonCore::CButtonCore(CView* pView, int Icon, int CursorToolID, bool DefaultCursorTool) :
	gui::CIconButton(pView->m_pConfig, Icon),
	m_pView(pView),
	m_CursorToolID(CursorToolID)
{
	if(DefaultCursorTool)
		m_pView->SetDefaultCursorToolButton(this);
	
	CView_CursorTool* pCursorTool = m_pView->GetCursorTool();
	if(pCursorTool && pCursorTool->GetCursorToolID() == m_CursorToolID)
		m_pView->CursorToolFound();
}

void CView_CursorTool::CButtonCore::MouseClickAction()
{
	CView_CursorTool* pCursorTool = m_pView->GetCursorTool();
	if(!pCursorTool || pCursorTool->GetCursorToolID() != m_CursorToolID)
	{
		pCursorTool = CreateCursorTool();
		m_pView->SetCursorTool(pCursorTool);
	}
}
		
void CView_CursorTool::CButtonCore::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_Rect.IsInside(X, Y))
		ShowHint(g_CursorToolHints[m_CursorToolID]);
	
	gui::CIconButton::OnMouseOver(X, Y, RelX, RelY, KeyState);
}

void CView_CursorTool::CButtonCore::Render()
{
	CView_CursorTool* pCursorTool = m_pView->GetCursorTool();
	if(pCursorTool && pCursorTool->GetCursorToolID() == m_CursorToolID)
		SetButtonStyle(gui::CConfig::BUTTONSTYLE_DEFAULT_HIGHLIGHT);
	else
		SetButtonStyle(gui::CConfig::BUTTONSTYLE_DEFAULT);
	
	gui::CIconButton::Render();
}

IGraphics* CView_CursorTool::Graphics() { return m_pView->Graphics(); }
CGraphics* CView_CursorTool::TUGraphics(int Type) { return m_pView->TUGraphics(Type); }
CAssetsManager* CView_CursorTool::AssetsManager() { return m_pView->AssetsManager(); }
CAssetsEditor* CView_CursorTool::AssetsEditor() { return m_pView->AssetsEditor(); }
CView* CView_CursorTool::View() { return m_pView; }

/* BONES **************************************************************/

CView_CursorTool_Bone_Picker::CView_CursorTool_Bone_Picker(CView* pView, int CursorToolID) :
	CView_CursorTool(pView, CursorToolID)
{
	
}
	
void CView_CursorTool_Bone_Picker::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1)
	{
		if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETON)
		{
			CSkeletonRenderer SkeletonRenderer(TUGraphics(), AssetsManager());
			SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
			SkeletonRenderer.Finalize();
			
			CAssetPath SkeletonPath;
			CAsset_Skeleton::CSubPath BonePath;
			if(SkeletonRenderer.BonePicking(View()->GetViewPos(), View()->GetViewZoom(), vec2(X, Y), SkeletonPath, BonePath) && SkeletonPath == AssetsEditor()->m_ViewedAssetPath)
			{
				View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
				m_SelectedAsset = AssetsEditor()->m_ViewedAssetPath;
				m_SelectedSqueleton = SkeletonPath;
				m_SelectedBone = BonePath;
				m_SelectedSubItem = BonePath;
				
				SetMemberInfo();
				
				m_CursorLastPos = vec2(X, Y);
				m_Pivot = vec2(X, Y);
				
				m_BackupValue0 = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, m_DefaultValue0);
				m_BackupValue1 = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, m_DefaultValue1);
			}
		}
		else if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETONANIMATION)
		{
			CAsset_SkeletonAnimation* pSkeletonAnimation = AssetsEditor()->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(AssetsEditor()->m_ViewedAssetPath);
			if(!pSkeletonAnimation)
				return;
			
			float Time = AssetsEditor()->GetTime();
			int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
			
			CSkeletonRenderer SkeletonRenderer(TUGraphics(), AssetsManager());
			SkeletonRenderer.AddSkeletonWithParents(pSkeletonAnimation->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
			SkeletonRenderer.ApplyAnimation(AssetsEditor()->m_ViewedAssetPath, Time);
			SkeletonRenderer.Finalize();
			
			CAssetPath SkeletonPath;
			CAsset_Skeleton::CSubPath BonePath;
			if(SkeletonRenderer.BonePicking(View()->GetViewPos(), View()->GetViewZoom(), vec2(X, Y), SkeletonPath, BonePath))
			{
				CAsset_SkeletonAnimation::CSubPath FramePath;
				
				if(SkeletonPath == pSkeletonAnimation->m_SkeletonPath)
					FramePath = pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CSubPath::LocalBone(BonePath.GetId()), Frame);
				
				if(FramePath.IsNull())
					return;
				
				View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
				m_SelectedAsset = AssetsEditor()->m_ViewedAssetPath;
				m_SelectedSqueleton = SkeletonPath;
				m_SelectedBone = BonePath;
				m_SelectedSubItem = FramePath;
				
				SetMemberInfo();
				
				m_CursorLastPos = vec2(X, Y);
				m_Pivot = vec2(X, Y);
				
				m_BackupValue0 = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, m_DefaultValue0);
				m_BackupValue1 = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, m_DefaultValue1);
			}
		}
	}
}

/* BONE TRANSFORM *****************************************************/

CView_CursorTool_Bone_Transform::CView_CursorTool_Bone_Transform(CView* pView, int CursorToolID, int Transform) :
	CView_CursorTool_Bone_Picker(pView, CursorToolID),
	m_Transform(Transform)
{
	
}

void CView_CursorTool_Bone_Transform::SetMemberInfo()
{
	if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETON)
	{
		switch(m_Transform)
		{
			case TRANSFORM_TRANSLATION_X:
				m_Member0 = CAsset_Skeleton::BONE_TRANSLATION_X;
				m_DefaultValue0 = 0.0f;
				break;
			case TRANSFORM_TRANSLATION_Y:
				m_Member1 = CAsset_Skeleton::BONE_TRANSLATION_Y;
				m_DefaultValue1 = 0.0f;
				break;
			case TRANSFORM_TRANSLATION_XY:
				m_Member0 = CAsset_Skeleton::BONE_TRANSLATION_X;
				m_Member1 = CAsset_Skeleton::BONE_TRANSLATION_Y;
				m_DefaultValue0 = 0.0f;
				m_DefaultValue1 = 0.0f;
				break;
			case TRANSFORM_SCALE_X:
				m_Member0 = CAsset_Skeleton::BONE_SCALE_X;
				m_DefaultValue0 = 1.0f;
				break;
			case TRANSFORM_SCALE_Y:
				m_Member1 = CAsset_Skeleton::BONE_SCALE_Y;
				m_DefaultValue1 = 1.0f;
				break;
			case TRANSFORM_SCALE_XY:
				m_Member0 = CAsset_Skeleton::BONE_SCALE_X;
				m_Member1 = CAsset_Skeleton::BONE_SCALE_Y;
				m_DefaultValue0 = 1.0f;
				m_DefaultValue1 = 1.0f;
				break;
			case TRANSFORM_ROTATE:
				m_Member0 = CAsset_Skeleton::BONE_ANGLE;
				m_DefaultValue0 = 0.0f;
				break;
			case TRANSFORM_LENGTH:
				m_Member0 = CAsset_Skeleton::BONE_ANGLE;
				m_Member1 = CAsset_Skeleton::BONE_LENGTH;
				m_DefaultValue0 = 0.0f;
				m_DefaultValue1 = 32.0f;
				break;
		}	
	}
	else if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETONANIMATION)
	{
		switch(m_Transform)
		{
			case TRANSFORM_TRANSLATION_X:
				m_Member0 = CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_X;
				m_DefaultValue0 = 0.0f;
				break;
			case TRANSFORM_TRANSLATION_Y:
				m_Member1 = CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_Y;
				m_DefaultValue1 = 0.0f;
				break;
			case TRANSFORM_TRANSLATION_XY:
				m_Member0 = CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_X;
				m_Member1 = CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_Y;
				m_DefaultValue0 = 0.0f;
				m_DefaultValue1 = 0.0f;
				break;
			case TRANSFORM_SCALE_X:
				m_Member0 = CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X;
				m_DefaultValue0 = 1.0f;
				break;
			case TRANSFORM_SCALE_Y:
				m_Member1 = CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y;
				m_DefaultValue1 = 1.0f;
				break;
			case TRANSFORM_SCALE_XY:
				m_Member0 = CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X;
				m_Member1 = CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y;
				m_DefaultValue0 = 1.0f;
				m_DefaultValue1 = 1.0f;
				break;
			case TRANSFORM_ROTATE:
				m_Member0 = CAsset_SkeletonAnimation::BONEKEYFRAME_ANGLE;
				m_DefaultValue0 = 1.0f;
				break;
		}	
	}
}

void CView_CursorTool_Bone_Transform::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	vec2 CursorPos = vec2(X, Y);
	
	CSkeletonRenderer SkeletonRenderer(TUGraphics(), AssetsManager());
	
	if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETON)
	{	
		SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
	}
	else if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETONANIMATION)
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(AssetsEditor()->m_ViewedAssetPath);
		if(!pSkeletonAnimation)
			return;
		
		SkeletonRenderer.AddSkeletonWithParents(pSkeletonAnimation->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
		SkeletonRenderer.ApplyAnimation(AssetsEditor()->m_ViewedAssetPath, AssetsEditor()->GetTime());
	}
	
	SkeletonRenderer.Finalize();
	
	vec2 Origin, AxisX, AxisY;
	switch(m_Transform)
	{
		case TRANSFORM_TRANSLATION_X:
		case TRANSFORM_TRANSLATION_Y:
		case TRANSFORM_TRANSLATION_XY:
			if(SkeletonRenderer.GetParentAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, Origin, AxisX, AxisY))
			{
				if(m_Transform != TRANSFORM_TRANSLATION_Y)
				{
					float TranslationX = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, m_DefaultValue0);
					TranslationX += dot(AxisX, vec2(RelX, RelY));
					AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, TranslationX);
				}
				
				if(m_Transform != TRANSFORM_TRANSLATION_X)
				{
					float TranslationY = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, m_DefaultValue1);
					TranslationY += dot(AxisY, vec2(RelX, RelY));
					AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, TranslationY);
				}
			}
			break;
		case TRANSFORM_SCALE_X:
			if(SkeletonRenderer.GetLocalAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, Origin, AxisX, AxisY))
			{
				float ScaleX = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, m_DefaultValue0);
				ScaleX += dot(AxisX, vec2(RelX, RelY))/5.0f;
				AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, ScaleX);
			}
			break;
		case TRANSFORM_SCALE_Y:
			if(SkeletonRenderer.GetLocalAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, Origin, AxisX, AxisY))
			{
				float ScaleY = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, m_DefaultValue1);
				ScaleY += dot(AxisY, vec2(RelX, RelY))/5.0f;
				AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, ScaleY);
			}
			break;
		case TRANSFORM_SCALE_XY:
			if(SkeletonRenderer.GetLocalAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, Origin, AxisX, AxisY))
			{
				float ScaleX = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, m_DefaultValue0);
				float ScaleY = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, m_DefaultValue1);
				float Scale = length(CursorPos - Origin)/length(m_Pivot - Origin);
				ScaleX = m_BackupValue0 * Scale;
				ScaleY = m_BackupValue1 * Scale;
				AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, ScaleX);
				AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, ScaleY);
			}
			break;
		case TRANSFORM_ROTATE:
			if(SkeletonRenderer.GetLocalAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, Origin, AxisX, AxisY))
			{
				float Angle = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, m_DefaultValue0);
				Angle += angle(m_CursorLastPos - Origin, CursorPos - Origin);
				AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, Angle);
			}
		case TRANSFORM_LENGTH:
			if(SkeletonRenderer.GetLocalAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, Origin, AxisX, AxisY))
			{
				float Angle = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, m_DefaultValue0);
				float Length = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, m_DefaultValue1);
				Angle += angle(m_CursorLastPos - Origin, CursorPos - Origin);
				Length += dot(AxisX, vec2(RelX, RelY));
				AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member1, m_SelectedSubItem, Length);
				AssetsManager()->SetAssetValue<float>(m_SelectedAsset, m_Member0, m_SelectedSubItem, Angle);
			}
	}
	
	m_CursorLastPos = CursorPos;
}

CView_CursorTool_Bone_TranslateXY::CView_CursorTool_Bone_TranslateXY(CView* pView, int CursorToolID) :
	CView_CursorTool_Bone_Transform(pView, CursorToolID, CView_CursorTool_Bone_Transform::TRANSFORM_TRANSLATION_XY)
{
	
}

CView_CursorTool_Bone_TranslateX::CView_CursorTool_Bone_TranslateX(CView* pView, int CursorToolID) :
	CView_CursorTool_Bone_Transform(pView, CursorToolID, CView_CursorTool_Bone_Transform::TRANSFORM_TRANSLATION_X)
{
	
}

CView_CursorTool_Bone_TranslateY::CView_CursorTool_Bone_TranslateY(CView* pView, int CursorToolID) :
	CView_CursorTool_Bone_Transform(pView, CursorToolID, CView_CursorTool_Bone_Transform::TRANSFORM_TRANSLATION_Y)
{
	
}

CView_CursorTool_Bone_ScaleXY::CView_CursorTool_Bone_ScaleXY(CView* pView, int CursorToolID) :
	CView_CursorTool_Bone_Transform(pView, CursorToolID, CView_CursorTool_Bone_Transform::TRANSFORM_SCALE_XY)
{
	
}

CView_CursorTool_Bone_ScaleX::CView_CursorTool_Bone_ScaleX(CView* pView, int CursorToolID) :
	CView_CursorTool_Bone_Transform(pView, CursorToolID, CView_CursorTool_Bone_Transform::TRANSFORM_SCALE_X)
{
	
}

CView_CursorTool_Bone_ScaleY::CView_CursorTool_Bone_ScaleY(CView* pView, int CursorToolID) :
	CView_CursorTool_Bone_Transform(pView, CursorToolID, CView_CursorTool_Bone_Transform::TRANSFORM_SCALE_Y)
{
	
}

CView_CursorTool_Bone_Rotate::CView_CursorTool_Bone_Rotate(CView* pView, int CursorToolID) :
	CView_CursorTool_Bone_Transform(pView, CursorToolID, CView_CursorTool_Bone_Transform::TRANSFORM_ROTATE)
{
	
}

CView_CursorTool_Bone_Length::CView_CursorTool_Bone_Length(CView* pView, int CursorToolID) :
	CView_CursorTool_Bone_Transform(pView, CursorToolID, CView_CursorTool_Bone_Transform::TRANSFORM_LENGTH)
{
	
}

/* BONE ADD ***********************************************************/

CView_CursorTool_Bone_Add::CView_CursorTool_Bone_Add(CView* pView, int CursorToolID) :
	CView_CursorTool(pView, CursorToolID)
{
	
}

void CView_CursorTool_Bone_Add::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1)
	{
		if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETON)
		{
			CAsset_Skeleton* pSkeleton = AssetsManager()->GetAsset<CAsset_Skeleton>(AssetsEditor()->m_ViewedAssetPath);
			if(!pSkeleton)
				return;
			
			CSkeletonRenderer SkeletonRenderer(TUGraphics(), AssetsManager());
			SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
			SkeletonRenderer.Finalize();
			
			CAssetPath SkeletonPath;
			CAsset_Skeleton::CSubPath BonePath;
			if(SkeletonRenderer.BonePicking(View()->GetViewPos(), View()->GetViewZoom(), vec2(X, Y), SkeletonPath, BonePath))
			{
				if(SkeletonPath == AssetsEditor()->m_ViewedAssetPath || SkeletonPath == pSkeleton->m_ParentPath)
				{
					CAsset_Skeleton::CSubPath NewBonePath = AssetsManager()->AddSubItem(AssetsEditor()->m_ViewedAssetPath, CAsset_Skeleton::CSubPath::TYPE_BONE);
					if(!NewBonePath.IsNull())
					{
						m_Pivot = vec2(X, Y);
						m_CursorLastPos = vec2(X, Y);
						
						View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
						
						m_SelectedSqueleton = AssetsEditor()->m_ViewedAssetPath;
						m_SelectedBone = NewBonePath;
						
						CAsset_Skeleton::CSubPath ParentPath;
						if(SkeletonPath == pSkeleton->m_ParentPath)
							ParentPath = BonePath.Parent();
						else
							ParentPath = BonePath;
						
						AssetsManager()->SetAssetValue<CAsset_Skeleton::CSubPath>(
							m_SelectedSqueleton,
							m_SelectedBone,
							CAsset_Skeleton::BONE_PARENT,
							ParentPath);
						
						AssetsEditor()->RefreshAssetsEditor();
					}
				}
			}
			else
			{
				CAsset_Skeleton::CSubPath NewBonePath = AssetsManager()->AddSubItem(AssetsEditor()->m_ViewedAssetPath, CAsset_Skeleton::CSubPath::TYPE_BONE);
				if(!NewBonePath.IsNull())
				{
					m_Pivot = vec2(X, Y);
					m_CursorLastPos = vec2(X, Y);
					
					View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
					
					m_SelectedSqueleton = AssetsEditor()->m_ViewedAssetPath;
					m_SelectedBone = NewBonePath;
					
					CAsset_Skeleton::CSubPath ParentPath;
					if(SkeletonPath == pSkeleton->m_ParentPath)
						ParentPath = CAsset_Skeleton::CSubPath::ParentBone(BonePath.GetId());
					else
						ParentPath = CAsset_Skeleton::CSubPath::LocalBone(BonePath.GetId());
					
					AssetsManager()->SetAssetValue<CAsset_Skeleton::CSubPath>(
						m_SelectedSqueleton,
						m_SelectedBone,
						CAsset_Skeleton::BONE_PARENT,
						ParentPath);
					
					AssetsEditor()->RefreshAssetsEditor();
				}
			}
		}
	}
}

void CView_CursorTool_Bone_Add::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	vec2 CursorPos = vec2(X, Y);
	
	if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETON)
	{
		CSkeletonRenderer SkeletonRenderer(TUGraphics(), AssetsManager());
		SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
		SkeletonRenderer.Finalize();
		
		vec2 Origin, AxisX, AxisY;
		if(SkeletonRenderer.GetLocalAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, Origin, AxisX, AxisY))
		{
			vec2 RelPos = (CursorPos - Origin);
			float LengthX = dot(AxisX, RelPos);
			float LengthY = dot(AxisY, RelPos);
			float Length = sqrt(LengthX*LengthX + LengthY*LengthY);
			AssetsManager()->SetAssetValue<float>(m_SelectedSqueleton, m_SelectedBone, CAsset_Skeleton::BONE_LENGTH, Length);
			float Angle = angle(AxisX, RelPos);
			AssetsManager()->SetAssetValue<float>(m_SelectedSqueleton, m_SelectedBone, CAsset_Skeleton::BONE_ANGLE, Angle);
		}
		vec2 OriginParent, AxisXParent, AxisYParent;
		if(SkeletonRenderer.GetParentAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, OriginParent, AxisXParent, AxisYParent))
		{
			float Angle = angle(AxisXParent, m_CursorLastPos - Origin);
			AssetsManager()->SetAssetValue<float>(m_SelectedSqueleton, m_SelectedBone, CAsset_Skeleton::BONE_ANGLE, Angle);
		}
	}
	
	m_CursorLastPos = CursorPos;
}

/* BONE DELETE ********************************************************/

CView_CursorTool_Bone_Delete::CView_CursorTool_Bone_Delete(CView* pView, int CursorToolID) :
	CView_CursorTool(pView, CursorToolID)
{
	
}

void CView_CursorTool_Bone_Delete::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1)
	{
		if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETON)
		{
			CSkeletonRenderer SkeletonRenderer(TUGraphics(), AssetsManager());
			SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
			SkeletonRenderer.Finalize();
			
			CAssetPath SkeletonPath;
			CAsset_Skeleton::CSubPath BonePath;
			if(SkeletonRenderer.BonePicking(View()->GetViewPos(), View()->GetViewZoom(), vec2(X, Y), SkeletonPath, BonePath) && SkeletonPath == AssetsEditor()->m_ViewedAssetPath)
			{
				if(SkeletonPath == AssetsEditor()->m_ViewedAssetPath)
				{
					AssetsManager()->DeleteSubItem(AssetsEditor()->m_ViewedAssetPath, BonePath);
					AssetsEditor()->RefreshAssetsEditor();
				}
			}
		}
	}
}

/* IMAGESTAMP *********************************************************/

	//Texture Palette
CView_CursorTool_MapStamp::CPalettePopup::CTexturePalette::CTexturePalette(gui::CConfig* pConfig, CPalettePopup* pPopup, CAssetPath ImagePath) :
	gui::CWidget(pConfig),
	m_pPopup(pPopup),
	m_ImagePath(ImagePath),
	m_FirstX(0),
	m_FirstY(0),
	m_CurrX(0),
	m_CurrY(0),
	m_Clicked(false)
{
	
}
		
void CView_CursorTool_MapStamp::CPalettePopup::CTexturePalette::Render()
{
	CAsset_Image* pImage = m_pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(!pImage)
		return;
		
	float ImgRatio = static_cast<float>(pImage->GetWidth())/static_cast<float>(pImage->GetHeight());
	float WindowRatio = static_cast<float>(m_Rect.w)/static_cast<float>(m_Rect.h);
	float SizeX;
	float SizeY;
	
	if(ImgRatio > WindowRatio)
	{
		SizeX = m_Rect.w;
		SizeY = m_Rect.w/ImgRatio;
	}
	else
	{
		SizeX = m_Rect.h*ImgRatio;
		SizeY = m_Rect.h;
	}
	
	float x0 = m_Rect.x + m_Rect.w/2 - SizeX/2;
	float y0 = m_Rect.y + m_Rect.h/2 - SizeY/2;
	float x1 = x0 + SizeX;
	float y1 = y0 + SizeY;
	float xStep = SizeX / static_cast<float>(max(1, pImage->GetGridWidth()));
	float yStep = SizeY / static_cast<float>(max(1, pImage->GetGridHeight()));
	
	//Draw image
	m_pPopup->m_pAssetsEditor->TUGraphics()->TextureSet(m_ImagePath);
	
	Graphics()->QuadsBegin();
	IGraphics::CQuadItem QuadItem(x0, y0, SizeX, SizeY);
	Graphics()->QuadsDrawTL(&QuadItem, 1);
	Graphics()->QuadsEnd();
	
	//Draw selection
	if(m_Clicked)
	{
		int X = min(m_FirstX, m_CurrX);
		int Y = min(m_FirstY, m_CurrY);
		int W = max(m_FirstX, m_CurrX) - X + 1;
		int H = max(m_FirstY, m_CurrY) - Y + 1;
		
		gui::CRect Rect;
		Rect.x = x0 + X * xStep;
		Rect.y = y0 + Y * yStep;
		Rect.w = W * xStep;
		Rect.h = H * yStep;
		
		TUGraphics()->DrawGuiRect(&Rect, CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, GUIRECTSTYLE_SELECTION));
	}
	
	//Draw grid
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(vec4(0.0f, 0.0f, 0.0f, 0.5f), true);
	
	for(int i=0; i<=pImage->GetGridWidth(); i++)
	{
		float x = x0 + i * xStep;
		IGraphics::CLineItem Line(x+0.5f, y0+0.5f, x+0.5f, y1+0.5f);
		Graphics()->LinesDraw(&Line, 1);
	}
	for(int i=0; i<=pImage->GetGridHeight(); i++)
	{
		float y = y0 + i * yStep;
		IGraphics::CLineItem Line(x0+0.5f, y+0.5f, x1+0.5f, y+0.5f);
		Graphics()->LinesDraw(&Line, 1);
	}
	
	Graphics()->LinesEnd();
	
	//Update image size
	m_ImageRect.x = x0;
	m_ImageRect.y = y0;
	m_ImageRect.w = SizeX;
	m_ImageRect.h = SizeY;
}
		
void CView_CursorTool_MapStamp::CPalettePopup::CTexturePalette::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	if(m_Clicked)
		return;
	
	if(!m_ImageRect.IsInside(X, Y))
		return;
	
	CAsset_Image* pImage = m_pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(!pImage)
		return;
	
	if(Button == KEY_MOUSE_1)
	{
		m_FirstX = (pImage->GetGridWidth()*(X - m_ImageRect.x))/(m_ImageRect.w);
		m_FirstY = (pImage->GetGridHeight()*(Y - m_ImageRect.y))/(m_ImageRect.h);
		m_CurrX = m_FirstX;
		m_CurrY = m_FirstY;
		
		m_Clicked = true;
	}
}
		
void CView_CursorTool_MapStamp::CPalettePopup::CTexturePalette::OnButtonRelease(int Button)
{
	if(Button != KEY_MOUSE_1)
		return;
		
	m_Clicked = false;
	
	CAsset_Image* pImage = m_pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(!pImage)
		return;
	
	int X0 = min(m_FirstX, m_CurrX);
	int Y0 = min(m_FirstY, m_CurrY);
	int X1 = max(m_FirstX, m_CurrX) + 1;
	int Y1 = max(m_FirstY, m_CurrY) + 1;
		
	m_pPopup->OnTexturePaletteSelection(X0, Y0, X1, Y1);
}

void CView_CursorTool_MapStamp::CPalettePopup::CTexturePalette::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(!m_Clicked)
		return;
	
	CAsset_Image* pImage = m_pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(!pImage)
		return;
	
	if(m_ImageRect.IsInside(X, Y))
	{
		m_CurrX = (pImage->GetGridWidth()*(X - m_ImageRect.x))/(m_ImageRect.w);
		m_CurrY = (pImage->GetGridHeight()*(Y - m_ImageRect.y))/(m_ImageRect.h);
	}
}

	//Zone Entry
CView_CursorTool_MapStamp::CPalettePopup::CListEntry::
	CListEntry(gui::CConfig* pConfig, CPalettePopup* pPopup, int SubPath, const char* pText) :
		gui::CTextButton(pConfig, pText, CAssetPath::SpriteUniverse(SPRITE_ICON_DEFAULT)),
		m_pPopup(pPopup),
		m_SubPath(SubPath)
{
	m_Centered = false;
}

void CView_CursorTool_MapStamp::CPalettePopup::CListEntry::
	MouseClickAction()
{
	m_pPopup->OnListPaletteSelection(m_SubPath);
}

	//Zone Palette
CView_CursorTool_MapStamp::CPalettePopup::CZonePalette::
	CZonePalette(gui::CConfig* pConfig, CPalettePopup* pPopup, CAssetPath ZoneTypePath) :
		gui::CVListLayout(pConfig),
		m_pPopup(pPopup),
		m_ZoneTypePath(ZoneTypePath)
{
	CAsset_ZoneType* pZoneType = m_pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_ZoneType>(m_ZoneTypePath);
	if(!pZoneType)
		return;
	
	bool Index0Found = false;
	CAsset_ZoneType::CIteratorIndex Iter;
	for(Iter = pZoneType->BeginIndex(); Iter != pZoneType->EndIndex(); ++Iter)
	{
		if(pZoneType->GetIndexNumber(*Iter) == 0)
			Index0Found = true;
	}
	
	if(!Index0Found)
	{
		CListEntry* pEntry = new CListEntry(m_pConfig, m_pPopup, CAsset_ZoneType::CSubPath::Null(), "No effect");
		Add(pEntry);
	}
	
	for(Iter = pZoneType->BeginIndex(); Iter != pZoneType->EndIndex(); ++Iter)
	{
		const char* pName = pZoneType->GetIndexName(*Iter);
		CListEntry* pEntry = new CListEntry(m_pConfig, m_pPopup, *Iter, pName);
		Add(pEntry);
	}
	
	Update();
}

CView_CursorTool_MapStamp::CPalettePopup::CPalettePopup(
	CAssetsEditor* pAssetsEditor,
	CView_CursorTool_MapStamp* pCursorTool,
	const gui::CRect& CreatorRect
) :
	gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, -1, -1, gui::CPopup::ALIGNMENT_INNER),
	m_pAssetsEditor(pAssetsEditor),
	m_pCursorTool(pCursorTool)
{
	if(m_pAssetsEditor->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
	{
		CAsset_MapLayerTiles* pLayer = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapLayerTiles>(m_pAssetsEditor->m_ViewedAssetPath);
		if(pLayer)
			Add(new CTexturePalette(m_pConfig, this, pLayer->GetImagePath()));
	}
	else if(m_pAssetsEditor->m_ViewedAssetPath.GetType() == CAsset_MapZoneTiles::TypeId)
	{
		CAsset_MapZoneTiles* pLayer = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapZoneTiles>(m_pAssetsEditor->m_ViewedAssetPath);
		if(pLayer)
			Add(new CZonePalette(m_pConfig, this, pLayer->GetZoneTypePath()));
	}
	else if(m_pAssetsEditor->m_ViewedAssetPath.GetType() == CAsset_MapLayerQuads::TypeId)
	{
		CAsset_MapLayerQuads* pLayer = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapLayerQuads>(m_pAssetsEditor->m_ViewedAssetPath);
		if(pLayer)
			Add(new CTexturePalette(m_pConfig, this, pLayer->GetImagePath()));
	}
	
	Update();
}
	
void CView_CursorTool_MapStamp::CPalettePopup::Close()
{
	m_pCursorTool->OnPaletteCloseCallback();
	
	gui::CPopup::Close();
}
	
void CView_CursorTool_MapStamp::CPalettePopup::OnTexturePaletteSelection(int X0, int Y0, int X1, int Y1)
{
	m_pCursorTool->OnTexturePaletteSelection(X0, Y0, X1, Y1);
	Close();
}
	
void CView_CursorTool_MapStamp::CPalettePopup::OnListPaletteSelection(int SubPathInt)
{
	m_pCursorTool->OnListPaletteSelection(SubPathInt);
	Close();
}

CView_CursorTool_MapStamp::CView_CursorTool_MapStamp(CView* pView, int CursorToolID) :
	CView_CursorTool(pView, CursorToolID),
	m_pPalette(0)
{
	
}

void CView_CursorTool_MapStamp::OnPaletteCloseCallback()
{
	m_pPalette = 0;
}

/* MAPLAYERTILES SELECTIONSTAMP ***************************************/

	// Buttons
CView_CursorTool_MapTiles_SelectionStamp::CHFlipButton::CHFlipButton(CView_CursorTool_MapTiles_SelectionStamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_HFLIP)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapTiles_SelectionStamp::CHFlipButton::MouseClickAction()
{
	if(m_pCursorTool->m_pSelection)
	{
		CAsset_MapLayerTiles::CTile TmpTile;
		for(int i=0; i<m_pCursorTool->m_SelectionWidth; i++)
		{
			int MaxHeight = m_pCursorTool->m_SelectionHeight/2;
			if(m_pCursorTool->m_SelectionHeight%2 == 1)
				MaxHeight++;
			
			for(int j=0; j<MaxHeight; j++)
			{
				int Index0 = j*m_pCursorTool->m_SelectionWidth+i;
				int Index1 = (m_pCursorTool->m_SelectionHeight-j-1)*m_pCursorTool->m_SelectionWidth+i;
				
				if(Index0 != Index1)
				{
					TmpTile = m_pCursorTool->m_pSelection[Index0];
					m_pCursorTool->m_pSelection[Index0] = m_pCursorTool->m_pSelection[Index1];
					m_pCursorTool->m_pSelection[Index1] = TmpTile;
					
					m_pCursorTool->m_pSelection[Index0].m_Flags ^= CAsset_MapLayerTiles::TILEFLAG_HFLIP;
				}
				
				m_pCursorTool->m_pSelection[Index1].m_Flags ^= CAsset_MapLayerTiles::TILEFLAG_HFLIP;
			}
		}
	}
}

CView_CursorTool_MapTiles_SelectionStamp::CVFlipButton::CVFlipButton(CView_CursorTool_MapTiles_SelectionStamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_VFLIP)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapTiles_SelectionStamp::CVFlipButton::MouseClickAction()
{
	if(m_pCursorTool->m_pSelection)
	{
		CAsset_MapLayerTiles::CTile TmpTile;
		for(int j=0; j<m_pCursorTool->m_SelectionHeight; j++)
		{
			int MaxWidth = m_pCursorTool->m_SelectionWidth/2;
			if(m_pCursorTool->m_SelectionWidth%2 == 1)
				MaxWidth++;
			
			for(int i=0; i<MaxWidth; i++)
			{
				int Index0 = j*m_pCursorTool->m_SelectionWidth+i;
				int Index1 = j*m_pCursorTool->m_SelectionWidth+(m_pCursorTool->m_SelectionWidth-i-1);
				
				if(Index0 != Index1)
				{
					TmpTile = m_pCursorTool->m_pSelection[Index0];
					m_pCursorTool->m_pSelection[Index0] = m_pCursorTool->m_pSelection[Index1];
					m_pCursorTool->m_pSelection[Index1] = TmpTile;
					
					m_pCursorTool->m_pSelection[Index0].m_Flags ^= CAsset_MapLayerTiles::TILEFLAG_VFLIP;
				}
				
				m_pCursorTool->m_pSelection[Index1].m_Flags ^= CAsset_MapLayerTiles::TILEFLAG_VFLIP;
			}
		}
	}
}

CView_CursorTool_MapTiles_SelectionStamp::CRotateCWButton::CRotateCWButton(CView_CursorTool_MapTiles_SelectionStamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_ROTATE_CW)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapTiles_SelectionStamp::CRotateCWButton::MouseClickAction()
{
	if(m_pCursorTool->m_pSelection)
	{
		CAsset_MapLayerTiles::CTile* pNewSelection = new CAsset_MapLayerTiles::CTile[m_pCursorTool->m_SelectionWidth*m_pCursorTool->m_SelectionHeight];
		
		for(int j=0; j<m_pCursorTool->m_SelectionHeight; j++)
		{
			for(int i=0; i<m_pCursorTool->m_SelectionWidth; i++)
			{
				int I = m_pCursorTool->m_SelectionHeight-j-1;
				int J = i;
				
				int Index0 = j*m_pCursorTool->m_SelectionWidth+i;
				int Index1 = J*m_pCursorTool->m_SelectionHeight+I;
				
				pNewSelection[Index1] = m_pCursorTool->m_pSelection[Index0];
			}
		}
		
		delete[] m_pCursorTool->m_pSelection;
		m_pCursorTool->m_pSelection = pNewSelection;
		
		int Tmp = m_pCursorTool->m_SelectionWidth;
		m_pCursorTool->m_SelectionWidth = m_pCursorTool->m_SelectionHeight;
		m_pCursorTool->m_SelectionHeight = Tmp;
	}
}

CView_CursorTool_MapTiles_SelectionStamp::CRotateCCWButton::CRotateCCWButton(CView_CursorTool_MapTiles_SelectionStamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_ROTATE_CCW)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapTiles_SelectionStamp::CRotateCCWButton::MouseClickAction()
{
	if(m_pCursorTool->m_pSelection)
	{
		CAsset_MapLayerTiles::CTile* pNewSelection = new CAsset_MapLayerTiles::CTile[m_pCursorTool->m_SelectionWidth*m_pCursorTool->m_SelectionHeight];
		
		for(int j=0; j<m_pCursorTool->m_SelectionHeight; j++)
		{
			for(int i=0; i<m_pCursorTool->m_SelectionWidth; i++)
			{
				int I = j;
				int J = m_pCursorTool->m_SelectionWidth-i-1;
				
				int Index0 = j*m_pCursorTool->m_SelectionWidth+i;
				int Index1 = J*m_pCursorTool->m_SelectionHeight+I;
				
				pNewSelection[Index1] = m_pCursorTool->m_pSelection[Index0];
				
				if(!(pNewSelection[Index1].m_Flags&TILEFLAG_ROTATE))
					pNewSelection[Index1].m_Flags ^= (TILEFLAG_HFLIP|TILEFLAG_VFLIP);
				
				pNewSelection[Index1].m_Flags ^= TILEFLAG_ROTATE;
			}
		}
		
		delete[] m_pCursorTool->m_pSelection;
		m_pCursorTool->m_pSelection = pNewSelection;
		
		int Tmp = m_pCursorTool->m_SelectionWidth;
		m_pCursorTool->m_SelectionWidth = m_pCursorTool->m_SelectionHeight;
		m_pCursorTool->m_SelectionHeight = Tmp;
	}
}

	// CursorTool
	
CView_CursorTool_MapTiles_SelectionStamp::CView_CursorTool_MapTiles_SelectionStamp(CView* pView, int CursorToolID) :
	CView_CursorTool_MapStamp(pView, CursorToolID),
	m_pSelection(0),
	m_DragSelection(false),
	m_Token(CAssetsHistory::NEW_TOKEN)
{
	
}

CView_CursorTool_MapTiles_SelectionStamp::~CView_CursorTool_MapTiles_SelectionStamp()
{
	if(m_pSelection)
		delete[] m_pSelection;
}

void CView_CursorTool_MapTiles_SelectionStamp::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1)
	{
		CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
		
		if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
			View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
		else
			View()->InitMapRenderer(&MapRenderer);
	
		if(!m_pSelection)
		{
			m_DragSelection = true;
		}
		else
		{
			m_Token = AssetsManager()->GenerateToken();
		}
		
		m_Pivot = MapRenderer.ScreenPosToTilePos(vec2(X, Y));
		View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
	}
	else if(Button == KEY_MOUSE_2)
	{
		if(m_pSelection)
		{
			delete[] m_pSelection;
			m_pSelection = 0;
			View()->RefreshToolBar();
		}
		else if(!m_pPalette)
		{
			m_pPalette = new CPalettePopup(AssetsEditor(), this, View()->GetViewRect());
			AssetsEditor()->DisplayPopup(m_pPalette);
		}
	}
}

void CView_CursorTool_MapTiles_SelectionStamp::OnButtonRelease(int Button)
{
	if(m_DragSelection)
	{
		if(m_pSelection)
		{
			delete[] m_pSelection;
			m_pSelection = 0;
		}
		
		CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
		
		if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
			View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
		else
			View()->InitMapRenderer(&MapRenderer);
		
		vec2 CursorTilePos = MapRenderer.ScreenPosToTilePos(AssetsEditor()->GetCursorPos());
	
		int TileX0 = static_cast<int>(m_Pivot.x);
		int TileY0 = static_cast<int>(m_Pivot.y);
		int TileX1 = static_cast<int>(CursorTilePos.x);
		int TileY1 = static_cast<int>(CursorTilePos.y);
	
		int X0 = min(TileX0, TileX1);
		int Y0 = min(TileY0, TileY1);
		int X1 = max(TileX0, TileX1) + 1;
		int Y1 = max(TileY0, TileY1) + 1;
		
		if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
		{
			CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_ViewedAssetPath);
			if(!pLayer)
				return;
			
			X0 = clamp(X0, 0, pLayer->GetWidth());
			Y0 = clamp(Y0, 0, pLayer->GetHeight());
			X1 = clamp(X1, 1, pLayer->GetWidth()+1);
			Y1 = clamp(Y1, 1, pLayer->GetHeight()+1);
				
			m_SelectionWidth = X1-X0;
			m_SelectionHeight = Y1-Y0;
			m_pSelection = new CAsset_MapLayerTiles::CTile[m_SelectionWidth*m_SelectionHeight];
		
			for(int j=0; j<m_SelectionHeight; j++)
			{
				for(int i=0; i<m_SelectionWidth; i++)
				{
					m_pSelection[j*m_SelectionWidth+i].m_Index = pLayer->GetTileIndex(X0+i, Y0+j);
					m_pSelection[j*m_SelectionWidth+i].m_Flags = pLayer->GetTileFlags(X0+i, Y0+j);
				}
			}
		}
		else if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapZoneTiles::TypeId)
		{
			CAsset_MapZoneTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapZoneTiles>(AssetsEditor()->m_ViewedAssetPath);
			if(!pLayer)
				return;
			
			X0 = clamp(X0, 0, pLayer->GetWidth());
			Y0 = clamp(Y0, 0, pLayer->GetHeight());
			X1 = clamp(X1, 1, pLayer->GetWidth()+1);
			Y1 = clamp(Y1, 1, pLayer->GetHeight()+1);
				
			m_SelectionWidth = X1-X0;
			m_SelectionHeight = Y1-Y0;
			m_pSelection = new CAsset_MapLayerTiles::CTile[m_SelectionWidth*m_SelectionHeight];
		
			for(int j=0; j<m_SelectionHeight; j++)
			{
				for(int i=0; i<m_SelectionWidth; i++)
				{
					m_pSelection[j*m_SelectionWidth+i].m_Index = pLayer->GetTileIndex(X0+i, Y0+j);
				}
			}
		}
		
		View()->RefreshToolBar();
		
		m_DragSelection = false;
	}
	
	m_Token = CAssetsHistory::NEW_TOKEN;
}

void CView_CursorTool_MapTiles_SelectionStamp::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{			
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	
	if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
		View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	else
		View()->InitMapRenderer(&MapRenderer);
	
	vec2 CursorMapPos = MapRenderer.ScreenPosToMapPos(vec2(X, Y));

	int TileX = CursorMapPos.x/32.0f;
	int TileY = CursorMapPos.y/32.0f;
	
	if(!m_pSelection)
		return;
	
	if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
	{
		CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_ViewedAssetPath);
		if(!pLayer)
			return;
		
		for(int j=0; j<m_SelectionHeight; j++)
		{
			for(int i=0; i<m_SelectionWidth; i++)
			{
				CAsset_MapLayerTiles::CSubPath TilePath = CAsset_MapLayerTiles::CSubPath::Tile(TileX+i, TileY+j);
				AssetsManager()->SetAssetValue<int>(AssetsEditor()->m_ViewedAssetPath, TilePath, CAsset_MapLayerTiles::TILE_INDEX, m_pSelection[j*m_SelectionWidth+i].m_Index, m_Token);
				AssetsManager()->SetAssetValue<int>(AssetsEditor()->m_ViewedAssetPath, TilePath, CAsset_MapLayerTiles::TILE_FLAGS, m_pSelection[j*m_SelectionWidth+i].m_Flags, m_Token);
			}
		}
	}
	else if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapZoneTiles::TypeId)
	{
		CAsset_MapZoneTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapZoneTiles>(AssetsEditor()->m_ViewedAssetPath);
		if(!pLayer)
			return;
		
		for(int j=0; j<m_SelectionHeight; j++)
		{
			for(int i=0; i<m_SelectionWidth; i++)
			{
				CAsset_MapLayerTiles::CSubPath TilePath = CAsset_MapLayerTiles::CSubPath::Tile(TileX+i, TileY+j);
				AssetsManager()->SetAssetValue<int>(AssetsEditor()->m_ViewedAssetPath, TilePath, CAsset_MapZoneTiles::TILE_INDEX, m_pSelection[j*m_SelectionWidth+i].m_Index, m_Token);
			}
		}
	}
}

void CView_CursorTool_MapTiles_SelectionStamp::Render()
{	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	
	if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
		View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	else
		View()->InitMapRenderer(&MapRenderer);
	
	CAssetPath ImagePath;
	vec4 Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
	{
		CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_ViewedAssetPath);
		if(!pLayer)
			return;
		
		ImagePath = pLayer->GetImagePath();
		Color = pLayer->GetColor();
	}
	
	float Time = fmod(AssetsEditor()->Client()->LocalTime(), 1.0f);
	Color.a *= 0.3f + 0.7f*(1.0f+cos(2.0f*pi*Time))/2.0f;
	
	vec2 CursorTilePos = MapRenderer.ScreenPosToTilePos(AssetsEditor()->GetCursorPos());
	
	if(m_pSelection)
	{
		vec2 LayerPos = MapRenderer.TilePosToMapPos(vec2(floor(CursorTilePos.x), floor(CursorTilePos.y)));
		
		if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
		{
			MapRenderer.RenderTiles_Image(m_pSelection, m_SelectionWidth, m_SelectionHeight, LayerPos, ImagePath, Color);
		}
		else if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAsset_MapZoneTiles::TypeId)
		{
			CAsset_MapZoneTiles* pZone = AssetsManager()->GetAsset<CAsset_MapZoneTiles>(AssetsEditor()->m_ViewedAssetPath);
			if(!pZone)
				return;
				
			MapRenderer.RenderTiles_Zone(pZone->GetZoneTypePath(), m_pSelection, m_SelectionWidth, m_SelectionHeight, LayerPos, AssetsEditor()->m_ZoneTexture);
		}
		
		vec2 LayerScreenPos0 = MapRenderer.MapPosToScreenPos(LayerPos);
		vec2 LayerScreenPos1 = MapRenderer.MapPosToScreenPos(LayerPos + vec2(m_SelectionWidth, m_SelectionHeight)*32.0f);
		vec2 LayerScreenSize = LayerScreenPos1 - LayerScreenPos0;
		
		gui::CRect Rect;
		Rect.x = LayerScreenPos0.x;
		Rect.y = LayerScreenPos0.y;
		Rect.w = LayerScreenSize.x;
		Rect.h = LayerScreenSize.y;
		
		TUGraphics(ASSETS_EDITORGUI)->DrawGuiRect(&Rect, CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, GUIRECTSTYLE_STAMP));
	}
	else if(m_DragSelection)
	{
		int TileX0 = static_cast<int>(m_Pivot.x);
		int TileY0 = static_cast<int>(m_Pivot.y);
		int TileX1 = static_cast<int>(CursorTilePos.x);
		int TileY1 = static_cast<int>(CursorTilePos.y);
	
		int X0 = min(TileX0, TileX1);
		int Y0 = min(TileY0, TileY1);
		int X1 = max(TileX0, TileX1) + 1;
		int Y1 = max(TileY0, TileY1) + 1;
		
		vec2 SelectionRectPos0 = MapRenderer.MapPosToScreenPos(vec2(X0*32.0f, Y0*32.0f));
		vec2 SelectionRectPos1 = MapRenderer.MapPosToScreenPos(vec2(X1*32.0f, Y1*32.0f));
		vec2 SelectionRectSize = SelectionRectPos1 - SelectionRectPos0;
		
		gui::CRect Rect;
		Rect.x = SelectionRectPos0.x;
		Rect.y = SelectionRectPos0.y;
		Rect.w = SelectionRectSize.x;
		Rect.h = SelectionRectSize.y;
		
		TUGraphics(ASSETS_EDITORGUI)->DrawGuiRect(&Rect, CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, GUIRECTSTYLE_SELECTION));
	}
}

void CView_CursorTool_MapTiles_SelectionStamp::RefreshToolBar()
{
	if(m_pSelection)
	{
		View()->Toolbar()->AddSeparator();
		View()->Toolbar()->Add(new gui::CLabel(View()->m_pConfig, "Selection:"));
		View()->Toolbar()->Add(new CHFlipButton(this));
		View()->Toolbar()->Add(new CVFlipButton(this));
		View()->Toolbar()->Add(new CRotateCWButton(this));
		View()->Toolbar()->Add(new CRotateCCWButton(this));
	}
}

void CView_CursorTool_MapTiles_SelectionStamp::OnTexturePaletteSelection(int X0, int Y0, int X1, int Y1)
{
	CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_ViewedAssetPath);
	if(!pLayer)
		return;
	
	int W = X1-X0;
	int H = Y1-Y0;
	
	int GridWidth = 1;
	int GridHeight = 1;
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pLayer->GetImagePath());	
	if(pImage)
	{
		GridWidth = pImage->GetGridWidth();
		GridHeight = pImage->GetGridHeight();
	}
	
	int Index = Y0 * pImage->GetGridWidth() + X0;
	
	if(m_pSelection)
	{
		delete[] m_pSelection;
		m_pSelection = 0;
	}
	
	m_SelectionWidth = W;
	m_SelectionHeight = H;
	m_pSelection = new CAsset_MapLayerTiles::CTile[m_SelectionWidth*m_SelectionHeight];
	for(int j=0; j<m_SelectionHeight; j++)
	{
		for(int i=0; i<m_SelectionWidth; i++)
		{
			CAsset_MapLayerTiles::CTile* pTile = &m_pSelection[j*m_SelectionWidth+i];
			pTile->m_Index = Index + j*GridWidth + i;
		}
	}
	
	View()->RefreshToolBar();
}

void CView_CursorTool_MapTiles_SelectionStamp::OnListPaletteSelection(int SubPathInt)
{
	CAsset_MapZoneTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapZoneTiles>(AssetsEditor()->m_ViewedAssetPath);
	if(!pLayer)
		return;
	
	int Index = 0;
	
	CAsset_ZoneType* pZoneType = AssetsManager()->GetAsset<CAsset_ZoneType>(pLayer->GetZoneTypePath());
	if(pZoneType)
	{
		CAsset_ZoneType::CSubPath IndexPath(SubPathInt);
		if(pZoneType->IsValidIndex(IndexPath))
			Index = pZoneType->GetIndexNumber(IndexPath);
	}
	
	if(m_pSelection)
	{
		delete[] m_pSelection;
		m_pSelection = 0;
	}
	
	m_SelectionWidth = 1;
	m_SelectionHeight = 1;
	m_pSelection = new CAsset_MapLayerTiles::CTile[m_SelectionWidth*m_SelectionHeight];

	m_pSelection[0].m_Index = Index;
	
	View()->RefreshToolBar();
	
	m_DragSelection = false;
}

/* MAPLAYERQUADS PICKER ***********************************************/

CView_CursorTool_MapQuads_Picker::CView_CursorTool_MapQuads_Picker(CView* pView, int CursorToolID) :
	CView_CursorTool(pView, CursorToolID)
{
	
}

CAsset_MapLayerQuads::CSubPath CView_CursorTool_MapQuads_Picker::PickQuad(vec2 CursorPos)
{
	CAsset_MapLayerQuads* pMapLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pMapLayer)
		return CAsset_MapLayerQuads::CSubPath::Null();
	
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pMapLayer->GetImagePath());
	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	CAsset_MapLayerQuads::CSubPath QuadFound = CAsset_MapLayerQuads::CSubPath::Null();
	
	float GizmoSize = 16.0f;
	{
		CAsset_MapLayerQuads::CIteratorQuad Iter;
		for(Iter = pMapLayer->BeginQuad(); Iter != pMapLayer->EndQuad(); ++Iter)
		{
			vec2 Position;
			matrix2x2 Transform;
			pMapLayer->GetQuadTransform(*Iter, AssetsEditor()->GetTime(), &Transform, &Position);
			
			vec2 PivotPos = MapRenderer.MapPosToScreenPos(Transform*vec2(0.0f, 0.0f) + Position);
			if(length(CursorPos - PivotPos) < GizmoSize*(12.0f/16.0f))
				QuadFound = *Iter;
			
			if(!QuadFound.IsNull())
				break;
		}
	}
	
	{
		CAsset_MapLayerQuads::CIteratorQuad Iter;
		for(Iter = pMapLayer->ReverseBeginQuad(); Iter != pMapLayer->ReverseEndQuad(); ++Iter)
		{
			vec2 Position;
			matrix2x2 Transform;
			pMapLayer->GetQuadTransform(*Iter, AssetsEditor()->GetTime(), &Transform, &Position);
			
			vec2 Vertices[4];
			for(int p=0; p<4; p++)
			{
				vec2 Vertex = pMapLayer->GetVertexPosition((*Iter).Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p));
				Vertices[p] = MapRenderer.MapPosToScreenPos(Transform*Vertex + Position);
			}
			
			if(geometry::InsideQuad(Vertices[0], Vertices[1], Vertices[2], Vertices[3], CursorPos))
			{
				if(pImage)
				{
					vec2 uv = geometry::SampleQuad(
						Vertices[0],
						Vertices[1],
						Vertices[2],
						Vertices[3],
						CursorPos,
						pMapLayer->GetVertexTexture((*Iter).Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0)),
						pMapLayer->GetVertexTexture((*Iter).Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX1)),
						pMapLayer->GetVertexTexture((*Iter).Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX2)),
						pMapLayer->GetVertexTexture((*Iter).Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX3))
					);
					
					vec4 Color = pImage->Sample(uv);
					int Alpha = 255.0f*Color.a;
					if(Alpha > 0)
						QuadFound = *Iter;
				}
				else
					QuadFound = *Iter;
			}
			
			if(!QuadFound.IsNull())
				break;
		}
	}
	
	return QuadFound;
}

void CView_CursorTool_MapQuads_Picker::RenderPivots()
{
	CAsset_MapLayerQuads* pMapLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pMapLayer)
		return;
	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	float GizmoSize = 16.0f;
	
	CAsset_MapLayerQuads::CIteratorQuad Iter;
	for(Iter = pMapLayer->BeginQuad(); Iter != pMapLayer->EndQuad(); ++Iter)
	{
		vec2 Position;
		matrix2x2 Transform;
		pMapLayer->GetQuadTransform(*Iter, AssetsEditor()->GetTime(), &Transform, &Position);
				
		//Pivot
		{
			vec2 QuadPivot = MapRenderer.MapPosToScreenPos(Transform*vec2(0.0f, 0.0f) + Position);
			
			TUGraphics(ASSETS_EDITORGUI)->DrawSprite(
				CAssetPath::SpriteUniverse(SPRITE_GIZMO_PIVOT),
				vec2(QuadPivot.x, QuadPivot.y),
				1.0f, 0.0f, 0x0, 1.0f
			);
		}
	}
}

/* MAPLAYERQUADS EDIT *************************************************/

CView_CursorTool_MapQuads_Edit::CView_CursorTool_MapQuads_Edit(CView* pView, int CursorToolID) :
	CView_CursorTool_MapQuads_Picker(pView, CursorToolID),
	m_SelectedQuad(CAsset_MapLayerQuads::CSubPath::Null()),
	m_Token(CAssetsHistory::NEW_TOKEN)
{
	
}

void CView_CursorTool_MapQuads_Edit::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	CAsset_MapLayerQuads* pMapLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pMapLayer)
		return;
	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	//Find gizmo
	float GizmoSize = 16.0f;
	vec2 CursorPos = vec2(X, Y);

	if(pMapLayer->IsValidQuad(m_SelectedQuad))
	{
		vec2 Position;
		matrix2x2 Transform;
		pMapLayer->GetQuadTransform(m_SelectedQuad, AssetsEditor()->GetTime(), &Transform, &Position);
		
		vec2 PivotPos = MapRenderer.MapPosToScreenPos(Transform*vec2(0.0f, 0.0f) + Position);
		if(length(CursorPos - PivotPos) < GizmoSize*(12.0f/16.0f))
		{
			m_ClickDiff = CursorPos - PivotPos;
			m_SelectedQuad.SetId2(CAsset_MapLayerQuads::CSubPath::POINT_PIVOT);
			
			View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
			AssetsEditor()->EditAssetSubItem(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CEditor::TAB_MAPLAYERQUADS_QUADS);
				
			m_Token = AssetsManager()->GenerateToken();
			
			return;
		}
		else
		{
			for(int p=3; p>=0; p--)
			{
				vec2 Vertex = pMapLayer->GetVertexPosition(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p));
				vec2 VertexScreenPos = MapRenderer.MapPosToScreenPos(Transform*Vertex + Position);
				if(length(CursorPos - VertexScreenPos) < GizmoSize*(12.0f/16.0f))
				{
					m_ClickDiff = CursorPos - VertexScreenPos;
					m_SelectedQuad.SetId2(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p);
					
					View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
					AssetsEditor()->EditAssetSubItem(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CEditor::TAB_MAPLAYERQUADS_QUADS);
						
					m_Token = AssetsManager()->GenerateToken();
					
					return;
				}
			}
		}
	}
	
	CAsset_MapLayerQuads::CSubPath QuadFound = PickQuad(CursorPos);
		
	if(!QuadFound.IsNull())
	{
		if(m_SelectedQuad != QuadFound)
		{
			m_SelectedQuad = QuadFound;
			m_Token = AssetsManager()->GenerateToken();
		}
		
		View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
		AssetsEditor()->EditAssetSubItem(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CEditor::TAB_MAPLAYERQUADS_QUADS);
	}
	else
	{
		m_SelectedQuad = CAsset_MapLayerQuads::CSubPath::Null();
	}
}

void CView_CursorTool_MapQuads_Edit::OnButtonRelease(int Button)
{
	m_SelectedQuad.SetId2(CAsset_MapLayerQuads::CSubPath::POINT_NONE);
	m_Token = CAssetsHistory::NEW_TOKEN;
}

void CView_CursorTool_MapQuads_Edit::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(CAsset_MapLayerQuads::CSubPath(m_SelectedQuad).IsNull())
		return;
	
	CAsset_MapLayerQuads* pMapLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pMapLayer)
		return;
	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	vec2 MousePos = MapRenderer.ScreenPosToMapPos(vec2(X, Y) - m_ClickDiff);
	
	vec2 Position;
	matrix2x2 Transform;
	pMapLayer->GetQuadTransform(m_SelectedQuad, AssetsEditor()->GetTime(), &Transform, &Position);
	matrix2x2 InvTransform = matrix2x2::inverse(Transform);
	
	if(m_SelectedQuad.GetId2() >= CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0 && m_SelectedQuad.GetId2() <= CAsset_MapLayerQuads::CSubPath::POINT_VERTEX3)
	{
		vec2 NewPos = InvTransform*(MousePos - Position);
		
		AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_POINT_POSITION_X, NewPos.x, m_Token);
		AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_POINT_POSITION_Y, NewPos.y, m_Token);
	}
	else if(m_SelectedQuad.GetId2() == CAsset_MapLayerQuads::CSubPath::POINT_PIVOT)
	{
		vec2 Diff = InvTransform*(MousePos - Position);
		vec2 NewPivot = Diff + pMapLayer->GetQuadPivot(m_SelectedQuad);
		
		for(int p=0; p<4; p++)
		{
			CAsset_MapLayerQuads::CSubPath VertexPath = m_SelectedQuad;
			VertexPath.SetId2(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p);
			
			vec2 OldVertex;
			OldVertex.x = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_MapLayerQuads::QUAD_POINT_POSITION_X, VertexPath, 0.0f);
			OldVertex.y = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_MapLayerQuads::QUAD_POINT_POSITION_Y, VertexPath, 0.0f);
			
			vec2 NewVertex = OldVertex - Diff;
			AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, VertexPath, CAsset_MapLayerQuads::QUAD_POINT_POSITION_X, NewVertex.x, m_Token);
			AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, VertexPath, CAsset_MapLayerQuads::QUAD_POINT_POSITION_Y, NewVertex.y, m_Token);
		}
		
		AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_POSITION_X, NewPivot.x, m_Token);
		AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_POSITION_Y, NewPivot.y, m_Token);
	}
}

void CView_CursorTool_MapQuads_Edit::Render()
{
	CAsset_MapLayerQuads* pMapLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pMapLayer)
		return;
	
	RenderPivots();
	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	float GizmoSize = 16.0f;
	
	if(pMapLayer->IsValidQuad(m_SelectedQuad))
	{
		MapRenderer.RenderQuads_Mesh(pMapLayer->GetQuadPointer(m_SelectedQuad), 1);
		
		vec2 Position;
		matrix2x2 Transform;
		pMapLayer->GetQuadTransform(m_SelectedQuad, AssetsEditor()->GetTime(), &Transform, &Position);
		
		for(int p=0; p<4; p++)
		{
			vec2 VertexPos = pMapLayer->GetVertexPosition(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p));
			vec2 VertexScreenPos = MapRenderer.MapPosToScreenPos(Transform*VertexPos + Position);
			
			TUGraphics(ASSETS_EDITORGUI)->DrawSprite(
				CAssetPath::SpriteUniverse(SPRITE_GIZMO_VERTEX_BG),
				vec2(VertexScreenPos.x, VertexScreenPos.y),
				1.0f, 0.0, 0x0, 1.0f
			);
			
			vec4 Color = pMapLayer->GetVertexColor(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p));
			Color.a = 1.0f;
			
			TUGraphics(ASSETS_EDITORGUI)->DrawSprite(
				CAssetPath::SpriteUniverse(SPRITE_GIZMO_VERTEX_FG),
				vec2(VertexScreenPos.x, VertexScreenPos.y),
				1.0f, 0.0, 0x0, Color
			);
		}
	}
}

/* MAPLAYERQUADS TRANSFORM ********************************************/

	// Buttons
CView_CursorTool_MapQuads_Transform::CFrameTransformToggle::CFrameTransformToggle(CView_CursorTool_MapQuads_Transform* pCursorTool) :
	gui::CIconToggle(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_BONE)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapQuads_Transform::CFrameTransformToggle::OnToggle(bool Toggle)
{
	m_pCursorTool->m_FrameTransform = Toggle;
}

	//CursorTool
CView_CursorTool_MapQuads_Transform::CView_CursorTool_MapQuads_Transform(CView* pView, int CursorToolID) :
	CView_CursorTool_MapQuads_Picker(pView, CursorToolID),
	m_SelectedQuad(CAsset_MapLayerQuads::CSubPath::Null()),
	m_SelectedGizmo(-1),
	m_DragType(DRAGTYPE_NONE),
	m_FrameTransform(false),
	m_Token(CAssetsHistory::NEW_TOKEN)
{
	
}

void CView_CursorTool_MapQuads_Transform::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button != KEY_MOUSE_1)
		return;
	
	CAsset_MapLayerQuads* pMapLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pMapLayer)
		return;
	
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pMapLayer->GetImagePath());
		
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	//Find gizmo
	float GizmoSize = 16.0f;
	vec2 CursorPos = vec2(X, Y);
	
	if(pMapLayer->IsValidQuad(m_SelectedQuad))
	{
		vec2 Position;
		matrix2x2 Transform;
		pMapLayer->GetQuadTransform(m_SelectedQuad, AssetsEditor()->GetTime(), &Transform, &Position);
		
		vec2 BBMin = pMapLayer->GetVertexPosition(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0));
		vec2 BBMax = BBMin;
		
		for(int p=1; p<4; p++)
		{
			vec2 Vertex = pMapLayer->GetVertexPosition(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p));
			geometry::UpdateBoundingBox(Vertex, &BBMin, &BBMax);
		}
		
		vec2 GizmosPos[4];
		GizmosPos[0] = MapRenderer.MapPosToScreenPos(Transform*vec2(BBMin.x, BBMin.y) + Position);
		GizmosPos[1] = MapRenderer.MapPosToScreenPos(Transform*vec2(BBMax.x, BBMin.y) + Position);
		GizmosPos[2] = MapRenderer.MapPosToScreenPos(Transform*vec2(BBMin.x, BBMax.y) + Position);
		GizmosPos[3] = MapRenderer.MapPosToScreenPos(Transform*vec2(BBMax.x, BBMax.y) + Position);
		
		for(int i=0; i<4; i++)
		{
			if(length(CursorPos - GizmosPos[i]) < GizmoSize*(12.0f/16.0f))
			{
				m_Token = AssetsManager()->GenerateToken();
				m_ClickDiff = CursorPos - GizmosPos[i];
				m_SelectedGizmo = i;
				m_DragType = DRAGTYPE_GIZMO;
				View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
				return;
			}
		}
	}
	
	CAsset_MapLayerQuads::CSubPath QuadFound = PickQuad(CursorPos);
	
	if(!QuadFound.IsNull())
	{
		if(m_SelectedQuad != QuadFound)
		{
			m_SelectedQuad = QuadFound;
			
			m_GizmoType = GIZMOTYPE_ROTATION;
			m_Transformed = true;
		}
		else
			m_Transformed = false;
		
		m_Token = AssetsManager()->GenerateToken();
		m_DragType = DRAGTYPE_TRANSLATION;
		View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
		
		AssetsEditor()->EditAssetSubItem(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CEditor::TAB_MAPLAYERQUADS_QUADS);
		View()->RefreshToolBar();
	}
	else
	{
		m_SelectedQuad = CAsset_MapLayerQuads::CSubPath::Null();
		View()->RefreshToolBar();
	}
}

void CView_CursorTool_MapQuads_Transform::OnButtonRelease(int Button)
{
	m_SelectedGizmo = -1;
	m_DragType = DRAGTYPE_NONE;
	m_Token = CAssetsHistory::NEW_TOKEN;
	
	if(!m_Transformed)
	{
		if(m_GizmoType == GIZMOTYPE_ROTATION)
			m_GizmoType = GIZMOTYPE_SCALE;
		else
			m_GizmoType = GIZMOTYPE_ROTATION;
	}
}

void CView_CursorTool_MapQuads_Transform::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	CAsset_MapLayerQuads* pMapLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pMapLayer)
		return;
	
	if(!pMapLayer->IsValidQuad(m_SelectedQuad))
		return;
	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	vec2 Position;
	matrix2x2 Transform;
	pMapLayer->GetQuadTransform(m_SelectedQuad, AssetsEditor()->GetTime(), &Transform, &Position);
	matrix2x2 InvTransform = matrix2x2::inverse(Transform);
	
	if(m_DragType == DRAGTYPE_TRANSLATION)
	{
		if(RelX != 0 || RelY != 0)
		{
			vec2 PivotScreenPos = MapRenderer.MapPosToScreenPos(Position);
			vec2 Diff = InvTransform*(MapRenderer.ScreenPosToMapPos(PivotScreenPos + vec2(RelX, RelY))) - Position;
			vec2 NewPivotPos = Diff + pMapLayer->GetQuadPivot(m_SelectedQuad);
			AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_POSITION_X, NewPivotPos.x, m_Token);
			AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_POSITION_Y, NewPivotPos.y, m_Token);
			m_Transformed = true;
		}
	}
	else if(m_DragType == DRAGTYPE_GIZMO)
	{
		if(RelX != 0 || RelY != 0)
		{
			if(m_SelectedGizmo < 0)
				return;
			
			vec2 BBMin = pMapLayer->GetVertexPosition(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0));
			vec2 BBMax = BBMin;
			
			for(int p=1; p<4; p++)
			{
				vec2 Vertex = pMapLayer->GetVertexPosition(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p));
				geometry::UpdateBoundingBox(Vertex, &BBMin, &BBMax);
			}
			
			vec2 GizmosPos;
			switch(m_SelectedGizmo)
			{
				case 0:
					GizmosPos = Transform * vec2(BBMin.x, BBMin.y) + Position;
					break;
				case 1:
					GizmosPos = Transform * vec2(BBMax.x, BBMin.y) + Position;
					break;
				case 2:
					GizmosPos = Transform * vec2(BBMin.x, BBMax.y) + Position;
					break;
				case 3:
					GizmosPos = Transform * vec2(BBMax.x, BBMax.y) + Position;
					break;
			}
			
			vec2 MousePos = MapRenderer.ScreenPosToMapPos(vec2(X, Y) - m_ClickDiff);
			vec2 PivotPos = Position;
			
			if(m_GizmoType == GIZMOTYPE_ROTATION)
			{
				float Angle = pMapLayer->GetQuadAngle(m_SelectedQuad);
				Angle -= angle(normalize(MousePos - PivotPos), normalize(GizmosPos - PivotPos));
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_ANGLE, Angle, m_Token);
			}
			else
			{
				vec2 Size = pMapLayer->GetQuadSize(m_SelectedQuad);
				
				float Ratio = length(MousePos - PivotPos)/length(GizmosPos - PivotPos);
				Size *= Ratio;
				
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_SIZE_X, Size.x, m_Token);
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, m_SelectedQuad, CAsset_MapLayerQuads::QUAD_SIZE_Y, Size.y, m_Token);
			}
			
			m_Transformed = true;
		}
	}
}

void CView_CursorTool_MapQuads_Transform::Render()
{
	CAsset_MapLayerQuads* pMapLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pMapLayer)
		return;
	
	RenderPivots();
	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	if(!m_SelectedQuad.IsNull())
	{
		MapRenderer.RenderQuads_Mesh(pMapLayer->GetQuadPointer(m_SelectedQuad), 1);
	}
	
	float GizmoSize = 16.0f;
	
	if(pMapLayer->IsValidQuad(m_SelectedQuad))
	{
		vec2 Position;
		matrix2x2 Transform;
		pMapLayer->GetQuadTransform(m_SelectedQuad, AssetsEditor()->GetTime(), &Transform, &Position);
		
		CAssetPath GizmoPath;
		if(m_GizmoType == GIZMOTYPE_ROTATION)
			GizmoPath = CAssetPath::SpriteUniverse(SPRITE_GIZMO_ROTATE);
		else if(m_GizmoType == GIZMOTYPE_SCALE)
			GizmoPath = CAssetPath::SpriteUniverse(SPRITE_GIZMO_SCALE);
		
		vec2 BBMin = pMapLayer->GetVertexPosition(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0));
		vec2 BBMax = BBMin;
		
		for(int p=1; p<4; p++)
		{
			vec2 Vertex = pMapLayer->GetVertexPosition(m_SelectedQuad.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p));
			geometry::UpdateBoundingBox(Vertex, &BBMin, &BBMax);
		}
		
		vec2 Vertex0Pos = MapRenderer.MapPosToScreenPos(Transform * vec2(BBMin.x, BBMin.y) + Position);
		vec2 Vertex1Pos = MapRenderer.MapPosToScreenPos(Transform * vec2(BBMax.x, BBMin.y) + Position);
		vec2 Vertex2Pos = MapRenderer.MapPosToScreenPos(Transform * vec2(BBMin.x, BBMax.y) + Position);
		vec2 Vertex3Pos = MapRenderer.MapPosToScreenPos(Transform * vec2(BBMax.x, BBMax.y) + Position);
		
		float GizmoAngle = angle(vec2(1.0f, 0.0f), Transform * vec2(1.0f, 0.0f));
		
		TUGraphics(ASSETS_EDITORGUI)->DrawSprite(GizmoPath, vec2(Vertex0Pos.x, Vertex0Pos.y), 1.0f, GizmoAngle, 0x0, 1.0f);
		TUGraphics(ASSETS_EDITORGUI)->DrawSprite(GizmoPath, vec2(Vertex1Pos.x, Vertex1Pos.y), 1.0f, GizmoAngle+pi/2.0f, 0x0, 1.0f);
		TUGraphics(ASSETS_EDITORGUI)->DrawSprite(GizmoPath, vec2(Vertex2Pos.x, Vertex2Pos.y), 1.0f, GizmoAngle-pi/2.0f, 0x0, 1.0f);
		TUGraphics(ASSETS_EDITORGUI)->DrawSprite(GizmoPath, vec2(Vertex3Pos.x, Vertex3Pos.y), 1.0f, GizmoAngle+pi, 0x0, 1.0f);
	}
}

void CView_CursorTool_MapQuads_Transform::RefreshToolBar()
{
	if(!m_SelectedQuad.IsNull())
	{
		View()->Toolbar()->AddSeparator();
		View()->Toolbar()->Add(new gui::CLabel(View()->m_pConfig, "Option:"));
		View()->Toolbar()->Add(new CFrameTransformToggle(this));
	}
}

/* MAPLAYERQUADS STAMP ************************************************/

	// Buttons
CView_CursorTool_MapQuads_Stamp::CHFlipButton::CHFlipButton(CView_CursorTool_MapQuads_Stamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_HFLIP)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapQuads_Stamp::CHFlipButton::MouseClickAction()
{
	if(m_pCursorTool->m_Selection.size() <= 0)
		return;

	for(int i=0; i<m_pCursorTool->m_Selection.size(); i++)
	{
		CAsset_MapLayerQuads::CQuad* pQuad = &m_pCursorTool->m_Selection[i];
		pQuad->m_Size.y *= -1;
	}
}

CView_CursorTool_MapQuads_Stamp::CVFlipButton::CVFlipButton(CView_CursorTool_MapQuads_Stamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_VFLIP)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapQuads_Stamp::CVFlipButton::MouseClickAction()
{
	if(m_pCursorTool->m_Selection.size() <= 0)
		return;

	for(int i=0; i<m_pCursorTool->m_Selection.size(); i++)
	{
		CAsset_MapLayerQuads::CQuad* pQuad = &m_pCursorTool->m_Selection[i];
		pQuad->m_Size.x *= -1;
	}
}

CView_CursorTool_MapQuads_Stamp::CRotateCWButton::CRotateCWButton(CView_CursorTool_MapQuads_Stamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_ROTATE_CW)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapQuads_Stamp::CRotateCWButton::MouseClickAction()
{
	if(m_pCursorTool->m_Selection.size() <= 0)
		return;

	for(int i=0; i<m_pCursorTool->m_Selection.size(); i++)
	{
		CAsset_MapLayerQuads::CQuad* pQuad = &m_pCursorTool->m_Selection[i];
		pQuad->m_Angle += pi/4.0f;
	}
}

CView_CursorTool_MapQuads_Stamp::CRotateCCWButton::CRotateCCWButton(CView_CursorTool_MapQuads_Stamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetPath::SpriteUniverse(SPRITE_ICON_ROTATE_CCW)),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapQuads_Stamp::CRotateCCWButton::MouseClickAction()
{
	if(m_pCursorTool->m_Selection.size() <= 0)
		return;

	for(int i=0; i<m_pCursorTool->m_Selection.size(); i++)
	{
		CAsset_MapLayerQuads::CQuad* pQuad = &m_pCursorTool->m_Selection[i];
		pQuad->m_Angle -= pi/4.0f;
	}
}

	// CursorTool
CView_CursorTool_MapQuads_Stamp::CView_CursorTool_MapQuads_Stamp(CView* pView, int CursorToolID) :
	CView_CursorTool_MapStamp(pView, CursorToolID)
{
	
}

void CView_CursorTool_MapQuads_Stamp::OnButtonClick(int X, int Y, int Button, int Count)
{
	CAsset_MapLayerQuads* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pLayer)
		return;
	
	if(Button == KEY_MOUSE_1)
	{
		if(m_Selection.size())
		{
			int Token = AssetsManager()->GenerateToken();
			
			CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
			View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
			vec2 CursorPos = MapRenderer.ScreenPosToMapPos(vec2(X, Y));
			
			for(int i=0; i<m_Selection.size(); i++)
			{
				CAsset_MapLayerQuads::CSubPath NewQuadPath = AssetsManager()->AddSubItem(AssetsEditor()->m_ViewedAssetPath, CAsset_MapLayerQuads::CSubPath::TYPE_QUAD, Token);
				
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, NewQuadPath, CAsset_MapLayerQuads::QUAD_POSITION_X, CursorPos.x, Token);
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, NewQuadPath, CAsset_MapLayerQuads::QUAD_POSITION_Y, CursorPos.y, Token);
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, NewQuadPath, CAsset_MapLayerQuads::QUAD_SIZE_X, m_Selection[i].m_Size.x, Token);
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, NewQuadPath, CAsset_MapLayerQuads::QUAD_SIZE_Y, m_Selection[i].m_Size.y, Token);
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, NewQuadPath, CAsset_MapLayerQuads::QUAD_ANGLE, m_Selection[i].m_Angle, Token);
				AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, NewQuadPath, CAsset_MapLayerQuads::QUAD_ANIMATIONPATH, m_Selection[i].m_AnimationPath, Token);
				
				for(int p=0; p<4; p++)
				{
					CAsset_MapLayerQuads::CSubPath VertexPath = NewQuadPath.Point(CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0+p);
					AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, VertexPath, CAsset_MapLayerQuads::QUAD_POINT_POSITION_X, m_Selection[i].m_Vertices[p].x, Token);
					AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, VertexPath, CAsset_MapLayerQuads::QUAD_POINT_POSITION_Y, m_Selection[i].m_Vertices[p].y, Token);
					AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, VertexPath, CAsset_MapLayerQuads::QUAD_POINT_UV_X, m_Selection[i].m_UVs[p].x, Token);
					AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, VertexPath, CAsset_MapLayerQuads::QUAD_POINT_UV_Y, m_Selection[i].m_UVs[p].y, Token);
					AssetsManager()->SetAssetValue<vec4>(AssetsEditor()->m_ViewedAssetPath, VertexPath, CAsset_MapLayerQuads::QUAD_POINT_COLOR, m_Selection[i].m_Colors[p], Token);
				}
			}
		}
	}
	else if(Button == KEY_MOUSE_2)
	{
		if(m_Selection.size())
		{
			m_Selection.clear();
			View()->RefreshToolBar();
		}
		else if(!m_pPalette)
		{
			m_pPalette = new CPalettePopup(AssetsEditor(), this, View()->GetViewRect());
			AssetsEditor()->DisplayPopup(m_pPalette);
		}
	}
}

void CView_CursorTool_MapQuads_Stamp::OnButtonRelease(int Button)
{
	
}

void CView_CursorTool_MapQuads_Stamp::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	
}

void CView_CursorTool_MapQuads_Stamp::Render()
{
	CAsset_MapLayerQuads* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pLayer)
		return;
	
	CMapRenderer MapRenderer(TUGraphics(), AssetsManager());
	View()->InitMapRenderer_Layer(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	float Time = fmod(AssetsEditor()->Client()->LocalTime(), 1.0f);
	vec4 Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Color.a *= 0.3f + 0.7f*(1.0f+cos(2.0f*pi*Time))/2.0f;
	
	vec2 CursorPos = MapRenderer.ScreenPosToMapPos(AssetsEditor()->GetCursorPos());
	
	if(m_Selection.size())
	{
		vec2 LayerPos = CursorPos;
		
		MapRenderer.RenderQuads(&m_Selection[0], m_Selection.size(), CursorPos, pLayer->GetImagePath(), Color);

		vec2 BBMin = AssetsEditor()->GetScreenSize();
		vec2 BBMax = vec2(0.0f, 0.0f);
		for(int i=0; i<m_Selection.size(); i++)
		{
			vec2 Position;
			matrix2x2 Transform;
			m_Selection[i].GetTransform(AssetsManager(), AssetsEditor()->GetTime(), &Transform, &Position);
			Position += CursorPos;
			
			for(int p=0; p<4; p++)
			{
				vec2 Vertex = MapRenderer.MapPosToScreenPos(Transform*m_Selection[i].m_Vertices[p] + Position);
				geometry::UpdateBoundingBox(Vertex, &BBMin, &BBMax);
			}
		}
		
		vec2 BBSize = BBMax - BBMin;
		
		gui::CRect Rect;
		Rect.x = BBMin.x;
		Rect.y = BBMin.y;
		Rect.w = BBSize.x;
		Rect.h = BBSize.y;
		
		TUGraphics(ASSETS_EDITORGUI)->DrawGuiRect(&Rect, CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, GUIRECTSTYLE_STAMP));
	}
}

void CView_CursorTool_MapQuads_Stamp::OnTexturePaletteSelection(int X0, int Y0, int X1, int Y1)
{
	CAsset_MapLayerQuads* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(AssetsEditor()->m_ViewedAssetPath);
	if(!pLayer)
		return;
		
	int W = X1-X0;
	int H = Y1-Y0;
	
	int GridWidth = 1;
	int GridHeight = 1;
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pLayer->GetImagePath());	
	if(pImage)
	{
		GridWidth = pImage->GetGridWidth();
		GridHeight = pImage->GetGridHeight();
	}
	
	vec2 UVMin = vec2(static_cast<float>(X0)/GridWidth, static_cast<float>(Y0)/GridHeight);
	vec2 UVMax = vec2(static_cast<float>(X1)/GridWidth, static_cast<float>(Y1)/GridHeight);
	
	CAsset_MapLayerQuads::CQuad Quad;
	Quad.m_Pivot = vec2(0.0f, 0.0f);
	
	Quad.m_Vertices[0] = vec2(-(W*32.0f)/2.0f, -(H*32.0f)/2.0f);
	Quad.m_Vertices[1] = vec2((W*32.0f)/2.0f, -(H*32.0f)/2.0f);
	Quad.m_Vertices[2] = vec2(-(W*32.0f)/2.0f, (H*32.0f)/2.0f);
	Quad.m_Vertices[3] = vec2((W*32.0f)/2.0f, (H*32.0f)/2.0f);
	
	Quad.m_UVs[0] = vec2(UVMin.x, UVMin.y);
	Quad.m_UVs[1] = vec2(UVMax.x, UVMin.y);
	Quad.m_UVs[2] = vec2(UVMin.x, UVMax.y);
	Quad.m_UVs[3] = vec2(UVMax.x, UVMax.y);
	
	m_Selection.clear();
	m_Selection.add(Quad);
	
	View()->RefreshToolBar();
}

void CView_CursorTool_MapQuads_Stamp::RefreshToolBar()
{
	if(m_Selection.size())
	{
		View()->Toolbar()->AddSeparator();
		View()->Toolbar()->Add(new gui::CLabel(View()->m_pConfig, "Selection:"));
		View()->Toolbar()->Add(new CHFlipButton(this));
		View()->Toolbar()->Add(new CVFlipButton(this));
		View()->Toolbar()->Add(new CRotateCWButton(this));
		View()->Toolbar()->Add(new CRotateCCWButton(this));
	}
}

}

}
