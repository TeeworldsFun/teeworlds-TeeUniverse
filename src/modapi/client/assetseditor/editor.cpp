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

#include <cstddef>

#include "editor.h"

namespace tu
{

const char* CAssetsEditorGui_Editor::CAssetEdit::m_aNoneText = "None";
const char* CAssetsEditorGui_Editor::CBoneEdit::m_aNoneText = "None";
const char* CAssetsEditorGui_Editor::CLayerEdit::m_aNoneText = "None";
const char* CAssetsEditorGui_Editor::CCharacterPartEdit::m_aNoneText = "None";

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
		
class CAssetsEditorGui_AssetListItem : public gui::CHListLayout
{
protected:
	class CEditButton : public gui::CIconButton
	{
	protected:
		CAssetsEditorGui_AssetListItem* m_pAssetListItem;
	
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetListItem->EditAsset();
		}
		
	public:
		CEditButton(CAssetsEditorGui_AssetListItem* pAssetListItem) :
			gui::CIconButton(pAssetListItem->m_pConfig, CAssetsEditor::ICON_EDIT),
			m_pAssetListItem(pAssetListItem)
		{
			
		}
	};
	
	class CDisplayButton : public gui::CIconButton
	{
	protected:
		CAssetsEditorGui_AssetListItem* m_pAssetListItem;
	
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetListItem->DisplayAsset();
		}
		
	public:
		CDisplayButton(CAssetsEditorGui_AssetListItem* pAssetListItem) :
			gui::CIconButton(pAssetListItem->m_pConfig, CAssetsEditor::ICON_VIEW),
			m_pAssetListItem(pAssetListItem)
		{
			
		}
	};
	
	class CItemListButton : public gui::CExternalTextButton
	{
	protected:
		CAssetsEditorGui_AssetListItem* m_pAssetListItem;
	
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetListItem->EditAsset();
			m_pAssetListItem->DisplayAsset();
		}
		
	public:
		CItemListButton(CAssetsEditorGui_AssetListItem* pAssetListItem, char* pText) :
			gui::CExternalTextButton(pAssetListItem->m_pConfig, pText),
			m_pAssetListItem(pAssetListItem)
		{
			m_Centered = false;
		}
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	
	CEditButton* m_pEditButton;
	CDisplayButton* m_pDisplayButton;

public:
	CAssetsEditorGui_AssetListItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath) :
		gui::CHListLayout(pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_FIRST),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* aName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		Add(new CItemListButton(this, aName));
		
		m_pDisplayButton = new CDisplayButton(this);
		Add(m_pDisplayButton);
		
		m_pEditButton = new CEditButton(this);
		Add(m_pEditButton);
		
		Update();
	}
	
	void EditAsset()
	{
		m_pAssetsEditor->EditAsset(m_AssetPath);
	}
	
	void DisplayAsset()
	{
		m_pAssetsEditor->DisplayAsset(m_AssetPath);
	}
	
	virtual void Render()
	{
		if(m_pAssetsEditor->IsEditedAsset(m_AssetPath))
		{
			m_pEditButton->SetButtonStyle(gui::BUTTONSTYLE_HIGHLIGHT);
		}
		else
		{
			m_pEditButton->SetButtonStyle(gui::BUTTONSTYLE_NORMAL);
		}
		
		if(m_pAssetsEditor->IsDisplayedAsset(m_AssetPath))
		{
			m_pDisplayButton->SetButtonStyle(gui::BUTTONSTYLE_HIGHLIGHT);
		}
		else
		{
			m_pDisplayButton->SetButtonStyle(gui::BUTTONSTYLE_NORMAL);
		}
		
		gui::CHListLayout::Render();
	}
};

CAssetsEditorGui_Editor::CAssetsEditorGui_Editor(CAssetsEditor* pAssetsEditor) :
	gui::CTabs(pAssetsEditor->m_pGuiConfig),
	m_LastEditedAssetType(-1),
	m_pAssetsEditor(pAssetsEditor)
{
	for(int i=0; i<NUM_TABS; i++)
	{
		m_pTabs[i] = 0;
	}
}

