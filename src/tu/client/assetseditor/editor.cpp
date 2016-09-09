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

#include "editor.h"

namespace tu
{

namespace assetseditor
{

const char* CEditor::CAssetEdit::m_aNoneText = "None";
const char* CEditor::CBoneEdit::m_aNoneText = "None";
const char* CEditor::CLayerEdit::m_aNoneText = "None";
const char* CEditor::CCharacterPartEdit::m_aNoneText = "None";

const char* g_aCycleTypeText[] = {
    "Clamp",
    "Loop",
};
const char* g_aSpriteAlignText[] = {
    "World",
    "Bone",
};
const char* g_aLayerStateText[] = {
    "Visible",
    "Hidden",
};
const char* g_aBoneAlignText[] = {
    "Parent",
    "World",
    "Aim",
    "Motion",
    "Hook",
};

CEditor::CMeshPreview::CMeshPreview(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int SubPath) :
	gui::CWidget(pAssetsEditor->m_pGuiConfig),
	m_pAssetsEditor(pAssetsEditor),
	m_AssetPath(AssetPath),
	m_SubPath(SubPath)
{
	
}

void CEditor::CMeshPreview::Render()
{
	
}

CEditor::CEditor(CAssetsEditor* pAssetsEditor) :
	gui::CTabs(pAssetsEditor->m_pGuiConfig),
	m_LastEditedAssetType(-1),
	m_pAssetsEditor(pAssetsEditor)
{
	for(int i=0; i<NUM_TABS; i++)
	{
		m_pTabs[i] = 0;
	}
}

void CEditor::AddAssetTabCommons(gui::CVListLayout* pList)
{
	pList->Clear();
	AddTextField(pList, CAsset::NAME, -1, sizeof(CAsset::m_aName), "Name:");
	{
		gui::CLabel* pLabel;
		char aBuf[64];
		
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
			str_format(aBuf, sizeof(aBuf), "%s #%d", AssetTypeName, m_pAssetsEditor->m_EditedAssetPath.GetId());\
			break;

		switch(m_pAssetsEditor->m_EditedAssetPath.GetType())
		{
			#include <tu/client/assetsmacro.h>
		}
		
		#undef TU_MACRO_ASSETTYPE
		
		pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, aBuf);
		AddField(pList, pLabel, "Asset reference");
	}
	
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
		pLayout->SetHeight(m_pConfig->m_ButtonHeight);
		pList->Add(pLayout);
		
		pLayout->Add(new CEditor::CDuplicateAsset(m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath));
		pLayout->Add(new CEditor::CDeleteAsset(m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath));
	}
	
	pList->AddSeparator();
}

void CEditor::AddSubTitle(gui::CVListLayout* pList, const char* pText)
{
	pList->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, pText, gui::TEXTSTYLE_HEADER2));
}

void CEditor::AddField(gui::CVListLayout* pList, gui::CWidget* pWidget, const char* pLabelText)
{
	if(pLabelText)
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
		pLayout->SetHeight(m_pConfig->m_ButtonHeight);
		pList->Add(pLayout);
		
		gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, pLabelText);
		pLayout->Add(pLabel);
		
		pLayout->Add(pWidget);
	}
	else
	{
		pList->Add(pWidget);
	}
}

void CEditor::AddFloatField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CEditor::CFloatAssetMemberEdit* pWidget = new CEditor::CFloatAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId);
	pWidget->SetToken(m_pAssetsEditor->AssetsManager()->GenerateToken());
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddAngleField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CEditor::CAngleEdit* pWidget = new CEditor::CAngleEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId);
	pWidget->SetToken(m_pAssetsEditor->AssetsManager()->GenerateToken());
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddFloat2Field(gui::CVListLayout* pList, int Member1, int Member2, int SubId, const char* pLabelText)
{
	gui::CHListLayout* pWidget = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
	pWidget->SetHeight(m_pConfig->m_ButtonHeight);
	
	int Token = m_pAssetsEditor->AssetsManager()->GenerateToken();
	
	CEditor::CFloatAssetMemberEdit* pEdit1 = new CEditor::CFloatAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member1, SubId);
	pEdit1->SetToken(Token);
	pWidget->Add(pEdit1);
	
	CEditor::CFloatAssetMemberEdit* pEdit2 = new CEditor::CFloatAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member2, SubId);
	pEdit2->SetToken(Token);
	pWidget->Add(pEdit2);
			
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddIntegerField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CEditor::CIntegerAssetMemberEdit* pWidget = new CEditor::CIntegerAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId);
	pWidget->SetToken(m_pAssetsEditor->AssetsManager()->GenerateToken());
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddCycleField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CEditor::CEnumEdit* pWidget = new CEditor::CEnumEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, g_aCycleTypeText, sizeof(g_aCycleTypeText)/sizeof(const char*));
	pWidget->SetToken(m_pAssetsEditor->AssetsManager()->GenerateToken());
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddSpriteAlignField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CEditor::CEnumEdit* pWidget = new CEditor::CEnumEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, g_aSpriteAlignText, sizeof(g_aSpriteAlignText)/sizeof(const char*));
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddLayerStateField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CEditor::CEnumEdit* pWidget = new CEditor::CEnumEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, g_aLayerStateText, sizeof(g_aLayerStateText)/sizeof(const char*));
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddBoneAlignField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CEditor::CEnumEdit* pWidget = new CEditor::CEnumEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, g_aBoneAlignText, sizeof(g_aBoneAlignText)/sizeof(const char*));
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddColorField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CEditor::CColorEdit* pWidget = new CEditor::CColorEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId);
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddTextField(gui::CVListLayout* pList, int Member, int SubId, int TextSize, const char* pLabelText)
{
	CEditor::CTextAssetMemberEdit* pWidget = new CEditor::CTextAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, TextSize);
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddAssetField(gui::CVListLayout* pList, int Member, int AssetType, int SubId, const char* pLabelText)
{
	CEditor::CAssetEdit* pWidget = new CEditor::CAssetEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, AssetType);
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddBoneField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath SkeletonPath, const char* pLabelText)
{
	CEditor::CBoneEdit* pWidget = new CEditor::CBoneEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, SkeletonPath);
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddLayerField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath SkeletonPath, const char* pLabelText)
{
	CEditor::CLayerEdit* pWidget = new CEditor::CLayerEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, SkeletonPath);
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::AddCharacterPartField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath CharacterPath, const char* pLabelText)
{
	CEditor::CCharacterPartEdit* pWidget = new CEditor::CCharacterPartEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, CharacterPath);
	
	AddField(pList, pWidget, pLabelText);
}

