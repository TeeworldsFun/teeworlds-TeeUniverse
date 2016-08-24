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
#include <tu/client/skeletonrenderer.h>
#include <tu/client/maprenderer.h>

#include <cstddef>

#include "view.h"

namespace tu
{

namespace assetseditor
{
		
const char* CView::s_CursorToolHints[] = {
	"View: move the view (not implemented)",
	"Translation: move an object",
	"Horizontal Translation: move an object along his local X axis",
	"Vertical Translation: move an object along his local Y axis",
	"Rotation: rotate an object",
	"Scale: scale an object",
	"Horizontal Scale: scale an object along his local X axis",
	"Vertical Scale: scale an object along his local Y axis",
	"Bone Length: edit the length of a bone",
	"Bone Creator: create a new bone from an existing one",
	"Bone Eraser: delete a bone",
	"Sprite Creator: create sprite from an image",
};

const char* CView::s_GizmoHints[] = {
	"Aim Direction: set the aim direction",
	"Motion Direction: set the motion direction",
	"Hook Direction: set the hook direction",
};

const char* CView::s_HintText[] = {
	"Show/Hide skeleton bones",
};

CView::CView(CAssetsEditor* pAssetsEditor) :
	gui::CWidget(pAssetsEditor->m_pGuiConfig),
	m_pAssetsEditor(pAssetsEditor),
	m_StartPointPos(-128.0f, 0.0f),
	m_EndPointPos(128.0f, 0.0f),
	m_ToolbarHeight(30),
	m_pToolbar(0),
	m_pZoomSlider(0),
	m_LastEditedAssetType(-1),
	m_pCursorTool(0),
	m_ShowSkeleton(true),
	m_ViewType(VIEWTYPE_OBJECT),
	m_ObjectZoom(1.5f),
	m_MapPos(vec2(0.0f, 0.0f)),
	m_MapZoom(1.5f)
{	
	m_pToolbar = new gui::CHListLayout(m_pConfig);
	RefreshToolBar();
	
	for(int i=0; i<NUM_GIZMOS; i++)
	{
		m_GizmoPos[i] = vec2(1.0f, 0.0f);
		m_GizmoEnabled[i] = 0;
	}
}
	
CView::~CView()
{
	if(m_pToolbar) delete m_pToolbar;
}

void CView::RefreshToolBar()
{
	m_pToolbar->Clear();
	
	gui::CLabel* pZoomLabel = new gui::CLabel(m_pConfig, "Zoom:");
	pZoomLabel->SetRect(gui::CRect(
		0, 0, //Positions will be filled when the toolbar is updated
		90,
		pZoomLabel->m_Rect.h
	));
	
	m_pToolbar->Add(pZoomLabel);
	
	m_pZoomSlider = new CView::CZoomSlider(this);
	m_pToolbar->Add(m_pZoomSlider);
	
	//~ m_CursorToolButtons[CURSORTOOL_MOVE] = new CView::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_MOVE, CURSORTOOL_MOVE);
	//~ m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_MOVE]);
	
	switch(AssetsEditor()->m_ViewedAssetPath.GetType())
	{
		case CAssetPath::TYPE_IMAGE:
			//~ m_CursorToolButtons[CURSORTOOL_SPRITE_CREATOR] = new CView::CCursorToolButton(this, CAssetsEditor::ICON_SPRITE, CURSORTOOL_SPRITE_CREATOR);
			//~ m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_SPRITE_CREATOR]);
			break;
		case CAssetPath::TYPE_SKELETON:
		{
			m_pToolbar->AddSeparator();
			m_pToolbar->Add(new gui::CLabel(m_pConfig, "Show:"));
			m_pToolbar->Add(new CViewSwitch(this, CAssetsEditor::ICON_SKELETON, &m_ShowSkeleton, s_HintText[HINT_SHOW_SKELETON]));
			
			m_pToolbar->AddSeparator();
			m_pToolbar->Add(new gui::CLabel(m_pConfig, "Tools:"));
			
			m_pToolbar->Add(new CView_CursorTool_Bone_TranslateXY::CButton(this, CURSORTOOL_TRANSLATE));
			m_pToolbar->Add(new CView_CursorTool_Bone_TranslateX::CButton(this, CURSORTOOL_TRANSLATE_X));
			m_pToolbar->Add(new CView_CursorTool_Bone_TranslateY::CButton(this, CURSORTOOL_TRANSLATE_Y));
			m_pToolbar->Add(new CView_CursorTool_Bone_Rotate::CButton(this, CURSORTOOL_ROTATE));
			m_pToolbar->Add(new CView_CursorTool_Bone_ScaleXY::CButton(this, CURSORTOOL_SCALE));
			m_pToolbar->Add(new CView_CursorTool_Bone_ScaleX::CButton(this, CURSORTOOL_SCALE_X));
			m_pToolbar->Add(new CView_CursorTool_Bone_ScaleY::CButton(this, CURSORTOOL_SCALE_Y));
			m_pToolbar->Add(new CView_CursorTool_Bone_Length::CButton(this, CURSORTOOL_BONE_LENGTH));
			m_pToolbar->Add(new CView_CursorTool_Bone_Add::CButton(this, CURSORTOOL_BONE_ADD));
			m_pToolbar->Add(new CView_CursorTool_Bone_Delete::CButton(this, CURSORTOOL_BONE_DELETE));
			
			break;
		}
		case CAssetPath::TYPE_SKELETONANIMATION:
		{
			m_pToolbar->AddSeparator();
			m_pToolbar->Add(new gui::CLabel(m_pConfig, "Show:"));
			m_pToolbar->Add(new CViewSwitch(this, CAssetsEditor::ICON_SKELETON, &m_ShowSkeleton, s_HintText[HINT_SHOW_SKELETON]));
			
			m_pToolbar->AddSeparator();
			m_pToolbar->Add(new gui::CLabel(m_pConfig, "Tools:"));
			
			m_pToolbar->Add(new CView_CursorTool_Bone_TranslateXY::CButton(this, CURSORTOOL_TRANSLATE));
			m_pToolbar->Add(new CView_CursorTool_Bone_TranslateX::CButton(this, CURSORTOOL_TRANSLATE_X));
			m_pToolbar->Add(new CView_CursorTool_Bone_TranslateY::CButton(this, CURSORTOOL_TRANSLATE_Y));
			m_pToolbar->Add(new CView_CursorTool_Bone_Rotate::CButton(this, CURSORTOOL_ROTATE));
			m_pToolbar->Add(new CView_CursorTool_Bone_ScaleXY::CButton(this, CURSORTOOL_SCALE));
			m_pToolbar->Add(new CView_CursorTool_Bone_ScaleX::CButton(this, CURSORTOOL_SCALE_X));
			m_pToolbar->Add(new CView_CursorTool_Bone_ScaleY::CButton(this, CURSORTOOL_SCALE_Y));
			break;
		}
		case CAssetPath::TYPE_SKELETONSKIN:
		{
			m_pToolbar->AddSeparator();
			m_pToolbar->Add(new gui::CLabel(m_pConfig, "Show:"));
			m_pToolbar->Add(new CViewSwitch(this, CAssetsEditor::ICON_SKELETON, &m_ShowSkeleton, s_HintText[HINT_SHOW_SKELETON]));
		}
		case CAssetPath::TYPE_MAPLAYERTILES:
		{
			m_pToolbar->AddSeparator();
			m_pToolbar->Add(new gui::CLabel(m_pConfig, "Tools:"));
			
			m_pToolbar->Add(new CView_CursorTool_MapLayerTiles_SelectionStamp::CButton(this, CURSORTOOL_SELECTIONSTAMP));
			break;
		}
	}
	
	if(m_pCursorTool)
		m_pCursorTool->RefreshToolBar();
	
	m_pToolbar->Update();
}

vec2 CView::GetViewCenter()
{
	vec2 Pos;
	Pos.x = m_ViewRect.x + m_ViewRect.w/2;
	Pos.y = m_ViewRect.y + m_ViewRect.h/2;
	return Pos;
}

vec2 CView::GetViewPos()
{
	switch(m_ViewType)
	{
		case VIEWTYPE_MAP:
		{
			return m_MapPos;
		}
	}
	
	return GetViewCenter();
}

float CView::GetViewZoom()
{
	switch(m_ViewType)
	{
		case VIEWTYPE_MAP:
		{
			return m_MapZoom;
		}
	}
	
	return m_ObjectZoom;
}

vec2 CView::GetAimPos()
{
	float AimDist = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f) - 48.0f;
	vec2 AimPos = GetViewPos() + m_AimDir * AimDist - vec2(16.0f, 16.0f);
	