void CAssetsEditorGui_Editor::AddAssetTabCommons(gui::CVListLayout* pList)
{
	pList->Clear();
	AddTextField(pList, CAsset::NAME, -1, sizeof(CAsset::m_aName), "Asset name:");
	
	pList->Add(new CAssetsEditorGui_Editor::CDeleteAsset(m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath));
	
	pList->AddSeparator();
}

void CAssetsEditorGui_Editor::AddSubTitle(gui::CVListLayout* pList, const char* pText)
{
	pList->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, pText, gui::TEXTSTYLE_HEADER2));
}

void CAssetsEditorGui_Editor::AddField(gui::CVListLayout* pList, gui::CWidget* pWidget, const char* pLabelText)
{
	if(pLabelText)
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
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

void CAssetsEditorGui_Editor::AddFloatField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CFloatAssetMemberEdit* pWidget = new CAssetsEditorGui_Editor::CFloatAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddAngleField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CAnglerEdit* pWidget = new CAssetsEditorGui_Editor::CAnglerEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddFloat2Field(gui::CVListLayout* pList, int Member1, int Member2, int SubId, const char* pLabelText)
{
	gui::CHListLayout* pWidget = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
	pWidget->SetHeight(m_pConfig->m_ButtonHeight);
	
	CAssetsEditorGui_Editor::CFloatAssetMemberEdit* pEdit1 = new CAssetsEditorGui_Editor::CFloatAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member1, SubId);
	pWidget->Add(pEdit1);
	
	CAssetsEditorGui_Editor::CFloatAssetMemberEdit* pEdit2 = new CAssetsEditorGui_Editor::CFloatAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member2, SubId);
	pWidget->Add(pEdit2);
			
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddIntegerField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CIntegerAssetMemberEdit* pWidget = new CAssetsEditorGui_Editor::CIntegerAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddCycleField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CEnumEdit* pWidget = new CAssetsEditorGui_Editor::CEnumEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, g_aCycleTypeText, sizeof(g_aCycleTypeText)/sizeof(const char*));
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddSpriteAlignField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CEnumEdit* pWidget = new CAssetsEditorGui_Editor::CEnumEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, g_aSpriteAlignText, sizeof(g_aSpriteAlignText)/sizeof(const char*));
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddLayerStateField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CEnumEdit* pWidget = new CAssetsEditorGui_Editor::CEnumEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, g_aLayerStateText, sizeof(g_aLayerStateText)/sizeof(const char*));
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddBoneAlignField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CEnumEdit* pWidget = new CAssetsEditorGui_Editor::CEnumEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, g_aBoneAlignText, sizeof(g_aBoneAlignText)/sizeof(const char*));
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddColorField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CColorEdit* pWidget = new CAssetsEditorGui_Editor::CColorEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddTextField(gui::CVListLayout* pList, int Member, int SubId, int TextSize, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CTextAssetMemberEdit* pWidget = new CAssetsEditorGui_Editor::CTextAssetMemberEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, TextSize);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddAssetField(gui::CVListLayout* pList, int Member, int AssetType, int SubId, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CAssetEdit* pWidget = new CAssetsEditorGui_Editor::CAssetEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, AssetType);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddBoneField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath SkeletonPath, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CBoneEdit* pWidget = new CAssetsEditorGui_Editor::CBoneEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, SkeletonPath);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddLayerField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath SkeletonPath, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CLayerEdit* pWidget = new CAssetsEditorGui_Editor::CLayerEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, SkeletonPath);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::AddCharacterPartField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath CharacterPath, const char* pLabelText)
{
	CAssetsEditorGui_Editor::CCharacterPartEdit* pWidget = new CAssetsEditorGui_Editor::CCharacterPartEdit(
		m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath, Member, SubId, CharacterPath);
	
	AddField(pList, pWidget, pLabelText);
}

