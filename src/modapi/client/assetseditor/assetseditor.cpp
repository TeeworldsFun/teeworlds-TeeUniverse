#include <base/tl/sorted_array.h>

#include <engine/shared/config.h>
#include <engine/shared/datafile.h>

#include <engine/client.h>
#include <engine/input.h>
#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/storage.h>

#include <modapi/client/clientmode.h>
#include <modapi/client/gui/button.h>
#include <modapi/client/gui/label.h>
#include <modapi/client/gui/layout.h>
#include <modapi/client/gui/integer-edit.h>
#include <modapi/client/gui/text-edit.h>
#include <modapi/client/gui/expand.h>

#include <cstddef>

#include "assetseditor.h"
#include "popup.h"
#include "editor.h"
#include "view.h"
#include "timeline.h"

namespace tu
{

/* BUTTONS ************************************************************/

class CAssetsEditorGui_TextButton : public gui::CTextButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;

public:
	CAssetsEditorGui_TextButton(CAssetsEditor* pAssetsEditor, const char* pText = 0, int IconId = -1) :
		gui::CTextButton(pAssetsEditor->m_pGuiConfig, pText, IconId),
		m_pAssetsEditor(pAssetsEditor)
	{
		
	}
};

class CAssetsEditorGui_IconButton : public gui::CIconButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;

public:
	CAssetsEditorGui_IconButton(CAssetsEditor* pAssetsEditor, int IconId) :
		gui::CIconButton(pAssetsEditor->m_pGuiConfig, IconId),
		m_pAssetsEditor(pAssetsEditor)
	{
		
	}
};

class CAssetsEditorGui_Button_ToolbarExit : public CAssetsEditorGui_TextButton
{
protected:
	virtual void MouseClickAction()
	{
		m_pAssetsEditor->CloseEditor();
	}

public:
	CAssetsEditorGui_Button_ToolbarExit(CAssetsEditor* pAssetsEditor) :
		CAssetsEditorGui_TextButton(pAssetsEditor, "Exit", -1)
	{
		SetWidth(100);
	}
};

class CAssetsEditorGui_Button_ToolbarLoad : public CAssetsEditorGui_TextButton
{	
protected:
	virtual void MouseClickAction()
	{
		m_pAssetsEditor->DisplayPopup(new CAssetsEditorGui_Popup_LoadAssets(
			m_pAssetsEditor, m_Rect, gui::CPopup::ALIGNMENT_RIGHT
		));
	}

public:
	CAssetsEditorGui_Button_ToolbarLoad(CAssetsEditor* pAssetsEditor) :
		CAssetsEditorGui_TextButton(pAssetsEditor, "Load", -1)
	{
		SetWidth(100);
	}
};

class CAssetsEditorGui_Button_ToolbarSave : public CAssetsEditorGui_TextButton
{	
protected:
	virtual void MouseClickAction()
	{
		m_pAssetsEditor->DisplayPopup(new CAssetsEditorGui_Popup_SaveAssets(
			m_pAssetsEditor, m_Rect, gui::CPopup::ALIGNMENT_RIGHT
		));
	}

public:
	CAssetsEditorGui_Button_ToolbarSave(CAssetsEditor* pAssetsEditor) :
		CAssetsEditorGui_TextButton(pAssetsEditor, "Save", -1)
	{
		SetWidth(100);
	}
};

/* ITEM LIST **********************************************************/

class CAssetsEditorGui_AssetListHeader : public gui::CHListLayout
{
protected:
	class CAddButton : public gui::CIconButton
	{
	protected:
		CAssetsEditorGui_AssetListHeader* m_pAssetListHeader;
	
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetListHeader->AddAsset();
		}
		
	public:
		CAddButton(CAssetsEditorGui_AssetListHeader* pAssetListHeader) :
			gui::CIconButton(pAssetListHeader->m_pConfig, CAssetsEditor::ICON_INCREASE),
			m_pAssetListHeader(pAssetListHeader)
		{
			
		}
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_AssetType;
	int m_Source;

public:
	CAssetsEditorGui_AssetListHeader(CAssetsEditor* pAssetsEditor, int AssetType, int Source) :
		gui::CHListLayout(pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_FIRST),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetType(AssetType),
		m_Source(Source)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		#define ADD_ASSETTYPE_HEADER(TypeCode, TypeHeader) case TypeCode:\
			Add(new gui::CLabel(m_pConfig, TypeHeader, gui::TEXTSTYLE_HEADER2));\
			break;
		