void CEditor::RefreshTab_Image_Asset(bool KeepStatus)
{
	CAsset_Image* pImage = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Image>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pImage)
		return;
	
	//Size
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
		pLayout->SetHeight(m_pConfig->m_ButtonHeight);
		m_pTabs[TAB_ASSET]->Add(pLayout);
		
		gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Size:");
		pLayout->Add(pLabel);
		
		int Width = m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(m_pAssetsEditor->m_EditedAssetPath, CAsset_Image::WIDTH, -1, 0);
		int Height = m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(m_pAssetsEditor->m_EditedAssetPath, CAsset_Image::HEIGHT, -1, 0);
		char aBuf[64];
		str_format(aBuf, sizeof(aBuf), "%d x %d", Width, Height);
		
		gui::CLabel* pValue = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, aBuf);
		pLayout->Add(pValue);
	}
	
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Image::GRIDWIDTH, -1, "Grid width:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Image::GRIDHEIGHT, -1, "Grid height:");
}

void CEditor::RefreshTab_Sprite_Asset(bool KeepStatus)
{	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Sprite::IMAGEPATH, CAssetPath::TYPE_IMAGE, -1, "Image:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Sprite::X, -1, "X:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Sprite::Y, -1, "Y:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Sprite::WIDTH, -1, "Width:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Sprite::HEIGHT, -1, "Height:");
}

void CEditor::RefreshTab_Skeleton_Asset(bool KeepStatus)
{	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Skeleton::PARENTPATH, CAssetPath::TYPE_SKELETON, -1, "Parent:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Skeleton::SKINPATH, CAssetPath::TYPE_SKELETONSKIN, -1, "Default Skin:");
}

void CEditor::RefreshTab_Skeleton_Bones(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETON_BONES])
	{
		Scroll = m_pLists[LIST_SKELETON_BONES]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETON_BONES]->Clear();
	m_pLists[LIST_SKELETON_BONES] = 0;
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeleton)
		return;
		
	m_pLists[LIST_SKELETON_BONES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETON_BONES]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETON_BONES]->Add(m_pLists[LIST_SKELETON_BONES]);
	m_pLists[LIST_SKELETON_BONES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Bones", gui::TEXTSTYLE_HEADER2));
	
	for(int i=0; i<pSkeleton->m_Bones.size(); i++)
	{
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::SpriteUniverse(SPRITE_ICON_BONE));
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_Skeleton::CSubPath::LocalBone(i));
		pItem->SetText(m_pAssetsEditor->m_EditedAssetPath, CAsset_Skeleton::BONE_NAME, CAsset_Skeleton::CSubPath::LocalBone(i));
		m_pLists[LIST_SKELETON_BONES]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_SKELETON_BONES]->SetScrollPos(Scroll);
	}
	
	m_pTabs[TAB_SKELETON_BONES]->Add(new CAddSubItem(
		m_pAssetsEditor,
		m_pAssetsEditor->m_EditedAssetPath,
		CAsset_Skeleton::CSubPath::TYPE_BONE,
		"Add bone",
		CAssetPath::SpriteUniverse(SPRITE_ICON_INCREASE),
		TAB_SKELETON_BONES
	));

	m_pTabs[TAB_SKELETON_BONES]->AddSeparator();	
		
	if(m_pAssetsEditor->m_EditedAssetSubPath >= 0 && CAsset_Skeleton::CSubPath(m_pAssetsEditor->m_EditedAssetSubPath).GetType() == CAsset_Skeleton::CSubPath::TYPE_BONE)
	{
		AddTextField(m_pTabs[TAB_SKELETON_BONES], CAsset_Skeleton::BONE_NAME, m_pAssetsEditor->m_EditedAssetSubPath, sizeof(CAsset_Skeleton::CBone::m_aName), "Name:");
		AddColorField(m_pTabs[TAB_SKELETON_BONES], CAsset_Skeleton::BONE_COLOR, m_pAssetsEditor->m_EditedAssetSubPath, "Color:");
		AddBoneField(m_pTabs[TAB_SKELETON_BONES], CAsset_Skeleton::BONE_PARENT, m_pAssetsEditor->m_EditedAssetSubPath, m_pAssetsEditor->m_EditedAssetPath, "Parent:");
		AddFloatField(m_pTabs[TAB_SKELETON_BONES], CAsset_Skeleton::BONE_LENGTH, m_pAssetsEditor->m_EditedAssetSubPath, "Length:");
		AddFloatField(m_pTabs[TAB_SKELETON_BONES], CAsset_Skeleton::BONE_ANCHOR, m_pAssetsEditor->m_EditedAssetSubPath, "Anchor:");
		AddFloat2Field(m_pTabs[TAB_SKELETON_BONES], CAsset_Skeleton::BONE_TRANSLATION_X, CAsset_Skeleton::BONE_TRANSLATION_Y, m_pAssetsEditor->m_EditedAssetSubPath, "Translation:");
		AddFloat2Field(m_pTabs[TAB_SKELETON_BONES], CAsset_Skeleton::BONE_SCALE_X, CAsset_Skeleton::BONE_SCALE_Y, m_pAssetsEditor->m_EditedAssetSubPath, "Scale:");
		AddAngleField(m_pTabs[TAB_SKELETON_BONES], CAsset_Skeleton::BONE_ANGLE, m_pAssetsEditor->m_EditedAssetSubPath, "Angle:");	
	}
}
	
