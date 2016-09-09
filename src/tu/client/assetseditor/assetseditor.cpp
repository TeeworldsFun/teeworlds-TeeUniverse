#include <base/tl/sorted_array.h>

#include <engine/shared/config.h>
#include <engine/shared/datafile.h>

#include <engine/client.h>
#include <engine/input.h>
#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/storage.h>

#include <tu/client/clientmode.h>
#include <tu/client/gui/button.h>
#include <tu/client/gui/label.h>
#include <tu/client/gui/layout.h>
#include <tu/client/gui/integer-edit.h>
#include <tu/client/gui/text-edit.h>
#include <tu/client/gui/expand.h>

#include <cstddef>

#include "assetseditor.h"
#include "popup.h"
#include "editor.h"
#include "view.h"
#include "timeline.h"

namespace tu
{

namespace assetseditor
{

/* ITEM LIST **********************************************************/

class CAssetListTitleDisabled : public gui::CIconLabel
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	
public:
	CAssetListTitleDisabled(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, const char* pText, CAssetPath IconPath) :
		gui::CIconLabel(pAssetsEditor->m_pGuiConfig, pText, IconPath),
		m_pAssetsEditor(pAssetsEditor)
	{ }
};

class CAssetListTitle : public gui::CExternalTextButton
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
	CAssetListTitle(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, char* pText, int IconId) :
		gui::CExternalTextButton(pAssetsEditor->m_pGuiConfig, pText, IconId),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_AssetSubPath(-1),
		m_Tab(0)
	{
		m_Centered = false;
		
		CAssetState* pState = m_pAssetsEditor->AssetsManager()->GetAssetState(m_AssetPath);
		if(pState)
			pState->m_ListedInEditor = true;
	}
	
	void SetSubPath(int SubPath, int Tab)
	{
		m_AssetSubPath = SubPath;
		m_Tab = Tab;
	}
	
	virtual void Render()
	{
		if(m_pAssetsEditor->IsEditedAsset(m_AssetPath))
			SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK_HIGHLIGHT);
		else
			SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
		
		gui::CExternalTextButton::Render();
	}
};

class CAssetListItem : public gui::CHListLayout
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CAssetListTitle* m_Button;
	int m_Source;

public:
	CAssetListItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source = -1) :
		gui::CHListLayout(pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_FIRST),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		int IconId = -1;
		switch(AssetPath.GetType())
		{
			case CAssetPath::TYPE_IMAGE:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_IMAGE);
				break;
			case CAssetPath::TYPE_SPRITE:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_SPRITE);
				break;
			case CAssetPath::TYPE_SKELETON:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_SKELETON);
				break;
			case CAssetPath::TYPE_SKELETONSKIN:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_SKELETONSKIN);
				break;
			case CAssetPath::TYPE_SKELETONANIMATION:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_SKELETONANIMATION);
				break;
			case CAssetPath::TYPE_CHARACTER:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_CHARACTER);
				break;
			case CAssetPath::TYPE_CHARACTERPART:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_CHARACTERPART);
				break;
			case CAssetPath::TYPE_WEAPON:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_WEAPON);
				break;
			case CAssetPath::TYPE_MAPZONETILES:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_ZONETILES);
				break;
			case CAssetPath::TYPE_MAPGROUP:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_FOLDER);
				break;
			case CAssetPath::TYPE_MAPLAYERTILES:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_TILES);
				break;
			case CAssetPath::TYPE_MAPLAYERQUADS:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_QUAD);
				break;
			case CAssetPath::TYPE_ZONETYPE:
				IconId = CAssetPath::SpriteUniverse(SPRITE_ICON_ZONETYPE);
				break;
		}
		
		char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		m_Button = new CAssetListTitle(m_pAssetsEditor, m_AssetPath, pName, IconId);
		Add(m_Button);
		
		Update();
	}
	
	virtual void Render()
	{
		if(m_pAssetsEditor->IsEditedAsset(m_AssetPath))
			m_Button->SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK_HIGHLIGHT);
		else
			m_Button->SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
		
		gui::CHListLayout::Render();
	}
};

