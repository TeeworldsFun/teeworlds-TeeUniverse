#include <tu/client/assetseditor/assetslist.h>
#include <tu/client/assetseditor/assetseditor.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/gui/expand.h>
#include <tu/client/gui/popup.h>
#include <tu/client/gui/text-edit.h>
#include <game/client/localization.h>
#include <engine/storage.h>

namespace tu
{

namespace assetseditor
{

/* SAVE/LOAD ASSETS ***************************************************/

class CPopup_SaveLoadAssets : public gui::CPopup
{	
protected:	
	class CSave : public gui::CButton
	{
	protected:
		CPopup_SaveLoadAssets* m_pPopup;
		
	public:
		CSave(CPopup_SaveLoadAssets* pPopup) :
			gui::CButton(pPopup->Context(), "Save"),
			m_pPopup(pPopup)
		{
			
		}

		virtual void MouseClickAction() { m_pPopup->Save(); }
	};
	
	class CItem_Load : public gui::CButton
	{
	protected:
		CPopup_SaveLoadAssets* m_pPopup;
		CAssetsEditor* m_pAssetsEditor;
		char m_aFilename[256];
		char m_aDir[256];
		int m_StorageType;
		int m_IsDirectory;
		
	protected:
		virtual void MouseClickAction()
		{
			if(!m_IsDirectory)
			{
				char aBuf[512];
				str_format(aBuf, sizeof(aBuf), "%s/%s", m_aDir, m_aFilename);
				
				AssetsManager()->LoadAssetsFile(aBuf, m_pPopup->m_Source);
				m_pPopup->Close();
			}
		}
		
	public:
		CItem_Load(CPopup_SaveLoadAssets* pPopup, const char* pDir, const char* pFilename, int StorageType, int IsDir) :
			gui::CButton(pPopup->Context(), pFilename, CAssetPath::SpriteSystem(SPRITE_EDITOR_ASSET)),
			m_pPopup(pPopup),
			m_pAssetsEditor(pPopup->m_pAssetsEditor)
		{
			str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
			str_copy(m_aDir, pDir, sizeof(m_aDir));
			
			m_StorageType = StorageType;
			m_IsDirectory = IsDir;
		}
	};
	
	class CItem_Save : public gui::CButton
	{
	protected:
		CPopup_SaveLoadAssets* m_pPopup;
		CAssetsEditor* m_pAssetsEditor;
		char m_aFilename[256];
		int m_StorageType;
		int m_IsDirectory;
		
	protected:
		virtual void MouseClickAction()
		{
			if(!m_IsDirectory)
			{
				m_pPopup->SetFilename(m_aFilename);
			}
		}
		