		switch(m_AssetType)
		{
			//Search Tag: TAG_NEW_ASSET
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_IMAGE, "Images")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_SPRITE, "Sprites")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_SKELETON, "Skeletons")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_SKELETONSKIN, "Skeleton Skins")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_SKELETONANIMATION, "Skeleton Animations")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_CHARACTER, "Characters")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_CHARACTERPART, "Character Parts")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_WEAPON, "Weapons")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_MAPGROUP, "Groups")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_MAPLAYERTILES, "Tile Layers")
			ADD_ASSETTYPE_HEADER(CAssetPath::TYPE_MAPLAYERQUADS, "Quad Layers")
		}
		Add(new CAddButton(this));
		
		Update();
	}
	
	void AddAsset()
	{
		#define ON_NEW_ASSET(TypeName, AssetName) case TypeName::TypeId:\
		{\
			TypeName* pAsset = m_pAssetsEditor->AssetManager()->GetAssetCatalog<TypeName>()->NewAsset(&NewAssetPath, m_Source);\
			char aBuf[128];\
			str_format(aBuf, sizeof(aBuf), AssetName, NewAssetPath.GetId());\
			pAsset->SetName(aBuf);\
			m_pAssetsEditor->NewAsset(NewAssetPath);\
			break;\
		}
			
		CAssetPath NewAssetPath;
		
		switch(m_AssetType)
		{
			case CAssetPath::TYPE_IMAGE:
			{
				m_pAssetsEditor->DisplayPopup(new CAssetsEditorGui_Popup_AddImage(
					m_pAssetsEditor, m_Rect, gui::CPopup::ALIGNMENT_RIGHT
				));
				break;
			}
			//Search Tag: TAG_NEW_ASSET
			ON_NEW_ASSET(CAsset_Sprite, "sprite%d")
			ON_NEW_ASSET(CAsset_Skeleton, "skeleton%d")
			ON_NEW_ASSET(CAsset_SkeletonSkin, "skin%d")
			ON_NEW_ASSET(CAsset_SkeletonAnimation, "animation%d")
			ON_NEW_ASSET(CAsset_Character, "character%d")
			ON_NEW_ASSET(CAsset_CharacterPart, "characterpart%d")
			ON_NEW_ASSET(CAsset_Weapon, "weapon%d")
			ON_NEW_ASSET(CAsset_MapGroup, "group%d")
			ON_NEW_ASSET(CAsset_MapLayerTiles, "tilelayer%d")
			ON_NEW_ASSET(CAsset_MapLayerQuads, "quadlayer%d")
		}
	}
};

class CAssetsEditorGui_AssetListTitle : public gui::CExternalTextButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_AssetSubPath;
	int m_Tab;

protected:
	virtual void MouseClickAction()
	{
		if(m_AssetSubPath >= 0)
			m_pAssetsEditor->EditAssetSubItem(m_AssetPath, m_AssetSubPath, m_Tab);
		else
			m_pAssetsEditor->EditAsset(m_AssetPath);
		
		m_pAssetsEditor->DisplayAsset(m_AssetPath);
	}
	
public:
	CAssetsEditorGui_AssetListTitle(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, char* pText, int IconId) :
		gui::CExternalTextButton(pAssetsEditor->m_pGuiConfig, pText, IconId),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_AssetSubPath(-1),
		m_Tab(0)
	{
		m_Centered = false;
	}
	
	void SetSubPath(int SubPath, int Tab)
	{
		m_AssetSubPath = SubPath;
		m_Tab = Tab;
	}
};

class CAssetsEditorGui_AssetListItem : public gui::CHListLayout
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CAssetsEditorGui_AssetListTitle* m_Button;
	int m_Source;

public:
	CAssetsEditorGui_AssetListItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source = -1) :
		gui::CHListLayout(pAssetsEditor->m_pGuiConfig, gui::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_FIRST),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		int IconId = -1;
		switch(AssetPath.GetType())
		{
			case CAssetPath::TYPE_IMAGE:
				IconId = CAssetsEditor::ICON_IMAGE;
				break;
			case CAssetPath::TYPE_SPRITE:
				IconId = CAssetsEditor::ICON_SPRITE;
				break;
			case CAssetPath::TYPE_SKELETON:
				IconId = CAssetsEditor::ICON_SKELETON;
				break;
			case CAssetPath::TYPE_SKELETONSKIN:
				IconId = CAssetsEditor::ICON_SKELETONSKIN;
				break;
			case CAssetPath::TYPE_SKELETONANIMATION:
				IconId = CAssetsEditor::ICON_SKELETONANIMATION;
				break;
			case CAssetPath::TYPE_CHARACTER:
				IconId = CAssetsEditor::ICON_CHARACTER;
				break;
			case CAssetPath::TYPE_CHARACTERPART:
				IconId = CAssetsEditor::ICON_CHARACTERPART;
				break;
			case CAssetPath::TYPE_WEAPON:
				IconId = CAssetsEditor::ICON_CHARACTERPART;
				break;
		}
		
		char* pName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		m_Button = new CAssetsEditorGui_AssetListTitle(m_pAssetsEditor, m_AssetPath, pName, IconId);
		Add(m_Button);
		
		Update();
	}
	
	virtual void Render()
	{
		if(m_pAssetsEditor->IsEditedAsset(m_AssetPath))
			m_Button->SetButtonStyle(gui::BUTTONSTYLE_HIGHLIGHT);
		else
			m_Button->SetButtonStyle(gui::BUTTONSTYLE_NORMAL);
		
		gui::CHListLayout::Render();
	}
};

class CAssetsEditorGui_AssetListImage : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CAssetsEditorGui_AssetListTitle* m_Button;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetsEditorGui_AssetListImage(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source),
		m_SourceFound(false)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* pName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		m_Button = new CAssetsEditorGui_AssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetsEditor::ICON_IMAGE);
		SetTitle(m_Button);
		
		for(int i=0; i<m_pAssetsEditor->AssetManager()->GetNumAssets<CAsset_Sprite>(m_Source); i++)
		{
			CAssetPath SpritePath = CAssetPath::Asset(CAsset_Sprite::TypeId, m_Source, i);
			CAssetPath ImagePath = m_pAssetsEditor->AssetManager()->GetAssetValue<CAssetPath>(SpritePath, CAsset_Sprite::IMAGEPATH, -1, 0);
			if(ImagePath == m_AssetPath)
			{
				CAssetsEditorGui_AssetListItem* pItem = new CAssetsEditorGui_AssetListItem(m_pAssetsEditor, SpritePath, m_Source);
				Add(pItem);
				m_SourceFound = true;
			}
		}
		
		Update();
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