void CAssetsEditorGui_Editor::RefreshTab_Image_Asset(bool KeepStatus)
{
	CAsset_Image* pImage = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Image>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pImage)
		return;
	
	//Size
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
		pLayout->SetHeight(m_pConfig->m_ButtonHeight);
		m_pTabs[TAB_ASSET]->Add(pLayout);
		
		gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Size:");
		pLayout->Add(pLabel);
		
		int Width = m_pAssetsEditor->AssetManager()->GetAssetValue<int>(m_pAssetsEditor->m_EditedAssetPath, CAsset_Image::WIDTH, -1, 0);
		int Height = m_pAssetsEditor->AssetManager()->GetAssetValue<int>(m_pAssetsEditor->m_EditedAssetPath, CAsset_Image::HEIGHT, -1, 0);
		char aBuf[64];
		str_format(aBuf, sizeof(aBuf), "%d x %d", Width, Height);
		
		gui::CLabel* pValue = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, aBuf);
		pLayout->Add(pValue);
	}
	
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Image::GRIDWIDTH, -1, "Grid width:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Image::GRIDHEIGHT, -1, "Grid height:");
}

void CAssetsEditorGui_Editor::RefreshTab_Sprite_Asset(bool KeepStatus)
{	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Sprite::IMAGEPATH, CAssetPath::TYPE_IMAGE, -1, "Image:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Sprite::X, -1, "X:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Sprite::Y, -1, "Y:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Sprite::WIDTH, -1, "Width:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_Sprite::HEIGHT, -1, "Height:");
}

void CAssetsEditorGui_Editor::RefreshTab_Skeleton_Asset(bool KeepStatus)
{	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Skeleton::PARENTPATH, CAssetPath::TYPE_SKELETON, -1, "Parent:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Skeleton::SKINPATH, CAssetPath::TYPE_SKELETONSKIN, -1, "Default Skin:");
}

void CAssetsEditorGui_Editor::RefreshTab_Skeleton_Bones(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETON_BONES])
	{
		Scroll = m_pLists[LIST_SKELETON_BONES]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETON_BONES]->Clear();
	m_pLists[LIST_SKELETON_BONES] = 0;
	
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeleton)
		return;
		
	m_pLists[LIST_SKELETON_BONES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETON_BONES]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETON_BONES]->Add(m_pLists[LIST_SKELETON_BONES]);
	m_pLists[LIST_SKELETON_BONES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Bones", gui::TEXTSTYLE_HEADER2));
	
	for(int i=0; i<pSkeleton->m_Bones.size(); i++)
	{
		CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, CAssetsEditor::ICON_BONE);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, pSkeleton->GetBonePath(i));
		pItem->SetText(m_pAssetsEditor->m_EditedAssetPath, CAsset_Skeleton::BONE_NAME, pSkeleton->GetBonePath(i));
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
		CAssetsEditor::ICON_INCREASE,
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
	
void CAssetsEditorGui_Editor::RefreshTab_Skeleton_Layers(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETON_LAYERS])
	{
		Scroll = m_pLists[LIST_SKELETON_LAYERS]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETON_LAYERS]->Clear();
			
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeleton)
		return;
			
	m_pLists[LIST_SKELETON_LAYERS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETON_LAYERS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETON_LAYERS]->Add(m_pLists[LIST_SKELETON_LAYERS]);
	m_pLists[LIST_SKELETON_LAYERS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Layers", gui::TEXTSTYLE_HEADER2));
	
	for(int i=0; i<pSkeleton->m_Layers.size(); i++)
	{
		CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, CAssetsEditor::ICON_LAYERS);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, pSkeleton->GetLayerPath(i));
		pItem->SetText(m_pAssetsEditor->m_EditedAssetPath, CAsset_Skeleton::LAYER_NAME, pSkeleton->GetLayerPath(i));
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
		CAssetsEditor::ICON_INCREASE,
		TAB_SKELETON_LAYERS
	));
	
	m_pTabs[TAB_SKELETON_LAYERS]->AddSeparator();
		
	if(m_pAssetsEditor->m_EditedAssetSubPath >= 0 && CAsset_Skeleton::CSubPath(m_pAssetsEditor->m_EditedAssetSubPath).GetType() == CAsset_Skeleton::CSubPath::TYPE_LAYER)
	{
		AddTextField(m_pTabs[TAB_SKELETON_LAYERS], CAsset_Skeleton::LAYER_NAME, m_pAssetsEditor->m_EditedAssetSubPath, sizeof(CAsset_Skeleton::CLayer::m_aName), "Name:");
	}
}

void CAssetsEditorGui_Editor::RefreshTab_SkeletonSkin_Asset(bool KeepStatus)
{
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_SkeletonSkin::SKELETONPATH, CAssetPath::TYPE_SKELETON, -1, "Skeleton:");
}