class CAssetListImage : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetListImage(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source),
		m_SourceFound(false)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		
		if(m_AssetPath.GetSource() == m_Source)
		{
			CAssetListTitle* pButton = new CAssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_IMAGE));
			pButton->SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			SetTitle(pButton);
		}
		else
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_IMAGE)));
		
		
		for(int i=0; i<m_pAssetsEditor->AssetsManager()->GetNumAssets<CAsset_Sprite>(m_Source); i++)
		{
			CAssetPath SpritePath = CAssetPath::Asset(CAsset_Sprite::TypeId, m_Source, i);
			CAssetPath ImagePath = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(SpritePath, CAsset_Sprite::IMAGEPATH, -1, 0);
			if(ImagePath == m_AssetPath)
			{
				CAssetListItem* pItem = new CAssetListItem(m_pAssetsEditor, SpritePath, m_Source);
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

class CAssetListSkeleton : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetListSkeleton(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source),
		m_SourceFound(false)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		
		if(m_AssetPath.GetSource() == m_Source)
		{
			CAssetListTitle* pButton = new CAssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_SKELETON));
			pButton->SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			SetTitle(pButton);
		}
		else
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_SKELETON)));
			
		
		for(int i=0; i<m_pAssetsEditor->AssetsManager()->GetNumAssets<CAsset_SkeletonSkin>(m_Source); i++)
		{
			CAssetPath SkeletonSkinPath = CAssetPath::Asset(CAsset_SkeletonSkin::TypeId, m_Source, i);
			CAssetPath SkeletonPath = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(SkeletonSkinPath, CAsset_SkeletonSkin::SKELETONPATH, -1, 0);
			if(SkeletonPath == m_AssetPath)
			{
				Add(new CAssetListItem(m_pAssetsEditor, SkeletonSkinPath));
				m_SourceFound = true;
			}
		}
		
		for(int i=0; i<m_pAssetsEditor->AssetsManager()->GetNumAssets<CAsset_SkeletonAnimation>(m_Source); i++)
		{
			CAssetPath SkeletonAnimationPath = CAssetPath::Asset(CAsset_SkeletonAnimation::TypeId, m_Source, i);
			CAssetPath SkeletonPath = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(SkeletonAnimationPath, CAsset_SkeletonAnimation::SKELETONPATH, -1, 0);
			if(SkeletonPath == m_AssetPath)
			{
				Add(new CAssetListItem(m_pAssetsEditor, SkeletonAnimationPath));
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

class CAssetListCharacterPartType : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CAsset_Character::CSubPath m_SubPath;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetListCharacterPartType(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, CAsset_Character::CSubPath SubPath, int Source) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_SubPath(SubPath),
		m_Source(Source),
		m_SourceFound(false)
	{
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset_Character::PART_NAME, SubPath, 0);
		
		if(m_AssetPath.GetSource() == m_Source)
		{
			CAssetListTitle* pButton = new CAssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_DEFAULT));
			pButton->SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			pButton->SetSubPath(m_SubPath, CEditor::TAB_CHARACTER_PARTS);
			SetTitle(pButton);
		}
		else
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_DEFAULT)));
		
		
		for(int i=0; i<m_pAssetsEditor->AssetsManager()->GetNumAssets<CAsset_CharacterPart>(m_Source); i++)
		{
			CAssetPath CharacterPartPath = CAssetPath::Asset(CAsset_CharacterPart::TypeId, m_Source, i);
			CAssetPath CharacterPath = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(CharacterPartPath, CAsset_CharacterPart::CHARACTERPATH, -1, CAssetPath::Null());
			int CharacterPart = m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(CharacterPartPath, CAsset_CharacterPart::CHARACTERPART, -1, 0);
			if(CharacterPath == m_AssetPath && CharacterPart == m_SubPath)
			{
				Add(new CAssetListItem(m_pAssetsEditor, CharacterPartPath));
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

class CAssetListCharacter : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetListCharacter(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source),
		m_SourceFound(AssetPath.GetSource() == Source)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
		
		char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		
		if(m_AssetPath.GetSource() == m_Source)
		{
			CAssetListTitle* pButton = new CAssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_CHARACTER));
			pButton->SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			SetTitle(pButton);
		}
		else
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_CHARACTER)));
		
		
		CAsset_Character* pCharacter = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Character>(m_AssetPath);
		if(pCharacter)
		{
			CAsset_Character::CIteratorPart Iter;
			for(Iter = pCharacter->BeginPart(); Iter != pCharacter->EndPart(); ++Iter)
			{
				CAssetListCharacterPartType* pItem = new CAssetListCharacterPartType(m_pAssetsEditor, m_AssetPath, *Iter, m_Source);
				
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

class CAssetListMapGroup : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CAssetListMapGroup(
		CAssetsEditor* pAssetsEditor,
		CAssetPath AssetPath,
		int Source
	) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source),
		m_SourceFound(false)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
			
		char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		
		if(m_AssetPath.GetSource() == m_Source)
		{
			CAssetListTitle* pButton = new CAssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_FOLDER));
			pButton->SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			SetTitle(pButton);
		}
		else
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_FOLDER)));
		
		
		CAsset_MapGroup* pMapGroup = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_MapGroup>(m_AssetPath);
		if(pMapGroup)
		{
			CAsset_MapGroup::CIteratorLayer Iter;
			for(Iter = pMapGroup->BeginLayer(); Iter != pMapGroup->EndLayer(); ++Iter)
			{
				bool AddItem = false;
				
				if(pMapGroup->GetLayer(*Iter).GetType() == CAssetPath::TYPE_MAPLAYERTILES)
				{
					AddItem = true;
				}
				else if(pMapGroup->GetLayer(*Iter).GetType() == CAssetPath::TYPE_MAPLAYERQUADS)
				{
					AddItem = true;
				}
				
				if(AddItem)
				{
					CAssetListItem* pItem = new CAssetListItem(m_pAssetsEditor, pMapGroup->GetLayer(*Iter), m_Source);
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

class CAssetListMap : public gui::CExpand
{
protected:
	class CZoneLayers : public gui::CExpand
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		gui::CWidget* m_Button;
		int m_Source;
		bool m_SourceFound;
	public:
		CZoneLayers( CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source ) :
			gui::CExpand(pAssetsEditor->m_pGuiConfig),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_Source(Source),
			m_SourceFound(false)
		{		
			SetHeight(m_pConfig->m_ButtonHeight);
				
			char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
			
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, "Zones", CAssetPath::SpriteUniverse(SPRITE_ICON_DEFAULT)));
			
			CAsset_Map* pMap = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Map>(m_AssetPath);
			if(pMap)
			{
				CAsset_Map::CIteratorZoneLayer Iter;
				for(Iter = pMap->BeginZoneLayer(); Iter != pMap->EndZoneLayer(); ++Iter)
				{
					if(pMap->GetZoneLayer(*Iter).GetSource() == m_Source)
					{					
						CAssetListItem* pItem = new CAssetListItem(m_pAssetsEditor, pMap->GetZoneLayer(*Iter), m_Source);
						Add(pItem);
						m_SourceFound = true;
					}
				}
			}
			
			Update();
		}
		
		bool SourceFound() { return true; }
	};
	
	class CBgGroups : public gui::CExpand
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		gui::CWidget* m_Button;
		int m_Source;
		bool m_SourceFound;
	public:
		CBgGroups( CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source ) :
			gui::CExpand(pAssetsEditor->m_pGuiConfig),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_Source(Source),
			m_SourceFound(false)
		{		
			SetHeight(m_pConfig->m_ButtonHeight);
				
			char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
			
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, "Background", CAssetPath::SpriteUniverse(SPRITE_ICON_DEFAULT)));
			
			CAsset_Map* pMap = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Map>(m_AssetPath);
			if(pMap)
			{
				CAsset_Map::CIteratorBgGroup Iter;
				for(Iter = pMap->BeginBgGroup(); Iter != pMap->EndBgGroup(); ++Iter)
				{
					if(pMap->GetBgGroup(*Iter).GetSource() == m_Source)
					{					
						CAssetListMapGroup* pItem = new CAssetListMapGroup(m_pAssetsEditor, pMap->GetBgGroup(*Iter), m_Source);
						Add(pItem);
						m_SourceFound = true;
					}
				}
			}
			
			Update();
		}
		
		bool SourceFound() { return true; }
	};
	
	class CFgGroups : public gui::CExpand
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		gui::CWidget* m_Button;
		int m_Source;
		bool m_SourceFound;
	public:
		CFgGroups( CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source ) :
			gui::CExpand(pAssetsEditor->m_pGuiConfig),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_Source(Source),
			m_SourceFound(false)
		{		
			SetHeight(m_pConfig->m_ButtonHeight);
				
			char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
			
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, "Foreground", CAssetPath::SpriteUniverse(SPRITE_ICON_DEFAULT)));
			
			CAsset_Map* pMap = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Map>(m_AssetPath);
			if(pMap)
			{
				CAsset_Map::CIteratorFgGroup Iter;
				for(Iter = pMap->BeginFgGroup(); Iter != pMap->EndFgGroup(); ++Iter)
				{
					if(pMap->GetFgGroup(*Iter).GetSource() == m_Source)
					{					
						CAssetListMapGroup* pItem = new CAssetListMapGroup(m_pAssetsEditor, pMap->GetFgGroup(*Iter), m_Source);
						Add(pItem);
						m_SourceFound = true;
					}
				}
			}
			
			Update();
		}
		
		bool SourceFound() { return true; }
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	CBgGroups* m_pBgGroups;
	CFgGroups* m_pFgGroups;
	CZoneLayers* m_pZoneLayers;
	gui::CWidget* m_Button;
	int m_Source;