	return AimPos;
}

vec2 CView::GetMotionPos()
{
	float MotionDist = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f);
	vec2 MotionPos = GetViewPos() + m_MotionDir * MotionDist - vec2(16.0f, 16.0f);
	
	return MotionPos;
}

void CView::InitMapRenderer_LayerTiles(CMapRenderer* pMapRenderer, CAssetPath LayerPath)
{
	pMapRenderer->SetCanvas(GetViewCenter());
	pMapRenderer->SetCamera(GetViewPos(), GetViewZoom());
	
	//Find the group of this layer
	CAssetPath ParentGroupPath;
	{
		int Source = CAssetPath::SRC_LAND;
		CAsset_MapGroup* pParentGroup = 0;

		for(int g=0; g<AssetsManager()->GetNumAssets<CAsset_MapGroup>(Source); g++)
		{
			CAssetPath GroupPath = CAssetPath::Asset(CAsset_MapGroup::TypeId, Source, g);
			CAsset_MapGroup* pMapGroup = AssetsManager()->GetAsset<CAsset_MapGroup>(GroupPath);
			if(!pMapGroup)
				continue;
			
			for(int i=0; i<pMapGroup->m_Layers.size(); i++)
			{
				if(pMapGroup->m_Layers[i] == LayerPath)
				{
					pMapRenderer->SetGroup(ParentGroupPath);
					pParentGroup = pMapGroup;
					break;
				}
			}
			
			if(pParentGroup)
				break;
		}
	}
}

void CView::Update()
{
	m_pToolbar->SetRect(gui::CRect(
		m_Rect.x,
		m_Rect.y,
		m_Rect.w,
		m_ToolbarHeight
	));
	
	m_ViewRect.x = m_Rect.x;
	m_ViewRect.y = m_pToolbar->m_Rect.y + m_pToolbar->m_Rect.h + m_pConfig->m_LayoutMargin;
	m_ViewRect.w = m_Rect.w;
	m_ViewRect.h = m_Rect.h - m_pToolbar->m_Rect.h - m_pConfig->m_LayoutMargin;
	
	m_pToolbar->Update();
}