void CAssetsEditorGui_Editor::RefreshTab_SkeletonSkin_Sprites(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETONSKIN_SPRITES])
	{
		Scroll = m_pLists[LIST_SKELETONSKIN_SPRITES]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETONSKIN_SPRITES]->Clear();
			
	CAsset_SkeletonSkin* pSkeletonSkin = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_SkeletonSkin>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonSkin)
		return;
		
	m_pLists[LIST_SKELETONSKIN_SPRITES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETONSKIN_SPRITES]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETONSKIN_SPRITES]->Add(m_pLists[LIST_SKELETONSKIN_SPRITES]);
	m_pLists[LIST_SKELETONSKIN_SPRITES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Sprites", gui::TEXTSTYLE_HEADER2));
	
	for(int i=0; i<pSkeletonSkin->m_Sprites.size(); i++)
	{
		CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, CAssetsEditor::ICON_SPRITE);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, pSkeletonSkin->GetSpritePath(i));
		pItem->SetText(pSkeletonSkin->m_Sprites[i].m_SpritePath, CAsset::NAME, -1);
		m_pLists[LIST_SKELETONSKIN_SPRITES]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_SKELETONSKIN_SPRITES]->SetScrollPos(Scroll);
	}

	m_pTabs[TAB_SKELETONSKIN_SPRITES]->Add(new CAddSubItem(
		m_pAssetsEditor,
		m_pAssetsEditor->m_EditedAssetPath,
		CAsset_SkeletonSkin::CSubPath::TYPE_SPRITE,
		"Add sprite",
		CAssetsEditor::ICON_INCREASE,
		TAB_SKELETONSKIN_SPRITES
	));
	
	m_pTabs[TAB_SKELETONSKIN_SPRITES]->AddSeparator();	
		
	if(m_pAssetsEditor->m_EditedAssetSubPath >= 0)
	{
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

void CAssetsEditorGui_Editor::RefreshTab_SkeletonAnimation_Asset(bool KeepStatus)
{
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_SkeletonAnimation::SKELETONPATH, CAssetPath::TYPE_SKELETON, -1, "Skeleton:");
}

void CAssetsEditorGui_Editor::RefreshTab_SkeletonAnimation_Animations(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETONANIMATION_ANIMATIONS])
	{
		Scroll = m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS]->Clear();
			
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonAnimation)
		return;
			
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return;
		
	m_pLists[LIST_SKELETONANIMATION_ANIMATIONS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS]->Add(m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]);
	
	m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Bone Animations", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pSkeletonAnimation->m_BoneAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath BonePath = CAsset_Skeleton::CSubPath::Bone(pSkeletonAnimation->m_BoneAnimations[i].m_BonePath.GetId());
		
		CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, CAssetsEditor::ICON_SKELETONANIMATION);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::CSubPath::BoneAnimation(i).ConvertToInteger());
		
		if(pSkeletonAnimation->m_BoneAnimations[i].m_BonePath.GetSource() == CAsset_Skeleton::CBonePath::SRC_PARENT)
			pItem->SetText(pSkeleton->m_ParentPath, CAsset_Skeleton::BONE_NAME, BonePath.ConvertToInteger());
		else
			pItem->SetText(pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::BONE_NAME, BonePath.ConvertToInteger());
		
		m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->Add(pItem);
	}
	
	m_pLists[LIST_SKELETONANIMATION_ANIMATIONS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Layer Animations", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pSkeletonAnimation->m_LayerAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath LayerPath = CAsset_Skeleton::CSubPath::Layer(pSkeletonAnimation->m_LayerAnimations[i].m_LayerPath.GetId());
		
		CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, CAssetsEditor::ICON_LAYERANIMATION);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::CSubPath::LayerAnimation(i).ConvertToInteger());
		
		if(pSkeletonAnimation->m_LayerAnimations[i].m_LayerPath.GetSource() == CAsset_Skeleton::CBonePath::SRC_PARENT)
			pItem->SetText(pSkeleton->m_ParentPath, CAsset_Skeleton::LAYER_NAME, LayerPath.ConvertToInteger());
		else
			pItem->SetText(pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::LAYER_NAME, LayerPath.ConvertToInteger());
		
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