	public:
		CItem_Save(CPopup_SaveLoadAssets* pPopup, const char* pFilename, int StorageType, int IsDir) :
			gui::CButton(pPopup->Context(), pFilename, CAssetPath::SpriteSystem(SPRITE_EDITOR_ASSET)),
			m_pPopup(pPopup),
			m_pAssetsEditor(pPopup->m_pAssetsEditor)
		{
			str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
			m_StorageType = StorageType;
			m_IsDirectory = IsDir;
		}
	};

public:
	enum
	{
		MODE_SAVE=0,
		MODE_LOAD,
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	char m_aFilename[256];
	int m_Source;
	int m_Mode;
	gui::CVScrollLayout* m_pFilelist;
	
public:
	CPopup_SaveLoadAssets(CAssetsEditor* pAssetsEditor, int Source, int Mode, const gui::CRect& CreatorRect, int Alignment) :
		gui::CPopup(pAssetsEditor, CreatorRect, 500, 500, Alignment),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_Mode(Mode),
		m_pFilelist(0)
	{
		gui::CVScrollLayout* pLayout = new gui::CVScrollLayout(Context());
		Add(pLayout);
		
		if(m_Mode == MODE_SAVE)
		{
			switch(m_Source)
			{
				case CAssetPath::SRC_UNIVERSE:
					str_copy(m_aFilename, "myuniverse.assets", sizeof(m_aFilename));
					break;
				case CAssetPath::SRC_WORLD:
					str_copy(m_aFilename, "myworld.assets", sizeof(m_aFilename));
					break;
				case CAssetPath::SRC_LAND:
					str_copy(m_aFilename, "myland.assets", sizeof(m_aFilename));
					break;
				case CAssetPath::SRC_SKIN:
					str_copy(m_aFilename, "myskin.assets", sizeof(m_aFilename));
					break;
			}
		
			{
				gui::CHListLayout* pHLayout = new gui::CHListLayout(Context());
				pLayout->Add(pHLayout, false);
				
				gui::CExternalTextEdit* pTextEdit = new gui::CExternalTextEdit(Context(), m_aFilename, sizeof(m_aFilename));
				pHLayout->Add(pTextEdit, true);
				
				pHLayout->Add(new CSave(this), false);
			}
		}
		
		m_pFilelist = new gui::CVScrollLayout(Context());
		pLayout->Add(m_pFilelist, true);
		
		m_pFilelist->Add(new gui::CLabelHeader(Context(), "Load asset package"), false);
		switch(m_Source)
		{
			case CAssetPath::SRC_UNIVERSE:
				Storage()->ListDirectory(IStorage::TYPE_ALL, "assets/universes", FileListFetchCallback_Universe, this);
				break;
			case CAssetPath::SRC_WORLD:
				Storage()->ListDirectory(IStorage::TYPE_ALL, "assets/worlds", FileListFetchCallback_World, this);
				break;
			case CAssetPath::SRC_LAND:
				{
					Storage()->ListDirectory(IStorage::TYPE_ALL, "assets/lands", FileListFetchCallback_Land, this);
					if(m_Mode == MODE_LOAD)
					{
						m_pFilelist->AddSeparator();
						m_pFilelist->Add(new gui::CLabelHeader(Context(), "Import TeeWorlds map"), false);
						Storage()->ListDirectory(IStorage::TYPE_ALL, "maps", FileListFetchCallback_Map, this);
					}
				}
				break;
		}
	}
	
	static int FileListFetchCallback_Universe(const char *pName, int IsDir, int StorageType, void *pUser)
	{
		CPopup_SaveLoadAssets* pPopup = static_cast<CPopup_SaveLoadAssets*>(pUser);
		
		int Length = str_length(pName);
		if(pName[0] == '.' && (pName[1] == 0))
			return 0;
		
		if(Length < 7 || str_comp(pName+Length-7, ".assets"))
			return 0;
		
		if(pPopup->m_Mode == MODE_SAVE)
			pPopup->m_pFilelist->Add(new CItem_Save(pPopup, pName, StorageType, IsDir), false);
		else
			pPopup->m_pFilelist->Add(new CItem_Load(pPopup, "assets/universes", pName, StorageType, IsDir), false);
		
		return 0;
	}
	
	static int FileListFetchCallback_World(const char *pName, int IsDir, int StorageType, void *pUser)
	{
		CPopup_SaveLoadAssets* pPopup = static_cast<CPopup_SaveLoadAssets*>(pUser);
		
		int Length = str_length(pName);
		if(pName[0] == '.' && (pName[1] == 0))
			return 0;
		
		if(Length < 7 || str_comp(pName+Length-7, ".assets"))
			return 0;
		
		if(pPopup->m_Mode == MODE_SAVE)
			pPopup->m_pFilelist->Add(new CItem_Save(pPopup, pName, StorageType, IsDir), false);
		else
			pPopup->m_pFilelist->Add(new CItem_Load(pPopup, "assets/worlds", pName, StorageType, IsDir), false);

		return 0;
	}
	
	static int FileListFetchCallback_Land(const char *pName, int IsDir, int StorageType, void *pUser)
	{
		CPopup_SaveLoadAssets* pPopup = static_cast<CPopup_SaveLoadAssets*>(pUser);
		
		int Length = str_length(pName);
		if(pName[0] == '.' && (pName[1] == 0))
			return 0;
		
		if(Length < 7 || str_comp(pName+Length-7, ".assets"))
			return 0;
		
		if(pPopup->m_Mode == MODE_SAVE)
			pPopup->m_pFilelist->Add(new CItem_Save(pPopup, pName, StorageType, IsDir), false);
		else
			pPopup->m_pFilelist->Add(new CItem_Load(pPopup, "assets/lands", pName, StorageType, IsDir), false);

		return 0;
	}
	