void CView::RenderImage()
{
	m_ViewType = VIEWTYPE_OBJECT;
	
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(AssetsEditor()->m_ViewedAssetPath);
	if(!pImage)
		return;
		
	float ImgRatio = static_cast<float>(pImage->m_Width)/static_cast<float>(pImage->m_Height);
	float WindowRatio = static_cast<float>(m_ViewRect.w)/static_cast<float>(m_ViewRect.h);
	float SizeX;
	float SizeY;
	
	if(ImgRatio > WindowRatio)
	{
		SizeX = m_ViewRect.w;
		SizeY = m_ViewRect.w/ImgRatio;
	}
	else
	{
		SizeX = m_ViewRect.h*ImgRatio;
		SizeY = m_ViewRect.h;
	}
	
	float x0 = m_ViewRect.x + m_ViewRect.w/2 - SizeX/2;
	float y0 = m_ViewRect.y + m_ViewRect.h/2 - SizeY/2;
	float x1 = x0 + SizeX;
	float y1 = y0 + SizeY;
	float xStep = SizeX / static_cast<float>(max(1, pImage->m_GridWidth));
	float yStep = SizeY / static_cast<float>(max(1, pImage->m_GridHeight));
	
	//Draw sprites
	Graphics()->TextureClear();
	Graphics()->QuadsBegin();
	float alpha = 0.75f;
	for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Sprite>(CAssetPath::SRC_UNIVERSE); i++)
	{
		CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(CAssetPath::Universe(CAssetPath::TYPE_SPRITE, i));
		if(pSprite->m_ImagePath == AssetsEditor()->m_ViewedAssetPath)
		{
			if(AssetsEditor()->IsEditedAsset(CAssetPath::Universe(CAssetPath::TYPE_SPRITE, i)))
				Graphics()->SetColor(alpha, alpha*0.5f, alpha*0.5f, alpha);
			else
				Graphics()->SetColor(alpha, alpha, alpha*0.5f, alpha);
				
			IGraphics::CQuadItem QuadItem(x0 + xStep * pSprite->m_X, y0 + yStep * pSprite->m_Y, xStep * pSprite->m_Width, yStep * pSprite->m_Height);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
		}
	}
	for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Sprite>(CAssetPath::SRC_WORLD); i++)
	{
		CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(CAssetPath::World(CAssetPath::TYPE_SPRITE, i));
		if(pSprite->m_ImagePath == AssetsEditor()->m_ViewedAssetPath)
		{
			if(AssetsEditor()->IsEditedAsset(CAssetPath::World(CAssetPath::TYPE_SPRITE, i)))
				Graphics()->SetColor(alpha, alpha*0.5f, alpha*0.5f, alpha);
			else
				Graphics()->SetColor(alpha, alpha, alpha*0.5f, alpha);
				
			IGraphics::CQuadItem QuadItem(x0 + xStep * pSprite->m_X, y0 + yStep * pSprite->m_Y, xStep * pSprite->m_Width, yStep * pSprite->m_Height);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
		}
	}
	Graphics()->QuadsEnd();
	
	//Draw image
	AssetsEditor()->TUGraphics()->TextureSet(AssetsEditor()->m_ViewedAssetPath);
	
	Graphics()->QuadsBegin();
	IGraphics::CQuadItem QuadItem(x0, y0, SizeX, SizeY);
	Graphics()->QuadsDrawTL(&QuadItem, 1);
	Graphics()->QuadsEnd();
	
	//Draw grid
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
	
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
}

void CView::RenderSprite()
{
	m_ViewType = VIEWTYPE_OBJECT;
	
	CAsset_Sprite* pSprite = AssetsManager()->GetAsset<CAsset_Sprite>(AssetsEditor()->m_ViewedAssetPath);
	if(!pSprite)
		return;
	
	CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(pSprite->m_ImagePath);
	if(!pImage)
		return;
	
	float SpriteWidthPix = (pImage->m_Width / pImage->m_GridWidth) * pSprite->m_Width;
	float SpriteHeightPix = (pImage->m_Height / pImage->m_GridHeight) * pSprite->m_Height;
	float SpriteScaling = sqrtf(SpriteWidthPix*SpriteWidthPix + SpriteHeightPix*SpriteHeightPix);
	vec2 Pos;
	Pos.x = m_ViewRect.x + m_ViewRect.w/2;
	Pos.y = m_ViewRect.y + m_ViewRect.h/2;
	
	AssetsEditor()->TUGraphics()->DrawSprite(AssetsEditor()->m_ViewedAssetPath, Pos, SpriteScaling, 0.0f, 0x0);
}

void CView::RenderSkeleton()
{
	m_ViewType = VIEWTYPE_OBJECT;
	
	CAsset_Skeleton* pSkeleton = AssetsManager()->GetAsset<CAsset_Skeleton>(AssetsEditor()->m_ViewedAssetPath);
	if(!pSkeleton)
		return;
		
	CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
	SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
	SkeletonRenderer.Finalize();
	SkeletonRenderer.RenderSkins(GetViewPos(), GetViewZoom());
	
	if(m_ShowSkeleton)
	{
		CAsset_Skeleton* pParentSkeleton = AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
		if(pParentSkeleton)
		{
			for(int i=0; i<pParentSkeleton->m_Bones.size(); i++)
			{
				if(m_pCursorTool && m_pCursorTool->GetCursorToolID() == CURSORTOOL_BONE_ADD)
					SkeletonRenderer.RenderBone(GetViewPos(), GetViewZoom(), pSkeleton->m_ParentPath, CAsset_Skeleton::CSubPath::Bone(i));
				else
					SkeletonRenderer.RenderBoneOutline(GetViewPos(), GetViewZoom(), pSkeleton->m_ParentPath, CAsset_Skeleton::CSubPath::Bone(i));
			}
		}
		
		for(int i=0; i<pSkeleton->m_Bones.size(); i++)
		{
			SkeletonRenderer.RenderBone(GetViewPos(), GetViewZoom(), AssetsEditor()->m_ViewedAssetPath, CAsset_Skeleton::CSubPath::Bone(i));
		}
	}
}