void CAssetsEditorGui_Editor::RefreshTab_SkeletonAnimation_KeyFrames(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_SKELETONANIMATION_KEYFRAMES])
	{
		Scroll = m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->GetScrollPos();
	}
	m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES]->Clear();
	
	CAsset_SkeletonAnimation* pSkeletonAnimation = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_SkeletonAnimation>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pSkeletonAnimation)
		return;
			
	CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Skeleton>(pSkeletonAnimation->m_SkeletonPath);
	if(!pSkeleton)
		return;
		
	m_pLists[LIST_SKELETONANIMATION_KEYFRAMES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES]->Add(m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]);
	
	m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Bone Key Frames", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pSkeletonAnimation->m_BoneAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath BonePath = CAsset_Skeleton::CSubPath::Bone(pSkeletonAnimation->m_BoneAnimations[i].m_BonePath.GetId());
		const char* pBoneName;
		
		if(pSkeletonAnimation->m_BoneAnimations[i].m_BonePath.GetSource() == CAsset_Skeleton::CBonePath::SRC_PARENT)
			pBoneName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(pSkeleton->m_ParentPath, CAsset_Skeleton::BONE_NAME, BonePath.ConvertToInteger(), 0);
		else
			pBoneName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::BONE_NAME, BonePath.ConvertToInteger(), 0);
				
		for(int j=0; j<pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames.size(); j++)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "%s, frame #%d", pBoneName, pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames[j].m_Time);
			
			CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, CAssetsEditor::ICON_KEYFRAME_BONE);
			pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::CSubPath::BoneKeyFrame(i, j).ConvertToInteger());
			pItem->SetText(aBuf);
			m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->Add(pItem);
		}
	}

	m_pLists[LIST_SKELETONANIMATION_KEYFRAMES]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Layer Key Frames", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pSkeletonAnimation->m_LayerAnimations.size(); i++)
	{
		CAsset_Skeleton::CSubPath LayerPath = CAsset_Skeleton::CSubPath::Layer(pSkeletonAnimation->m_LayerAnimations[i].m_LayerPath.GetId());
		const char* pLayerName;
		
		if(pSkeletonAnimation->m_LayerAnimations[i].m_LayerPath.GetSource() == CAsset_Skeleton::CBonePath::SRC_PARENT)
			pLayerName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(pSkeleton->m_ParentPath, CAsset_Skeleton::LAYER_NAME, LayerPath.ConvertToInteger(), 0);
		else
			pLayerName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(pSkeletonAnimation->m_SkeletonPath, CAsset_Skeleton::LAYER_NAME, LayerPath.ConvertToInteger(), 0);
		
		for(int j=0; j<pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames.size(); j++)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "%s, frame #%d", pLayerName, pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames[j].m_Time);
			
			CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, CAssetsEditor::ICON_KEYFRAME_LAYER);
			pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_SkeletonAnimation::CSubPath::LayerKeyFrame(i, j).ConvertToInteger());
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

void CAssetsEditorGui_Editor::RefreshTab_Character_Asset(bool KeepStatus)
{
	CAsset_Character* pCharacter = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Character>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pCharacter)
		return;
	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Character::IDLEPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Idle Animation:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Character::WALKPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Walk Animation:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Character::CONTROLLEDJUMPPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Jump Animation:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Character::UNCONTROLLEDJUMPPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Jump2 Animation:");
}