	static int FileListFetchCallback_Map(const char *pName, int IsDir, int StorageType, void *pUser)
	{
		CPopup_SaveLoadAssets* pPopup = static_cast<CPopup_SaveLoadAssets*>(pUser);
				
		int Length = str_length(pName);
		if(pName[0] == '.' && (pName[1] == 0))
			return 0;
		
		if(Length < 4 || str_comp(pName+Length-4, ".map"))
			return 0;
		
		if(pPopup->m_Mode == MODE_LOAD)
			pPopup->m_pFilelist->Add(new CItem_Load(pPopup, "maps", pName, StorageType, IsDir), false);

		return 0;
	}
	
	void SetFilename(const char* pFilename)
	{
		str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
	}
	
	void Save()
	{
		char aBuf[256];
		switch(m_Source)
		{
			case CAssetPath::SRC_UNIVERSE:
				str_format(aBuf, sizeof(aBuf), "assets/universes/%s", m_aFilename);
				break;
			case CAssetPath::SRC_WORLD:
				str_format(aBuf, sizeof(aBuf), "assets/worlds/%s", m_aFilename);
				break;
			case CAssetPath::SRC_LAND:
				str_format(aBuf, sizeof(aBuf), "assets/lands/%s", m_aFilename);
				break;
		}
		
		m_pAssetsEditor->AssetsManager()->SaveInAssetsFile(aBuf, m_Source);
		Close();
	}
};

/* INACTIVE ITEM ******************************************************/

class CInactiveItem : public gui::CLabel
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_SubPath;

public:
	CInactiveItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int SubPath) :
		gui::CLabel(pAssetsEditor, "", CAssetPath::Null()),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_SubPath(SubPath)
	{
		SetLabelStyle(CAssetPath::GuiLabelStyleSystem(GUILABELSTYLE_EDITOR_INACTIVELISTITEM));
		SetIcon(m_pAssetsEditor->GetItemIcon(m_AssetPath, m_SubPath));
	}
	
	virtual void Update()
	{
		SetText(m_pAssetsEditor->GetItemName(m_AssetPath, m_SubPath));
		
		gui::CLabel::Update();
	}
};

/* ITEM ***************************************************************/

class CItem : public gui::CButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_SubPath;
	
protected:
	virtual void MouseClickAction()
	{
		m_pAssetsEditor->SetEditedAsset(m_AssetPath, m_SubPath);
	}

public:
	CItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int SubPath) :
		gui::CButton(pAssetsEditor, "", CAssetPath::Null()),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_SubPath(SubPath)
	{
		SetButtonStyle(CAssetPath::GuiButtonStyleSystem(GUIBUTTONSTYLE_EDITOR_LISTITEM));
		
		CAssetState* pState = AssetsManager()->GetAssetState(m_AssetPath);
		if(pState)
			pState->m_ListedInEditor = true;
		
		Update();
	}
	
	virtual void Update()
	{
		SetIcon(m_pAssetsEditor->GetItemIcon(m_AssetPath, m_SubPath));
		SetText(m_pAssetsEditor->GetItemName(m_AssetPath, m_SubPath));
		
		gui::CButton::Update();
	}
};

/* IMAGE ITEM *********************************************************/

class CImageItem : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CImageItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_AssetPath(AssetPath),
		m_SourceFound(false)
	{
		if(m_Source == m_AssetPath.GetSource())
			SetTitle(new CItem(m_pAssetsEditor, m_AssetPath, -1));
		else
			SetTitle(new CInactiveItem(m_pAssetsEditor, m_AssetPath, -1));
		
		Clear();
		
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Sprite>(m_Source); i++)
		{
			CAssetPath SpritePath = CAssetPath::Asset(CAsset_Sprite::TypeId, m_Source, i);
			CAssetPath ImagePath = AssetsManager()->GetAssetValue<CAssetPath>(SpritePath, CAsset_Sprite::IMAGEPATH, -1, 0);
			if(ImagePath == m_AssetPath)
			{
				Add(new CItem(m_pAssetsEditor, SpritePath, -1));
				m_SourceFound = true;
			}
		}
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