void CEditor::RefreshTab_Skeleton_Layers(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETON_LAYERS])
	{
		Scroll = m_pLists[LIST_SKELETON_LAYERS]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETON_LAYERS]->Clear();
			
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeleton)
		return;
			
	m_pLists[LIST_SKELETON_LAYERS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETON_LAYERS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETON_LAYERS]->Add(m_pLists[LIST_SKELETON_LAYERS]);
	m_pLists[LIST_SKELETON_LAYERS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Layers", gui::TEXTSTYLE_HEADER2));
	
	for(int i=0; i<pSkeleton->m_Layers.size(); i++)
	{
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::SpriteUniverse(SPRITE_ICON_LAYERS));
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_Skeleton::CSubPath::LocalLayer(i));
		pItem->SetText(m_pAssetsEditor->m_EditedAssetPath, CAsset_Skeleton::LAYER_NAME, CAsset_Skeleton::CSubPath::LocalLayer(i));
		m_pLists[LIST_SKELETON_LAYERS]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_SKELETON_LAYERS]->SetScrollPos(Scroll);
	}

	m_pTabs[TAB_SKELETON_LAYERS]->Add(new CAddSubItem(
		m_pAssetsEditor,
		m_pAssetsEditor->m_EditedAssetPath,
		CAsset_Skeleton::CSubPath::TYPE_LAYER,
		"Add layer",
		CAssetPath::SpriteUniverse(SPRITE_ICON_INCREASE),
		TAB_SKELETON_LAYERS
	));
	
	m_pTabs[TAB_SKELETON_LAYERS]->AddSeparator();
		
	if(m_pAssetsEditor->m_EditedAssetSubPath >= 0 && CAsset_Skeleton::CSubPath(m_pAssetsEditor->m_EditedAssetSubPath).GetType() == CAsset_Skeleton::CSubPath::TYPE_LAYER)
	{
		AddTextField(m_pTabs[TAB_SKELETON_LAYERS], CAsset_Skeleton::LAYER_NAME, m_pAssetsEditor->m_EditedAssetSubPath, sizeof(CAsset_Skeleton::CLayer::m_aName), "Name:");
	}
}

void CEditor::RefreshTab_SkeletonSkin_Asset(bool KeepStatus)
{
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_SkeletonSkin::SKELETONPATH, CAssetPath::TYPE_SKELETON, -1, "Skeleton:");
}

void CEditor::RefreshTab_SkeletonSkin_Sprites(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETONSKIN_SPRITES])
	{
		Scroll = m_pLists[LIST_SKELETONSKIN_SPRITES]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETONSKIN_SPRITES]->Clear();
			
	CAsset_SkeletonSkin* pSkeletonSkin = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonSkin>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonSkin)
		return;
		
	m_pLists[LIST_SKELETONSKIN_SPRITES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pTabs[TAB_SKELETONSKIN_SPRITES]->Add(m_pLists[LIST_SKELETONSKIN_SPRITES]);
	
	m_pLists[LIST_SKELETONSKIN_SPRITES]->Add(new CAddSubItem(
		m_pAssetsEditor,
		m_pAssetsEditor->m_EditedAssetPath,
		CAsset_SkeletonSkin::CSubPath::TYPE_SPRITE,
		"Add sprite",
		CAssetPath::SpriteUniverse(SPRITE_ICON_INCREASE),
		TAB_SKELETONSKIN_SPRITES
	));
	
	m_pLists[LIST_SKELETONSKIN_SPRITES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Sprites", gui::TEXTSTYLE_HEADER2));
	
	for(int i=0; i<pSkeletonSkin->m_Sprites.size(); i++)
	{
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::SpriteUniverse(SPRITE_ICON_SPRITE));
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonSkin::CSubPath::Sprite(i));
		pItem->SetText(pSkeletonSkin->m_Sprites[i].m_SpritePath, CAsset::NAME, -1);
		m_pLists[LIST_SKELETONSKIN_SPRITES]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_SKELETONSKIN_SPRITES]->SetScrollPos(Scroll);
	}
		
	if(m_pAssetsEditor->m_EditedAssetSubPath >= 0)
	{
		m_pTabs[TAB_SKELETONSKIN_SPRITES]->AddSeparator();	
		
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_pTabs[TAB_SKELETONSKIN_SPRITES]->Add(pLayout);
			
			//~ pLayout->Add(new CEditor::CDuplicateSubItem(m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath));
			pLayout->Add(new CEditor::CDeleteSubItem(m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, m_pAssetsEditor->m_EditedAssetSubPath));
		}
		
		AddAssetField(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAsset_SkeletonSkin::SPRITE_PATH, CAssetPath::TYPE_SPRITE, m_pAssetsEditor->m_EditedAssetSubPath, "Sprite:");
		AddBoneField(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAsset_SkeletonSkin::SPRITE_BONE, m_pAssetsEditor->m_EditedAssetSubPath, pSkeletonSkin->m_SkeletonPath, "Bone:");
		AddLayerField(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAsset_SkeletonSkin::SPRITE_LAYER, m_pAssetsEditor->m_EditedAssetSubPath, pSkeletonSkin->m_SkeletonPath, "Layer:");
		AddSpriteAlignField(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAsset_SkeletonSkin::SPRITE_ALIGNMENT, m_pAssetsEditor->m_EditedAssetSubPath, "Alignment:");
		AddFloat2Field(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAsset_SkeletonSkin::SPRITE_TRANSLATION_X, CAsset_SkeletonSkin::SPRITE_TRANSLATION_Y, m_pAssetsEditor->m_EditedAssetSubPath, "Translation:");
		AddFloat2Field(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAsset_SkeletonSkin::SPRITE_SCALE_X, CAsset_SkeletonSkin::SPRITE_SCALE_Y, m_pAssetsEditor->m_EditedAssetSubPath, "Scale:");
		AddAngleField(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAsset_SkeletonSkin::SPRITE_ANGLE, m_pAssetsEditor->m_EditedAssetSubPath, "Angle:");
		AddFloatField(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAsset_SkeletonSkin::SPRITE_ANCHOR, m_pAssetsEditor->m_EditedAssetSubPath, "Anchor:");
	}
}