void CAssetsEditorGui_Editor::RefreshTab_Character_Parts(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_CHARACTER_PARTS])
	{
		Scroll = m_pLists[LIST_CHARACTER_PARTS]->GetScrollPos();
	}
	m_pTabs[TAB_CHARACTER_PARTS]->Clear();
			
	CAsset_Character* pCharacter = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Character>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pCharacter)
		return;
		
	m_pLists[LIST_CHARACTER_PARTS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_CHARACTER_PARTS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_CHARACTER_PARTS]->Add(m_pLists[LIST_CHARACTER_PARTS]);
	
	m_pLists[LIST_CHARACTER_PARTS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Parts", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pCharacter->m_Parts.size(); i++)
	{
		CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, CAssetsEditor::ICON_CHARACTERPART);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_Character::CSubPath::Part(i).ConvertToInteger());
		pItem->SetText(m_pAssetsEditor->m_EditedAssetPath, CAsset_Character::PART_NAME, CAsset_Character::CSubPath::Part(i).ConvertToInteger());
		
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
		CAssetsEditor::ICON_INCREASE,
		TAB_CHARACTER_PARTS
	));

	m_pTabs[TAB_CHARACTER_PARTS]->AddSeparator();	
	
	CAsset_Character::CSubPath EditedSubPath(m_pAssetsEditor->m_EditedAssetSubPath);
	if(!EditedSubPath.IsNull() && EditedSubPath.GetType() == CAsset_Character::CSubPath::TYPE_PART)
	{
		AddTextField(m_pTabs[TAB_CHARACTER_PARTS], CAsset_Character::PART_NAME, m_pAssetsEditor->m_EditedAssetSubPath, sizeof(CAsset_Character::CPart::m_aName), "Name:");
		AddAssetField(m_pTabs[TAB_CHARACTER_PARTS], CAsset_Character::PART_DEFAULTPATH, CAssetPath::TYPE_CHARACTERPART, EditedSubPath.ConvertToInteger(), "Name:");
	}
}

void CAssetsEditorGui_Editor::RefreshTab_CharacterPart_Asset(bool KeepStatus)
{
	CAsset_CharacterPart* pCharacterPart = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_CharacterPart>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pCharacterPart)
		return;
	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_CharacterPart::CHARACTERPATH, CAssetPath::TYPE_CHARACTER, -1, "Character:");
	AddCharacterPartField(m_pTabs[TAB_ASSET], CAsset_CharacterPart::CHARACTERPART, -1, pCharacterPart->m_CharacterPath, "Part:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_CharacterPart::SKELETONSKINPATH, CAssetPath::TYPE_SKELETONSKIN, -1, "Skin:");
}

void CAssetsEditorGui_Editor::RefreshTab_Weapon_Asset(bool KeepStatus)
{
	CAsset_Weapon* pWeapon = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Weapon>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pWeapon)
		return;
	
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Weapon::CHARACTERPATH, CAssetPath::TYPE_CHARACTER, -1, "Character:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Weapon::CURSORPATH, CAssetPath::TYPE_SPRITE, -1, "Cursor:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Weapon::SKINPATH, CAssetPath::TYPE_SKELETONSKIN, -1, "Skin:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_Weapon::ATTACKANIMATIONPATH, CAssetPath::TYPE_SKELETONANIMATION, -1, "Attack Animation:");
}

void CAssetsEditorGui_Editor::RefreshTab_MapGroup_Asset(bool KeepStatus)
{
	CAsset_MapGroup* pMapGroup = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_MapGroup>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapGroup)
		return;
}

void CAssetsEditorGui_Editor::RefreshTab_MapGroup_Layers(bool KeepStatus)
{
	float Scroll = 0;
	if(KeepStatus && m_pLists[LIST_MAPGROUP_LAYERS])
	{
		Scroll = m_pLists[LIST_MAPGROUP_LAYERS]->GetScrollPos();
	}
	m_pTabs[TAB_MAPGROUP_LAYERS]->Clear();
			
	CAsset_MapGroup* pMapGroup = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_MapGroup>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapGroup)
		return;
		
	m_pLists[LIST_MAPGROUP_LAYERS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
	m_pLists[LIST_MAPGROUP_LAYERS]->SetHeight(m_Rect.h/2);
	m_pTabs[TAB_MAPGROUP_LAYERS]->Add(m_pLists[LIST_MAPGROUP_LAYERS]);
	
	m_pLists[LIST_MAPGROUP_LAYERS]->Add(new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Layers", gui::TEXTSTYLE_HEADER2));
	for(int i=0; i<pMapGroup->m_Layers.size(); i++)
	{
		CAssetsEditorGui_Editor::CSubItemListItem* pItem = new CAssetsEditorGui_Editor::CSubItemListItem(m_pAssetsEditor, -1);
		pItem->SetTarget(m_pAssetsEditor->m_EditedAssetPath, CAsset_MapGroup::CSubPath::Layer(i).ConvertToInteger());
		pItem->SetText(pMapGroup->m_Layers[i], CAsset::NAME, -1);
		
		m_pLists[LIST_MAPGROUP_LAYERS]->Add(pItem);
	}
	if(KeepStatus)
	{
		m_pLists[LIST_MAPGROUP_LAYERS]->SetScrollPos(Scroll);
	}

	m_pTabs[TAB_MAPGROUP_LAYERS]->AddSeparator();
}

void CAssetsEditorGui_Editor::RefreshTab_MapLayerTiles_Asset(bool KeepStatus)
{
	CAsset_MapLayerTiles* pMapLayer = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_MapLayerTiles>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapLayer)
		return;
	
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_MapLayerTiles::WIDTH, -1, "Width:");
	AddIntegerField(m_pTabs[TAB_ASSET], CAsset_MapLayerTiles::HEIGHT, -1, "Height:");
	AddAssetField(m_pTabs[TAB_ASSET], CAsset_MapLayerTiles::IMAGEPATH, CAsset_Image::TypeId, -1, "Image:");
}