class CAssetsEditorGui_AssetListSkeleton : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CAssetsEditorGui_AssetListTitle* m_Button;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetsEditorGui_AssetListSkeleton(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source),
		m_SourceFound(false)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* pName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		m_Button = new CAssetsEditorGui_AssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetsEditor::ICON_SKELETON);
		SetTitle(m_Button);
		
		for(int i=0; i<m_pAssetsEditor->AssetManager()->GetNumAssets<CAsset_SkeletonSkin>(m_Source); i++)
		{
			CAssetPath SkeletonSkinPath = CAssetPath::Asset(CAsset_SkeletonSkin::TypeId, m_Source, i);
			CAssetPath SkeletonPath = m_pAssetsEditor->AssetManager()->GetAssetValue<CAssetPath>(SkeletonSkinPath, CAsset_SkeletonSkin::SKELETONPATH, -1, 0);
			if(SkeletonPath == m_AssetPath)
			{
				Add(new CAssetsEditorGui_AssetListItem(m_pAssetsEditor, SkeletonSkinPath));
				m_SourceFound = true;
			}
		}
		
		for(int i=0; i<m_pAssetsEditor->AssetManager()->GetNumAssets<CAsset_SkeletonAnimation>(m_Source); i++)
		{
			CAssetPath SkeletonAnimationPath = CAssetPath::Asset(CAsset_SkeletonAnimation::TypeId, m_Source, i);
			CAssetPath SkeletonPath = m_pAssetsEditor->AssetManager()->GetAssetValue<CAssetPath>(SkeletonAnimationPath, CAsset_SkeletonAnimation::SKELETONPATH, -1, 0);
			if(SkeletonPath == m_AssetPath)
			{
				Add(new CAssetsEditorGui_AssetListItem(m_pAssetsEditor, SkeletonAnimationPath));
				m_SourceFound = true;
			}
		}
		
		Update();
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

class CAssetsEditorGui_AssetListCharacterPartType : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CAsset_Character::CSubPath m_SubPath;
	CAssetsEditorGui_AssetListTitle* m_Button;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetsEditorGui_AssetListCharacterPartType(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, CAsset_Character::CSubPath SubPath, int Source) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_SubPath(SubPath),
		m_Source(Source),
		m_SourceFound(false)
	{
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* pName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(m_AssetPath, CAsset_Character::PART_NAME, SubPath.ConvertToInteger(), 0);
		m_Button = new CAssetsEditorGui_AssetListTitle(m_pAssetsEditor, m_AssetPath, pName, -1);
		m_Button->SetSubPath(m_SubPath.ConvertToInteger(), CAssetsEditorGui_Editor::TAB_CHARACTER_PARTS);
		SetTitle(m_Button);
		
		for(int i=0; i<m_pAssetsEditor->AssetManager()->GetNumAssets<CAsset_CharacterPart>(m_Source); i++)
		{
			CAssetPath CharacterPartPath = CAssetPath::Asset(CAsset_CharacterPart::TypeId, m_Source, i);
			CAssetPath CharacterPath = m_pAssetsEditor->AssetManager()->GetAssetValue<CAssetPath>(CharacterPartPath, CAsset_CharacterPart::CHARACTERPATH, -1, CAssetPath::Null());
			int CharacterPart = m_pAssetsEditor->AssetManager()->GetAssetValue<int>(CharacterPartPath, CAsset_CharacterPart::CHARACTERPART, -1, 0);
			if(CharacterPath == m_AssetPath && CharacterPart == m_SubPath.ConvertToInteger())
			{
				Add(new CAssetsEditorGui_AssetListItem(m_pAssetsEditor, CharacterPartPath));
				m_SourceFound = true;
			}
		}
		
		Update();
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

class CAssetsEditorGui_AssetListCharacter : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CAssetsEditorGui_AssetListTitle* m_Button;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetsEditorGui_AssetListCharacter(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source),
		m_SourceFound(AssetPath.GetSource() == Source)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* pName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		m_Button = new CAssetsEditorGui_AssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetsEditor::ICON_CHARACTER);
		SetTitle(m_Button);
		
		CAsset_Character* pCharacter = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_Character>(m_AssetPath);
		if(pCharacter)
		{
			for(int i=0; i<pCharacter->m_Parts.size(); i++)
			{
				CAssetsEditorGui_AssetListCharacterPartType* pItem = new CAssetsEditorGui_AssetListCharacterPartType(m_pAssetsEditor, m_AssetPath, CAsset_Character::CSubPath::Part(i), m_Source);
				
				if(pItem->SourceFound())
				{
					Add(pItem);
					m_SourceFound = true;
				}
				else
					delete pItem;
			}
		}
		
		Update();
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

class CAssetsEditorGui_AssetListMapGroup : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CAssetsEditorGui_AssetListTitle* m_Button;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetsEditorGui_AssetListMapGroup(
		CAssetsEditor* pAssetsEditor,
		CAssetPath AssetPath,
		int Source,
		bool* MapLayerTilesFound,
		bool* MapLayerQuadsFound
	) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source),
		m_SourceFound(false)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		CAsset_MapGroup* pMapGroup = m_pAssetsEditor->AssetManager()->GetAsset<CAsset_MapGroup>(m_AssetPath);
		if(pMapGroup)
		{			
			char* pName = m_pAssetsEditor->AssetManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
			m_Button = new CAssetsEditorGui_AssetListTitle(m_pAssetsEditor, m_AssetPath, pName, -1);
			SetTitle(m_Button);
			
			for(int i=0; i<pMapGroup->m_Layers.size(); i++)
			{
				bool AddItem = false;
				
				if(pMapGroup->m_Layers[i].GetType() == CAssetPath::TYPE_MAPLAYERTILES)
				{
					MapLayerTilesFound[pMapGroup->m_Layers[i].GetId()] = true;
					AddItem = true;
				}
				else if(pMapGroup->m_Layers[i].GetType() == CAssetPath::TYPE_MAPLAYERQUADS)
				{
					MapLayerQuadsFound[pMapGroup->m_Layers[i].GetId()] = true;
					AddItem = true;
				}
				
				if(AddItem)
				{
					CAssetsEditorGui_AssetListItem* pItem = new CAssetsEditorGui_AssetListItem(m_pAssetsEditor, pMapGroup->m_Layers[i], m_Source);
					Add(pItem);
					m_SourceFound = true;
				}
			}
		}
		
		Update();
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