void CEditor::RefreshTab_SkeletonAnimation_Asset(bool KeepStatus)
{
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_SkeletonAnimation::SKELETONPATH, CAssetPath::TYPE_SKELETON, -1, "Skeleton:");
}

void CEditor::RefreshTab_SkeletonAnimation_Animations(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETONANIMATION_ANIMATIONS])
	{
		Scroll = m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS]->Clear();
			
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonAnimation)
		return;
			
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return;
		
	m_pLists[LIST_SKELETONANIMATION_ANIMATIONS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS]->Add(m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]);
	
	m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Bone Animations", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pSkeletonAnimation->m_BoneAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath BonePath = pSkeletonAnimation->m_BoneAnimations[i].m_BonePath;
		
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::SpriteUniverse(SPRITE_ICON_SKELETONANIMATION));
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::CSubPath::BoneAnimation(i));
		
		if(BonePath.GetSource() == CAsset_Skeleton::CSubPath::SRC_PARENT)
			pItem->SetText(pSkeleton->m_ParentPath, CAsset_Skeleton::BONE_NAME, BonePath.Local());
		else
			pItem->SetText(pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::BONE_NAME, BonePath);
		
		m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->Add(pItem);
	}
	
	m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Layer Animations", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pSkeletonAnimation->m_LayerAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath LayerPath = pSkeletonAnimation->m_LayerAnimations[i].m_LayerPath;
		
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::SpriteUniverse(SPRITE_ICON_LAYERANIMATION));
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::CSubPath::LayerAnimation(i));
		
		if(LayerPath.GetSource() == CAsset_Skeleton::CSubPath::SRC_PARENT)
			pItem->SetText(pSkeleton->m_ParentPath, CAsset_Skeleton::LAYER_NAME, LayerPath.Local());
		else
			pItem->SetText(pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::LAYER_NAME, LayerPath);
		
		m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->SetScrollPos(Scroll);
	}

	m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS]->AddSeparator();	
		
	CAsset_Skeleton::CSubPath EditedSubPath(m_pAssetsEditor->m_EditedAssetSubPath);
	if(!EditedSubPath.IsNull() && EditedSubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEANIMATION)
	{
		AddCycleField(m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS], CAsset_SkeletonAnimation::BONEANIMATION_CYCLETYPE, m_pAssetsEditor->m_EditedAssetSubPath, "Cycle type:");
	}
}

void CEditor::RefreshTab_SkeletonAnimation_KeyFrames(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETONANIMATION_KEYFRAMES])
	{
		Scroll = m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES]->Clear();
	
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonAnimation)
		return;
			
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return;
		
	m_pLists[LIST_SKELETONANIMATION_KEYFRAMES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES]->Add(m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]);
	
	m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Bone Key Frames", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pSkeletonAnimation->m_BoneAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath BonePath = pSkeletonAnimation->m_BoneAnimations[i].m_BonePath;
		const char* pBoneName;
		
		if(BonePath.GetSource() == CAsset_Skeleton::CSubPath::SRC_PARENT)
			pBoneName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(pSkeleton->m_ParentPath, CAsset_Skeleton::BONE_NAME, BonePath.Local(), 0);
		else
			pBoneName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::BONE_NAME, BonePath, 0);
				
		for(int j=0; j<pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames.size(); j++)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "%s, frame #%d", pBoneName, pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames[j].m_Time);
			
			CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::SpriteUniverse(SPRITE_ICON_KEYFRAME_BONE));
			pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::CSubPath::BoneKeyFrame(i, j));
			pItem->SetText(aBuf);
			m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->Add(pItem);
		}
	}

	m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Layer Key Frames", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pSkeletonAnimation->m_LayerAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath LayerPath = pSkeletonAnimation->m_LayerAnimations[i].m_LayerPath;
		const char* pLayerName;
		
		if(LayerPath.GetSource() == CAsset_Skeleton::CSubPath::SRC_PARENT)
			pLayerName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(pSkeleton->m_ParentPath, CAsset_Skeleton::LAYER_NAME, LayerPath.Local(), 0);
		else
			pLayerName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::LAYER_NAME, LayerPath, 0);
		
		for(int j=0; j<pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames.size(); j++)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "%s, frame #%d", pLayerName, pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames[j].m_Time);
			
			CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::SpriteUniverse(SPRITE_ICON_KEYFRAME_LAYER));
			pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::CSubPath::LayerKeyFrame(i, j));
			pItem->SetText(aBuf);
			m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->Add(pItem);
		}
	}
	if(KeepStatus)
	{
		m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->SetScrollPos(Scroll);
	}
	
	m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES]->AddSeparator();	
		
	CAsset_Skeleton::CSubPath EditedSubPath(m_pAssetsEditor->m_EditedAssetSubPath);
	if(!EditedSubPath.IsNull())
	{
		if(EditedSubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
		{
			AddFloat2Field(m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES], CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_X, CAsset_SkeletonAnimation::BONEKEYFRAME_TRANSLATION_Y, m_pAssetsEditor->m_EditedAssetSubPath, "Translation:");
			AddFloat2Field(m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES], CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_X, CAsset_SkeletonAnimation::BONEKEYFRAME_SCALE_Y, m_pAssetsEditor->m_EditedAssetSubPath, "Scale:");
			AddAngleField(m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES], CAsset_SkeletonAnimation::BONEKEYFRAME_ANGLE, m_pAssetsEditor->m_EditedAssetSubPath, "Angle:");
			AddBoneAlignField(m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES], CAsset_SkeletonAnimation::BONEKEYFRAME_ALIGNMENT, m_pAssetsEditor->m_EditedAssetSubPath, "Alignment:");
		}
		else if(EditedSubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
		{
			AddColorField(m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES], CAsset_SkeletonAnimation::LAYERKEYFRAME_COLOR, m_pAssetsEditor->m_EditedAssetSubPath, "Color:");
			AddLayerStateField(m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES], CAsset_SkeletonAnimation::LAYERKEYFRAME_STATE, m_pAssetsEditor->m_EditedAssetSubPath, "State:");
		}
	}
}