void CView::RenderSkeletonSkin()
{
	m_ViewType = VIEWTYPE_OBJECT;
	
	CAsset_SkeletonSkin* pSkeletonSkin = AssetsManager()->GetAsset<CAsset_SkeletonSkin>(AssetsEditor()->m_ViewedAssetPath);
	if(!pSkeletonSkin)
		return;
	
	CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
	SkeletonRenderer.AddSkeletonWithParents(pSkeletonSkin->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_ONLYPARENT);
	SkeletonRenderer.Finalize();
	SkeletonRenderer.AddSkin(AssetsEditor()->m_ViewedAssetPath, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SkeletonRenderer.RenderSkins(GetViewPos(), GetViewZoom());
	if(m_ShowSkeleton)
	{
		SkeletonRenderer.RenderBones(GetViewPos(), GetViewZoom());
	}
}

void CView::RenderSkeletonAnimation()
{
	m_ViewType = VIEWTYPE_OBJECT;
	
	CAsset_SkeletonAnimation* pSkeletonAnimation = AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(AssetsEditor()->m_ViewedAssetPath);
	if(!pSkeletonAnimation)
		return;
		
	CAsset_Skeleton* pSkeleton = AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return;
	
	CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
	
	if(m_GizmoEnabled[GIZMO_AIM])
		SkeletonRenderer.SetAim(m_GizmoPos[GIZMO_AIM]);
	
	if(m_GizmoEnabled[GIZMO_MOTION])
		SkeletonRenderer.SetMotion(m_GizmoPos[GIZMO_MOTION]);
	
	if(m_GizmoEnabled[GIZMO_HOOK])
		SkeletonRenderer.SetHook(m_GizmoPos[GIZMO_HOOK]);
	
	SkeletonRenderer.AddSkeletonWithParents(pSkeletonAnimation->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
	SkeletonRenderer.ApplyAnimation(AssetsEditor()->m_ViewedAssetPath, AssetsEditor()->GetTime());
	SkeletonRenderer.Finalize();
	SkeletonRenderer.RenderSkins(GetViewPos(), GetViewZoom());
	
	
	float Time = AssetsEditor()->GetTime();
	int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
	
	if(m_ShowSkeleton)
	{
		CAsset_Skeleton* pParentSkeleton = AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
		if(pParentSkeleton)
		{
			for(int i=0; i<pParentSkeleton->m_Bones.size(); i++)
			{
				if(!pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Parent(i), Frame).IsNull())
					SkeletonRenderer.RenderBone(GetViewPos(), GetViewZoom(), pSkeleton->m_ParentPath, CAsset_Skeleton::CSubPath::Bone(i));
				else
					SkeletonRenderer.RenderBoneOutline(GetViewPos(), GetViewZoom(), pSkeleton->m_ParentPath, CAsset_Skeleton::CSubPath::Bone(i));
			}
		}
		
		for(int i=0; i<pSkeleton->m_Bones.size(); i++)
		{
			if(!pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Local(i), Frame).IsNull())
				SkeletonRenderer.RenderBone(GetViewPos(), GetViewZoom(), pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::CSubPath::Bone(i));
			else
				SkeletonRenderer.RenderBoneOutline(GetViewPos(), GetViewZoom(), pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::CSubPath::Bone(i));
		}
	}
}