/* ASSETS EDITOR ******************************************************/

tu::IAssetsEditor *CreateAssetsEditor() { return new CAssetsEditor; }

CAssetsEditor::CAssetsEditor()
{
	m_pGuiConfig = 0;
	m_pHintLabel = 0;
}

CAssetsEditor::~CAssetsEditor()
{
	delete m_pGuiAssetListTabs;
	delete m_pGuiAssetEditor;
	delete m_pGuiView;
	delete m_pGuiToolbar;
	for(int i=0; i<m_GuiPopups.size(); i++)
	{
		if(m_GuiPopups[i]) delete m_GuiPopups[i];
	}
	
	if(m_pGuiConfig) delete m_pGuiConfig;
}
	
void CAssetsEditor::Init(CAssetManager* pAssetManager, CClient_Graphics* pTUGraphics)
{
	m_pClient = Kernel()->RequestInterface<IClient>();
	m_pInput = Kernel()->RequestInterface<IInput>();
	m_pGraphics = Kernel()->RequestInterface<IGraphics>();
	m_pTextRender = Kernel()->RequestInterface<ITextRender>();
	m_pStorage = Kernel()->RequestInterface<IStorage>();
	m_pAssetsFile = Kernel()->RequestInterface<tu::IAssetsFile>();
	m_pTUGraphics = pTUGraphics;
	m_pAssetManager = pAssetManager;
	
	m_RenderTools.m_pGraphics = m_pGraphics;
		
	m_CursorTexture = Graphics()->LoadTexture("editor/cursor.png", IStorage::TYPE_ALL, CImageInfo::FORMAT_AUTO, 0);
	m_ModEditorTexture = Graphics()->LoadTexture("modapi/modeditor.png", IStorage::TYPE_ALL, CImageInfo::FORMAT_AUTO, 0);
	
	m_pGuiConfig = new gui::CConfig(Graphics(), RenderTools(), TextRender(), Input(), m_ModEditorTexture);
	m_pGuiConfig->m_fShowHint = CAssetsEditor::ShowHint;
	m_pGuiConfig->m_pShowHintData = (void*) this;
	
	int Margin = 5;
	
	int MenuBarHeight = 30;
	int PanelWidth = 250;
	int PanelHeight = 250;
	gui::CRect ToolbarRect(Margin, Margin, Graphics()->ScreenWidth()-2*Margin, MenuBarHeight);
	gui::CRect AssetListRect(Margin, ToolbarRect.y+ToolbarRect.h+Margin, PanelWidth, Graphics()->ScreenHeight()-3*Margin-ToolbarRect.h);
	gui::CRect ViewRect(AssetListRect.x + AssetListRect.w + Margin, AssetListRect.y, Graphics()->ScreenWidth() - 2*PanelWidth - 4*Margin, Graphics()->ScreenHeight() - 4*Margin - PanelHeight - MenuBarHeight);
	gui::CRect AssetEditorRect(ViewRect.x + ViewRect.w + Margin, AssetListRect.y, PanelWidth, AssetListRect.h);
	gui::CRect TimelineRect(AssetListRect.x + AssetListRect.w + Margin, ViewRect.y + ViewRect.h + Margin, ViewRect.w, PanelHeight);
	
	m_pHintLabel = new gui::CLabel(m_pGuiConfig, "");
	
	m_pGuiToolbar = new gui::CHListLayout(m_pGuiConfig, gui::LAYOUTSTYLE_FRAME, gui::LAYOUTFILLING_LAST);
	m_pGuiToolbar->SetRect(ToolbarRect);
	m_pGuiToolbar->Add(new CAssetsEditorGui_Button_ToolbarLoad(this));
	m_pGuiToolbar->Add(new CAssetsEditorGui_Button_ToolbarSave(this));
	m_pGuiToolbar->Add(new CAssetsEditorGui_Button_ToolbarExit(this));
	m_pGuiToolbar->Add(m_pHintLabel);
	m_pGuiToolbar->Update();
	
	m_pGuiAssetListTabs = new gui::CTabs(m_pGuiConfig);
	m_pGuiAssetListTabs->SetRect(AssetListRect);
	
	m_pGuiAssetList[CAssetPath::SRC_EXTERNAL] = new gui::CVListLayout(m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
	m_pGuiAssetListTabs->AddTab(m_pGuiAssetList[CAssetPath::SRC_EXTERNAL], CAssetsEditor::ICON_EXTERNAL_ASSET, "External assets");
	
	m_pGuiAssetList[CAssetPath::SRC_INTERNAL] = new gui::CVListLayout(m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
	m_pGuiAssetListTabs->AddTab(m_pGuiAssetList[CAssetPath::SRC_INTERNAL], CAssetsEditor::ICON_INTERNAL_ASSET, "Internal assets");
	
	m_pGuiAssetList[CAssetPath::SRC_MAP] = new gui::CVListLayout(m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
	m_pGuiAssetListTabs->AddTab(m_pGuiAssetList[CAssetPath::SRC_MAP], CAssetsEditor::ICON_MAP_ASSET, "Map assets");
	
	m_pGuiAssetList[CAssetPath::SRC_SKIN] = new gui::CVListLayout(m_pGuiConfig, gui::LAYOUTSTYLE_NONE);
	m_pGuiAssetListTabs->AddTab(m_pGuiAssetList[CAssetPath::SRC_SKIN], CAssetsEditor::ICON_SKIN_ASSET, "Skin assets");
	
	m_pGuiAssetListTabs->Update();
	RefreshAssetList();
	
	m_pGuiView = new CAssetsEditorGui_View(this);
	m_pGuiView->SetRect(ViewRect);
	m_pGuiView->Update();
	
	m_pGuiTimeline = new CAssetsEditorGui_Timeline(this);
	m_pGuiTimeline->SetRect(TimelineRect);
	m_pGuiTimeline->Update();
	
	m_pGuiAssetEditor = new CAssetsEditorGui_Editor(this);
	m_pGuiAssetEditor->SetRect(AssetEditorRect);
	m_pGuiAssetEditor->Update();
	
	m_RefreshAssetEditor = false;
	m_EditorTab = -1;
	
	m_ShowCursor = true;
	
	m_LastTime = -1.0f;
	m_Time = 0.0f;
	m_Paused = true;
	
	m_EditedAssetSubPath = -1;
	m_AssetsListSource = CAssetPath::SRC_EXTERNAL;
}

void CAssetsEditor::ShowHint(const char* pText, void* pData)
{
	CAssetsEditor* pThis = (CAssetsEditor*) pData;
	if(pThis->m_pHintLabel)
	{
		pThis->m_pHintLabel->SetText(pText);
		pThis->m_Hint = true;
	}
}

void CAssetsEditor::RefreshAssetList(int Source)
{
	m_pGuiAssetList[Source]->Clear();
	
	switch(Source)
	{
		case CAssetPath::SRC_INTERNAL:
			m_pGuiAssetList[Source]->Add(new gui::CLabel(m_pGuiConfig, "Internal assets", gui::TEXTSTYLE_HEADER));
			break;
		case CAssetPath::SRC_EXTERNAL:
			m_pGuiAssetList[Source]->Add(new gui::CLabel(m_pGuiConfig, "External assets", gui::TEXTSTYLE_HEADER));
			break;
		case CAssetPath::SRC_MAP:
			m_pGuiAssetList[Source]->Add(new gui::CLabel(m_pGuiConfig, "Map assets", gui::TEXTSTYLE_HEADER));
			break;
		case CAssetPath::SRC_SKIN:
			m_pGuiAssetList[Source]->Add(new gui::CLabel(m_pGuiConfig, "Skin assets", gui::TEXTSTYLE_HEADER));
			break;
	}
	
	//AntiDoubleeEntries
	bool MapLayerTilesFound[1024];
	bool MapLayerQuadsFound[1024];
	
	for(int i=0; i<AssetManager()->GetNumAssets<CAsset_MapLayerTiles>(Source); i++)
		MapLayerTilesFound[i] = false;
	for(int i=0; i<AssetManager()->GetNumAssets<CAsset_MapLayerQuads>(Source); i++)
		MapLayerQuadsFound[i] = false;
	
	//Images
	m_pGuiAssetList[Source]->AddSeparator();
	m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_Image::TypeId, Source));
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetManager()->GetNumAssets<CAsset_Image>(s); i++)
		{
			CAssetsEditorGui_AssetListImage* pItem = new CAssetsEditorGui_AssetListImage(this, CAssetPath::Asset(CAsset_Image::TypeId, s, i), Source);
			if(pItem->SourceFound())
				m_pGuiAssetList[Source]->Add(pItem);
			else
				delete pItem;
		}
	}
	
	//Sprites
	{
		int nbAssets = AssetManager()->GetNumAssets<CAsset_Sprite>(Source);
		if(nbAssets > 0)
		{
			m_pGuiAssetList[Source]->AddSeparator();
			m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_Sprite::TypeId, Source));
			for(int i=0; i<nbAssets; i++)
			{
				CAssetPath SpritePath = CAssetPath::Asset(CAsset_Sprite::TypeId, Source, i);
				CAssetPath ImagePath = AssetManager()->GetAssetValue<CAssetPath>(SpritePath, CAsset_Sprite::IMAGEPATH, -1, 0);
				if(!AssetManager()->GetAsset<CAsset_Image>(ImagePath))
				{
					m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListItem(this, SpritePath));
				}
			}
		}
	}
	
	//Skeletons
	m_pGuiAssetList[Source]->AddSeparator();
	m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_Skeleton::TypeId, Source));
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetManager()->GetNumAssets<CAsset_Skeleton>(s); i++)
		{
			CAssetsEditorGui_AssetListSkeleton* pItem = new CAssetsEditorGui_AssetListSkeleton(this, CAssetPath::Asset(CAsset_Skeleton::TypeId, s, i), Source);
			if(pItem->SourceFound())
				m_pGuiAssetList[Source]->Add(pItem);
			else
				delete pItem;
		}
	}
	
	//SkeletonSkins
	{
		int nbAssets = AssetManager()->GetNumAssets<CAsset_SkeletonSkin>(Source);
		if(nbAssets > 0)
		{
			m_pGuiAssetList[Source]->AddSeparator();
			m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_SkeletonSkin::TypeId, Source));
			for(int i=0; i<nbAssets; i++)
			{
				CAssetPath SkeletonSkinPath = CAssetPath::Asset(CAsset_SkeletonSkin::TypeId, Source, i);
				CAssetPath SkeletonPath = AssetManager()->GetAssetValue<CAssetPath>(SkeletonSkinPath, CAsset_SkeletonSkin::SKELETONPATH, -1, 0);
				if(!AssetManager()->GetAsset<CAsset_Skeleton>(SkeletonPath))
				{
					m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListItem(this, SkeletonSkinPath));
				}
			}
		}
	}
	
	//SkeletonAnimations
	{
		int nbAssets = AssetManager()->GetNumAssets<CAsset_SkeletonAnimation>(Source);
		if(nbAssets > 0)
		{
			m_pGuiAssetList[Source]->AddSeparator();
			m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_SkeletonAnimation::TypeId, Source));
			for(int i=0; i<nbAssets; i++)
			{
				CAssetPath SkeletonAnimationPath = CAssetPath::Asset(CAsset_SkeletonAnimation::TypeId, Source, i);
				CAssetPath SkeletonPath = AssetManager()->GetAssetValue<CAssetPath>(SkeletonAnimationPath, CAsset_SkeletonAnimation::SKELETONPATH, -1, 0);
				if(!AssetManager()->GetAsset<CAsset_Skeleton>(SkeletonPath))
				{
					m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListItem(this, SkeletonAnimationPath));
				}
			}
		}
	}
	
	//Search Tag: TAG_NEW_ASSET
	
	//Character
	m_pGuiAssetList[Source]->AddSeparator();
	m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_Character::TypeId, Source));
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetManager()->GetNumAssets<CAsset_Character>(s); i++)
		{
			CAssetsEditorGui_AssetListCharacter* pItem = new CAssetsEditorGui_AssetListCharacter(this, CAssetPath::Asset(CAsset_Character::TypeId, s, i), Source);
			if(pItem->SourceFound())
				m_pGuiAssetList[Source]->Add(pItem);
			else
				delete pItem;
		}
	}
	
	//CharacterPart
	{
		int nbAssets = AssetManager()->GetNumAssets<CAsset_CharacterPart>(Source);
		if(nbAssets > 0)
		{
			m_pGuiAssetList[Source]->AddSeparator();
			m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_CharacterPart::TypeId, Source));
			for(int i=0; i<nbAssets; i++)
			{
				CAssetPath CharacterPartPath = CAssetPath::Asset(CAsset_CharacterPart::TypeId, Source, i);
				CAssetPath CharacterPath = AssetManager()->GetAssetValue<CAssetPath>(CharacterPartPath, CAsset_CharacterPart::CHARACTERPATH, -1, 0);
				CAsset_Character* pCharacter = AssetManager()->GetAsset<CAsset_Character>(CharacterPath);
				if(!pCharacter)
				{
					m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListItem(this, CharacterPartPath));
				}
				else
				{
					int CharacterPart = AssetManager()->GetAssetValue<int>(CharacterPartPath, CAsset_CharacterPart::CHARACTERPART, -1, 0);
					if(CharacterPart >= pCharacter->m_Parts.size() || CharacterPart < 0)
					{
						m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListItem(this, CharacterPartPath));
					}
				}
			}
		}
	}
	
	#define REFRESH_ASSET_LIST(TypeName) \
	{\
		int nbAssets = AssetManager()->GetNumAssets<TypeName>(Source);\
		if(nbAssets > 0)\
		{\
			m_pGuiAssetList[Source]->AddSeparator();\
			m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, TypeName::TypeId, Source));\
			for(int i=0; i<nbAssets; i++)\
			{\
				m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListItem(this, CAssetPath::Asset(TypeName::TypeId, Source, i)));\
			}\
		}\
	}
	
	REFRESH_ASSET_LIST(CAsset_Weapon)
	
	//MapGroup
	m_pGuiAssetList[Source]->AddSeparator();
	m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_MapGroup::TypeId, Source));
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetManager()->GetNumAssets<CAsset_MapGroup>(s); i++)
		{
			CAssetsEditorGui_AssetListMapGroup* pItem = new CAssetsEditorGui_AssetListMapGroup(
				this,
				CAssetPath::Asset(CAsset_MapGroup::TypeId, s, i),
				Source,
				MapLayerTilesFound,
				MapLayerQuadsFound
			);
			
			if(pItem->SourceFound())
				m_pGuiAssetList[Source]->Add(pItem);
			else
				delete pItem;
		}
	}
	
	//MapLayer
	{
		int nbAssets = AssetManager()->GetNumAssets<CAsset_MapLayerTiles>(Source);
		if(nbAssets > 0)
		{
			m_pGuiAssetList[Source]->AddSeparator();
			m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_MapLayerTiles::TypeId, Source));
			for(int i=0; i<nbAssets; i++)
			{
				if(!MapLayerTilesFound[i])
				{
					CAssetPath MapLayerPath = CAssetPath::Asset(CAsset_MapLayerTiles::TypeId, Source, i);
					m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListItem(this, MapLayerPath));
				}
			}
		}
	}
	{
		int nbAssets = AssetManager()->GetNumAssets<CAsset_MapLayerQuads>(Source);
		if(nbAssets > 0)
		{
			m_pGuiAssetList[Source]->AddSeparator();
			m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListHeader(this, CAsset_MapLayerQuads::TypeId, Source));
			for(int i=0; i<nbAssets; i++)
			{
				if(!MapLayerQuadsFound[i])
				{
					CAssetPath MapLayerPath = CAssetPath::Asset(CAsset_MapLayerQuads::TypeId, Source, i);
					m_pGuiAssetList[Source]->Add(new CAssetsEditorGui_AssetListItem(this, MapLayerPath));
				}
			}
		}
	}
	
	m_pGuiAssetList[Source]->Update();
}