void CEditor::RefreshTab_Character_Asset(bool KeepStatus)
{
	CAsset_Character* pCharacter = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Character>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pCharacter)
		return;
	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Character::IDLEPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Idle Animation:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Character::WALKPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Walk Animation:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Character::CONTROLLEDJUMPPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Jump Animation:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Character::UNCONTROLLEDJUMPPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Jump2 Animation:");
}

void CEditor::RefreshTab_Character_Parts(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_CHARACTER_PARTS])
	{
		Scroll = m_pLists[LIST_CHARACTER_PARTS]->GetScrollPos();
	}
	m_pTabs[TAB_CHARACTER_PARTS]->Clear();
			
	CAsset_Character* pCharacter = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Character>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pCharacter)
		return;
		
	m_pLists[LIST_CHARACTER_PARTS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_CHARACTER_PARTS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_CHARACTER_PARTS]->Add(m_pLists[LIST_CHARACTER_PARTS]);
	
	m_pLists[LIST_CHARACTER_PARTS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Parts", gui::TEXTSTYLE_HEADER2));
	CAsset_Character::CIteratorPart Iter;
	for(Iter = pCharacter->BeginPart(); Iter != pCharacter->EndPart(); ++Iter)
	{
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::SpriteUniverse(SPRITE_ICON_CHARACTERPART));
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, *Iter);
		pItem->SetText(m_pAssetsEditor->m_EditedAssetPath, CAsset_Character::PART_NAME, *Iter);
		
		m_pLists[LIST_CHARACTER_PARTS]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_CHARACTER_PARTS]->SetScrollPos(Scroll);
	}

	m_pTabs[TAB_CHARACTER_PARTS]->Add(new CAddSubItem(
		m_pAssetsEditor,
		m_pAssetsEditor->m_EditedAssetPath,
		CAsset_Character::CSubPath::TYPE_PART,
		"Add part",
		CAssetPath::SpriteUniverse(SPRITE_ICON_INCREASE),
		TAB_CHARACTER_PARTS
	));

	m_pTabs[TAB_CHARACTER_PARTS]->AddSeparator();	
	
	CAsset_Character::CSubPath EditedSubPath(m_pAssetsEditor->m_EditedAssetSubPath);
	if(!EditedSubPath.IsNull() && EditedSubPath.GetType() == CAsset_Character::CSubPath::TYPE_PART)
	{
		AddTextField(m_pTabs[TAB_CHARACTER_PARTS], CAsset_Character::PART_NAME, m_pAssetsEditor->m_EditedAssetSubPath, sizeof(CAsset_Character::CPart::m_aName), "Name:");
		AddAssetField(m_pTabs[TAB_CHARACTER_PARTS], CAsset_Character::PART_DEFAULTPATH, CAssetPath::TYPE_CHARACTERPART, EditedSubPath, "Default part:");
	}
}

void CEditor::RefreshTab_CharacterPart_Asset(bool KeepStatus)
{
	CAsset_CharacterPart* pCharacterPart = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_CharacterPart>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pCharacterPart)
		return;
	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_CharacterPart::CHARACTERPATH, CAssetPath::TYPE_CHARACTER, -1, "Character:");
	AddCharacterPartField(m_pTabs[TAB_ASSET], CAsset_CharacterPart::CHARACTERPART, -1, pCharacterPart->GetCharacterPath(), "Part:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_CharacterPart::SKELETONSKINPATH, CAssetPath::TYPE_SKELETONSKIN, -1, "Skin:");
}

void CEditor::RefreshTab_Weapon_Asset(bool KeepStatus)
{
	CAsset_Weapon* pWeapon = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Weapon>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pWeapon)
		return;
	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Weapon::CHARACTERPATH, CAssetPath::TYPE_CHARACTER, -1, "Character:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Weapon::CURSORPATH, CAssetPath::TYPE_SPRITE, -1, "Cursor:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Weapon::SKINPATH, CAssetPath::TYPE_SKELETONSKIN, -1, "Skin:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Weapon::ATTACKANIMATIONPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Attack Animation:");
}

void CEditor::RefreshTab_Map_Asset(bool KeepStatus)
{
	CAsset_Map* pMap = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Map>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMap)
		return;
}

void CEditor::RefreshTab_Map_Groups(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_MAP_GROUPS])
	{
		Scroll = m_pLists[LIST_MAP_GROUPS]->GetScrollPos();
	}
	m_pTabs[TAB_MAP_GROUPS]->Clear();
	
	CAsset_Map* pMap = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Map>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMap)
		return;
		
	m_pLists[LIST_MAP_GROUPS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_MAP_GROUPS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_MAP_GROUPS]->Add(m_pLists[LIST_MAP_GROUPS]);
	
	{
		m_pLists[LIST_MAP_GROUPS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Zone layers", gui::TEXTSTYLE_HEADER2));
	
		CAsset_Map::CIteratorZoneLayer Iter;
		for(Iter = pMap->BeginZoneLayer(); Iter != pMap->EndZoneLayer(); ++Iter)
		{
			CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, -1);
			pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, *Iter);
			pItem->SetText(pMap->GetZoneLayer(*Iter), CAsset::NAME, -1);
			
			m_pLists[LIST_MAP_GROUPS]->Add(pItem);
		}
	}
	{
		m_pLists[LIST_MAP_GROUPS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Background", gui::TEXTSTYLE_HEADER2));
	
		CAsset_Map::CIteratorBgGroup Iter;
		for(Iter = pMap->BeginBgGroup(); Iter != pMap->EndBgGroup(); ++Iter)
		{
			CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, -1);
			pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, *Iter);
			pItem->SetText(pMap->GetBgGroup(*Iter), CAsset::NAME, -1);
			
			m_pLists[LIST_MAP_GROUPS]->Add(pItem);
		}
	}
	{
		m_pLists[LIST_MAP_GROUPS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Foreground", gui::TEXTSTYLE_HEADER2));
	
		CAsset_Map::CIteratorFgGroup Iter;
		for(Iter = pMap->BeginFgGroup(); Iter != pMap->EndFgGroup(); ++Iter)
		{
			CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, -1);
			pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, *Iter);
			pItem->SetText(pMap->GetFgGroup(*Iter), CAsset::NAME, -1);
			
			m_pLists[LIST_MAP_GROUPS]->Add(pItem);
		}
	}
	if(KeepStatus)
	{
		m_pLists[LIST_MAP_GROUPS]->SetScrollPos(Scroll);
	}

	m_pTabs[TAB_MAP_GROUPS]->AddSeparator();
}