public:
	CAssetListMap(
		CAssetsEditor* pAssetsEditor,
		CAssetPath AssetPath,
		int Source
	) :
		gui::CExpand(pAssetsEditor->m_pGuiConfig),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_Source(Source)
	{		
		SetHeight(m_pConfig->m_ButtonHeight);
			
		char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		
		if(m_AssetPath.GetSource() == m_Source)
		{
			CAssetListTitle* pButton = new CAssetListTitle(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_MAP));
			pButton->SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			SetTitle(pButton);
		}
		else
			SetTitle(new CAssetListTitleDisabled(m_pAssetsEditor, m_AssetPath, pName, CAssetPath::SpriteUniverse(SPRITE_ICON_MAP)));
			
		CAsset_Map* pMap = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Map>(m_AssetPath);
		if(pMap)
		{	
			m_pZoneLayers = new CZoneLayers(m_pAssetsEditor, m_AssetPath, m_Source);
			Add(m_pZoneLayers);
			m_pBgGroups = new CBgGroups(m_pAssetsEditor, m_AssetPath, m_Source);
			Add(m_pBgGroups);
			m_pFgGroups = new CFgGroups(m_pAssetsEditor, m_AssetPath, m_Source);
			Add(m_pFgGroups);
		}
		
		Update();
	}
	
	bool SourceFound()
	{
		return m_pZoneLayers->SourceFound() || m_pBgGroups->SourceFound() || m_pFgGroups->SourceFound();
	}
};

