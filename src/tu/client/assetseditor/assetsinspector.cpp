#include <tu/client/assetseditor/assetsinspector.h>
#include <tu/client/assetseditor/assetseditor.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/gui/text-edit.h>
#include <tu/client/gui/integer-edit.h>
#include <tu/client/gui/float-edit.h>
#include <tu/client/gui/popup.h>
#include <game/client/localization.h>

namespace tu
{

namespace assetseditor
{

/* ASSETS INSPECTOR ***************************************************/

CAssetsInspector::CAssetsInspector(CAssetsEditor* pAssetsEditor) :
	gui::CTabs(pAssetsEditor),
	m_pAssetsEditor(pAssetsEditor)
{
	for(int i=0; i<NUM_TABS; i++)
		m_pTabs[TAB_IMAGE_ASSET] = 0;
	
	//TAG_NEW_ASSET
	m_pTabs[TAB_GENERIC_ASSET] = CreateTab_Generic_Asset();
	m_pTabs[TAB_IMAGE_ASSET] = CreateTab_Image_Asset();
	m_pTabs[TAB_SPRITE_ASSET] = CreateTab_Sprite_Asset();
	m_pTabs[TAB_GUIRECTSTYLE_ASSET] = CreateTab_GuiRectStyle_Asset();
	m_pTabs[TAB_GUILINESTYLE_ASSET] = CreateTab_GuiLineStyle_Asset();
	m_pTabs[TAB_GUIBOXSTYLE_ASSET] = CreateTab_GuiBoxStyle_Asset();
	m_pTabs[TAB_GUILABELSTYLE_ASSET] = CreateTab_GuiLabelStyle_Asset();
	m_pTabs[TAB_GUIBUTTONSTYLE_ASSET] = CreateTab_GuiButtonStyle_Asset();
	m_pTabs[TAB_GUITOGGLESTYLE_ASSET] = CreateTab_GuiToggleStyle_Asset();
	m_pTabs[TAB_GUISLIDERSTYLE_ASSET] = CreateTab_GuiSliderStyle_Asset();
	m_pTabs[TAB_GUISCROLLBARSTYLE_ASSET] = CreateTab_GuiScrollbarStyle_Asset();
	m_pTabs[TAB_GUITABSSTYLE_ASSET] = CreateTab_GuiTabsStyle_Asset();
}

void CAssetsInspector::Update()
{
	if(AssetsManager()->GetAssetState(m_pAssetsEditor->GetEditedAssetPath()))
		Enable();
	else
	{
		Disable();
		return;
	}
	
	for(int i=0; i<NUM_TABS; i++)
		m_pTabs[i]->Disable();
	
	if(!m_pAssetsEditor->GetEditedAssetPath().IsNull())
	{
		//TAG_NEW_ASSET
		switch(m_pAssetsEditor->GetEditedAssetPath().GetType())
		{
			case CAssetPath::TYPE_IMAGE:
				m_pTabs[TAB_IMAGE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_SPRITE:
				m_pTabs[TAB_SPRITE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUIRECTSTYLE:
				m_pTabs[TAB_GUIRECTSTYLE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUILINESTYLE:
				m_pTabs[TAB_GUILINESTYLE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUIBOXSTYLE:
				m_pTabs[TAB_GUIBOXSTYLE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUILABELSTYLE:
				m_pTabs[TAB_GUILABELSTYLE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUIBUTTONSTYLE:
				m_pTabs[TAB_GUIBUTTONSTYLE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUITOGGLESTYLE:
				m_pTabs[TAB_GUITOGGLESTYLE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUISLIDERSTYLE:
				m_pTabs[TAB_GUISLIDERSTYLE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUISCROLLBARSTYLE:
				m_pTabs[TAB_GUISCROLLBARSTYLE_ASSET]->Enable();
				break;
			case CAssetPath::TYPE_GUITABSSTYLE:
				m_pTabs[TAB_GUITABSSTYLE_ASSET]->Enable();
				break;
			default:
				m_pTabs[TAB_GENERIC_ASSET]->Enable();
				break;
		}
	}
	
	gui::CTabs::Update();
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_Generic_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Asset", CAssetPath::SpriteSystem(SPRITE_EDITOR_ASSET));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_Image_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Image", CAssetPath::SpriteSystem(SPRITE_EDITOR_IMAGE));
	
	AddField_AssetProperties(pTab);
	
	AddField_Integer(pTab, CAsset_Image::GRIDWIDTH, "Grid width");
	AddField_Integer(pTab, CAsset_Image::GRIDHEIGHT, "Grid height");
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_Sprite_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Sprite", CAssetPath::SpriteSystem(SPRITE_EDITOR_SPRITE));
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_Sprite::IMAGEPATH, CAssetPath::TYPE_IMAGE, "Image");
	AddField_Integer(pTab, CAsset_Sprite::X, "X");
	AddField_Integer(pTab, CAsset_Sprite::Y, "Y");
	AddField_Integer(pTab, CAsset_Sprite::WIDTH, "Width");
	AddField_Integer(pTab, CAsset_Sprite::HEIGHT, "Height");
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiRectStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Rectangle Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUIRECT));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiLineStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Line Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUILINE));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiBoxStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Box Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUIBOX));
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiBoxStyle::RECTSTYLEPATH, CAssetPath::TYPE_GUIRECTSTYLE, "Rectangle");
	AddField_Integer(pTab, CAsset_GuiBoxStyle::MINWIDTH, "Width");
	AddField_Integer(pTab, CAsset_GuiBoxStyle::MINHEIGHT, "Height");
	AddField_Integer(pTab, CAsset_GuiBoxStyle::MARGIN, "Margin");
	AddField_Integer(pTab, CAsset_GuiBoxStyle::PADDING, "Padding");
	AddField_Integer(pTab, CAsset_GuiBoxStyle::SPACING, "Spacing");
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiLabelStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Label Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUILABEL));
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiLabelStyle::RECTPATH, CAssetPath::TYPE_GUIRECTSTYLE, "Rectangle");
	AddField_Integer(pTab, CAsset_GuiLabelStyle::MINWIDTH, "Width");
	AddField_Integer(pTab, CAsset_GuiLabelStyle::MINHEIGHT, "Height");
	AddField_Integer(pTab, CAsset_GuiLabelStyle::MARGIN, "Margin");
	AddField_Integer(pTab, CAsset_GuiLabelStyle::PADDING, "Padding");
	AddField_Integer(pTab, CAsset_GuiLabelStyle::SPACING, "Spacing");
	AddField_Integer(pTab, CAsset_GuiLabelStyle::FONTSIZE, "Font Size");
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiButtonStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Button Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUIBUTTON));
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiButtonStyle::IDLESTYLEPATH, CAssetPath::TYPE_GUILABELSTYLE, "Idle Style");
	AddField_Asset(pTab, CAsset_GuiButtonStyle::MOUSEOVERSTYLEPATH, CAssetPath::TYPE_GUILABELSTYLE, "Mouseover Style");
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiToggleStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Toggle Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUITOGGLE));
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiToggleStyle::IDLETRUESTYLEPATH, CAssetPath::TYPE_GUILABELSTYLE, "Idle Style (true)");
	AddField_Asset(pTab, CAsset_GuiToggleStyle::MOUSEOVERTRUESTYLEPATH, CAssetPath::TYPE_GUILABELSTYLE, "Mouseover Style (true)");
	AddField_Asset(pTab, CAsset_GuiToggleStyle::IDLEFALSESTYLEPATH, CAssetPath::TYPE_GUILABELSTYLE, "Idle Style (false)");
	AddField_Asset(pTab, CAsset_GuiToggleStyle::MOUSEOVERFALSESTYLEPATH, CAssetPath::TYPE_GUILABELSTYLE, "Mouseover Style (false)");
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiSliderStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Slider Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUISLIDER));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiScrollbarStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Scrollbar Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUISCROLLBAR));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiTabsStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, "Tabs Style", CAssetPath::SpriteSystem(SPRITE_EDITOR_GUITABS));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

