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
	
	m_pTabs[TAB_GENERIC_ASSET] = CreateTab_Generic_Asset();
	m_pTabs[TAB_IMAGE_ASSET] = CreateTab_Image_Asset();
	m_pTabs[TAB_SPRITE_ASSET] = CreateTab_Sprite_Asset();
	m_pTabs[TAB_GUIRECTSTYLE_ASSET] = CreateTab_GuiRectStyle_Asset();
	m_pTabs[TAB_GUILINESTYLE_ASSET] = CreateTab_GuiLineStyle_Asset();
	m_pTabs[TAB_GUIBOXSTYLE_ASSET] = CreateTab_GuiBoxStyle_Asset();
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

gui::CVListLayout* CAssetsInspector::CreateTab_Generic_Asset()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Context());
	pTab->Disable();
	AddTab(pTab, Context()->Localize("Asset"), CAssetPath::SpriteSystem(SPRITE_ICON_ASSET));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVListLayout* CAssetsInspector::CreateTab_Image_Asset()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Context());
	pTab->Disable();
	AddTab(pTab, Context()->Localize("Image"), CAssetPath::SpriteSystem(SPRITE_ICON_IMAGE));
	
	AddField_AssetProperties(pTab);
	
	AddField_Integer(pTab, CAsset_Image::GRIDWIDTH, Context()->Localize("Grid width"));
	AddField_Integer(pTab, CAsset_Image::GRIDHEIGHT, Context()->Localize("Grid height"));
	
	return pTab;
}

gui::CVListLayout* CAssetsInspector::CreateTab_Sprite_Asset()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Context());
	pTab->Disable();
	AddTab(pTab, Context()->Localize("Sprite"), CAssetPath::SpriteSystem(SPRITE_ICON_SPRITE));
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_Sprite::IMAGEPATH, CAssetPath::TYPE_IMAGE, Context()->Localize("Image"));
	AddField_Integer(pTab, CAsset_Sprite::X, Context()->Localize("X"));
	AddField_Integer(pTab, CAsset_Sprite::Y, Context()->Localize("Y"));
	AddField_Integer(pTab, CAsset_Sprite::WIDTH, Context()->Localize("Width"));
	AddField_Integer(pTab, CAsset_Sprite::HEIGHT, Context()->Localize("Height"));
	
	return pTab;
}

gui::CVListLayout* CAssetsInspector::CreateTab_GuiRectStyle_Asset()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Context());
	pTab->Disable();
	AddTab(pTab, Context()->Localize("Rectangle Style"), CAssetPath::SpriteSystem(SPRITE_ICON_GUIRECT));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVListLayout* CAssetsInspector::CreateTab_GuiLineStyle_Asset()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Context());
	pTab->Disable();
	AddTab(pTab, Context()->Localize("Line Style"), CAssetPath::SpriteSystem(SPRITE_ICON_GUILINE));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVListLayout* CAssetsInspector::CreateTab_GuiBoxStyle_Asset()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Context());
	pTab->Disable();
	AddTab(pTab, Context()->Localize("Box Style"), CAssetPath::SpriteSystem(SPRITE_ICON_GUIBOX));
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiBoxStyle::DEFAULTRECTPATH, CAssetPath::TYPE_GUIRECTSTYLE, Context()->Localize("Default Rectangle"));
	AddField_Asset(pTab, CAsset_GuiBoxStyle::MOUSEOVERRECTPATH, CAssetPath::TYPE_GUIRECTSTYLE, Context()->Localize("MouseOver Rectangle"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::MARGIN, Context()->Localize("Margin"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::PADDING, Context()->Localize("Padding"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::SPACING, Context()->Localize("Spacing"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::FONTSIZE, Context()->Localize("Font Size"));
	
	return pTab;
}

gui::CVListLayout* CAssetsInspector::CreateTab_GuiScrollbarStyle_Asset()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Context());
	pTab->Disable();
	AddTab(pTab, Context()->Localize("Scrollbar Style"), CAssetPath::SpriteSystem(SPRITE_ICON_GUISCROLLBAR));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVListLayout* CAssetsInspector::CreateTab_GuiTabsStyle_Asset()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Context());
	pTab->Disable();
	AddTab(pTab, Context()->Localize("Tabs Style"), CAssetPath::SpriteSystem(SPRITE_ICON_GUITABS));
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

void CAssetsInspector::AddField(gui::CVListLayout* pList, gui::CWidget* pWidget, const char* pLabelText)
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

void CAssetsInspector::AddField_AssetProperties(gui::CVListLayout* pTab)
{
	AddField_Text(pTab, CAsset::NAME, sizeof(CAsset::m_aName), Context()->Localize("Name"));
	
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

void CAssetsInspector::AddField_Text(gui::CVListLayout* pList, int Member, int TextSize, const char* pLabelText)
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

void CAssetsInspector::AddField_Integer(gui::CVListLayout* pList, int Member, const char* pLabelText)
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
				SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_ASSETEDITBUTTON));
				
				if(m_AssetPath.IsNull())
				{
					SetIcon(CAssetPath::SpriteSystem(SPRITE_ICON_NONE));
					SetText(Context()->Localize("None"));
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
			gui::CVListLayout* pLayout = new gui::CVListLayout(Context());
			Add(pLayout);
			
			pLayout->Add(new CItem(this, CAssetPath::Null()), false);
			
			#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), Context()->Localize("System")), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_SYSTEM); i++)\
					pLayout->Add(new CItem(this, CAssetPath::System(ClassName::TypeId, i)), false);\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), Context()->Localize("Universe")), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_UNIVERSE); i++)\
					pLayout->Add(new CItem(this, CAssetPath::Universe(ClassName::TypeId, i)), false);\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), Context()->Localize("World")), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_WORLD); i++)\
					pLayout->Add(new CItem(this, CAssetPath::World(ClassName::TypeId, i)), false);\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), Context()->Localize("Land")), false);\
				for(int i=0; i<AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_LAND); i++)\
					pLayout->Add(new CItem(this, CAssetPath::Land(ClassName::TypeId, i)), false);\
				pLayout->AddSeparator();\
				pLayout->Add(new gui::CLabelHeader(Context(), Context()->Localize("Skin")), false);\
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
		SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_ASSETEDITBUTTON));
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
			SetIcon(CAssetPath::SpriteSystem(SPRITE_ICON_NONE));
			SetText(Context()->Localize("None"));
		}
		else
		{
			SetIcon(m_pAssetsEditor->GetItemIcon(Value, -1));
			SetText(m_pAssetsEditor->GetItemName(Value, -1));
		}
		
		gui::CButton::Update();
	}
};

void CAssetsInspector::AddField_Asset(gui::CVListLayout* pList, int Member, int AssetType, const char* pLabelText)
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
