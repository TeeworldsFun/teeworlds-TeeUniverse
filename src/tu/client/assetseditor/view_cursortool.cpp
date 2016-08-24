#include <engine/client.h>
#include <tu/client/assetseditor/view.h>
#include <tu/client/skeletonrenderer.h>

#include "view_cursortool.h"

namespace tu
{

namespace assetseditor
{
	
CView_CursorTool::CButtonCore::CButtonCore(CView* pView, int Icon, int CursorToolID) :
	gui::CIconButton(pView->m_pConfig, Icon),
	m_pView(pView),
	m_CursorToolID(CursorToolID)
{
	
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
		ShowHint(CView::s_CursorToolHints[m_CursorToolID]);
	
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
CAssetsManager* CView_CursorTool::AssetsManager() { return m_pView->AssetsManager(); }
CAssetsEditor* CView_CursorTool::AssetsEditor() { return m_pView->AssetsEditor(); }
CView* CView_CursorTool::View() { return m_pView; }

/* BONES **************************************************************/

CView_CursorTool_Bone_Picking::CView_CursorTool_Bone_Picking(CView* pView, int CursorToolID) :
	CView_CursorTool(pView, CursorToolID)
{
	
}
	
void CView_CursorTool_Bone_Picking::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1)
	{
		if(AssetsEditor()->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETON)
		{
			CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
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
				m_SelectedSubItem = BonePath.ConvertToInteger();
				
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
			
			CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
			SkeletonRenderer.AddSkeletonWithParents(pSkeletonAnimation->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
			SkeletonRenderer.ApplyAnimation(AssetsEditor()->m_ViewedAssetPath, Time);
			SkeletonRenderer.Finalize();
			
			CAssetPath SkeletonPath;
			CAsset_Skeleton::CSubPath BonePath;
			if(SkeletonRenderer.BonePicking(View()->GetViewPos(), View()->GetViewZoom(), vec2(X, Y), SkeletonPath, BonePath))
			{
				CAsset_SkeletonAnimation::CSubPath FramePath;
				
				if(SkeletonPath == pSkeletonAnimation->m_SkeletonPath)
					FramePath = pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Local(BonePath.GetId()), Frame);
				
				if(FramePath.IsNull())
					return;
				
				View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
				m_SelectedAsset = AssetsEditor()->m_ViewedAssetPath;
				m_SelectedSqueleton = SkeletonPath;
				m_SelectedBone = BonePath;
				m_SelectedSubItem = FramePath.ConvertToInteger();
				
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
	CView_CursorTool_Bone_Picking(pView, CursorToolID),
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
	
	CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
	
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
			
			CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
			SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
			SkeletonRenderer.Finalize();
			
			CAssetPath SkeletonPath;
			CAsset_Skeleton::CSubPath BonePath;
			if(SkeletonRenderer.BonePicking(View()->GetViewPos(), View()->GetViewZoom(), vec2(X, Y), SkeletonPath, BonePath))
			{
				if(SkeletonPath == AssetsEditor()->m_ViewedAssetPath || SkeletonPath == pSkeleton->m_ParentPath)
				{
					CAsset_Skeleton::CSubPath NewBonePath = AssetsManager()->AddSubItem(AssetsEditor()->m_ViewedAssetPath, CAsset_Skeleton::SUBITEM_BONE);
					if(!NewBonePath.IsNull())
					{
						m_Pivot = vec2(X, Y);
						m_CursorLastPos = vec2(X, Y);
						
						View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
						
						m_SelectedSqueleton = AssetsEditor()->m_ViewedAssetPath;
						m_SelectedBone = NewBonePath;
						
						CAsset_Skeleton::CBonePath ParentPath;
						if(SkeletonPath == pSkeleton->m_ParentPath)
							ParentPath = CAsset_Skeleton::CBonePath::Parent(BonePath.GetId());
						else
							ParentPath = CAsset_Skeleton::CBonePath::Local(BonePath.GetId());
						
						AssetsManager()->SetAssetValue<CAsset_Skeleton::CBonePath>(
							m_SelectedSqueleton,
							CAsset_Skeleton::BONE_PARENT,
							m_SelectedBone.ConvertToInteger(),
							ParentPath);
						
						AssetsEditor()->RefreshAssetsEditor();
					}
				}
			}
			else
			{
				CAsset_Skeleton::CSubPath NewBonePath = AssetsManager()->AddSubItem(AssetsEditor()->m_ViewedAssetPath, CAsset_Skeleton::SUBITEM_BONE);
				if(!NewBonePath.IsNull())
				{
					m_Pivot = vec2(X, Y);
					m_CursorLastPos = vec2(X, Y);
					
					View()->SetDragType(CView::DRAGTYPE_CURSORTOOL);
					
					m_SelectedSqueleton = AssetsEditor()->m_ViewedAssetPath;
					m_SelectedBone = NewBonePath;
					
					CAsset_Skeleton::CBonePath ParentPath;
					if(SkeletonPath == pSkeleton->m_ParentPath)
						ParentPath = CAsset_Skeleton::CBonePath::Parent(BonePath.GetId());
					else
						ParentPath = CAsset_Skeleton::CBonePath::Local(BonePath.GetId());
					
					AssetsManager()->SetAssetValue<CAsset_Skeleton::CBonePath>(
						m_SelectedSqueleton,
						CAsset_Skeleton::BONE_PARENT,
						CAsset_Skeleton::CBonePath::Null().ConvertToInteger(),
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
		CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
		SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
		SkeletonRenderer.Finalize();
		
		vec2 Origin, AxisX, AxisY;
		if(SkeletonRenderer.GetLocalAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, Origin, AxisX, AxisY))
		{
			vec2 RelPos = (CursorPos - Origin);
			float LengthX = dot(AxisX, RelPos);
			float LengthY = dot(AxisY, RelPos);
			float Length = sqrt(LengthX*LengthX + LengthY*LengthY);
			AssetsManager()->SetAssetValue<float>(m_SelectedSqueleton, CAsset_Skeleton::BONE_LENGTH, m_SelectedBone.ConvertToInteger(), Length);
			float Angle = angle(AxisX, RelPos);
			AssetsManager()->SetAssetValue<float>(m_SelectedSqueleton, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
		}
		vec2 OriginParent, AxisXParent, AxisYParent;
		if(SkeletonRenderer.GetParentAxis(View()->GetViewPos(), View()->GetViewZoom(), m_SelectedSqueleton, m_SelectedBone, OriginParent, AxisXParent, AxisYParent))
		{
			float Angle = angle(AxisXParent, m_CursorLastPos - Origin);
			AssetsManager()->SetAssetValue<float>(m_SelectedSqueleton, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
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
			CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
			SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
			SkeletonRenderer.Finalize();
			
			CAssetPath SkeletonPath;
			CAsset_Skeleton::CSubPath BonePath;
			if(SkeletonRenderer.BonePicking(View()->GetViewPos(), View()->GetViewZoom(), vec2(X, Y), SkeletonPath, BonePath) && SkeletonPath == AssetsEditor()->m_ViewedAssetPath)
			{
				if(SkeletonPath == AssetsEditor()->m_ViewedAssetPath)
				{
					AssetsManager()->DeleteSubItem(AssetsEditor()->m_ViewedAssetPath, BonePath.ConvertToInteger());
					AssetsEditor()->RefreshAssetsEditor();
				}
			}
		}
	}
}

/* MAPLAYERTILES SELECTIONSTAMP ***************************************/

	//Palette
CView_CursorTool_MapLayerTiles_SelectionStamp::CPalettePopup::CTilePalette::CTilePalette(gui::CConfig* pConfig, CPalettePopup* pPopup, CAssetPath ImagePath) :
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
		
void CView_CursorTool_MapLayerTiles_SelectionStamp::CPalettePopup::CTilePalette::Render()
{
	CAsset_Image* pImage = m_pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(!pImage)
		return;
		
	float ImgRatio = static_cast<float>(pImage->m_Width)/static_cast<float>(pImage->m_Height);
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
	float xStep = SizeX / static_cast<float>(max(1, pImage->m_GridWidth));
	float yStep = SizeY / static_cast<float>(max(1, pImage->m_GridHeight));
	
	//Draw image
	m_pPopup->m_pAssetsEditor->TUGraphics()->TextureSet(m_ImagePath);
	
	Graphics()->QuadsBegin();
	IGraphics::CQuadItem QuadItem(x0, y0, SizeX, SizeY);
	Graphics()->QuadsDrawTL(&QuadItem, 1);
	Graphics()->QuadsEnd();
	
	//Draw selection
	if(m_Clicked)
	{
		Graphics()->TextureClear();
		Graphics()->QuadsBegin();
		
		Graphics()->SetColor(vec4(1.0f, 0.0f, 0.0f, 0.5f), true);
		
		int X = min(m_FirstX, m_CurrX);
		int Y = min(m_FirstY, m_CurrY);
		int W = max(m_FirstX, m_CurrX) - X + 1;
		int H = max(m_FirstY, m_CurrY) - Y + 1;
		
		IGraphics::CQuadItem QuadItem(x0 + X * xStep, y0 + Y * yStep, W * xStep, H * yStep);
		Graphics()->QuadsDrawTL(&QuadItem, 1);
	
		Graphics()->QuadsEnd();
	}
	
	//Draw grid
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(vec4(0.0f, 0.0f, 0.0f, 0.5f), true);
	
	for(int i=0; i<=pImage->m_GridWidth; i++)
	{
		float x = x0 + i * xStep;
		IGraphics::CLineItem Line(x, y0, x, y1);
		Graphics()->LinesDraw(&Line, 1);
	}
	for(int i=0; i<=pImage->m_GridHeight; i++)
	{
		float y = y0 + i * yStep;
		IGraphics::CLineItem Line(x0, y, x1, y);
		Graphics()->LinesDraw(&Line, 1);
	}
	
	Graphics()->LinesEnd();
	
	//Update image size
	m_ImageRect.x = x0;
	m_ImageRect.y = y0;
	m_ImageRect.w = SizeX;
	m_ImageRect.h = SizeY;
}
		
void CView_CursorTool_MapLayerTiles_SelectionStamp::CPalettePopup::CTilePalette::OnButtonClick(int X, int Y, int Button, int Count)
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
		m_FirstX = (pImage->m_GridWidth*(X - m_ImageRect.x))/(m_ImageRect.w);
		m_FirstY = (pImage->m_GridHeight*(Y - m_ImageRect.y))/(m_ImageRect.h);
		m_CurrX = m_FirstX;
		m_CurrY = m_FirstY;
		
		m_Clicked = true;
	}
}
		
void CView_CursorTool_MapLayerTiles_SelectionStamp::CPalettePopup::CTilePalette::OnButtonRelease(int Button)
{
	if(Button != KEY_MOUSE_1)
		return;
		
	m_Clicked = false;
	
	CAsset_Image* pImage = m_pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(!pImage)
		return;
	
	int X = min(m_FirstX, m_CurrX);
	int Y = min(m_FirstY, m_CurrY);
	int W = max(m_FirstX, m_CurrX) - X + 1;
	int H = max(m_FirstY, m_CurrY) - Y + 1;
	
	int Index = Y*pImage->m_GridWidth + X;
		
	m_pPopup->SelectTile(Index, W, H);
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::CPalettePopup::CTilePalette::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(!m_Clicked)
		return;
	
	CAsset_Image* pImage = m_pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(!pImage)
		return;
	
	if(m_ImageRect.IsInside(X, Y))
	{
		m_CurrX = (pImage->m_GridWidth*(X - m_ImageRect.x))/(m_ImageRect.w);
		m_CurrY = (pImage->m_GridHeight*(Y - m_ImageRect.y))/(m_ImageRect.h);
	}
}

CView_CursorTool_MapLayerTiles_SelectionStamp::CPalettePopup::CPalettePopup(
	CAssetsEditor* pAssetsEditor,
	CView_CursorTool_MapLayerTiles_SelectionStamp* pCursorTool,
	const gui::CRect& CreatorRect
) :
	gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, -1, -1, gui::CPopup::ALIGNMENT_INNER),
	m_pAssetsEditor(pAssetsEditor),
	m_pCursorTool(pCursorTool)
{
	if(m_pAssetsEditor->m_EditedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
	{
		CAsset_MapLayerTiles* pLayer = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapLayerTiles>(m_pAssetsEditor->m_EditedAssetPath);
		if(pLayer)
		{
			Add(new CTilePalette(m_pConfig, this, pLayer->m_ImagePath));
		}
	}
	
	Update();
}
	
void CView_CursorTool_MapLayerTiles_SelectionStamp::CPalettePopup::Close()
{
	m_pCursorTool->OnPaletteCloseCallback();
	
	gui::CPopup::Close();
}
	
void CView_CursorTool_MapLayerTiles_SelectionStamp::CPalettePopup::SelectTile(int Index, int W, int H)
{
	m_pCursorTool->OnPaletteSelection_Tile(Index, W, H);
	Close();
}
	// Buttons

CView_CursorTool_MapLayerTiles_SelectionStamp::CHFlipButton::CHFlipButton(CView_CursorTool_MapLayerTiles_SelectionStamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetsEditor::ICON_HFLIP),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::CHFlipButton::MouseClickAction()
{
	if(m_pCursorTool->AssetsEditor()->m_EditedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
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
}

CView_CursorTool_MapLayerTiles_SelectionStamp::CVFlipButton::CVFlipButton(CView_CursorTool_MapLayerTiles_SelectionStamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetsEditor::ICON_VFLIP),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::CVFlipButton::MouseClickAction()
{
	if(m_pCursorTool->AssetsEditor()->m_EditedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
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
}

CView_CursorTool_MapLayerTiles_SelectionStamp::CRotateCWButton::CRotateCWButton(CView_CursorTool_MapLayerTiles_SelectionStamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetsEditor::ICON_ROTATE_CW),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::CRotateCWButton::MouseClickAction()
{
	if(m_pCursorTool->AssetsEditor()->m_EditedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
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
					
					if(pNewSelection[Index1].m_Flags&TILEFLAG_ROTATE)
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
}

CView_CursorTool_MapLayerTiles_SelectionStamp::CRotateCCWButton::CRotateCCWButton(CView_CursorTool_MapLayerTiles_SelectionStamp* pCursorTool) :
	gui::CIconButton(pCursorTool->View()->m_pConfig, CAssetsEditor::ICON_ROTATE_CCW),
	m_pCursorTool(pCursorTool)
{
	
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::CRotateCCWButton::MouseClickAction()
{
	if(m_pCursorTool->AssetsEditor()->m_EditedAssetPath.GetType() == CAsset_MapLayerTiles::TypeId)
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
}

	// CursorTool
	
CView_CursorTool_MapLayerTiles_SelectionStamp::CView_CursorTool_MapLayerTiles_SelectionStamp(CView* pView, int CursorToolID) :
	CView_CursorTool(pView, CursorToolID),
	m_pSelection(0),
	m_pPalette(0)
{
	
}

CView_CursorTool_MapLayerTiles_SelectionStamp::~CView_CursorTool_MapLayerTiles_SelectionStamp()
{
	if(m_pSelection)
		delete[] m_pSelection;
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(Button == KEY_MOUSE_1)
	{
		if(!m_pSelection)
			return;
		
		CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_ViewedAssetPath);
		if(!pLayer)
			return;
			
		CMapRenderer MapRenderer(Graphics(), AssetsManager());
		View()->InitMapRenderer_LayerTiles(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
		
		m_Pivot = MapRenderer.ScreenPosToMapPos(vec2(X, Y));
		
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

void CView_CursorTool_MapLayerTiles_SelectionStamp::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{	
	CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_ViewedAssetPath);
	if(!pLayer)
		return;
	
	CMapRenderer MapRenderer(Graphics(), AssetsManager());
	View()->InitMapRenderer_LayerTiles(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	vec2 CursorMapPos = MapRenderer.ScreenPosToMapPos(vec2(X, Y));
	
	if(m_pSelection)
	{
		int TileX = CursorMapPos.x/32.0f;
		int TileY = CursorMapPos.y/32.0f;
		
		for(int j=0; j<m_SelectionHeight; j++)
		{
			for(int i=0; i<m_SelectionWidth; i++)
			{
				CAsset_MapLayerTiles::CTile* pTile = pLayer->GetTile(TileX+i, TileY+j);
				if(pTile)
				{
					pTile->m_Index = m_pSelection[j*m_SelectionWidth+i].m_Index;
					pTile->m_Flags = m_pSelection[j*m_SelectionWidth+i].m_Flags;
					pTile->m_Index = m_pSelection[j*m_SelectionWidth+i].m_Index;
				}
			}
		}
	}
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::Render()
{	
	if(!m_pSelection)
		return;
	
	CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_ViewedAssetPath);
	if(!pLayer)
		return;
	
	CMapRenderer MapRenderer(Graphics(), AssetsManager());
	View()->InitMapRenderer_LayerTiles(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
	
	vec2 CursorMapPos = MapRenderer.ScreenPosToMapPos(AssetsEditor()->GetCursorPos());
	CursorMapPos.x = static_cast<int>(CursorMapPos.x/32.0f)*32.0f;
	CursorMapPos.y = static_cast<int>(CursorMapPos.y/32.0f)*32.0f;
	
	vec4 Color = pLayer->m_Color;
	float Time = fmod(AssetsEditor()->Client()->LocalTime(), 1.0f);
	Color.a *= 0.3f + 0.7f*(1.0f+cos(2.0f*pi*Time))/2.0f;
	
	MapRenderer.RenderTiles(m_pSelection, m_SelectionWidth, m_SelectionHeight, CursorMapPos, pLayer->m_ImagePath, Color);
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::RefreshToolBar()
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

void CView_CursorTool_MapLayerTiles_SelectionStamp::OnPaletteCloseCallback()
{
	m_pPalette = 0;
}

void CView_CursorTool_MapLayerTiles_SelectionStamp::OnPaletteSelection_Tile(int Index, int W, int H)
{
	CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_EditedAssetPath);
	if(!pLayer)
		return;
	
	int GridWidth = 1;
	int GridHeight = 1;
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pLayer->m_ImagePath);	
	if(pImage)
	{
		GridWidth = pImage->m_GridWidth;
		GridHeight = pImage->m_GridHeight;
	}
	
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

}

}
