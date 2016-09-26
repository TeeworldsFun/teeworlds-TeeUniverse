#include <tu/client/assetseditor/assetseditor.h>
#include <tu/client/assetseditor/assetslist.h>
#include <tu/client/assetseditor/view.h>
#include <tu/client/assetseditor/timeline.h>
#include <tu/client/assetseditor/assetsinspector.h>
#include <engine/shared/config.h>
#include <engine/keys.h>
#include <engine/input.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/clientmode.h>
#include <tu/client/gui/panellayout.h>

namespace tu
{

namespace assetseditor
{

CAssetsEditor::CAssetsEditor(CKernel* pKernel) :
	gui::CContext(pKernel),
	m_EditedSubPath(-1)
{	
	m_LabelStyle = CAssetPath::GuiLabelStyleSystem(GUILABELSTYLE_EDITOR_TEXT);
	m_LabelHeaderStyle = CAssetPath::GuiLabelStyleSystem(GUILABELSTYLE_EDITOR_HEADER);
	m_ButtonStyle = CAssetPath::GuiButtonStyleSystem(GUIBUTTONSTYLE_EDITOR_DEFAULT);
	m_TextEntryStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_TEXTENTRY);
	m_NumericEntryStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_NUMERICENTRY);
	m_ScrollbarStyle = CAssetPath::GuiScrollbarStyleSystem(GUISCROLLBARSTYLE_EDITOR);
	m_TabsStyle = CAssetPath::GuiTabsStyleSystem(GUITABSSTYLE_EDITOR);
	m_PopupStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_POPUP);
}

CAssetsEditor::~CAssetsEditor()
{
	AssetsManager()->RemoveListener(AssetManager_GetToken());
}

void CAssetsEditor::CreateMainWidget()
{
	gui::CHPanelLayout* pHPanel = new gui::CHPanelLayout(this);
	gui::CVPanelLayout* pVPanel = new gui::CVPanelLayout(this);
	
	m_pAssetsOrganizer = new CAssetsOrganizer(this);
	pHPanel->Add(m_pAssetsOrganizer, 280);
	
	pVPanel->Add(new CViewManager(this), -1);
	pVPanel->Add(new CTimeLine(this), 200);
	pHPanel->Add(pVPanel, -1);
	
	pHPanel->Add(new CAssetsInspector(this), 280);
	
	m_pMainWidget = pHPanel;
	
	AssetsManager()->AddListener(this);
}

void CAssetsEditor::DoShortcuts()
{
	if(Input()->KeyIsPressed(KEY_ESCAPE))
	{
		Close();
		return;
	}
	
	if(Input()->KeyIsPressed(KEY_LCTRL) && Input()->KeyPress(KEY_Z, true))
		AssetsManager()->Undo();
	
	//TAG_DEBUG
	if(Input()->KeyIsPressed(KEY_LCTRL) && Input()->KeyPress(KEY_A, true))
		m_GuiScale -= 0.25f;
	
	if(Input()->KeyIsPressed(KEY_LCTRL) && Input()->KeyPress(KEY_E, true))
		m_GuiScale += 0.25f;
}

void CAssetsEditor::RenderBackground()
{
	Graphics()->TextureClear();
	Graphics()->QuadsBegin();
	Graphics()->SetColor(58.0f/255.0f, 58.0f/255.0f, 58.0f/255.0f, 1.0f);
	
	IGraphics::CQuadItem QuadItem(m_DrawRect.w/2, m_DrawRect.h/2, m_DrawRect.w, m_DrawRect.h);
	Graphics()->QuadsDraw(&QuadItem, 1);
	
	Graphics()->QuadsEnd();
}

void CAssetsEditor::Close()
{
	g_Config.m_ClMode = TU_CLIENTMODE_GAME;
}

void CAssetsEditor::OnAssetsListModified()
{
	m_pAssetsOrganizer->OnAssetsListModified();
}

CAssetPath CAssetsEditor::GetItemIcon(CAssetPath AssetPath, int SubPath)
{
	CAssetPath IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_DEFAULT);
	
	if(SubPath == -1)
	{
		//TAG_NEW_ASSET
		switch(AssetPath.GetType())
		{
			case CAssetPath::TYPE_IMAGE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_IMAGE);
				break;
			case CAssetPath::TYPE_SPRITE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_SPRITE);
				break;
			case CAssetPath::TYPE_SKELETON:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_SKELETON);
				break;
			case CAssetPath::TYPE_SKELETONSKIN:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_SKELETONSKIN);
				break;
			case CAssetPath::TYPE_SKELETONANIMATION:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_SKELETONANIMATION);
				break;
			case CAssetPath::TYPE_CHARACTER:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_CHARACTER);
				break;
			case CAssetPath::TYPE_CHARACTERPART:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_CHARACTERPART);
				break;
			case CAssetPath::TYPE_WEAPON:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_WEAPON);
				break;
			case CAssetPath::TYPE_MAPZONETILES:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_ZONETILES);
				break;
			case CAssetPath::TYPE_MAP:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_MAP);
				break;
			case CAssetPath::TYPE_MAPGROUP:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_FOLDER);
				break;
			case CAssetPath::TYPE_MAPLAYERTILES:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_TILES);
				break;
			case CAssetPath::TYPE_MAPLAYERQUADS:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_QUAD);
				break;
			case CAssetPath::TYPE_ZONETYPE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_ZONETYPE);
				break;
			case CAssetPath::TYPE_GUIRECTSTYLE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_GUIRECT);
				break;
			case CAssetPath::TYPE_GUILINESTYLE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_GUILINE);
				break;
			case CAssetPath::TYPE_GUIBOXSTYLE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_GUIBOX);
				break;
			case CAssetPath::TYPE_GUILABELSTYLE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_GUILABEL);
				break;
			case CAssetPath::TYPE_GUIBUTTONSTYLE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_GUIBUTTON);
				break;
			case CAssetPath::TYPE_GUITOGGLESTYLE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_GUITOGGLE);
				break;
			case CAssetPath::TYPE_GUISCROLLBARSTYLE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_GUISCROLLBAR);
				break;
			case CAssetPath::TYPE_GUITABSSTYLE:
				IconPath = CAssetPath::SpriteSystem(SPRITE_EDITOR_GUITABS);
				break;
		}
	}
	
	return IconPath;
}

const char* CAssetsEditor::GetItemName(CAssetPath AssetPath, int SubPath)
{
	if(SubPath == -1)
		return AssetsManager()->GetAssetValue<const char*>(AssetPath, CAsset::NAME, SubPath, 0);
	else if(AssetPath.GetType() == CAssetPath::TYPE_CHARACTER)
		return AssetsManager()->GetAssetValue<const char*>(AssetPath, CAsset_Character::PART_NAME, SubPath, 0);
	else
		return 0;
}

}

}