void CEditor::RefreshTab_MapGroup_Asset(bool KeepStatus)
{
	CAsset_MapGroup* pMapGroup = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapGroup>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapGroup)
		return;
	
	AddFloat2Field(m_pTabs[TAB_ASSET], CAsset_MapGroup::POSITION_X, CAsset_MapGroup::POSITION_Y, -1, "Position");
	AddFloat2Field(m_pTabs[TAB_ASSET], CAsset_MapGroup::HARDPARALLAX_X, CAsset_MapGroup::HARDPARALLAX_Y, -1, "Parallax");
}

void CEditor::RefreshTab_MapGroup_Layers(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_MAPGROUP_LAYERS])
	{
		Scroll = m_pLists[LIST_MAPGROUP_LAYERS]->GetScrollPos();
	}
	m_pTabs[TAB_MAPGROUP_LAYERS]->Clear();
			
	CAsset_MapGroup* pMapGroup = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapGroup>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapGroup)
		return;
		
	m_pLists[LIST_MAPGROUP_LAYERS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_MAPGROUP_LAYERS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_MAPGROUP_LAYERS]->Add(m_pLists[LIST_MAPGROUP_LAYERS]);
	
	m_pLists[LIST_MAPGROUP_LAYERS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Layers", gui::TEXTSTYLE_HEADER2));
	
	CAsset_MapGroup::CIteratorLayer Iter;
	for(Iter = pMapGroup->BeginLayer(); Iter != pMapGroup->EndLayer(); ++Iter)
	{
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, -1);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, *Iter);
		pItem->SetText(pMapGroup->GetLayer(*Iter), CAsset::NAME, -1);
		
		m_pLists[LIST_MAPGROUP_LAYERS]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_MAPGROUP_LAYERS]->SetScrollPos(Scroll);
	}

	m_pTabs[TAB_MAPGROUP_LAYERS]->AddSeparator();
}

void CEditor::RefreshTab_MapZoneTiles_Asset(bool KeepStatus)
{
	CAsset_MapZoneTiles* pMapZoneTiles = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapZoneTiles>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapZoneTiles)
		return;
	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_MapZoneTiles::ZONETYPEPATH, CAssetPath::TYPE_ZONETYPE, -1, "Zone type:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_MapZoneTiles::WIDTH, -1, "Width:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_MapZoneTiles::HEIGHT, -1, "Height:");
}

void CEditor::RefreshTab_MapLayerTiles_Asset(bool KeepStatus)
{
	CAsset_MapLayerTiles* pMapLayer = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapLayerTiles>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapLayer)
		return;
	
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_MapLayerTiles::WIDTH, -1, "Width:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_MapLayerTiles::HEIGHT, -1, "Height:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_MapLayerTiles::IMAGEPATH, CAsset_Image::TypeId, -1, "Image:");
	AddColorField(m_pTabs[TAB_ASSET], CAsset_MapLayerTiles::COLOR, -1, "Color:");
}

void CEditor::RefreshTab_MapLayerQuads_Asset(bool KeepStatus)
{
	CAsset_MapLayerQuads* pMapLayer = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapLayerQuads>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapLayer)
		return;
		
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_MapLayerQuads::IMAGEPATH, CAsset_Image::TypeId, -1, "Image:");
}