void CAssetsEditor::RefreshAssetList()
{
	RefreshAssetList(CAssetPath::SRC_INTERNAL);
	RefreshAssetList(CAssetPath::SRC_EXTERNAL);
	RefreshAssetList(CAssetPath::SRC_MAP);
	RefreshAssetList(CAssetPath::SRC_SKIN);
}

void CAssetsEditor::RefreshAssetEditor(int Tab)
{
	m_EditorTab = Tab;
	m_RefreshAssetEditor = true;
}

void CAssetsEditor::Render()
{
	Graphics()->MapScreen(0, 0, Graphics()->ScreenWidth(), Graphics()->ScreenHeight());
	m_pGuiToolbar->Render();
	m_pGuiAssetListTabs->Render();
	m_pGuiAssetEditor->Render();
	m_pGuiView->Render();
	m_pGuiTimeline->Render();
	
	for(int i=0; i<m_GuiPopups.size(); i++)
	{
		if(m_GuiPopups[i])
			m_GuiPopups[i]->Render();
	}
	
	//Cursor
	if(m_ShowCursor)
	{
		Graphics()->TextureSet(m_CursorTexture);
		Graphics()->QuadsBegin();
		IGraphics::CQuadItem QuadItem(m_MousePos.x,m_MousePos.y, 16.0f, 16.0f);
		Graphics()->QuadsDrawTL(&QuadItem, 1);
		Graphics()->QuadsEnd();
	}
}