void CAssetsEditorGui_Editor::RefreshTab_MapLayerQuads_Asset(bool KeepStatus)
{
	CAsset_MapLayerTiles* pMapLayer = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_MapLayerTiles>(m_pAssetsEditor->m_EditedAssetPath);
	if(!pMapLayer)
		return;
}

void CAssetsEditorGui_Editor::Refresh(int Tab)
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
		m_pTabs[TAB_ASSET] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
		AddTab(m_pTabs[TAB_ASSET], CAssetsEditor::ICON_ASSET, "Asset properties");
		
		//Add asset-specific tabs
		switch(m_pAssetsEditor->m_EditedAssetPath.GetType())
		{
			case CAssetPath::TYPE_SKELETON:
				m_pTabs[TAB_SKELETON_BONES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
				AddTab(m_pTabs[TAB_SKELETON_BONES], CAssetsEditor::ICON_BONE, "Bones: create, edit and remove bones from the skeleton");
				m_pTabs[TAB_SKELETON_LAYERS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
				AddTab(m_pTabs[TAB_SKELETON_LAYERS], CAssetsEditor::ICON_LAYERS, "Layers: create, edit and remove layers from the skeleton");
				break;
			case CAssetPath::TYPE_SKELETONSKIN:
				m_pTabs[TAB_SKELETONSKIN_SPRITES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
				AddTab(m_pTabs[TAB_SKELETONSKIN_SPRITES], CAssetsEditor::ICON_SPRITE, "Sprites: create, edit and remove sprites from the skin");
				break;
			case CAssetPath::TYPE_SKELETONANIMATION:
				m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
				AddTab(m_pTabs[TAB_SKELETONANIMATION_ANIMATIONS], CAssetsEditor::ICON_SKELETONANIMATION, "Animations: set propeties for bone and layer animations");
				m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
				AddTab(m_pTabs[TAB_SKELETONANIMATION_KEYFRAMES], CAssetsEditor::ICON_FRAMES, "Key Frames: edit properties of key frames");
				break;
			case CAssetPath::TYPE_CHARACTER:
				m_pTabs[TAB_CHARACTER_PARTS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
				AddTab(m_pTabs[TAB_CHARACTER_PARTS], CAssetsEditor::ICON_CHARACTERPART, "Parts: create, edit and remove parts of a character");
				break;
			case CAssetPath::TYPE_MAPGROUP:
				m_pTabs[TAB_MAPGROUP_LAYERS] = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
				AddTab(m_pTabs[TAB_MAPGROUP_LAYERS], CAssetsEditor::ICON_LAYERS, "Layers: organize layers in the group");
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
		case CAssetPath::TYPE_MAPGROUP:
			RefreshTab_MapGroup_Asset(KeepStatus);
			RefreshTab_MapGroup_Layers(KeepStatus);
			break;
		case CAssetPath::TYPE_MAPLAYERTILES:
			RefreshTab_MapLayerTiles_Asset(KeepStatus);
			break;
		case CAssetPath::TYPE_MAPLAYERQUADS:
			RefreshTab_MapLayerQuads_Asset(KeepStatus);
			break;
	}
	
	
	Update();
}

void CAssetsEditorGui_Editor::OnEditedAssetChange()
{
	Refresh();
}

}