void CEditor::RefreshTab_MapLayerQuads_Quads(bool KeepStatus)
{
	char aBuf[128];
	
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_MAPLAYERQUADS_QUADS])
	{
		Scroll = m_pLists[LIST_MAPLAYERQUADS_QUADS]->GetScrollPos();
	}
	m_pTabs[TAB_MAPLAYERQUADS_QUADS]->Clear();
	
	CAsset_MapLayerQuads* pMapLayer = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapLayerQuads>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapLayer)
		return;
		
	m_pLists[LIST_MAPLAYERQUADS_QUADS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_MAPLAYERQUADS_QUADS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_MAPLAYERQUADS_QUADS]->Add(m_pLists[LIST_MAPLAYERQUADS_QUADS]);
	
	m_pLists[LIST_MAPLAYERQUADS_QUADS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Quads", gui::TEXTSTYLE_HEADER2));
	CAsset_MapLayerQuads::CIteratorQuad Iter;
	for(Iter = pMapLayer->BeginQuad(); Iter != pMapLayer->EndQuad(); ++Iter)
	{
		str_format(aBuf, sizeof(aBuf), "Quad #%d", (*Iter).GetId());
		
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, -1);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, *Iter);
		pItem->SetText(aBuf);
		
		m_pLists[LIST_MAPLAYERQUADS_QUADS]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_MAPLAYERQUADS_QUADS]->SetScrollPos(Scroll);
	}
	
	m_pTabs[TAB_MAPLAYERQUADS_QUADS]->AddSeparator();	
	
	CAsset_MapLayerQuads::CSubPath EditedSubPath(m_pAssetsEditor->m_EditedAssetSubPath);
	
	if(!EditedSubPath.IsNull())
	{
		if(EditedSubPath.GetType() == CAsset_MapLayerQuads::CSubPath::TYPE_QUAD)
		{
			AddFloat2Field(m_pTabs[TAB_MAPLAYERQUADS_QUADS], CAsset_MapLayerQuads::QUAD_POSITION_X, CAsset_MapLayerQuads::QUAD_POSITION_Y, m_pAssetsEditor->m_EditedAssetSubPath, "Position:");
			AddFloat2Field(m_pTabs[TAB_MAPLAYERQUADS_QUADS], CAsset_MapLayerQuads::QUAD_SIZE_X, CAsset_MapLayerQuads::QUAD_SIZE_Y, m_pAssetsEditor->m_EditedAssetSubPath, "Size:");
			AddAngleField(m_pTabs[TAB_MAPLAYERQUADS_QUADS], CAsset_MapLayerQuads::QUAD_ANGLE, m_pAssetsEditor->m_EditedAssetSubPath, "Angle:");
			AddAssetField(m_pTabs[TAB_MAPLAYERQUADS_QUADS], CAsset_MapLayerQuads::QUAD_ANIMATIONPATH, CAssetPath::TYPE_SKELETONANIMATION, m_pAssetsEditor->m_EditedAssetSubPath, "Animation:");

			if(EditedSubPath.GetId2() >= CAsset_MapLayerQuads::CSubPath::POINT_VERTEX0 && EditedSubPath.GetId2() <= CAsset_MapLayerQuads::CSubPath::POINT_VERTEX3)
			{
				AddFloat2Field(m_pTabs[TAB_MAPLAYERQUADS_QUADS], CAsset_MapLayerQuads::QUAD_POINT_POSITION_X, CAsset_MapLayerQuads::QUAD_POINT_POSITION_Y, EditedSubPath, "Vertex position:");
				AddColorField(m_pTabs[TAB_MAPLAYERQUADS_QUADS], CAsset_MapLayerQuads::QUAD_POINT_COLOR, EditedSubPath, "Vertex Color:");
				AddFloat2Field(m_pTabs[TAB_MAPLAYERQUADS_QUADS], CAsset_MapLayerQuads::QUAD_POINT_UV_X, CAsset_MapLayerQuads::QUAD_POINT_UV_Y, EditedSubPath, "Texture:");
			}
		}
	}
}

void CEditor::RefreshTab_ZoneType_Asset(bool KeepStatus)
{
	CAsset_ZoneType* pZoneType = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_ZoneType>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pZoneType)
		return;
}

void CEditor::RefreshTab_ZoneType_Indices(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_ZONETYPE_INDICES])
	{
		Scroll = m_pLists[LIST_ZONETYPE_INDICES]->GetScrollPos();
	}
	m_pTabs[TAB_ZONETYPE_INDICES]->Clear();
			
	CAsset_ZoneType* pZoneType = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_ZoneType>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pZoneType)
		return;
		
	m_pLists[LIST_ZONETYPE_INDICES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_ZONETYPE_INDICES]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_ZONETYPE_INDICES]->Add(m_pLists[LIST_ZONETYPE_INDICES]);
	
	m_pLists[LIST_ZONETYPE_INDICES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Indices", gui::TEXTSTYLE_HEADER2));
	CAsset_ZoneType::CIteratorIndex Iter;
	for(Iter = pZoneType->BeginIndex(); Iter != pZoneType->EndIndex(); ++Iter)
	{
		CEditor::CSubItemListItem* pItem = new CEditor::CSubItemListItem(m_pAssetsEditor, CAssetPath::Null());
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, *Iter);
		pItem->SetText(m_pAssetsEditor->m_EditedAssetPath, CAsset_ZoneType::INDEX_NAME, *Iter);
		
		m_pLists[LIST_ZONETYPE_INDICES]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_ZONETYPE_INDICES]->SetScrollPos(Scroll);
	}

	m_pTabs[TAB_ZONETYPE_INDICES]->Add(new CAddSubItem(
		m_pAssetsEditor,
		m_pAssetsEditor->m_EditedAssetPath,
		CAsset_ZoneType::CSubPath::TYPE_INDEX,
		"Add index",
		CAssetPath::SpriteUniverse(SPRITE_ICON_INCREASE),
		TAB_ZONETYPE_INDICES
	));

	m_pTabs[TAB_ZONETYPE_INDICES]->AddSeparator();	
	
	CAsset_ZoneType::CSubPath EditedSubPath(m_pAssetsEditor->m_EditedAssetSubPath);
	if(pZoneType->IsValidIndex(EditedSubPath))
	{
		AddTextField(m_pTabs[TAB_ZONETYPE_INDICES], CAsset_ZoneType::INDEX_NAME, EditedSubPath, sizeof(CAsset_ZoneType::CIndex::m_aName), "Name:");
		AddIntegerField(m_pTabs[TAB_ZONETYPE_INDICES], CAsset_ZoneType::INDEX_NUMBER, EditedSubPath, "Server Id:");
		AddColorField(m_pTabs[TAB_ZONETYPE_INDICES], CAsset_ZoneType::INDEX_COLOR, EditedSubPath, "HUD Color:");
	}
}