/* SKELETON ITEM ******************************************************/

class CSkeletonItem : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CSkeletonItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_AssetPath(AssetPath),
		m_SourceFound(false)
	{
		if(m_Source == m_AssetPath.GetSource())
			SetTitle(new CItem(m_pAssetsEditor, m_AssetPath, -1));
		else
			SetTitle(new CInactiveItem(m_pAssetsEditor, m_AssetPath, -1));
		
		Clear();
		
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_SkeletonAnimation>(m_Source); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_SkeletonAnimation::TypeId, m_Source, i);
			CAssetPath SkeletonPath = AssetsManager()->GetAssetValue<CAssetPath>(Path, CAsset_SkeletonAnimation::SKELETONPATH, -1, 0);
			if(SkeletonPath == m_AssetPath)
			{
				Add(new CItem(m_pAssetsEditor, Path, -1));
				m_SourceFound = true;
			}
		}
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_SkeletonSkin>(m_Source); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_SkeletonSkin::TypeId, m_Source, i);
			CAssetPath SkeletonPath = AssetsManager()->GetAssetValue<CAssetPath>(Path, CAsset_SkeletonSkin::SKELETONPATH, -1, 0);
			if(SkeletonPath == m_AssetPath)
			{
				Add(new CItem(m_pAssetsEditor, Path, -1));
				m_SourceFound = true;
			}
		}
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

/* CHARACTER ITEM *****************************************************/

class CCharacterPartItem : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_SubPath;
	int m_Source;
	bool m_SourceFound;

public:
	CCharacterPartItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int SubPath, int Source) :
		gui::CExpand(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_AssetPath(AssetPath),
		m_SubPath(SubPath),
		m_SourceFound(false)
	{
		if(m_Source == m_AssetPath.GetSource())
			SetTitle(new CItem(m_pAssetsEditor, m_AssetPath, m_SubPath));
		else
			SetTitle(new CInactiveItem(m_pAssetsEditor, m_AssetPath, m_SubPath));
		
		Clear();
		
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_SkeletonSkin>(m_Source); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_CharacterPart::TypeId, m_Source, i);
			CAssetPath CharacterPath = AssetsManager()->GetAssetValue<CAssetPath>(Path, CAsset_CharacterPart::CHARACTERPATH, -1, 0);
			int CharacterPart = AssetsManager()->GetAssetValue<int>(Path, CAsset_CharacterPart::CHARACTERPART, -1, 0);
			if(CharacterPath == m_AssetPath && CharacterPart == m_SubPath)
			{
				Add(new CItem(m_pAssetsEditor, Path, -1));
				m_SourceFound = true;
			}
		}
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

class CCharacterItem : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CCharacterItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_AssetPath(AssetPath),
		m_SourceFound(false)
	{
		if(m_Source == m_AssetPath.GetSource())
			SetTitle(new CItem(m_pAssetsEditor, m_AssetPath, -1));
		else
			SetTitle(new CInactiveItem(m_pAssetsEditor, m_AssetPath, -1));
		
		Clear();
		
		const CAsset_Character* pCharacter = AssetsManager()->GetAsset<CAsset_Character>(m_AssetPath);
		if(pCharacter)
		{
			CAsset_Character::CIteratorPart Iter;
			for(Iter = pCharacter->BeginPart(); Iter != pCharacter->EndPart(); ++Iter)
			{
				CCharacterPartItem* pItem = new CCharacterPartItem(m_pAssetsEditor, m_AssetPath, *Iter, m_Source);
				
				if(pItem->SourceFound())
				{
					Add(pItem);
					m_SourceFound = true;
				}
				else
					delete pItem;
			}
		}
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Weapon>(m_Source); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_Weapon::TypeId, m_Source, i);
			CAssetPath CharacterPath = AssetsManager()->GetAssetValue<CAssetPath>(Path, CAsset_Weapon::CHARACTERPATH, -1, 0);
			if(CharacterPath == m_AssetPath)
			{
				Add(new CItem(m_pAssetsEditor, Path, -1));
				m_SourceFound = true;
			}
		}
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};