void CAssetsEditor::HideCursor()
{
	//~ m_ShowCursor = false;
}

void CAssetsEditor::ShowCursor()
{
	//~ m_ShowCursor = true;
}

void CAssetsEditor::UpdateAndRender()
{
	if(Input()->KeyIsPressed(KEY_ESCAPE))
	{
		CloseEditor();
	}
	
	m_Hint = false;
	
	//Update time
	if(!m_Paused)
	{
		if(m_LastTime < 0.0f) m_Time = 0.0f;
		else m_Time += (Client()->LocalTime() - m_LastTime);
	}
	m_LastTime = Client()->LocalTime();
	
	//Update popup state
	for(int i=0; i<m_GuiPopups.size(); i++)
	{
		if(m_GuiPopups[i]->IsClosed())
		{
			delete m_GuiPopups[i];
			m_GuiPopups.remove_index(i);
		}
	}
	
	if(m_RefreshAssetEditor)
	{
		m_pGuiAssetEditor->Refresh(m_EditorTab);
		m_RefreshAssetEditor = false;
	}
	
	//Input
	int Keys = 0x0;
		
	if(Input()->KeyIsPressed(KEY_LCTRL) || Input()->KeyIsPressed(KEY_RCTRL))
		Keys |= TU_INPUT_CTRL;
		
	if(Input()->KeyIsPressed(KEY_LSHIFT) || Input()->KeyIsPressed(KEY_RSHIFT))
		Keys |= TU_INPUT_SHIFT;
	
	if(Input()->KeyIsPressed(KEY_LALT) || Input()->KeyIsPressed(KEY_RALT))
		Keys |= TU_INPUT_ALT;
	
	Input()->MouseRelative(&m_MouseDelta.x, &m_MouseDelta.y);

	m_MousePos += m_MouseDelta;
	m_MousePos.x = clamp(m_MousePos.x, 0.0f, static_cast<float>(Graphics()->ScreenWidth()));
	m_MousePos.y = clamp(m_MousePos.y, 0.0f, static_cast<float>(Graphics()->ScreenHeight()));
	
	if(m_GuiPopups.size() > 0)
	{
		for(int i=0; i<m_GuiPopups.size(); i++)
		{
			m_GuiPopups[i]->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
			m_GuiPopups[i]->OnInputEvent();
		}
	}
	else
	{
		m_pGuiToolbar->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		m_pGuiAssetListTabs->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		m_pGuiAssetEditor->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		m_pGuiView->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		m_pGuiTimeline->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		
		m_pGuiToolbar->OnInputEvent();
		m_pGuiAssetListTabs->OnInputEvent();
		m_pGuiAssetEditor->OnInputEvent();
		m_pGuiView->OnInputEvent();
		m_pGuiTimeline->OnInputEvent();
	}
	
	for(int i=0; i<3; i++)
	{
		int Button = KEY_MOUSE_1+i;
		
		if(Input()->KeyIsPressed(Button))
		{
			if(m_MouseButton[i] == 0)
			{
				m_MouseButton[i] = 1;
				
				if(m_GuiPopups.size() > 0)
				{
					for(int i=0; i<m_GuiPopups.size(); i++)
					{
						m_GuiPopups[i]->OnButtonClick(m_MousePos.x, m_MousePos.y, Button);
					}
				}
				else
				{
					m_pGuiToolbar->OnButtonClick(m_MousePos.x, m_MousePos.y, Button);
					m_pGuiAssetListTabs->OnButtonClick(m_MousePos.x, m_MousePos.y, Button);
					m_pGuiAssetEditor->OnButtonClick(m_MousePos.x, m_MousePos.y, Button);
					m_pGuiView->OnButtonClick(m_MousePos.x, m_MousePos.y, Button);
					m_pGuiTimeline->OnButtonClick(m_MousePos.x, m_MousePos.y, Button);
				}
			}
		}
		else
		{
			if(m_MouseButton[i] == 1)
			{
				m_MouseButton[i] = 0;
				
				if(m_GuiPopups.size() > 0)
				{
					for(int i=0; i<m_GuiPopups.size(); i++)
					{
						m_GuiPopups[i]->OnButtonRelease(Button);
					}
				}
				else
				{
					m_pGuiToolbar->OnButtonRelease(Button);
					m_pGuiAssetListTabs->OnButtonRelease(Button);
					m_pGuiAssetEditor->OnButtonRelease(Button);
					m_pGuiView->OnButtonRelease(Button);
					m_pGuiTimeline->OnButtonRelease(Button);
				}
			}
		}
	}
	
	//Rendering
	Render();
	
	if(!m_Hint)
	{
		m_pHintLabel->SetText("");
	}

	Input()->Clear();
}