void CAssetsInspector::AddField(gui::CVScrollLayout* pList, gui::CWidget* pWidget, const char* pLabelText)
{
	if(pLabelText)
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(Context());
		pList->Add(pLayout, false);
		
		gui::CLabel* pLabel = new gui::CLabel(Context(), pLabelText);
		pLayout->Add(pLabel, true);
		
		pLayout->Add(pWidget, true);
	}
	else
	{
		pList->Add(pWidget, false);
	}
}

void CAssetsInspector::AddField_AssetProperties(gui::CVScrollLayout* pTab)
{
	AddField_Text(pTab, CAsset::NAME, sizeof(CAsset::m_aName), "Name");
	
	//~ {
		//~ char aBuf[64];
		
		//~ #define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
			//~ str_format(aBuf, sizeof(aBuf), "%s #%d", AssetTypeName, m_pAssetsEditor->GetEditedAssetPath().GetId());\
			//~ break;

		//~ switch(AssetsEditor()->GetEditedAssetPath().GetType())
		//~ {
			//~ #include <tu/client/assetsmacro.h>
		//~ }
		
		//~ #undef TU_MACRO_ASSETTYPE
		
		//~ AddField(pTab, new gui::CLabel(Context(), aBuf), "Asset reference:");
	//~ }
	
	//~ {
		//~ gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig);
		//~ pList->Add(pLayout, false);
		
		//~ pLayout->Add(new CEditor::CDuplicateAsset(m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath), true);
		//~ pLayout->Add(new CEditor::CDeleteAsset(m_pAssetsEditor, m_pAssetsEditor->m_EditedAssetPath), true);
	//~ }
	
	pTab->AddSeparator();
}