class CLoadAssetsButton : public gui::CTextButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Source;
	
protected:
	virtual void MouseClickAction()
	{
		m_pAssetsEditor->DisplayPopup(new CPopup_SaveLoadAssets(
			m_pAssetsEditor, m_Source, CPopup_SaveLoadAssets::MODE_LOAD, m_Rect, gui::CPopup::ALIGNMENT_RIGHT
		));
	}

public:
	CLoadAssetsButton(CAssetsEditor* pAssetsEditor, int Source) :
		gui::CTextButton(pAssetsEditor->m_pGuiConfig, "Load", CAssetPath::SpriteUniverse(SPRITE_ICON_LOAD)),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source)
	{ }
};

class CSaveAssetsButton : public gui::CTextButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Source;
	
protected:
	virtual void MouseClickAction()
	{
		m_pAssetsEditor->DisplayPopup(new CPopup_SaveLoadAssets(
			m_pAssetsEditor, m_Source, CPopup_SaveLoadAssets::MODE_SAVE, m_Rect, gui::CPopup::ALIGNMENT_RIGHT
		));
	}

public:
	CSaveAssetsButton(CAssetsEditor* pAssetsEditor, int Source) :
		gui::CTextButton(pAssetsEditor->m_pGuiConfig, "Save", CAssetPath::SpriteUniverse(SPRITE_ICON_SAVE)),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source)
	{ }
};

class CNewAssetButton : public gui::CTextButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Source;
	
protected:
	virtual void MouseClickAction()
	{
		m_pAssetsEditor->DisplayPopup(new CPopup_NewAsset(
			m_pAssetsEditor, m_Source, m_Rect, gui::CPopup::ALIGNMENT_RIGHT
		));
	}

public:
	CNewAssetButton(CAssetsEditor* pAssetsEditor, int Source) :
		gui::CTextButton(pAssetsEditor->m_pGuiConfig, "New asset", CAssetPath::SpriteUniverse(SPRITE_ICON_INCREASE)),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source)
	{ }
};

/* ASSETS EDITOR ******************************************************/

IAssetsEditor *CreateAssetsEditor() { return new CAssetsEditor; }

CAssetsEditor::CAssetsEditor()
{
	m_pGuiConfig = 0;
	m_pHintLabel = 0;
	m_pStatusLabel = 0;
}

CAssetsEditor::~CAssetsEditor()
{
	delete m_pGuiAssetListTabs;
	delete m_pGuiAssetsEditor;
	delete m_pGuiView;
	delete m_pGuiStatusBar;
	for(int i=0; i<m_GuiPopups.size(); i++)
	{
		if(m_GuiPopups[i]) delete m_GuiPopups[i];
	}
	
	if(m_pGuiConfig) delete m_pGuiConfig;
}
	