bool CAssetsEditor::HasUnsavedData() const
{
	
}

void CAssetsEditor::DisplayPopup(gui::CPopup* pWidget)
{
	m_GuiPopups.add(pWidget);
}

bool CAssetsEditor::IsEditedAsset(CAssetPath AssetPath)
{
	return (m_EditedAssetPath == AssetPath);
}

bool CAssetsEditor::IsDisplayedAsset(CAssetPath AssetPath)
{
	return (m_ViewedAssetPath == AssetPath);
}

void CAssetsEditor::EditAsset(CAssetPath AssetPath)
{
	m_EditedAssetPath = AssetPath;
	m_EditedAssetSubPath = -1;
	m_Paused = true;
	RefreshAssetEditor(-1);
}

void CAssetsEditor::EditAssetSubItem(CAssetPath AssetPath, int SubPath, int Tab)
{
	m_EditedAssetPath = AssetPath;
	m_EditedAssetSubPath = SubPath;
	m_Paused = true;
	RefreshAssetEditor(Tab);
}

void CAssetsEditor::EditAssetFirstFrame()
{
	m_Time = 0.0f;
}

void CAssetsEditor::EditAssetPrevFrame()
{
	if(m_EditedAssetPath.GetType() == CAssetPath::TYPE_SKELETONANIMATION)
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = AssetManager()->GetAsset<CAsset_SkeletonAnimation>(m_EditedAssetPath);
		if(pSkeletonAnimation)
		{
			float MaxTime = m_Time;
			m_Time = 0.0f;
			
			//Bones
			for(int i=0; i<pSkeletonAnimation->m_BoneAnimations.size(); i++)
			{
				for(int j=0; j<pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames.size(); j++)
				{
					float Time = pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames[j].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
					
					if(Time >= MaxTime)
						break;
						
					if(Time > m_Time)
						m_Time = Time;
				}
			}
			
			//Layers
			for(int i=0; i<pSkeletonAnimation->m_LayerAnimations.size(); i++)
			{
				for(int j=0; j<pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames.size(); j++)
				{
					float Time = pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames[j].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
					
					if(Time >= MaxTime)
						break;
						
					if(Time > m_Time)
						m_Time = Time;
				}
			}
		}
	}
}