/* MAPGROUP ITEM ******************************************************/

class CMapGroupItem : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CMapGroupItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_AssetPath(AssetPath),
		m_SourceFound(false)
	{
		if(m_Source == m_AssetPath.GetSource())
			SetTitle(new CItem(m_pAssetsEditor, m_AssetPath, -1));
		else
			SetTitle(new CInactiveItem(m_pAssetsEditor, m_AssetPath, -1));
		
		Clear();
		
		const CAsset_MapGroup* pMapGroup = AssetsManager()->GetAsset<CAsset_MapGroup>(m_AssetPath);
		if(pMapGroup)
		{
			CAsset_MapGroup::CIteratorLayer IterLayer;
			for(IterLayer = pMapGroup->BeginLayer(); IterLayer != pMapGroup->EndLayer(); ++IterLayer)
			{
				CAssetPath Child = pMapGroup->GetLayer(*IterLayer);
				if(Child.GetSource() == m_Source)
				{
					Add(new CItem(m_pAssetsEditor, Child, -1));
					m_SourceFound = true;
				}
			}
		}
		
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};


/* MAP ITEM ***********************************************************/

class CMapItem : public gui::CExpand
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_Source;
	bool m_SourceFound;

public:
	CMapItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Source) :
		gui::CExpand(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_AssetPath(AssetPath),
		m_SourceFound(false)
	{
		if(m_Source == m_AssetPath.GetSource())
			SetTitle(new CItem(m_pAssetsEditor, m_AssetPath, -1));
		else
			SetTitle(new CInactiveItem(m_pAssetsEditor, m_AssetPath, -1));
		
		Clear();
		
		const CAsset_Map* pMap = AssetsManager()->GetAsset<CAsset_Map>(m_AssetPath);
		if(pMap)
		{
			CAsset_Map::CIteratorZoneLayer IterZoneLayer;
			for(IterZoneLayer = pMap->BeginZoneLayer(); IterZoneLayer != pMap->EndZoneLayer(); ++IterZoneLayer)
			{
				CAssetPath Child = pMap->GetZoneLayer(*IterZoneLayer);
				if(Child.GetSource() == m_Source)
				{
					Add(new CItem(m_pAssetsEditor, Child, -1));
					m_SourceFound = true;
				}
			}
			
			CAsset_Map::CIteratorBgGroup IterBgGroup;
			for(IterBgGroup = pMap->BeginBgGroup(); IterBgGroup != pMap->EndBgGroup(); ++IterBgGroup)
			{
				CAssetPath Child = pMap->GetBgGroup(*IterBgGroup);
				if(Child.GetSource() == m_Source)
				{
					Add(new CMapGroupItem(m_pAssetsEditor, Child, m_Source));
					m_SourceFound = true;
				}
			}
			
			CAsset_Map::CIteratorFgGroup IterFgGroup;
			for(IterFgGroup = pMap->BeginFgGroup(); IterFgGroup != pMap->EndFgGroup(); ++IterFgGroup)
			{
				CAssetPath Child = pMap->GetFgGroup(*IterFgGroup);
				if(Child.GetSource() == m_Source)
				{
					Add(new CMapGroupItem(m_pAssetsEditor, Child, m_Source));
					m_SourceFound = true;
				}
			}
		}
	}
	
	bool SourceFound()
	{
		return m_SourceFound || (m_AssetPath.GetSource() == m_Source);
	}
};


/* LOAD SOURCE BUTTON *************************************************/

class CLoadSourceButton : public gui::CButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Source;
	