void CAssetsEditor::Init(CAssetsManager** ppAssetsManager, CGraphics** ppTUGraphics)
{
	m_pClient = Kernel()->RequestInterface<IClient>();
	m_pInput = Kernel()->RequestInterface<IInput>();
	m_pGraphics = Kernel()->RequestInterface<IGraphics>();
	m_pTextRender = Kernel()->RequestInterface<ITextRender>();
	m_pStorage = Kernel()->RequestInterface<IStorage>();
	m_pAssetsFile = Kernel()->RequestInterface<tu::IAssetsFile>();
	m_ppAssetsManager = ppAssetsManager;
	m_ppTUGraphics = ppTUGraphics;
	
	m_RenderTools.m_pGraphics = m_pGraphics;
	
	m_CursorTexture = Graphics()->LoadTexture("editor/cursor.png", IStorage::TYPE_ALL, CImageInfo::FORMAT_AUTO, 0);
	m_ZoneTexture = Graphics()->LoadTexture("tu/assetseditor_zone.png", IStorage::TYPE_ALL, CImageInfo::FORMAT_AUTO, 0);
	
	m_pGuiConfig = new gui::CConfig(TUGraphics(ASSETS_EDITORGUI), RenderTools(), TextRender(), Input(), AssetsManager(ASSETS_EDITORGUI));
	m_pGuiConfig->m_fShowHint = CAssetsEditor::ShowHint;
	m_pGuiConfig->m_pShowHintData = (void*) this;
	
	int Margin = 1;
	
	int StatusBarHeight = 26;
	int PanelWidth = 250;
	int PanelHeight = 250;
	gui::CRect StatusBarRect(Margin, Graphics()->ScreenHeight()-StatusBarHeight-Margin, Graphics()->ScreenWidth()-2*Margin, StatusBarHeight);
	gui::CRect AssetListRect(Margin, Margin, PanelWidth, Graphics()->ScreenHeight()-3*Margin-StatusBarRect.h);
	gui::CRect ViewRect(AssetListRect.x + AssetListRect.w + Margin, AssetListRect.y, Graphics()->ScreenWidth() - 2*PanelWidth - 4*Margin, Graphics()->ScreenHeight() - 4*Margin - PanelHeight - StatusBarHeight);
	gui::CRect AssetsEditorRect(ViewRect.x + ViewRect.w + Margin, AssetListRect.y, PanelWidth, AssetListRect.h);
	gui::CRect TimelineRect(AssetListRect.x + AssetListRect.w + Margin, ViewRect.y + ViewRect.h + Margin, ViewRect.w, PanelHeight);
	
	m_pHintLabel = new gui::CLabel(m_pGuiConfig, "");
	m_pStatusLabel = new gui::CLabel(m_pGuiConfig, "");
	
	m_pGuiAssetListTabs = new gui::CTabs(m_pGuiConfig);
	m_pGuiAssetListTabs->SetRect(AssetListRect);
	
	m_pGuiAssetList[CAssetPath::SRC_UNIVERSE] = new gui::CVListLayout(m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
	m_pGuiAssetListTabs->AddTab(m_pGuiAssetList[CAssetPath::SRC_UNIVERSE], CAssetPath::SpriteUniverse(SPRITE_ICON_UNIVERSE), "Universe");
	
	m_pGuiAssetList[CAssetPath::SRC_WORLD] = new gui::CVListLayout(m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
	m_pGuiAssetListTabs->AddTab(m_pGuiAssetList[CAssetPath::SRC_WORLD], CAssetPath::SpriteUniverse(SPRITE_ICON_WORLD), "World");
	
	m_pGuiAssetList[CAssetPath::SRC_LAND] = new gui::CVListLayout(m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
	m_pGuiAssetListTabs->AddTab(m_pGuiAssetList[CAssetPath::SRC_LAND], CAssetPath::SpriteUniverse(SPRITE_ICON_LAND), "Land");
	
	m_pGuiAssetList[CAssetPath::SRC_SKIN] = new gui::CVListLayout(m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_INVISIBLE);
	m_pGuiAssetListTabs->AddTab(m_pGuiAssetList[CAssetPath::SRC_SKIN], CAssetPath::SpriteUniverse(SPRITE_ICON_SKIN), "Skins");
	
	m_pGuiAssetListTabs->Update();
	RefreshAssetsList();
	
	m_pGuiView = new CView(this);
	m_pGuiView->SetRect(ViewRect);
	m_pGuiView->Update();
	
	m_pGuiTimeline = new CTimeline(this);
	m_pGuiTimeline->SetRect(TimelineRect);
	m_pGuiTimeline->Update();
	
	m_pGuiAssetsEditor = new CEditor(this);
	m_pGuiAssetsEditor->SetRect(AssetsEditorRect);
	m_pGuiAssetsEditor->Update();
	
	m_pGuiStatusBar = new gui::CHListLayout(m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_TOOLBAR, gui::LAYOUTFILLING_ALL);
	m_pGuiStatusBar->SetRect(StatusBarRect);
	m_pGuiStatusBar->Add(m_pStatusLabel);
	m_pGuiStatusBar->Add(m_pHintLabel);
	m_pGuiStatusBar->Update();
	
	m_RefreshAssetsEditor = false;
	m_EditorTab = -1;
	
	m_ShowCursor = true;
	
	m_LastTime = -1.0f;
	m_Time = 0.0f;
	m_Paused = true;
	
	m_EditedAssetSubPath = -1;
	m_AssetsListSource = CAssetPath::SRC_WORLD;
}

void CAssetsEditor::ShowStatus(const char* pText)
{
	if(m_pHintLabel)
	{
		m_pStatusLabel->SetText(pText);
		m_Status = true;
	}
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

void CAssetsEditor::RefreshAssetsList(int Source)
{
	float Scroll = m_pGuiAssetList[Source]->GetScrollPos();
	
	m_pGuiAssetList[Source]->Clear();
	
	switch(Source)
	{
		case CAssetPath::SRC_UNIVERSE:
			m_pGuiAssetList[Source]->Add(new gui::CLabel(m_pGuiConfig, "Universe", gui::TEXTSTYLE_HEADER));
			break;
		case CAssetPath::SRC_WORLD:
			m_pGuiAssetList[Source]->Add(new gui::CLabel(m_pGuiConfig, "World", gui::TEXTSTYLE_HEADER));
			break;
		case CAssetPath::SRC_LAND:
			m_pGuiAssetList[Source]->Add(new gui::CLabel(m_pGuiConfig, "Land", gui::TEXTSTYLE_HEADER));
			break;
		case CAssetPath::SRC_SKIN:
			m_pGuiAssetList[Source]->Add(new gui::CLabel(m_pGuiConfig, "Skins", gui::TEXTSTYLE_HEADER));
			break;
	}
	
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
		pLayout->SetHeight(m_pGuiConfig->m_ButtonHeight);
		m_pGuiAssetList[Source]->Add(pLayout);
		
		pLayout->Add(new CLoadAssetsButton(this, Source));
		pLayout->Add(new CSaveAssetsButton(this, Source));
	}
	m_pGuiAssetList[Source]->Add(new CNewAssetButton(this, Source));
	
	//Init asset state
	{
		CAssetState State;
		State.m_ListedInEditor = false;
		AssetsManager()->InitAssetState(Source, State);
	}
	
	#define REFRESH_ASSET_LIST(ClassName) {\
		int nbAssets = AssetsManager()->GetNumAssets<ClassName>(Source);\
		for(int i=0; i<nbAssets; i++)\
		{\
			CAssetPath Path = CAssetPath::Asset(ClassName::TypeId, Source, i);\
			CAssetState* pState = AssetsManager()->GetAssetState(Path);\
			if(pState && !pState->m_ListedInEditor)\
			{\
				m_pGuiAssetList[Source]->Add(new CAssetListItem(this, Path));\
			}\
		}\
	}
	
	//Images
	//	Sprites
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Image>(s); i++)
		{
			CAssetListImage* pItem = new CAssetListImage(this, CAssetPath::Asset(CAsset_Image::TypeId, s, i), Source);
			if(pItem->SourceFound())
				m_pGuiAssetList[Source]->Add(pItem);
			else
				delete pItem;
		}
	}
	
	REFRESH_ASSET_LIST(CAsset_Sprite)
	
	//Skeletons
	//	SkeletonSkins
	//	SkeletonAnimations
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Skeleton>(s); i++)
		{
			CAssetListSkeleton* pItem = new CAssetListSkeleton(this, CAssetPath::Asset(CAsset_Skeleton::TypeId, s, i), Source);
			if(pItem->SourceFound())
				m_pGuiAssetList[Source]->Add(pItem);
			else
				delete pItem;
		}
	}
	
	REFRESH_ASSET_LIST(CAsset_SkeletonSkin)
	REFRESH_ASSET_LIST(CAsset_SkeletonAnimation)
	
	//Search Tag: TAG_NEW_ASSET
	
	//Characters
	//	CharacterParts
	//	Weapons
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Character>(s); i++)
		{
			CAssetListCharacter* pItem = new CAssetListCharacter(this, CAssetPath::Asset(CAsset_Character::TypeId, s, i), Source);
			if(pItem->SourceFound())
				m_pGuiAssetList[Source]->Add(pItem);
			else
				delete pItem;
		}
	}
	
	REFRESH_ASSET_LIST(CAsset_CharacterPart)
	REFRESH_ASSET_LIST(CAsset_Weapon)
	
	//Map
	for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Map>(Source); i++)
	{
		CAssetListMap* pItem = new CAssetListMap(
			this,
			CAssetPath::Asset(CAsset_Map::TypeId, Source, i),
			Source
		);
		m_pGuiAssetList[Source]->Add(pItem);
	}
	
	//MapGroup
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_MapGroup>(s); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_MapGroup::TypeId, Source, i);
			CAssetState* pState = AssetsManager()->GetAssetState(Path);
			if(pState && !pState->m_ListedInEditor)
			{
				CAssetListMapGroup* pItem = new CAssetListMapGroup(
					this,
					CAssetPath::Asset(CAsset_MapGroup::TypeId, s, i),
					Source
				);
			
				if(pItem->SourceFound())
					m_pGuiAssetList[Source]->Add(pItem);
				else
					delete pItem;
			}
		}
	}
	
	REFRESH_ASSET_LIST(CAsset_MapLayerTiles)
	REFRESH_ASSET_LIST(CAsset_MapLayerQuads)
	REFRESH_ASSET_LIST(CAsset_ZoneType)
	
	m_pGuiAssetList[Source]->SetScrollPos(Scroll);
	
	m_pGuiAssetList[Source]->Update();
}