void CAssetsEditor::EditAssetNextFrame()
{
	if(m_EditedAssetPath.GetType() == CAssetPath::TYPE_SKELETONANIMATION)
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = AssetManager()->GetAsset<CAsset_SkeletonAnimation>(m_EditedAssetPath);
		if(pSkeletonAnimation)
		{
			float MinTime = m_Time;
			bool Initialized = false;
			
			//Bones
			for(int i=0; i<pSkeletonAnimation->m_BoneAnimations.size(); i++)
			{
				for(int j=pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames.size()-1; j>=0; j--)
				{
					float Time = pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames[j].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
					
					if(Time <= MinTime)
						break;
						
					if(Time < m_Time || !Initialized)
					{
						Initialized = true;
						m_Time = Time;
					}
				}
			}
			
			//Layers
			for(int i=0; i<pSkeletonAnimation->m_LayerAnimations.size(); i++)
			{
				for(int j=pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames.size()-1; j>=0; j--)
				{
					float Time = pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames[j].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
					
					if(Time <= MinTime)
						break;
						
					if(Time < m_Time || !Initialized)
					{
						Initialized = true;
						m_Time = Time;
					}
				}
			}
		}
	}
}

void CAssetsEditor::EditAssetLastFrame()
{
	if(m_EditedAssetPath.GetType() == CAssetPath::TYPE_SKELETONANIMATION)
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = AssetManager()->GetAsset<CAsset_SkeletonAnimation>(m_EditedAssetPath);
		if(pSkeletonAnimation)
		{
			m_Time = 0.0f;
			
			//Bones
			for(int i=0; i<pSkeletonAnimation->m_BoneAnimations.size(); i++)
			{
				if(pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames.size() > 0)
				{
					int KeyId = pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames.size()-1;
					float Time = pSkeletonAnimation->m_BoneAnimations[i].m_KeyFrames[KeyId].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
					if(Time > m_Time)
						m_Time = Time;
				}
			}
			
			//Layers
			for(int i=0; i<pSkeletonAnimation->m_LayerAnimations.size(); i++)
			{
				if(pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames.size() > 0)
				{
					int KeyId = pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames.size()-1;
					float Time = pSkeletonAnimation->m_LayerAnimations[i].m_KeyFrames[KeyId].m_Time / static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
					if(Time > m_Time)
						m_Time = Time;
				}
			}
		}
	}
}

void CAssetsEditor::DisplayAsset(CAssetPath AssetPath)
{
	m_ViewedAssetPath = AssetPath;
}

void CAssetsEditor::NewAsset(CAssetPath AssetPath)
{
	RefreshAssetList();
	DisplayAsset(AssetPath);
	EditAsset(AssetPath);
}

void CAssetsEditor::DeleteAsset(CAssetPath AssetPath)
{
	AssetManager()->DeleteAsset(AssetPath);
	
	RefreshAssetList();
	
	m_EditedAssetPath = CAssetPath::Null();
	m_ViewedAssetPath = CAssetPath::Null();
}

void CAssetsEditor::SetPause(bool Pause)
{
	m_Paused = Pause;
}

bool CAssetsEditor::IsPaused()
{
	return m_Paused;
}

void CAssetsEditor::SetTime(float Time)
{
	m_Time = Time;
}

float CAssetsEditor::GetTime()
{
	return m_Time;
}

void CAssetsEditor::CloseEditor()
{
	g_Config.m_ClMode = TU_CLIENTMODE_GAME;
}

void CAssetsEditor::LoadAssetsFile(const char* pFilename)
{
	Client()->LoadAssetsFile(pFilename);
	RefreshAssetList();
}

}