protected:
	virtual void MouseClickAction()
	{
		Context()->DisplayPopup(new CPopup_SaveLoadAssets(
			m_pAssetsEditor, m_Source, CPopup_SaveLoadAssets::MODE_LOAD, m_DrawRect, gui::CPopup::ALIGNMENT_SIDE
		));
	}

public:
	CLoadSourceButton(CAssetsEditor* pAssetsEditor, int Source) :
		gui::CButton(pAssetsEditor, "Load", CAssetPath::SpriteSystem(SPRITE_EDITOR_LOAD)),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source)
	{ }
};


/* SAVE SOURCE BUTTON *************************************************/

class CSaveSourceButton : public gui::CButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Source;
	
protected:
	virtual void MouseClickAction()
	{
		Context()->DisplayPopup(new CPopup_SaveLoadAssets(
			m_pAssetsEditor, m_Source, CPopup_SaveLoadAssets::MODE_SAVE, m_DrawRect, gui::CPopup::ALIGNMENT_SIDE
		));
	}

public:
	CSaveSourceButton(CAssetsEditor* pAssetsEditor, int Source) :
		gui::CButton(pAssetsEditor, "Save", CAssetPath::SpriteSystem(SPRITE_EDITOR_SAVE)),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source)
	{ }
};


/* NEW ASSET BUTTON ***************************************************/

class CNewAssetButton : public gui::CButton
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Source;
	
protected:
	virtual void MouseClickAction()
	{
		//~ Context()->DisplayPopup(new CPopup_NewAsset(
			//~ m_pAssetsEditor, m_Source, m_DrawRect, gui::CPopup::ALIGNMENT_SIDE
		//~ ));
	}

public:
	CNewAssetButton(CAssetsEditor* pAssetsEditor, int Source) :
		gui::CButton(pAssetsEditor, "New asset", CAssetPath::SpriteSystem(SPRITE_EDITOR_INCREASE)),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source)
	{ }
};


/* ASSETS SOURCE ******************************************************/

CAssetsSource::CAssetsSource(CAssetsEditor* pAssetsEditor, int Source) :
	gui::CVScrollLayout(pAssetsEditor),
	m_pAssetsEditor(pAssetsEditor),
	m_Source(Source)
{
	RefreshAssetsList();
}

void CAssetsSource::OnAssetsListModified()
{
	RefreshAssetsList();
}

void CAssetsSource::RefreshAssetsList()
{
	Clear();
	
	//Load/Save/New
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(Context());
		Add(pLayout, false);
		
		pLayout->Add(new CLoadSourceButton(AssetsEditor(), m_Source), true);
		pLayout->Add(new CSaveSourceButton(AssetsEditor(), m_Source), true);
	}
	Add(new CNewAssetButton(AssetsEditor(), m_Source), false);
	
	m_pAssetsList = new gui::CVScrollLayout(Context());
	Add(m_pAssetsList, true);
	
	#define REFRESH_ASSET_LIST(ClassName) {\
		for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(m_Source); i++)\
		{\
			CAssetPath Path = CAssetPath::Asset(ClassName::TypeId, m_Source, i);\
			CAssetState* pState = AssetsManager()->GetAssetState(Path);\
			if(pState && !pState->m_ListedInEditor)\
			{\
				m_pAssetsList->Add(new CItem(AssetsEditor(), Path, -1));\
			}\
		}\
	}
	
	//Images
	//	Sprites
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Image>(s); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_Image::TypeId, s, i);
			CImageItem* pItem = new CImageItem(AssetsEditor(), Path, m_Source);
			if(pItem->SourceFound())
				m_pAssetsList->Add(pItem);
			else
				delete pItem;
		}
	}
	
	REFRESH_ASSET_LIST(CAsset_Sprite)
	
	//Maps
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Map>(s); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_Map::TypeId, s, i);
			CMapItem* pItem = new CMapItem(AssetsEditor(), Path, m_Source);
			if(pItem->SourceFound())
				m_pAssetsList->Add(pItem);
			else
				delete pItem;
		}
	}
	
	REFRESH_ASSET_LIST(CAsset_MapGroup)
	REFRESH_ASSET_LIST(CAsset_MapLayerTiles)
	REFRESH_ASSET_LIST(CAsset_MapLayerQuads)
	REFRESH_ASSET_LIST(CAsset_MapZoneTiles)
	
	//Skeletons
	//	SkeletonAnimations
	//	SkeletonSkins
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Skeleton>(s); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_Skeleton::TypeId, s, i);
			CSkeletonItem* pItem = new CSkeletonItem(AssetsEditor(), Path, m_Source);
			if(pItem->SourceFound())
				m_pAssetsList->Add(pItem);
			else
				delete pItem;
		}
	}
	
	REFRESH_ASSET_LIST(CAsset_SkeletonAnimation)
	REFRESH_ASSET_LIST(CAsset_SkeletonSkin)
	
	//Charaters
	//	CharacterParts
	//	Weapons
	for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
	{
		for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Character>(s); i++)
		{
			CAssetPath Path = CAssetPath::Asset(CAsset_Character::TypeId, s, i);
			CCharacterItem* pItem = new CCharacterItem(AssetsEditor(), Path, m_Source);
			if(pItem->SourceFound())
				m_pAssetsList->Add(pItem);
			else
				delete pItem;
		}
	}
	
	REFRESH_ASSET_LIST(CAsset_CharacterPart)
	
	REFRESH_ASSET_LIST(CAsset_GuiRectStyle)
	REFRESH_ASSET_LIST(CAsset_GuiLineStyle)
	REFRESH_ASSET_LIST(CAsset_GuiBoxStyle)
	REFRESH_ASSET_LIST(CAsset_GuiLabelStyle)
	REFRESH_ASSET_LIST(CAsset_GuiButtonStyle)
	REFRESH_ASSET_LIST(CAsset_GuiToggleStyle)
	REFRESH_ASSET_LIST(CAsset_GuiScrollbarStyle)
	REFRESH_ASSET_LIST(CAsset_GuiTabsStyle)
}