void CView::RenderCharacter()
{
	m_ViewType = VIEWTYPE_OBJECT;
	
	CAsset_Character* pCharacter = AssetsManager()->GetAsset<CAsset_Character>(AssetsEditor()->m_ViewedAssetPath);
	if(!pCharacter)
		return;
		
	CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
	
	if(m_GizmoEnabled[GIZMO_AIM])
		SkeletonRenderer.SetAim(m_GizmoPos[GIZMO_AIM]);
	
	if(m_GizmoEnabled[GIZMO_MOTION])
		SkeletonRenderer.SetMotion(m_GizmoPos[GIZMO_MOTION]);
	
	if(m_GizmoEnabled[GIZMO_HOOK])
		SkeletonRenderer.SetHook(m_GizmoPos[GIZMO_HOOK]);
	
	CAsset_CharacterPart* pCharacterPart;
	for(int i=0; i<pCharacter->m_Parts.size(); i++)
	{
		if(!pCharacter->m_Parts[i].m_DefaultPath.IsNull())
		{
			CAssetPath DefaultCharacterPartPath = pCharacter->m_Parts[i].m_DefaultPath;
			CAsset_CharacterPart* pCharacterPart = AssetsManager()->GetAsset<CAsset_CharacterPart>(DefaultCharacterPartPath);

			if(pCharacterPart)
			{
				SkeletonRenderer.AddSkinWithSkeleton(pCharacterPart->m_SkeletonSkinPath, vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
	
	SkeletonRenderer.ApplyAnimation(pCharacter->m_IdlePath, 0.0f);
	
	SkeletonRenderer.Finalize();
	SkeletonRenderer.RenderSkins(GetViewPos(), GetViewZoom());	
}

void CView::RenderCharacterPart()
{
	m_ViewType = VIEWTYPE_OBJECT;
}

void CView::RenderWeapon()
{
	m_ViewType = VIEWTYPE_OBJECT;
	
	CAsset_Weapon* pWeapon = AssetsManager()->GetAsset<CAsset_Weapon>(AssetsEditor()->m_ViewedAssetPath);
	if(!pWeapon)
		return;
		
	CAsset_Character* pCharacter = AssetsManager()->GetAsset<CAsset_Character>(pWeapon->m_CharacterPath);
	if(!pCharacter)
		return;
		
	CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
	
	if(m_GizmoEnabled[GIZMO_AIM])
		SkeletonRenderer.SetAim(m_GizmoPos[GIZMO_AIM]);
	
	if(m_GizmoEnabled[GIZMO_MOTION])
		SkeletonRenderer.SetMotion(m_GizmoPos[GIZMO_MOTION]);
	
	if(m_GizmoEnabled[GIZMO_HOOK])
		SkeletonRenderer.SetHook(m_GizmoPos[GIZMO_HOOK]);
	
	CAsset_CharacterPart* pCharacterPart;
	for(int i=0; i<pCharacter->m_Parts.size(); i++)
	{
		if(!pCharacter->m_Parts[i].m_DefaultPath.IsNull())
		{
			CAssetPath DefaultCharacterPartPath = pCharacter->m_Parts[i].m_DefaultPath;
			CAsset_CharacterPart* pCharacterPart = AssetsManager()->GetAsset<CAsset_CharacterPart>(DefaultCharacterPartPath);

			if(pCharacterPart)
			{
				SkeletonRenderer.AddSkinWithSkeleton(pCharacterPart->m_SkeletonSkinPath, vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
	
	SkeletonRenderer.ApplyAnimation(pCharacter->m_IdlePath, 0.0f);
	
	// Weapon
	SkeletonRenderer.AddSkinWithSkeleton(pWeapon->m_SkinPath, vec4(1.0, 1.0f, 1.0f, 1.0f));
	SkeletonRenderer.ApplyAnimation(pWeapon->m_AttackAnimationPath, 0.0f);
	
	SkeletonRenderer.Finalize();
	SkeletonRenderer.RenderSkins(GetViewPos(), GetViewZoom());	
}

void CView::RenderMap()
{
	m_ViewType = VIEWTYPE_MAP;
	
	int Source = CAssetPath::SRC_LAND;
	
	//Render map
	{
		CMapRenderer MapRenderer(Graphics(), AssetsManager());
		
		MapRenderer.SetCanvas(GetViewCenter());
		MapRenderer.SetCamera(GetViewPos(), GetViewZoom());
		MapRenderer.RenderSource(Source);
	}
	
	//Render additional informations
	switch(AssetsEditor()->m_ViewedAssetPath.GetType())
	{
		case CAssetPath::TYPE_MAPLAYERTILES:
		{
			CMapRenderer MapRenderer(Graphics(), AssetsManager());
			InitMapRenderer_LayerTiles(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);			
			MapRenderer.RenderTileLayerGrid(AssetsEditor()->m_ViewedAssetPath);
			break;
		}
	}
}

void CView::RenderGizmos()
{
	vec2 TeePos = GetViewPos();
		
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
			
	for(int i=0; i<NUM_GIZMOS; i++)
	{
		if(m_GizmoEnabled[i] == 0)
			continue;
		
		float Radius = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f) - 48.0f;
		
		vec2 GizmoPos = TeePos + m_GizmoPos[i]*Radius;
		
		IGraphics::CLineItem Line(TeePos.x, TeePos.y, GizmoPos.x, GizmoPos.y);
		Graphics()->LinesDraw(&Line, 1);
	}
	
	Graphics()->LinesEnd();
	
	Graphics()->TextureSet(m_pConfig->m_Texture);
	Graphics()->QuadsBegin();
			
	for(int i=0; i<NUM_GIZMOS; i++)
	{
		float Radius = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f) - 48.0f;
		if(m_GizmoEnabled[i] == 0)
		{
			Radius += 48.0f;
			Graphics()->SetColor(1.0f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			Graphics()->SetColor(0.5f, 1.0f, 0.5f, 1.0f);
		}
		
		vec2 GizmoPos = TeePos + m_GizmoPos[i]*Radius;
		
		int SubX;
		int SubY = 3;
		switch(i)
		{
			case GIZMO_AIM:
				SubX = 0;
				break;
			case GIZMO_MOTION:
				SubX = 1;
				break;
			case GIZMO_HOOK:
				SubX = 2;
				break;
		}
		
		Graphics()->QuadsSetSubset(SubX/4.0f, SubY/4.0f, (SubX+1)/4.0f, (SubY+1)/4.0f);
		IGraphics::CQuadItem QuadItem(GizmoPos.x - 32.0f, GizmoPos.y - 32.0f, 64.0f, 64.0f);
		Graphics()->QuadsDrawTL(&QuadItem, 1);
	}
	
	Graphics()->QuadsEnd();
}

void CView::Render()
{
	//Update zoom slider
	if(m_pZoomSlider)
	{
		float SliderPos = 0.0f;
		
		switch(m_ViewType)
		{
			case VIEWTYPE_MAP:
				SliderPos = CZoomSlider::ZoomSliderConversion(m_ViewType, m_MapZoom);
				break;
			default:
				SliderPos = CZoomSlider::ZoomSliderConversion(m_ViewType, m_ObjectZoom);
				break;
		}
		
		m_pZoomSlider->SetSliderPos(SliderPos, false);
		m_pZoomSlider->Update();
	}
	
	//Render
	CUIRect rect;
	rect.x = m_ViewRect.x;
	rect.y = m_ViewRect.y;
	rect.w = m_ViewRect.w;
	rect.h = m_ViewRect.h;
	
	RenderTools()->DrawRoundRect(&rect, vec4(1.0f, 1.0f, 1.0f, 0.5f), 5.0f);
	
	//Search Tag: TAG_NEW_ASSET
	Graphics()->ClipEnable(m_ViewRect.x, m_ViewRect.y, m_ViewRect.w, m_ViewRect.h);
	switch(AssetsEditor()->m_ViewedAssetPath.GetType())
	{
		case CAssetPath::TYPE_IMAGE:
			RenderImage();
			break;
		case CAssetPath::TYPE_SPRITE:
			RenderSprite();
			break;
		case CAssetPath::TYPE_SKELETON:
			RenderSkeleton();
			break;
		case CAssetPath::TYPE_SKELETONSKIN:
			RenderSkeletonSkin();
			break;
		case CAssetPath::TYPE_SKELETONANIMATION:
			RenderSkeletonAnimation();
			RenderGizmos();
			break;
		case CAssetPath::TYPE_CHARACTER:
			RenderCharacter();
			RenderGizmos();
			break;
		case CAssetPath::TYPE_CHARACTERPART:
			RenderCharacterPart();
			RenderGizmos();
			break;
		case CAssetPath::TYPE_WEAPON:
			RenderWeapon();
			RenderGizmos();
			break;
		case CAssetPath::TYPE_MAPGROUP:
		case CAssetPath::TYPE_MAPLAYERTILES:
		case CAssetPath::TYPE_MAPLAYERQUADS:
			RenderMap();
			break;
	}
	
	if(m_pCursorTool)
		m_pCursorTool->Render();
	
	Graphics()->ClipDisable();
	
	if(m_LastEditedAssetType != AssetsEditor()->m_ViewedAssetPath.GetType())
	{
		m_LastEditedAssetType = AssetsEditor()->m_ViewedAssetPath.GetType();
		RefreshToolBar();
	}
	
	m_pToolbar->Render();
}

bool CView::IsOnGizmo(int GizmoId, int X, int Y)
{
	float Radius = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f) - 48.0f;
	if(m_GizmoEnabled[GizmoId] == 0)
	{
		Radius += 48.0f;
	}
	vec2 GizmoPos = GetViewPos() + m_GizmoPos[GizmoId]*Radius;
	return (length(GizmoPos - vec2(X, Y)) < 32.0f);
}

void CView::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(m_ViewRect.IsInside(X, Y))
	{
		if(Button == KEY_MOUSE_WHEEL_UP || Button == KEY_MOUSE_WHEEL_DOWN)
		{
			int Scroll = Count;
			if(Button == KEY_MOUSE_WHEEL_DOWN)
				Scroll = -Scroll;
			
			float Pos;
			switch(m_ViewType)
			{
				case VIEWTYPE_MAP:
					Pos = CZoomSlider::ZoomSliderConversion(m_ViewType, m_MapZoom);
					break;
				default:
					Pos = CZoomSlider::ZoomSliderConversion(m_ViewType, m_ObjectZoom);
					break;
			}
			
			Pos += 0.1f*Scroll;
			
			switch(m_ViewType)
			{
				case VIEWTYPE_MAP:
					m_MapZoom = CZoomSlider::SliderZoomConversion(m_ViewType, Pos);
					break;
				default:
					m_ObjectZoom = CZoomSlider::SliderZoomConversion(m_ViewType, Pos);
					break;
			}
		}
		else if(Button == KEY_MOUSE_1)
		{
			switch(AssetsEditor()->m_ViewedAssetPath.GetType())
			{
				case CAssetPath::TYPE_SKELETONANIMATION:
				case CAssetPath::TYPE_CHARACTER:
				case CAssetPath::TYPE_CHARACTERPART:
				case CAssetPath::TYPE_WEAPON:
					//Check gizmo first
					int GizmoFound = -1;
					for(int i=NUM_GIZMOS-1; i>=0; i--)
					{
						if(IsOnGizmo(i, X, Y))
						{
							GizmoFound = i;
							break;
						}
					}
					
					if(GizmoFound >= 0)
					{
						m_DragType = DRAGTYPE_GIZMO;
						m_SelectedGizmo = GizmoFound;
						return;
					}
					break;
			}
		}
		else if(Button == KEY_MOUSE_3)
		{
			m_DragType = DRAGTYPE_VIEW;
			return;
		}
		
		if(m_pCursorTool)
			m_pCursorTool->OnButtonClick(X, Y, Button, Count);
	}
	else
	{
		m_pToolbar->OnButtonClick(X, Y, Button, Count);
	}
}

void CView::OnButtonRelease(int Button)
{
	if(Button == KEY_MOUSE_3 && m_DragType == DRAGTYPE_VIEW)
	{
		m_DragType = DRAGTYPE_NONE;
	}
	else if(Button == KEY_MOUSE_1 && m_DragType != DRAGTYPE_VIEW)
	{
		m_DragType = DRAGTYPE_NONE;
	}
	
	m_pToolbar->OnButtonRelease(Button);
}

void CView::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_DragType == DRAGTYPE_VIEW)
	{
		switch(m_ViewType)
		{
			case VIEWTYPE_MAP:
			{
				m_MapPos -= vec2(RelX, RelY)/m_MapZoom;
				break;
			}
		}	
	}
	else if(m_DragType == DRAGTYPE_GIZMO)
	{
		float Radius = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f) - 48.0f;
		vec2 Diff = vec2(X, Y) - GetViewPos();
		m_GizmoPos[m_SelectedGizmo] = normalize(Diff);
		
		float Dist = length(Diff);
		if(Dist > Radius + 32.0f)
			m_GizmoEnabled[m_SelectedGizmo] = 0;
		else
			m_GizmoEnabled[m_SelectedGizmo] = 1;
		
	}
	else if(m_DragType == DRAGTYPE_CURSORTOOL)
	{
		if(m_pCursorTool)
			m_pCursorTool->OnMouseOver(X, Y, RelX, RelY, KeyState);
		
		//TAG_CURSORTOOL
		/*
		m_CursorToolLastPosition = m_CursorToolPosition;
		m_CursorToolPosition = vec2(X, Y);
		switch(AssetsEditor()->m_ViewedAssetPath.GetType())
		{
			case CAssetPath::TYPE_SKELETON:
			{
				vec2 Origin, AxisX, AxisY;
						
				CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
				SkeletonRenderer.AddSkeletonWithParents(AssetsEditor()->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
				SkeletonRenderer.Finalize();
				
				switch(m_CursorTool)
				{
					case CURSORTOOL_TRANSLATE:
					case CURSORTOOL_TRANSLATE_X:
					case CURSORTOOL_TRANSLATE_Y:
					{
						if(SkeletonRenderer.GetParentAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							if(m_CursorTool != CURSORTOOL_TRANSLATE_Y)
							{
								float TranslationX = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_TRANSLATION_X, m_SelectedBone.ConvertToInteger(), 0.0f);
								TranslationX += dot(AxisX, vec2(RelX, RelY));
								AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_TRANSLATION_X, m_SelectedBone.ConvertToInteger(), TranslationX);
							}
							
							if(m_CursorTool != CURSORTOOL_TRANSLATE_X)
							{
								float TranslationY = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_TRANSLATION_Y, m_SelectedBone.ConvertToInteger(), 0.0f);
								TranslationY += dot(AxisY, vec2(RelX, RelY));
								AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_TRANSLATION_Y, m_SelectedBone.ConvertToInteger(), TranslationY);
							}
						}
						break;					
					}
					case CURSORTOOL_SCALE:
					{
						if(SkeletonRenderer.GetParentAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float TranslationX = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_X, m_SelectedBone.ConvertToInteger(), 0.0f);
							float TranslationY = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_Y, m_SelectedBone.ConvertToInteger(), 0.0f);
							float Translation =  (length(m_CursorToolPosition - m_CursorPivot) - length(m_CursorToolLastPosition - m_CursorPivot))/10.0f;
							TranslationX += Translation;
							TranslationY += Translation;
							AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_X, m_SelectedBone.ConvertToInteger(), TranslationX);
							AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_Y, m_SelectedBone.ConvertToInteger(), TranslationY);
						}
						break;					
					}
					case CURSORTOOL_SCALE_X:
					case CURSORTOOL_SCALE_Y:
					{
						if(SkeletonRenderer.GetParentAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							if(m_CursorTool != CURSORTOOL_SCALE_Y)
							{
								float TranslationX = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_X, m_SelectedBone.ConvertToInteger(), 0.0f);
								TranslationX += dot(AxisX, vec2(RelX, RelY))/5.0f;
								AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_X, m_SelectedBone.ConvertToInteger(), TranslationX);
							}
							
							if(m_CursorTool != CURSORTOOL_SCALE_X)
							{
								float TranslationY = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_Y, m_SelectedBone.ConvertToInteger(), 0.0f);
								TranslationY += dot(AxisY, vec2(RelX, RelY))/5.0f;
								AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_Y, m_SelectedBone.ConvertToInteger(), TranslationY);
							}
						}
						break;					
					}
					case CURSORTOOL_ROTATE:
					{
						if(SkeletonRenderer.GetLocalAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float Angle = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), 0.0f);
							Angle += angle(m_CursorToolLastPosition - Origin, m_CursorToolPosition - Origin);
							AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
						}
						break;				
					}
					case CURSORTOOL_BONE_LENGTH:
					{
						if(SkeletonRenderer.GetLocalAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float Angle = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), 0.0f);
							float Length = AssetsManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_LENGTH, m_SelectedBone.ConvertToInteger(), 32.0f);
							Angle += angle(m_CursorToolLastPosition - Origin, m_CursorToolPosition - Origin);
							Length += dot(AxisX, vec2(RelX, RelY));
							AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_LENGTH, m_SelectedBone.ConvertToInteger(), Length);
							AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
						}
						break;					
					}
					case CURSORTOOL_BONE_ADD:
					{
						if(SkeletonRenderer.GetLocalAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							vec2 RelPos = (m_CursorToolPosition - Origin);
							float LengthX = dot(AxisX, RelPos);
							float LengthY = dot(AxisY, RelPos);
							float Length = sqrt(LengthX*LengthX + LengthY*LengthY);
							AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_LENGTH, m_SelectedBone.ConvertToInteger(), Length);
							float Angle = angle(AxisX, RelPos);
							AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
						}
						vec2 OriginParent, AxisXParent, AxisYParent;
						if(SkeletonRenderer.GetParentAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, OriginParent, AxisXParent, AxisYParent))
						{
							float Angle = angle(AxisXParent, m_CursorToolPosition - Origin);
							AssetsManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
						}
						break;					
					}
				}
				break;
			}
			case CAssetPath::TYPE_SKELETONANIMATION:
			{
				CAsset_SkeletonAnimation* pSkeletonAnimation = AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(AssetsEditor()->m_ViewedAssetPath);
				if(!pSkeletonAnimation)
					return;
				
				float Time = AssetsEditor()->GetTime();
				int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
				int BoneId = m_SelectedBone.GetId();
				CAsset_SkeletonAnimation::CSubPath FramePath = pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Local(BoneId), Frame);
				if(FramePath.IsNull())
					return;
				
				CAsset_Skeleton* pSkeleton = AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
				if(!pSkeleton)
					return;
					
				vec2 Origin, AxisX, AxisY;
						
				CSkeletonRenderer SkeletonRenderer(Graphics(), AssetsManager());
				SkeletonRenderer.AddSkeletonWithParents(pSkeletonAnimation->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
				SkeletonRenderer.ApplyAnimation(AssetsEditor()->m_ViewedAssetPath, AssetsEditor()->GetTime());
				SkeletonRenderer.Finalize();
						
				switch(m_CursorTool)
				{
					case CURSORTOOL_TRANSLATE:
					case CURSORTOOL_TRANSLATE_X:
					case CURSORTOOL_TRANSLATE_Y:
					{
						if(SkeletonRenderer.GetParentAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							if(m_CursorTool != CURSORTOOL_TRANSLATE_Y)
							{
								float TranslationX = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_X, FramePath.ConvertToInteger(), 0.0f);
								TranslationX += dot(AxisX, vec2(RelX, RelY));
								AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_X, FramePath.ConvertToInteger(), TranslationX);
							}
							
							if(m_CursorTool != CURSORTOOL_TRANSLATE_X)
							{
								float TranslationY = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_Y, FramePath.ConvertToInteger(), 0.0f);
								TranslationY += dot(AxisY, vec2(RelX, RelY));
								AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_Y, FramePath.ConvertToInteger(), TranslationY);
							}
						}
						break;					
					}
					case CURSORTOOL_SCALE:
					{
						if(SkeletonRenderer.GetParentAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float TranslationX = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, FramePath.ConvertToInteger(), 0.0f);
							float TranslationY = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, FramePath.ConvertToInteger(), 0.0f);
							float Translation =  (length(m_CursorToolPosition - m_CursorPivot) - length(m_CursorToolLastPosition - m_CursorPivot))/10.0f;
							TranslationX += Translation;
							TranslationY += Translation;
							AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, FramePath.ConvertToInteger(), TranslationX);
							AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, FramePath.ConvertToInteger(), TranslationY);
						}
						break;					
					}
					case CURSORTOOL_SCALE_X:
					case CURSORTOOL_SCALE_Y:
					{
						if(SkeletonRenderer.GetParentAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							if(m_CursorTool != CURSORTOOL_SCALE_Y)
							{
								float TranslationX = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, FramePath.ConvertToInteger(), 0.0f);
								TranslationX += dot(AxisX, vec2(RelX, RelY))/5.0f;
								AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, FramePath.ConvertToInteger(), TranslationX);
							}
							
							if(m_CursorTool != CURSORTOOL_SCALE_X)
							{
								float TranslationY = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, FramePath.ConvertToInteger(), 0.0f);
								TranslationY += dot(AxisY, vec2(RelX, RelY))/5.0f;
								AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, FramePath.ConvertToInteger(), TranslationY);
							}
						}
						break;					
					}
					case CURSORTOOL_ROTATE:
					{
						if(SkeletonRenderer.GetLocalAxis(GetViewPos(), GetViewZoom(), m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float Angle = AssetsManager()->GetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_ANGLE, FramePath.ConvertToInteger(), 0.0f);
							Angle += angle(m_CursorToolLastPosition - Origin, m_CursorToolPosition - Origin);
							AssetsManager()->SetAssetValue<float>(AssetsEditor()->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_ANGLE, FramePath.ConvertToInteger(), Angle);
						}
						break;				
					}
				}
				break;
			}
			case CAssetPath::TYPE_MAPLAYERTILES:
			{
				CAsset_MapLayerTiles* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerTiles>(AssetsEditor()->m_ViewedAssetPath);
				if(!pLayer)
					return;
				
				CMapRenderer MapRenderer(Graphics(), AssetsManager());
				InitMapRenderer_LayerTiles(&MapRenderer, AssetsEditor()->m_ViewedAssetPath);
				
				vec2 CursorMapPos = MapRenderer.ScreenPosToMapPos(vec2(X, Y));
				
				switch(m_CursorTool)
				{
					case CURSORTOOL_SELECTIONSTAMP:
					{
						if(m_pMapTilesetSelection && m_DragType == DRAGTYPE_CURSORTOOL)
						{
							int TileX = CursorMapPos.x/32.0f;
							int TileY = CursorMapPos.y/32.0f;
							
							for(int j=0; j<m_MapTilesetSelectionHeight; j++)
							{
								for(int i=0; i<m_MapTilesetSelectionWidth; i++)
								{
									CAsset_MapLayerTiles::CTile* pTile = pLayer->GetTile(TileX+i, TileY+j);
									if(pTile)
									{
										pTile->m_Index = m_pMapTilesetSelection[j*m_MapTilesetSelectionWidth+i].m_Index;
										pTile->m_Flags = m_pMapTilesetSelection[j*m_MapTilesetSelectionWidth+i].m_Flags;
										pTile->m_Index = m_pMapTilesetSelection[j*m_MapTilesetSelectionWidth+i].m_Index;
									}
								}
							}
						}
						break;					
					}
				}
				break;
			}
		}
		*/
	}
	else
	{
		int GizmoFound = -1;
		for(int i=NUM_GIZMOS-1; i>=0; i--)
		{
			if(IsOnGizmo(i, X, Y))
			{
				GizmoFound = i;
				break;
			}
		}
		if(GizmoFound >= 0)
		{
			ShowHint(s_GizmoHints[GizmoFound]);
		}
		
		m_pToolbar->OnMouseOver(X, Y, RelX, RelY, KeyState);
	}
}

void CView::ZoomCallback(float Pos)
{
	float Zoom = CZoomSlider::SliderZoomConversion(m_ViewType, Pos);
	
	switch(m_ViewType)
	{
		case VIEWTYPE_MAP:
			m_MapZoom = Zoom;
			break;
		default:
			m_ObjectZoom = Zoom;
			break;
	}
}

}

}