/* TEXT EDIT **********************************************************/
	
class CMemberTextEdit : public gui::CAbstractTextEdit
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Member;
	int m_TextSize;
	
	virtual void SaveFromTextBuffer()
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<const char*>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			GetText()
		);
	}
	
	virtual void CopyToTextBuffer()
	{
		const char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<const char*>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_Member,
			m_pAssetsEditor->GetEditedSubPath(),
			GetText()
		);
		
		if(pName)
		{
			if(str_comp(pName, m_aText) != 0)
				SetText(pName);
		}
		else
			SetText("");
	}
	
public:
	CMemberTextEdit(CAssetsEditor* pAssetsEditor, int Member, int TextSize) :
		gui::CAbstractTextEdit(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member),
		m_TextSize(TextSize)
	{ }
};

void CAssetsInspector::AddField_Text(gui::CVScrollLayout* pList, int Member, int TextSize, const char* pLabelText)
{
	CMemberTextEdit* pWidget = new CMemberTextEdit(
		m_pAssetsEditor,
		Member,
		TextSize
	);
	
	AddField(pList, pWidget, pLabelText);
}

/* INTEGER EDIT *******************************************************/
	
class CMemberIntegerEdit : public gui::CAbstractIntegerEdit
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Member;
	
	virtual int GetValue() const
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_Member,
			m_pAssetsEditor->GetEditedSubPath(),
			0
		);
	}
	
	virtual void SetValue(int Value)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			Value
		);
	}
	
public:
	CMemberIntegerEdit(CAssetsEditor* pAssetsEditor, int Member) :
		gui::CAbstractIntegerEdit(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member)
	{ }
};

void CAssetsInspector::AddField_Integer(gui::CVScrollLayout* pList, int Member, const char* pLabelText)
{
	CMemberIntegerEdit* pWidget = new CMemberIntegerEdit(
		m_pAssetsEditor,
		Member
	);
	
	AddField(pList, pWidget, pLabelText);
}

/* INTEGER EDIT *******************************************************/

class CMemberAssetEdit : public gui::CButton
{
public:
	class CPopup : public gui::CPopup
	{
	public:
		class CItem : public gui::CButton
		{
		protected:
			CAssetPath m_AssetPath;
			CPopup* m_pPopup;
		
		protected:
			virtual void MouseClickAction()
			{
				m_pPopup->SetValue(m_AssetPath);
			}
			