void CAssetsEditor::RefreshAssetsList()
{
	RefreshAssetsList(CAssetPath::SRC_UNIVERSE);
	RefreshAssetsList(CAssetPath::SRC_WORLD);
	RefreshAssetsList(CAssetPath::SRC_LAND);
	RefreshAssetsList(CAssetPath::SRC_SKIN);
}

void CAssetsEditor::RefreshAssetsEditor(int Tab)
{
	m_EditorTab = Tab;
	m_RefreshAssetsEditor = true;
}

void CAssetsEditor::Render()
{
	Graphics()->MapScreen(0, 0, Graphics()->ScreenWidth(), Graphics()->ScreenHeight());
	
	{
		Graphics()->TextureClear();
		Graphics()->QuadsBegin();
		Graphics()->SetColor(58.0f/255.0f, 58.0f/255.0f, 58.0f/255.0f, 1.0f);
		
		IGraphics::CQuadItem QuadItem(Graphics()->ScreenWidth()/2, Graphics()->ScreenHeight()/2, Graphics()->ScreenWidth(), Graphics()->ScreenHeight());
		Graphics()->QuadsDraw(&QuadItem, 1);
		
		Graphics()->QuadsEnd();
	}
	
	m_pGuiStatusBar->Render();
	m_pGuiAssetListTabs->Render();
	m_pGuiAssetsEditor->Render();
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
	
	if(Input()->KeyIsPressed(KEY_LCTRL) && Input()->KeyPress(KEY_Z, true))
	{
		AssetsManager()->Undo();
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
	
	if(m_RefreshAssetsEditor)
	{
		m_pGuiAssetsEditor->Refresh(m_EditorTab);
		m_RefreshAssetsEditor = false;
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
		m_pGuiStatusBar->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		m_pGuiAssetListTabs->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		m_pGuiAssetsEditor->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		m_pGuiView->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		m_pGuiTimeline->OnMouseOver(m_MousePos.x, m_MousePos.y, m_MouseDelta.x, m_MouseDelta.y, Keys);
		
		m_pGuiStatusBar->OnInputEvent();
		m_pGuiAssetListTabs->OnInputEvent();
		m_pGuiAssetsEditor->OnInputEvent();
		m_pGuiView->OnInputEvent();
		m_pGuiTimeline->OnInputEvent();
	}
	
	int aButtons[] = {
		KEY_MOUSE_1,
		KEY_MOUSE_2,
		KEY_MOUSE_3,
		KEY_MOUSE_WHEEL_UP,
		KEY_MOUSE_WHEEL_DOWN
	};
	
	for(int i=0; i<(sizeof(aButtons)/sizeof(int)); i++)
	{
		int Button = aButtons[i];
		
		int Count = 0;
		if(Button == KEY_MOUSE_WHEEL_UP || Button == KEY_MOUSE_WHEEL_DOWN)
			Count = Input()->KeyPress(Button);
		else
			Count = (Input()->KeyIsPressed(Button) ? 1 : 0);
		
		if(Count)
		{
			if(m_MouseButton[i] == 0)
			{
				m_MouseButton[i] = 1;
				
				if(m_GuiPopups.size() > 0)
				{
					for(int i=0; i<m_GuiPopups.size(); i++)
					{
						m_GuiPopups[i]->OnButtonClick(m_MousePos.x, m_MousePos.y, Button, Count);
					}
				}
				else
				{
					m_pGuiStatusBar->OnButtonClick(m_MousePos.x, m_MousePos.y, Button, Count);
					m_pGuiAssetListTabs->OnButtonClick(m_MousePos.x, m_MousePos.y, Button, Count);
					m_pGuiAssetsEditor->OnButtonClick(m_MousePos.x, m_MousePos.y, Button, Count);
					m_pGuiView->OnButtonClick(m_MousePos.x, m_MousePos.y, Button, Count);
					m_pGuiTimeline->OnButtonClick(m_MousePos.x, m_MousePos.y, Button, Count);
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
					m_pGuiStatusBar->OnButtonRelease(Button);
					m_pGuiAssetListTabs->OnButtonRelease(Button);
					m_pGuiAssetsEditor->OnButtonRelease(Button);
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
	if(!m_Status)
	{
		m_pStatusLabel->SetText("");
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

bool CAssetsEditor::IsEditedSubItem(CAssetPath AssetPath, int SubPath)
{
	return (m_EditedAssetPath == AssetPath && m_EditedAssetSubPath == SubPath);
}

bool CAssetsEditor::IsDisplayedAsset(CAssetPath AssetPath)
{
	return (m_ViewedAssetPath == AssetPath);
}

void CAssetsEditor::EditAsset(CAssetPath AssetPath)
{
	m_EditedAssetPath = AssetPath;
	m_EditedAssetSubPath = -1;
	RefreshAssetsEditor(-1);
}

void CAssetsEditor::EditAssetSubItem(CAssetPath AssetPath, int SubPath, int Tab)
{
	m_EditedAssetPath = AssetPath;
	m_EditedAssetSubPath = SubPath;
	RefreshAssetsEditor(Tab);
}

void CAssetsEditor::EditAssetFirstFrame()
{
	m_Time = 0.0f;
}

void CAssetsEditor::EditAssetPrevFrame()
{
	if(m_EditedAssetPath.GetType() == CAssetPath::TYPE_SKELETONANIMATION)
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_EditedAssetPath);
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
		CAsset_SkeletonAnimation* pSkeletonAnimation = AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_EditedAssetPath);
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
		CAsset_SkeletonAnimation* pSkeletonAnimation = AssetsManager()->GetAsset<CAsset_SkeletonAnimation>(m_EditedAssetPath);
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
	RefreshAssetsList();
	DisplayAsset(AssetPath);
	EditAsset(AssetPath);
}

void CAssetsEditor::DuplicateAsset(CAssetPath AssetPath)
{
	CAssetPath NewAssetPath = AssetsManager()->DuplicateAsset(AssetPath, AssetPath.GetSource());
	
	m_EditedAssetPath = NewAssetPath;
	m_ViewedAssetPath = NewAssetPath;
	
	RefreshAssetsList();
}

void CAssetsEditor::DeleteAsset(CAssetPath AssetPath)
{
	AssetsManager()->DeleteAsset(AssetPath);
	
	m_EditedAssetPath = CAssetPath::Null();
	m_ViewedAssetPath = CAssetPath::Null();
	
	RefreshAssetsList();
}

void CAssetsEditor::DeleteSubItem(CAssetPath AssetPath, int SubPath)
{
	AssetsManager()->DeleteSubItem(AssetPath, SubPath);
	
	RefreshAssetsEditor(-1);
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

vec2 CAssetsEditor::GetScreenSize() const
{
	return vec2(Graphics()->ScreenWidth(), Graphics()->ScreenHeight());
}

vec2 CAssetsEditor::GetCursorPos() const
{
	return m_MousePos;
}

void CAssetsEditor::CloseEditor()
{
	g_Config.m_ClMode = TU_CLIENTMODE_GAME;
}

void CAssetsEditor::LoadAssetsFile(const char* pFilename, int Source)
{
	Client()->LoadAssetsFile(pFilename, Source);
	RefreshAssetsList();
}

}

}
