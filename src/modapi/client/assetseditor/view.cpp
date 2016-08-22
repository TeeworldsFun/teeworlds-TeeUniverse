#include <base/tl/sorted_array.h>

#include <engine/shared/config.h>
#include <engine/shared/datafile.h>

#include <engine/client.h>
#include <engine/input.h>
#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/storage.h>
#include <engine/keys.h>

#include <modapi/client/clientmode.h>
#include <modapi/client/gui/button.h>
#include <modapi/client/gui/label.h>
#include <modapi/client/gui/layout.h>
#include <modapi/client/gui/integer-edit.h>
#include <modapi/client/gui/text-edit.h>
#include <modapi/client/skeletonrenderer.h>
#include <modapi/client/maprenderer.h>

#include <cstddef>

#include "view.h"

namespace tu
{
		
const char* CAssetsEditorGui_View::s_CursorToolHints[] = {
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

const char* CAssetsEditorGui_View::s_GizmoHints[] = {
	"Aim Direction: set the aim direction",
	"Motion Direction: set the motion direction",
	"Hook Direction: set the hook direction",
};

const char* CAssetsEditorGui_View::s_HintText[] = {
	"Show/Hide skeleton bones",
};

CAssetsEditorGui_View::CAssetsEditorGui_View(CAssetsEditor* pAssetsEditor) :
	gui::CWidget(pAssetsEditor->m_pGuiConfig),
	m_pAssetsEditor(pAssetsEditor),
	m_StartPointPos(-128.0f, 0.0f),
	m_EndPointPos(128.0f, 0.0f),
	m_Zoom(1.5f),
	m_ToolbarHeight(30),
	m_pToolbar(0),
	m_LastEditedAssetType(-1),
	m_CursorTool(CURSORTOOL_MOVE),
	m_ShowSkeleton(true)
{	
	m_pToolbar = new gui::CHListLayout(m_pConfig);
	RefreshToolBar();
	
	for(int i=0; i<NUM_GIZMOS; i++)
	{
		m_GizmoPos[i] = vec2(1.0f, 0.0f);
		m_GizmoEnabled[i] = 0;
	}
}
	
CAssetsEditorGui_View::~CAssetsEditorGui_View()
{
	if(m_pToolbar) delete m_pToolbar;
}

void CAssetsEditorGui_View::RefreshToolBar()
{
	m_pToolbar->Clear();
	
	for(int i=0; i<NUM_CURSORTOOLS; i++)
	{
		m_CursorToolButtons[i] = 0;
	}
	
	gui::CLabel* pZoomLabel = new gui::CLabel(m_pConfig, "Zoom:");
	pZoomLabel->SetRect(gui::CRect(
		0, 0, //Positions will be filled when the toolbar is updated
		90,
		pZoomLabel->m_Rect.h
	));
	
	m_pToolbar->Add(pZoomLabel);
	m_pToolbar->Add(new CAssetsEditorGui_View::CZoomSlider(this));
	
	switch(m_pAssetsEditor->m_ViewedAssetPath.GetType())
	{
		case CAssetPath::TYPE_SKELETON:
		case CAssetPath::TYPE_SKELETONANIMATION:
		case CAssetPath::TYPE_SKELETONSKIN:
			m_pToolbar->AddSeparator();
			m_pToolbar->Add(new gui::CLabel(m_pConfig, "Show:"));
			m_pToolbar->Add(new CViewSwitch(this, CAssetsEditor::ICON_SKELETON, &m_ShowSkeleton, s_HintText[HINT_SHOW_SKELETON]));
			break;
	}
	
	m_pToolbar->AddSeparator();
	m_pToolbar->Add(new gui::CLabel(m_pConfig, "Tools:"));
	
	m_CursorToolButtons[CURSORTOOL_MOVE] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_MOVE, CURSORTOOL_MOVE);
	m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_MOVE]);
	
	switch(m_pAssetsEditor->m_ViewedAssetPath.GetType())
	{
		case CAssetPath::TYPE_IMAGE:
			//~ m_CursorToolButtons[CURSORTOOL_SPRITE_CREATOR] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_SPRITE, CURSORTOOL_SPRITE_CREATOR);
			//~ m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_SPRITE_CREATOR]);
			break;
		case CAssetPath::TYPE_SKELETON:
		case CAssetPath::TYPE_SKELETONANIMATION:
			m_CursorToolButtons[CURSORTOOL_TRANSLATE] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_TRANSLATE, CURSORTOOL_TRANSLATE);
			m_CursorToolButtons[CURSORTOOL_TRANSLATE_X] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_TRANSLATE_X, CURSORTOOL_TRANSLATE_X);
			m_CursorToolButtons[CURSORTOOL_TRANSLATE_Y] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_TRANSLATE_Y, CURSORTOOL_TRANSLATE_Y);
			m_CursorToolButtons[CURSORTOOL_ROTATE] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_ROTATE, CURSORTOOL_ROTATE);
			m_CursorToolButtons[CURSORTOOL_SCALE] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_SCALE, CURSORTOOL_SCALE);
			m_CursorToolButtons[CURSORTOOL_SCALE_X] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_SCALE_X, CURSORTOOL_SCALE_X);
			m_CursorToolButtons[CURSORTOOL_SCALE_Y] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_SCALE_Y, CURSORTOOL_SCALE_Y);
			m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_TRANSLATE]);
			m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_TRANSLATE_X]);
			m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_TRANSLATE_Y]);
			m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_ROTATE]);
			m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_SCALE]);
			m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_SCALE_X]);
			m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_SCALE_Y]);
			
			
			if(m_pAssetsEditor->m_ViewedAssetPath.GetType() == CAssetPath::TYPE_SKELETON)
			{
				m_CursorToolButtons[CURSORTOOL_BONE_LENGTH] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_BONE_LENGTH, CURSORTOOL_BONE_LENGTH);
				m_CursorToolButtons[CURSORTOOL_BONE_ADD] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_BONE_ADD, CURSORTOOL_BONE_ADD);
				m_CursorToolButtons[CURSORTOOL_BONE_DELETE] = new CAssetsEditorGui_View::CCursorToolButton(this, CAssetsEditor::ICON_CURSORTOOL_BONE_DELETE, CURSORTOOL_BONE_DELETE);
				m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_BONE_LENGTH]);
				m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_BONE_ADD]);
				m_pToolbar->Add(m_CursorToolButtons[CURSORTOOL_BONE_DELETE]);
			}
			break;
	}
	
	m_pToolbar->Update();
}