/* ASSETS LIST ********************************************************/

CAssetsOrganizer::CAssetsOrganizer(CAssetsEditor* pAssetsEditor) :
	gui::CTabs(pAssetsEditor),
	m_pAssetsEditor(pAssetsEditor)
{
	m_apAssetsSources[CAssetPath::SRC_SYSTEM] = new CAssetsSource(AssetsEditor(), CAssetPath::SRC_SYSTEM);
	AddTab(m_apAssetsSources[CAssetPath::SRC_SYSTEM], "System", CAssetPath::SpriteSystem(SPRITE_EDITOR_SYSTEM));
	
	m_apAssetsSources[CAssetPath::SRC_UNIVERSE] = new CAssetsSource(AssetsEditor(), CAssetPath::SRC_UNIVERSE);
	AddTab(m_apAssetsSources[CAssetPath::SRC_UNIVERSE], "Universe", CAssetPath::SpriteSystem(SPRITE_EDITOR_UNIVERSE));
	
	m_apAssetsSources[CAssetPath::SRC_WORLD] = new CAssetsSource(AssetsEditor(), CAssetPath::SRC_WORLD);
	AddTab(m_apAssetsSources[CAssetPath::SRC_WORLD], "World", CAssetPath::SpriteSystem(SPRITE_EDITOR_WORLD));
	
	m_apAssetsSources[CAssetPath::SRC_LAND] = new CAssetsSource(AssetsEditor(), CAssetPath::SRC_LAND);
	AddTab(m_apAssetsSources[CAssetPath::SRC_LAND], "Land", CAssetPath::SpriteSystem(SPRITE_EDITOR_LAND));
	
	m_apAssetsSources[CAssetPath::SRC_SKIN] = new CAssetsSource(AssetsEditor(), CAssetPath::SRC_SKIN);
	AddTab(m_apAssetsSources[CAssetPath::SRC_SKIN], "Skins", CAssetPath::SpriteSystem(SPRITE_EDITOR_SKIN));
}

void CAssetsOrganizer::OnAssetsListModified()
{
	for(int i=0; i<CAssetPath::NUM_SOURCES; i++)
		m_apAssetsSources[i]->OnAssetsListModified();
}

}

}