		public:
			CItem(CPopup* pPopup, CAssetPath AssetPath) :
				gui::CButton(pPopup->Context(), ""),
				m_pPopup(pPopup),
				m_AssetPath(AssetPath)
			{
				SetLabelStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_ASSETEDITBUTTON));
				
				if(m_AssetPath.IsNull())
				{
					SetIcon(CAssetPath::SpriteSystem(SPRITE_EDITOR_NONE));
					SetText("None");
				}
				else
				{
					SetIcon(m_pPopup->m_pAssetsEditor->GetItemIcon(m_AssetPath, -1));
					SetText(m_pPopup->m_pAssetsEditor->GetItemName(m_AssetPath, -1));
				}
			}
		};
	
	protected:
		CAssetsEditor* m_pAssetsEditor;
		int m_Member;
		int m_AssetType;
	
	public:
		CPopup(CAssetsEditor* pAssetsEditor, int Member, int AssetType, gui::CRect ParentRect) :
			gui::CPopup(pAssetsEditor, ParentRect, 250, 400, gui::CPopup::ALIGNMENT_SIDE),
			m_pAssetsEditor(pAssetsEditor),
			m_Member(Member),
			m_AssetType(AssetType)
		{
			gui::CVScrollLayout* pLayout = new gui::CVScrollLayout(Context());
			Add(pLayout);
			
			pLayout->Add(new CItem(this, CAssetPath::Null()), false);
			
			#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), "System"), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_SYSTEM); i++)\
					pLayout->Add(new CItem(this, CAssetPath::System(ClassName::TypeId, i)), false);\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), "Universe"), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_UNIVERSE); i++)\
					pLayout->Add(new CItem(this, CAssetPath::Universe(ClassName::TypeId, i)), false);\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), "World"), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_WORLD); i++)\
					pLayout->Add(new CItem(this, CAssetPath::World(ClassName::TypeId, i)), false);\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), "Land"), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_LAND); i++)\
					pLayout->Add(new CItem(this, CAssetPath::Land(ClassName::TypeId, i)), false);\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), "Skin"), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_SKIN); i++)\
					pLayout->Add(new CItem(this, CAssetPath::Skin(ClassName::TypeId, i)), false);\
				break;
			
			switch(m_AssetType)
			{
				#include <tu/client/assetsmacro.h>
			}
			
			#undef TU_MACRO_ASSETTYPE
		}
		
		void SetValue(CAssetPath Value)
		{
			m_pAssetsEditor->AssetsManager()->SetAssetValue<CAssetPath>(
				m_pAssetsEditor->GetEditedAssetPath(),
				m_pAssetsEditor->GetEditedSubPath(),
				m_Member,
				Value
			);
			m_pAssetsEditor->OnAssetsListModified();
		}
	};

protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Member;
	int m_AssetType;
	
protected:
	virtual void MouseClickAction()
	{
		Context()->DisplayPopup(new CPopup(m_pAssetsEditor, m_Member, m_AssetType, m_DrawRect));
	}

public:
	CMemberAssetEdit(CAssetsEditor* pAssetsEditor, int Member, int AssetType) :
		gui::CButton(pAssetsEditor, "", CAssetPath::Null()),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member),
		m_AssetType(AssetType)
	{
		SetLabelStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_ASSETEDITBUTTON));
	}
	
	virtual void Update()
	{		
		CAssetPath Value = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_Member,
			m_pAssetsEditor->GetEditedSubPath(),
			0
		);
		
		if(Value.IsNull())
		{
			SetIcon(CAssetPath::SpriteSystem(SPRITE_EDITOR_NONE));
			SetText("None");
		}
		else
		{
			SetIcon(m_pAssetsEditor->GetItemIcon(Value, -1));
			SetText(m_pAssetsEditor->GetItemName(Value, -1));
		}
		
		gui::CButton::Update();
	}
};

void CAssetsInspector::AddField_Asset(gui::CVScrollLayout* pList, int Member, int AssetType, const char* pLabelText)
{
	CMemberAssetEdit* pWidget = new CMemberAssetEdit(
		m_pAssetsEditor,
		Member,
		AssetType
	);
	
	AddField(pList, pWidget, pLabelText);
}

}

}