vec2 CAssetsEditorGui_View::GetMapPosition()
{
	vec2 MapPos;
	MapPos.x = m_ViewRect.x + m_ViewRect.w/2;
	MapPos.y = m_ViewRect.y + m_ViewRect.h/2;
	
	return MapPos;
}

vec2 CAssetsEditorGui_View::GetTeePosition()
{
	vec2 TeePos;
	TeePos.x = m_ViewRect.x + m_ViewRect.w/2;
	TeePos.y = m_ViewRect.y + m_ViewRect.h/2;
	
	return TeePos;
}

vec2 CAssetsEditorGui_View::GetAimPosition()
{
	float AimDist = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f) - 48.0f;
	vec2 AimPos = GetTeePosition() + m_AimDir * AimDist - vec2(16.0f, 16.0f);
	
	return AimPos;
}

vec2 CAssetsEditorGui_View::GetMotionPosition()
{
	float MotionDist = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f);
	vec2 MotionPos = GetTeePosition() + m_MotionDir * MotionDist - vec2(16.0f, 16.0f);
	
	return MotionPos;
}

void CAssetsEditorGui_View::Update()
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

void CAssetsEditorGui_View::RenderImage()
{
	CAsset_Image* pImage = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Image>(m_pAssetsEditor->m_ViewedAssetPath);
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
	for(int i=0; i<m_pAssetsEditor->AssetManager()->GetNumAssets<CAsset_Sprite>(CAssetPath::SRC_INTERNAL); i++)
	{
		CAsset_Sprite* pSprite = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Sprite>(CAssetPath::Internal(CAssetPath::TYPE_SPRITE, i));
		if(pSprite->m_ImagePath == m_pAssetsEditor->m_ViewedAssetPath)
		{
			if(m_pAssetsEditor->IsEditedAsset(CAssetPath::Internal(CAssetPath::TYPE_SPRITE, i)))
				Graphics()->SetColor(alpha, alpha*0.5f, alpha*0.5f, alpha);
			else
				Graphics()->SetColor(alpha, alpha, alpha*0.5f, alpha);
				
			IGraphics::CQuadItem QuadItem(x0 + xStep * pSprite->m_X, y0 + yStep * pSprite->m_Y, xStep * pSprite->m_Width, yStep * pSprite->m_Height);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
		}
	}
	for(int i=0; i<m_pAssetsEditor->AssetManager()->GetNumAssets<CAsset_Sprite>(CAssetPath::SRC_EXTERNAL); i++)
	{
		CAsset_Sprite* pSprite = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Sprite>(CAssetPath::External(CAssetPath::TYPE_SPRITE, i));
		if(pSprite->m_ImagePath == m_pAssetsEditor->m_ViewedAssetPath)
		{
			if(m_pAssetsEditor->IsEditedAsset(CAssetPath::External(CAssetPath::TYPE_SPRITE, i)))
				Graphics()->SetColor(alpha, alpha*0.5f, alpha*0.5f, alpha);
			else
				Graphics()->SetColor(alpha, alpha, alpha*0.5f, alpha);
				
			IGraphics::CQuadItem QuadItem(x0 + xStep * pSprite->m_X, y0 + yStep * pSprite->m_Y, xStep * pSprite->m_Width, yStep * pSprite->m_Height);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
		}
	}
	Graphics()->QuadsEnd();
	
	//Draw image
	m_pAssetsEditor->TUGraphics()->TextureSet(m_pAssetsEditor->m_ViewedAssetPath);
	
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

void CAssetsEditorGui_View::RenderSprite()
{
	CAsset_Sprite* pSprite = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Sprite>(m_pAssetsEditor->m_ViewedAssetPath);
	if(!pSprite)
		return;
	
	CAsset_Image* pImage = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Image>(pSprite->m_ImagePath);
	if(!pImage)
		return;
	
	float SpriteWidthPix = (pImage->m_Width / pImage->m_GridWidth) * pSprite->m_Width;
	float SpriteHeightPix = (pImage->m_Height / pImage->m_GridHeight) * pSprite->m_Height;
	float SpriteScaling = sqrtf(SpriteWidthPix*SpriteWidthPix + SpriteHeightPix*SpriteHeightPix);
	vec2 Pos;
	Pos.x = m_ViewRect.x + m_ViewRect.w/2;
	Pos.y = m_ViewRect.y + m_ViewRect.h/2;
	
	m_pAssetsEditor->TUGraphics()->DrawSprite(m_pAssetsEditor->m_ViewedAssetPath, Pos, SpriteScaling, 0.0f, 0x0);
}

void CAssetsEditorGui_View::RenderSkeleton()
{
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(m_pAssetsEditor->m_ViewedAssetPath);
	if(!pSkeleton)
		return;
		
	CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
	SkeletonRenderer.AddSkeletonWithParents(m_pAssetsEditor->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
	SkeletonRenderer.Finalize();
	SkeletonRenderer.RenderSkins(GetTeePosition(), m_Zoom);
	
	if(m_ShowSkeleton)
	{
		CAsset_Skeleton* pParentSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
		if(pParentSkeleton)
		{
			for(int i=0; i<pParentSkeleton->m_Bones.size(); i++)
			{
				if(m_CursorTool == CURSORTOOL_BONE_ADD)
					SkeletonRenderer.RenderBone(GetTeePosition(), m_Zoom, pSkeleton->m_ParentPath, CAsset_Skeleton::CSubPath::Bone(i));
				else
					SkeletonRenderer.RenderBoneOutline(GetTeePosition(), m_Zoom, pSkeleton->m_ParentPath, CAsset_Skeleton::CSubPath::Bone(i));
			}
		}
		
		for(int i=0; i<pSkeleton->m_Bones.size(); i++)
		{
			SkeletonRenderer.RenderBone(GetTeePosition(), m_Zoom, m_pAssetsEditor->m_ViewedAssetPath, CAsset_Skeleton::CSubPath::Bone(i));
		}
	}
}

void CAssetsEditorGui_View::RenderSkeletonSkin()
{
	CAsset_SkeletonSkin* pSkeletonSkin = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_SkeletonSkin>(m_pAssetsEditor->m_ViewedAssetPath);
	if(!pSkeletonSkin)
		return;
	
	CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
	SkeletonRenderer.AddSkeletonWithParents(pSkeletonSkin->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_ONLYPARENT);
	SkeletonRenderer.Finalize();
	SkeletonRenderer.AddSkin(m_pAssetsEditor->m_ViewedAssetPath, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SkeletonRenderer.RenderSkins(GetTeePosition(), m_Zoom);
	if(m_ShowSkeleton)
	{
		SkeletonRenderer.RenderBones(GetTeePosition(), m_Zoom);
	}
}

void CAssetsEditorGui_View::RenderSkeletonAnimation()
{
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_ViewedAssetPath);
	if(!pSkeletonAnimation)
		return;
		
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return;
	
	CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
	
	if(m_GizmoEnabled[GIZMO_AIM])
		SkeletonRenderer.SetAim(m_GizmoPos[GIZMO_AIM]);
	
	if(m_GizmoEnabled[GIZMO_MOTION])
		SkeletonRenderer.SetMotion(m_GizmoPos[GIZMO_MOTION]);
	
	if(m_GizmoEnabled[GIZMO_HOOK])
		SkeletonRenderer.SetHook(m_GizmoPos[GIZMO_HOOK]);
	
	SkeletonRenderer.AddSkeletonWithParents(pSkeletonAnimation->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
	SkeletonRenderer.ApplyAnimation(m_pAssetsEditor->m_ViewedAssetPath, m_pAssetsEditor->GetTime());
	SkeletonRenderer.Finalize();
	SkeletonRenderer.RenderSkins(GetTeePosition(), m_Zoom);
	
	
	float Time = m_pAssetsEditor->GetTime();
	int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
	
	if(m_ShowSkeleton)
	{
		CAsset_Skeleton* pParentSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
		if(pParentSkeleton)
		{
			for(int i=0; i<pParentSkeleton->m_Bones.size(); i++)
			{
				if(!pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Parent(i), Frame).IsNull())
					SkeletonRenderer.RenderBone(GetTeePosition(), m_Zoom, pSkeleton->m_ParentPath, CAsset_Skeleton::CSubPath::Bone(i));
				else
					SkeletonRenderer.RenderBoneOutline(GetTeePosition(), m_Zoom, pSkeleton->m_ParentPath, CAsset_Skeleton::CSubPath::Bone(i));
			}
		}
		
		for(int i=0; i<pSkeleton->m_Bones.size(); i++)
		{
			if(!pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Local(i), Frame).IsNull())
				SkeletonRenderer.RenderBone(GetTeePosition(), m_Zoom, pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::CSubPath::Bone(i));
			else
				SkeletonRenderer.RenderBoneOutline(GetTeePosition(), m_Zoom, pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::CSubPath::Bone(i));
		}
	}
}

void CAssetsEditorGui_View::RenderCharacter()
{
	CAsset_Character* pCharacter = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Character>(m_pAssetsEditor->m_ViewedAssetPath);
	if(!pCharacter)
		return;
		
	CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
	
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
			CAsset_CharacterPart* pCharacterPart = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_CharacterPart>(DefaultCharacterPartPath);

			if(pCharacterPart)
			{
				SkeletonRenderer.AddSkinWithSkeleton(pCharacterPart->m_SkeletonSkinPath, vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
	
	SkeletonRenderer.ApplyAnimation(pCharacter->m_IdlePath, 0.0f);
	
	SkeletonRenderer.Finalize();
	SkeletonRenderer.RenderSkins(GetTeePosition(), m_Zoom);	
}

void CAssetsEditorGui_View::RenderCharacterPart()
{
	
}

void CAssetsEditorGui_View::RenderWeapon()
{
	CAsset_Weapon* pWeapon = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Weapon>(m_pAssetsEditor->m_ViewedAssetPath);
	if(!pWeapon)
		return;
		
	CAsset_Character* pCharacter = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Character>(pWeapon->m_CharacterPath);
	if(!pCharacter)
		return;
		
	CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
	
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
			CAsset_CharacterPart* pCharacterPart = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_CharacterPart>(DefaultCharacterPartPath);

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
	SkeletonRenderer.RenderSkins(GetTeePosition(), m_Zoom);	
}

void CAssetsEditorGui_View::RenderMap()
{
	CMapRenderer MapRenderer(Graphics(), m_pAssetsEditor->AssetManager());
	
	//Render map
	MapRenderer.RenderSource(CAssetPath::SRC_MAP);
	
	//Render additional informations
	switch(m_pAssetsEditor->m_ViewedAssetPath.GetType())
	{
		case CAssetPath::TYPE_MAPLAYERTILES:
		{
			CAsset_MapLayerTiles* pLayer = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_MapLayerTiles>(m_pAssetsEditor->m_ViewedAssetPath);
			if(pLayer)
			{
				Graphics()->TextureClear();
				Graphics()->LinesBegin();
				Graphics()->SetColor(0.5f, 0.5f, 0.5f, 0.5f);
				
				vec2 MapPos = GetMapPosition();
				
				float xMap = pLayer->m_Width*m_Zoom*32.0f;
				float yMap = pLayer->m_Height*m_Zoom*32.0f;
				
				float x0 = MapPos.x - xMap/2.0f;
				float x1 = MapPos.x + 3.0f*xMap/2.0f;
				float y0 = MapPos.y - yMap/2.0f;
				float y1 = MapPos.y + 3.0f*yMap/2.0f;
				
				{
					IGraphics::CLineItem Lines[4];
					Lines[0] = IGraphics::CLineItem(x0, y0, x1, y0);
					Lines[1] = IGraphics::CLineItem(x1, y0, x1, y1);
					Lines[2] = IGraphics::CLineItem(x1, y1, x0, y1);
					Lines[3] = IGraphics::CLineItem(x0, y1, x0, y0);
					
					Graphics()->LinesDraw(Lines, 4);
				}
				
				Graphics()->SetColor(0.25f, 0.25f, 0.25f, 0.25f);
				for(int j=0; j<=pLayer->m_Height; j++)
				{
					float y = y0+(y1-y0)*(static_cast<float>(j)/pLayer->m_Height);
					IGraphics::CLineItem Line(x0, y, x1, y);
					Graphics()->LinesDraw(&Line, 1);
				}
				for(int i=0; i<pLayer->m_Width; i++)
				{
					float x = x0+(x1-x0)*(static_cast<float>(i)/pLayer->m_Width);
					IGraphics::CLineItem Line(x, y0, x, y1);
					Graphics()->LinesDraw(&Line, 1);
				}
				
				Graphics()->LinesEnd();
			}
			break;
		}
	}
}

void CAssetsEditorGui_View::RenderGizmos()
{
	vec2 TeePos = GetTeePosition();
		
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

void CAssetsEditorGui_View::Render()
{
	CUIRect rect;
	rect.x = m_ViewRect.x;
	rect.y = m_ViewRect.y;
	rect.w = m_ViewRect.w;
	rect.h = m_ViewRect.h;
	
	RenderTools()->DrawRoundRect(&rect, vec4(1.0f, 1.0f, 1.0f, 0.5f), 5.0f);
	
	//Search Tag: TAG_NEW_ASSET
	Graphics()->ClipEnable(m_ViewRect.x, m_ViewRect.y, m_ViewRect.w, m_ViewRect.h);
	switch(m_pAssetsEditor->m_ViewedAssetPath.GetType())
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
	
	Graphics()->ClipDisable();
	
	if(m_LastEditedAssetType != m_pAssetsEditor->m_ViewedAssetPath.GetType())
	{
		m_LastEditedAssetType = m_pAssetsEditor->m_ViewedAssetPath.GetType();
		RefreshToolBar();
	}
	
	for(int i=0; i<NUM_CURSORTOOLS; i++)
	{
		if(m_CursorToolButtons[i])
		{
			if(m_CursorTool == i)
				m_CursorToolButtons[i]->SetButtonStyle(gui::BUTTONSTYLE_HIGHLIGHT);
			else
				m_CursorToolButtons[i]->SetButtonStyle(gui::BUTTONSTYLE_NORMAL);
		}
	}
	
	m_pToolbar->Render();
}

bool CAssetsEditorGui_View::IsOnGizmo(int GizmoId, int X, int Y)
{
	float Radius = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f) - 48.0f;
	if(m_GizmoEnabled[GizmoId] == 0)
	{
		Radius += 48.0f;
	}
	vec2 GizmoPos = GetTeePosition() + m_GizmoPos[GizmoId]*Radius;
	return (length(GizmoPos - vec2(X, Y)) < 32.0f);
}

void CAssetsEditorGui_View::OnButtonClick(int X, int Y, int Button)
{
	if(m_ViewRect.IsInside(X, Y))
	{
		if(Button == KEY_MOUSE_1)
		{
			switch(m_pAssetsEditor->m_ViewedAssetPath.GetType())
			{
				case CAssetPath::TYPE_IMAGE:
				{
					
					break;
				}
				case CAssetPath::TYPE_SKELETON:
				{
					CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(m_pAssetsEditor->m_ViewedAssetPath);
					if(!pSkeleton)
						return;
						
					switch(m_CursorTool)
					{
						case CURSORTOOL_TRANSLATE:
						case CURSORTOOL_TRANSLATE_X:
						case CURSORTOOL_TRANSLATE_Y:
						case CURSORTOOL_ROTATE:
						case CURSORTOOL_SCALE:
						case CURSORTOOL_SCALE_X:
						case CURSORTOOL_SCALE_Y:
						case CURSORTOOL_BONE_LENGTH:
						{
							CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
							SkeletonRenderer.AddSkeletonWithParents(m_pAssetsEditor->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
							SkeletonRenderer.Finalize();
							
							CAssetPath SkeletonPath;
							CAsset_Skeleton::CSubPath BonePath;
							if(SkeletonRenderer.BonePicking(GetTeePosition(), m_Zoom, vec2(X, Y), SkeletonPath, BonePath) && SkeletonPath == m_pAssetsEditor->m_ViewedAssetPath)
							{
								m_DragType = DRAGTYPE_CURSORTOOL;
								m_SelectedAsset = SkeletonPath;
								m_SelectedBone = BonePath;
								
								m_CursorPivos = vec2(X, Y);
								m_CursorToolPosition = vec2(X, Y);
							}
							break;
						}
						case CURSORTOOL_BONE_ADD:
						{
							CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
							SkeletonRenderer.AddSkeletonWithParents(m_pAssetsEditor->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
							SkeletonRenderer.Finalize();
							
							CAssetPath SkeletonPath;
							CAsset_Skeleton::CSubPath BonePath;
							if(SkeletonRenderer.BonePicking(GetTeePosition(), m_Zoom, vec2(X, Y), SkeletonPath, BonePath))
							{
								if(SkeletonPath == m_pAssetsEditor->m_ViewedAssetPath || SkeletonPath == pSkeleton->m_ParentPath)
								{
									CAsset_Skeleton::CSubPath NewBonePath = m_pAssetsEditor->AssetManager()->AddSubItem(m_pAssetsEditor->m_ViewedAssetPath, CAsset_Skeleton::SUBITEM_BONE);
									if(!NewBonePath.IsNull())
									{
										m_CursorPivos = vec2(X, Y);
										m_CursorToolPosition = vec2(X, Y);
										
										m_DragType = DRAGTYPE_CURSORTOOL;
										m_SelectedAsset = m_pAssetsEditor->m_ViewedAssetPath;
										m_SelectedBone = NewBonePath;
										
										CAsset_Skeleton::CBonePath ParentPath;
										if(SkeletonPath == pSkeleton->m_ParentPath)
											ParentPath = CAsset_Skeleton::CBonePath::Parent(BonePath.GetId());
										else
											ParentPath = CAsset_Skeleton::CBonePath::Local(BonePath.GetId());
										
										m_pAssetsEditor->AssetManager()->SetAssetValue<CAsset_Skeleton::CBonePath>(
											m_SelectedAsset,
											CAsset_Skeleton::BONE_PARENT,
											m_SelectedBone.ConvertToInteger(),
											ParentPath);
										
										m_pAssetsEditor->RefreshAssetEditor();
									}
								}
							}
							else
							{
								CAsset_Skeleton::CSubPath NewBonePath = m_pAssetsEditor->AssetManager()->AddSubItem(m_pAssetsEditor->m_ViewedAssetPath, CAsset_Skeleton::SUBITEM_BONE);
								if(!NewBonePath.IsNull())
								{
									m_CursorPivos = vec2(X, Y);
									m_CursorToolPosition = vec2(X, Y);
									
									m_DragType = DRAGTYPE_CURSORTOOL;
									m_SelectedAsset = m_pAssetsEditor->m_ViewedAssetPath;
									m_SelectedBone = NewBonePath;
									
									CAsset_Skeleton::CBonePath ParentPath;
									if(SkeletonPath == pSkeleton->m_ParentPath)
										ParentPath = CAsset_Skeleton::CBonePath::Parent(BonePath.GetId());
									else
										ParentPath = CAsset_Skeleton::CBonePath::Local(BonePath.GetId());
									
									m_pAssetsEditor->AssetManager()->SetAssetValue<CAsset_Skeleton::CBonePath>(
										m_SelectedAsset,
										CAsset_Skeleton::BONE_PARENT,
										CAsset_Skeleton::CBonePath::Null().ConvertToInteger(),
										ParentPath);
									
									m_pAssetsEditor->RefreshAssetEditor();
								}
							}
							break;
						}
						case CURSORTOOL_BONE_DELETE:
						{
							CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
							SkeletonRenderer.AddSkeletonWithParents(m_pAssetsEditor->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
							SkeletonRenderer.Finalize();
							
							CAssetPath SkeletonPath;
							CAsset_Skeleton::CSubPath BonePath;
							if(SkeletonRenderer.BonePicking(GetTeePosition(), m_Zoom, vec2(X, Y), SkeletonPath, BonePath) && SkeletonPath == m_pAssetsEditor->m_ViewedAssetPath)
							{
								m_pAssetsEditor->AssetManager()->DeleteSubItem(m_pAssetsEditor->m_ViewedAssetPath, BonePath.ConvertToInteger());
								m_pAssetsEditor->RefreshAssetEditor();
							}
							break;
						}
					}
					break;
				}
				case CAssetPath::TYPE_SKELETONANIMATION:
				{
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
					}
					else
					{				
						CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_ViewedAssetPath);
						if(!pSkeletonAnimation)
							return;
							
						CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
						if(!pSkeleton)
							return;
							
						switch(m_CursorTool)
						{
							case CURSORTOOL_TRANSLATE:
							case CURSORTOOL_TRANSLATE_X:
							case CURSORTOOL_TRANSLATE_Y:
							case CURSORTOOL_ROTATE:
							case CURSORTOOL_SCALE:
							case CURSORTOOL_SCALE_X:
							case CURSORTOOL_SCALE_Y:
							{
								CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
								SkeletonRenderer.AddSkeletonWithParents(pSkeletonAnimation->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
								SkeletonRenderer.ApplyAnimation(m_pAssetsEditor->m_ViewedAssetPath, m_pAssetsEditor->GetTime());
								SkeletonRenderer.Finalize();
								
								CAssetPath SkeletonPath;
								CAsset_Skeleton::CSubPath BonePath;
								if(SkeletonRenderer.BonePicking(GetTeePosition(), m_Zoom, vec2(X, Y), SkeletonPath, BonePath))
								{
									m_DragType = DRAGTYPE_CURSORTOOL;
									m_SelectedAsset = SkeletonPath;
									m_SelectedBone = BonePath;
									
									m_CursorPivos = vec2(X, Y);
									m_CursorToolPosition = vec2(X, Y);
								}
								break;
							}
						}
					}
				}
				case CAssetPath::TYPE_CHARACTER:
				case CAssetPath::TYPE_CHARACTERPART:
				case CAssetPath::TYPE_WEAPON:
				{
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
					}
				}
			}
		}
	}
	else
	{
		m_pToolbar->OnButtonClick(X, Y, Button);
	}
}

void CAssetsEditorGui_View::OnButtonRelease(int Button)
{
	if(Button == KEY_MOUSE_1 && m_DragType != DRAGTYPE_TILT)
	{
		m_DragType = DRAGTYPE_NONE;
	}
	
	m_pToolbar->OnButtonRelease(Button);
}

void CAssetsEditorGui_View::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_DragType == DRAGTYPE_GIZMO)
	{
		float Radius = min(m_ViewRect.w/2.0f - 20.0f, m_ViewRect.h/2.0f - 20.0f) - 48.0f;
		vec2 Diff = vec2(X, Y) - GetTeePosition();
		m_GizmoPos[m_SelectedGizmo] = normalize(Diff);
		
		float Dist = length(Diff);
		if(Dist > Radius + 32.0f)
			m_GizmoEnabled[m_SelectedGizmo] = 0;
		else
			m_GizmoEnabled[m_SelectedGizmo] = 1;
		
	}
	else if(m_DragType == DRAGTYPE_CURSORTOOL)
	{
		m_CursorToolLastPosition = m_CursorToolPosition;
		m_CursorToolPosition = vec2(X, Y);
		
		switch(m_pAssetsEditor->m_ViewedAssetPath.GetType())
		{
			case CAssetPath::TYPE_SKELETON:
			{
				vec2 Origin, AxisX, AxisY;
						
				CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
				SkeletonRenderer.AddSkeletonWithParents(m_pAssetsEditor->m_ViewedAssetPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
				SkeletonRenderer.Finalize();
				
				switch(m_CursorTool)
				{
					case CURSORTOOL_TRANSLATE:
					case CURSORTOOL_TRANSLATE_X:
					case CURSORTOOL_TRANSLATE_Y:
					{
						if(SkeletonRenderer.GetParentAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							if(m_CursorTool != CURSORTOOL_TRANSLATE_Y)
							{
								float TranslationX = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_TRANSLATION_X, m_SelectedBone.ConvertToInteger(), 0.0f);
								TranslationX += dot(AxisX, vec2(RelX, RelY));
								m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_TRANSLATION_X, m_SelectedBone.ConvertToInteger(), TranslationX);
							}
							
							if(m_CursorTool != CURSORTOOL_TRANSLATE_X)
							{
								float TranslationY = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_TRANSLATION_Y, m_SelectedBone.ConvertToInteger(), 0.0f);
								TranslationY += dot(AxisY, vec2(RelX, RelY));
								m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_TRANSLATION_Y, m_SelectedBone.ConvertToInteger(), TranslationY);
							}
						}
						break;					
					}
					case CURSORTOOL_SCALE:
					{
						if(SkeletonRenderer.GetParentAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float TranslationX = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_X, m_SelectedBone.ConvertToInteger(), 0.0f);
							float TranslationY = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_Y, m_SelectedBone.ConvertToInteger(), 0.0f);
							float Translation =  (length(m_CursorToolPosition - m_CursorPivos) - length(m_CursorToolLastPosition - m_CursorPivos))/10.0f;
							TranslationX += Translation;
							TranslationY += Translation;
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_X, m_SelectedBone.ConvertToInteger(), TranslationX);
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_Y, m_SelectedBone.ConvertToInteger(), TranslationY);
						}
						break;					
					}
					case CURSORTOOL_SCALE_X:
					case CURSORTOOL_SCALE_Y:
					{
						if(SkeletonRenderer.GetParentAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							if(m_CursorTool != CURSORTOOL_SCALE_Y)
							{
								float TranslationX = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_X, m_SelectedBone.ConvertToInteger(), 0.0f);
								TranslationX += dot(AxisX, vec2(RelX, RelY))/5.0f;
								m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_X, m_SelectedBone.ConvertToInteger(), TranslationX);
							}
							
							if(m_CursorTool != CURSORTOOL_SCALE_X)
							{
								float TranslationY = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_Y, m_SelectedBone.ConvertToInteger(), 0.0f);
								TranslationY += dot(AxisY, vec2(RelX, RelY))/5.0f;
								m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_SCALE_Y, m_SelectedBone.ConvertToInteger(), TranslationY);
							}
						}
						break;					
					}
					case CURSORTOOL_ROTATE:
					{
						if(SkeletonRenderer.GetLocalAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float Angle = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), 0.0f);
							Angle += angle(m_CursorToolLastPosition - Origin, m_CursorToolPosition - Origin);
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
						}
						break;				
					}
					case CURSORTOOL_BONE_LENGTH:
					{
						if(SkeletonRenderer.GetLocalAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float Angle = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), 0.0f);
							float Length = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_LENGTH, m_SelectedBone.ConvertToInteger(), 32.0f);
							Angle += angle(m_CursorToolLastPosition - Origin, m_CursorToolPosition - Origin);
							Length += dot(AxisX, vec2(RelX, RelY));
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_LENGTH, m_SelectedBone.ConvertToInteger(), Length);
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
						}
						break;					
					}
					case CURSORTOOL_BONE_ADD:
					{
						if(SkeletonRenderer.GetLocalAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							vec2 RelPos = (m_CursorToolPosition - Origin);
							float LengthX = dot(AxisX, RelPos);
							float LengthY = dot(AxisY, RelPos);
							float Length = sqrt(LengthX*LengthX + LengthY*LengthY);
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_LENGTH, m_SelectedBone.ConvertToInteger(), Length);
							float Angle = angle(AxisX, RelPos);
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
						}
						vec2 OriginParent, AxisXParent, AxisYParent;
						if(SkeletonRenderer.GetParentAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, OriginParent, AxisXParent, AxisYParent))
						{
							float Angle = angle(AxisXParent, m_CursorToolPosition - Origin);
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_SelectedAsset, CAsset_Skeleton::BONE_ANGLE, m_SelectedBone.ConvertToInteger(), Angle);
						}
						break;					
					}
				}
				break;
			}
			case CAssetPath::TYPE_SKELETONANIMATION:
			{
				CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_ViewedAssetPath);
				if(!pSkeletonAnimation)
					return;
				
				float Time = m_pAssetsEditor->GetTime();
				int Frame = static_cast<int>(round(Time*static_cast<float>(TU_SKELETONANIMATION_TIMESTEP)));
				int BoneId = m_SelectedBone.GetId();
				CAsset_SkeletonAnimation::CSubPath FramePath = pSkeletonAnimation->GetBoneKeyFramePath(CAsset_Skeleton::CBonePath::Local(BoneId), Frame);
				if(FramePath.IsNull())
					return;
				
				CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
				if(!pSkeleton)
					return;
					
				vec2 Origin, AxisX, AxisY;
						
				CSkeletonRenderer SkeletonRenderer(Graphics(), m_pAssetsEditor->AssetManager());
				SkeletonRenderer.AddSkeletonWithParents(pSkeletonAnimation->m_SkeletonPath, CSkeletonRenderer::ADDDEFAULTSKIN_YES);
				SkeletonRenderer.ApplyAnimation(m_pAssetsEditor->m_ViewedAssetPath, m_pAssetsEditor->GetTime());
				SkeletonRenderer.Finalize();
						
				switch(m_CursorTool)
				{
					case CURSORTOOL_TRANSLATE:
					case CURSORTOOL_TRANSLATE_X:
					case CURSORTOOL_TRANSLATE_Y:
					{
						if(SkeletonRenderer.GetParentAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							if(m_CursorTool != CURSORTOOL_TRANSLATE_Y)
							{
								float TranslationX = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_X, FramePath.ConvertToInteger(), 0.0f);
								TranslationX += dot(AxisX, vec2(RelX, RelY));
								m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_X, FramePath.ConvertToInteger(), TranslationX);
							}
							
							if(m_CursorTool != CURSORTOOL_TRANSLATE_X)
							{
								float TranslationY = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_Y, FramePath.ConvertToInteger(), 0.0f);
								TranslationY += dot(AxisY, vec2(RelX, RelY));
								m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_Y, FramePath.ConvertToInteger(), TranslationY);
							}
						}
						break;					
					}
					case CURSORTOOL_SCALE:
					{
						if(SkeletonRenderer.GetParentAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float TranslationX = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, FramePath.ConvertToInteger(), 0.0f);
							float TranslationY = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, FramePath.ConvertToInteger(), 0.0f);
							float Translation =  (length(m_CursorToolPosition - m_CursorPivos) - length(m_CursorToolLastPosition - m_CursorPivos))/10.0f;
							TranslationX += Translation;
							TranslationY += Translation;
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, FramePath.ConvertToInteger(), TranslationX);
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, FramePath.ConvertToInteger(), TranslationY);
						}
						break;					
					}
					case CURSORTOOL_SCALE_X:
					case CURSORTOOL_SCALE_Y:
					{
						if(SkeletonRenderer.GetParentAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							if(m_CursorTool != CURSORTOOL_SCALE_Y)
							{
								float TranslationX = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, FramePath.ConvertToInteger(), 0.0f);
								TranslationX += dot(AxisX, vec2(RelX, RelY))/5.0f;
								m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, FramePath.ConvertToInteger(), TranslationX);
							}
							
							if(m_CursorTool != CURSORTOOL_SCALE_X)
							{
								float TranslationY = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, FramePath.ConvertToInteger(), 0.0f);
								TranslationY += dot(AxisY, vec2(RelX, RelY))/5.0f;
								m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, FramePath.ConvertToInteger(), TranslationY);
							}
						}
						break;					
					}
					case CURSORTOOL_ROTATE:
					{
						if(SkeletonRenderer.GetLocalAxis(GetTeePosition(), m_Zoom, m_SelectedAsset, m_SelectedBone, Origin, AxisX, AxisY))
						{
							float Angle = m_pAssetsEditor->AssetManager()->GetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_ANGLE, FramePath.ConvertToInteger(), 0.0f);
							Angle += angle(m_CursorToolLastPosition - Origin, m_CursorToolPosition - Origin);
							m_pAssetsEditor->AssetManager()->SetAssetValue<float>(m_pAssetsEditor->m_ViewedAssetPath, CAsset_SkeletonAnimation::BONEKEYFRAME_ANGLE, FramePath.ConvertToInteger(), Angle);
						}
						break;				
					}
				}
				break;
			}
		}
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

void CAssetsEditorGui_View::ZoomCallback(float Pos)
{
	float MinZoom = 0.1;
	float MaxScale = 10.0f;
	float ScaleWidth = MaxScale - MinZoom;
	m_Zoom = MinZoom + Pos * Pos * ScaleWidth;
}

}