void CEditor::Refresh(int Tab)
{
	bool KeepStatus = false;
	if(m_pAssetsEditor->m_EditedAssetPath.GetType() != m_LastEditedAssetType)
	{
		Clear();
		
		for(int i=0; i<NUM_TABS; i++)
		{
			m_pTabs[i] = 0;
		}
		for(int i=0; i<NUM_LISTS; i++)
		{
			m_pLists[i] = 0;
		}
		
		//Add main tab
		m_pTabs[TAB_ASSET] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
		AddTab(m_pTabs[TAB_ASSET], CAssetPath::SpriteUniverse(SPRITE_ICON_ASSET), "Asset properties");
		
		//Add asset-specific tabs
		switch(m_pAssetsEditor->m_EditedAssetPath.GetType())
		{
			case CAssetPath::TYPE_SKELETON:
				m_pTabs[TAB_SKELETON_BONES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_SKELETON_BONES], CAssetPath::SpriteUniverse(SPRITE_ICON_BONE), "Bones: create, edit and remove bones from the skeleton");
				m_pTabs[TAB_SKELETON_LAYERS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_SKELETON_LAYERS], CAssetPath::SpriteUniverse(SPRITE_ICON_LAYERS), "Layers: create, edit and remove layers from the skeleton");
				break;
			case CAssetPath::TYPE_SKELETONSKIN:
				m_pTabs[TAB_SKELETONSKIN_SPRITES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE, gui::LAYOUTFILLING_FIRST);
				AddTab(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAssetPath::SpriteUniverse(SPRITE_ICON_SPRITE), "Sprites: create, edit and remove sprites from the skin");
				break;
			case CAssetPath::TYPE_SKELETONANIMATION:
				m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS], CAssetPath::SpriteUniverse(SPRITE_ICON_SKELETONANIMATION), "Animations: set propeties for bone and layer animations");
				m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES], CAssetPath::SpriteUniverse(SPRITE_ICON_FRAMES), "Key Frames: edit properties of key frames");
				break;
			case CAssetPath::TYPE_CHARACTER:
				m_pTabs[TAB_CHARACTER_PARTS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_CHARACTER_PARTS], CAssetPath::SpriteUniverse(SPRITE_ICON_CHARACTERPART), "Parts: create, edit and remove parts of a character");
				break;
			case CAssetPath::TYPE_MAP:
				m_pTabs[TAB_MAP_GROUPS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_MAP_GROUPS], CAssetPath::SpriteUniverse(SPRITE_ICON_FOLDER), "Groups: organize groups in the map");
				break;
			case CAssetPath::TYPE_MAPGROUP:
				m_pTabs[TAB_MAPGROUP_LAYERS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_MAPGROUP_LAYERS], CAssetPath::SpriteUniverse(SPRITE_ICON_LAYERS), "Layers: organize layers in the group");
				break;
			case CAssetPath::TYPE_MAPLAYERQUADS:
				m_pTabs[TAB_MAPLAYERQUADS_QUADS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_MAPLAYERQUADS_QUADS], CAssetPath::SpriteUniverse(SPRITE_ICON_LAYERS), "Quads: organize and edit quads");
				break;
			case CAssetPath::TYPE_ZONETYPE:
				m_pTabs[TAB_ZONETYPE_INDICES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
				AddTab(m_pTabs[TAB_ZONETYPE_INDICES], CAssetPath::SpriteUniverse(SPRITE_ICON_DEFAULT), "Indices: organize and edit indices");
				break;
		}
		
		m_LastEditedAssetType = m_pAssetsEditor->m_EditedAssetPath.GetType();
	}
	else
	{
		KeepStatus = true;
	}
	AddAssetTabCommons(m_pTabs[TAB_ASSET]);
	
	if(Tab >= 0)
	{
		m_SelectedTab = Tab;
	}
	
	//Search Tag: TAG_NEW_ASSET
	switch(m_pAssetsEditor->m_EditedAssetPath.GetType())
	{
		case CAssetPath::TYPE_IMAGE:
			RefreshTab_Image_Asset(KeepStatus);
			break;
		case CAssetPath::TYPE_SPRITE:
			RefreshTab_Sprite_Asset(KeepStatus);
			break;
		case CAssetPath::TYPE_SKELETON:
			RefreshTab_Skeleton_Asset(KeepStatus);
			RefreshTab_Skeleton_Bones(KeepStatus);
			RefreshTab_Skeleton_Layers(KeepStatus);
			break;
		case CAssetPath::TYPE_SKELETONSKIN:
			RefreshTab_SkeletonSkin_Asset(KeepStatus);
			RefreshTab_SkeletonSkin_Sprites(KeepStatus);
			break;
		case CAssetPath::TYPE_SKELETONANIMATION:
			RefreshTab_SkeletonAnimation_Asset(KeepStatus);
			RefreshTab_SkeletonAnimation_Animations(KeepStatus);
			RefreshTab_SkeletonAnimation_KeyFrames(KeepStatus);
			break;
		case CAssetPath::TYPE_CHARACTER:
			RefreshTab_Character_Asset(KeepStatus);
			RefreshTab_Character_Parts(KeepStatus);
			break;
		case CAssetPath::TYPE_CHARACTERPART:
			RefreshTab_CharacterPart_Asset(KeepStatus);
			break;
		case CAssetPath::TYPE_WEAPON:
			RefreshTab_Weapon_Asset(KeepStatus);
			break;
		case CAssetPath::TYPE_MAP:
			RefreshTab_Map_Asset(KeepStatus);
			RefreshTab_Map_Groups(KeepStatus);
			break;
		case CAssetPath::TYPE_MAPGROUP:
			RefreshTab_MapGroup_Asset(KeepStatus);
			RefreshTab_MapGroup_Layers(KeepStatus);
			break;
		case CAssetPath::TYPE_MAPZONETILES:
			RefreshTab_MapZoneTiles_Asset(KeepStatus);
			break;
		case CAssetPath::TYPE_MAPLAYERTILES:
			RefreshTab_MapLayerTiles_Asset(KeepStatus);
			break;
		case CAssetPath::TYPE_MAPLAYERQUADS:
			RefreshTab_MapLayerQuads_Asset(KeepStatus);
			RefreshTab_MapLayerQuads_Quads(KeepStatus);
			break;
		case CAssetPath::TYPE_ZONETYPE:
			RefreshTab_ZoneType_Asset(KeepStatus);
			RefreshTab_ZoneType_Indices(KeepStatus);
			break;
	}
	
	
	Update();
}

void CEditor::OnEditedAssetChange()
{
	Refresh();
}

}

}
