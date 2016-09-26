#include <engine/storage.h>
#include <engine/shared/config.h>
#include <engine/shared/datafile.h>
#include <generated/client_data.h>
#include <game/mapitems.h>
#include <game/gamecore.h>
#include <game/client/render.h>

#include <engine/external/pnglite/pnglite.h>

#include <tu/client/assetsmanager.h>

namespace tu
{

#define CREATE_SPRITE(id, name, image, x, y, w, h) {\
	CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(CAssetPath::SpriteSystem(id));\
	pSprite->SetName(name);\
	pSprite->Init(CAssetPath::ImageSystem(image), x, y, w, h);\
}

void CAssetsManager::InitAssetsManager_System(CAssetsManager* pAssetsManager)
{
	//GuiRectStyle
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_COMPOSING));
		pGuiRectStyle->SetName("composing");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiRectStyle->m_CornerRadius = 3.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_SELECTION));
		pGuiRectStyle->SetName("selection");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*3)/512.0f, (256-16*6)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*0)/512.0f, (256-16*3)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_STAMP));
		pGuiRectStyle->SetName("stamp");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*6)/512.0f, (256-16*6)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*3)/512.0f, (256-16*3)/256.0f);
	}
	
	//GuiLabelStyles
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_COMPOSING));
		pGuiStyle->SetName("composing");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_COMPOSING));
		pGuiStyle->SetPadding(4);
		pGuiStyle->m_TextColor = vec4(15.0f/255.0f, 103.0f/255.0f, 146.0f/255.0f, 1.0f);
		pGuiStyle->m_TextAlignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_LEFT;
	}
	
	InitAssetsManager_System_Menu(pAssetsManager);
	InitAssetsManager_System_Editor(pAssetsManager);
}

void CAssetsManager::InitAssetsManager_System_Menu(CAssetsManager* pAssetsManager)
{
	//Images
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SYSTEM, IStorage::TYPE_ALL, "tu/menu.png", CAssetsHistory::NO_TOKEN);
		if(pImage)
			pImage->SetGridSize(16, 8);
	}
	
	CREATE_SPRITE(SPRITE_MENU_CHECKBOX_TRUE, "iconMenuMenuCheckBoxTrue", IMAGE_MENU, 0, 0, 1, 1);
	CREATE_SPRITE(SPRITE_MENU_CHECKBOX_FALSE, "iconMenuMenuCheckBoxFalse", IMAGE_MENU, 1, 0, 1, 1);
	
	//GuiRectStyles
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_BACKGROUND));
		pGuiRectStyle->SetName("menuRect");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.0f, 0.0f, 0.0f, 0.2f);
		pGuiRectStyle->m_CornerRadius = 5.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_MOUSEOVER));
		pGuiRectStyle->SetName("menuRectMO");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 0.4f);
		pGuiRectStyle->m_CornerRadius = 5.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_HIGHLIGHT));
		pGuiRectStyle->SetName("menuRectHL");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 0.7f);
		pGuiRectStyle->m_CornerRadius = 5.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	
	//GuiBoxStyles
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_MENU_COLUMNS));
		pGuiBoxStyle->SetName("menuColumns");
		pGuiBoxStyle->SetSpacing(20);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_MENU_SECTIONLIST));
		pGuiBoxStyle->SetName("menuSectionList");
		pGuiBoxStyle->SetSpacing(20);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_MENU_SECTION));
		pGuiBoxStyle->SetName("menuSection");
		pGuiBoxStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_BACKGROUND));
		pGuiBoxStyle->SetSpacing(4);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_MENU_LIST));
		pGuiBoxStyle->SetName("menuList");
		pGuiBoxStyle->SetSpacing(4);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_MENU_MAINTABS_LAYOUT));
		pGuiBoxStyle->SetName("menuMainTabsLayout");
		pGuiBoxStyle->SetMargin(50);
		pGuiBoxStyle->SetSpacing(50);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_MENU_MAINTABS_BUTTONLIST));
		pGuiBoxStyle->SetName("menuMainTabsButtonList");
		pGuiBoxStyle->SetSpacing(10);
	}
	
	//GuiLabelStyles
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_TEXT));
		pGuiStyle->SetName("menuText");
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->SetFontSize(18);
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_HEADER));
		pGuiStyle->SetName("menuHeader");
		pGuiStyle->SetPadding(4);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->SetFontSize(18);
		pGuiStyle->m_TextAlignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_IDLE));
		pGuiStyle->SetName("menuButton");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_BACKGROUND));
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->SetMinHeight(24);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->SetFontSize(18);
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_MO));
		pGuiStyle->SetName("menuButtonMO");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_MOUSEOVER));
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->SetMinHeight(24);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->SetFontSize(18);
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_IDLE));
		pGuiStyle->SetName("menuMainTabsActiveButton");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_BACKGROUND));
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->SetMinHeight(24);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->SetFontSize(18);
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_MAINTABS_INACTIVEBUTTON));
		pGuiStyle->SetName("menuMainTabsInactiveButton");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_BACKGROUND));
		pGuiStyle->SetPadding(8);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->SetFontSize(22);
		pGuiStyle->m_TextAlignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_MAINTABS_INACTIVEBUTTON_MO));
		pGuiStyle->SetName("menuMainTabsInactiveButtonMO");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_MOUSEOVER));
		pGuiStyle->SetPadding(8);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->SetFontSize(22);
		pGuiStyle->m_TextAlignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_MAINTABS_ACTIVEBUTTON));
		pGuiStyle->SetName("menuMainTabsActiveButton");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_MENU_HIGHLIGHT));
		pGuiStyle->SetPadding(8);
		pGuiStyle->m_TextColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		pGuiStyle->SetFontSize(22);
		pGuiStyle->m_TextAlignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_CENTER;
	}
	
	//GuiButtonStyles
	{
		CAsset_GuiButtonStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiButtonStyle>(CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_MENU_DEFAULT));
		pGuiStyle->SetName("menuButton");
		pGuiStyle->SetIdleStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_IDLE));
		pGuiStyle->SetMouseOverStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_MO));
	}
	{
		CAsset_GuiButtonStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiButtonStyle>(CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_MENU_MAINTABS_INACTIVEBUTTON));
		pGuiStyle->SetName("menuMainTabsInactiveButton");
		pGuiStyle->SetIdleStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_MAINTABS_INACTIVEBUTTON));
		pGuiStyle->SetMouseOverStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_MAINTABS_INACTIVEBUTTON_MO));
	}
	{
		CAsset_GuiButtonStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiButtonStyle>(CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_MENU_MAINTABS_ACTIVEBUTTON));
		pGuiStyle->SetName("menuMainTabsActiveButton");
		pGuiStyle->SetIdleStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_MAINTABS_ACTIVEBUTTON));
		pGuiStyle->SetMouseOverStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_MAINTABS_ACTIVEBUTTON));
	}
	
	//GuiToggleStyles
	{
		CAsset_GuiToggleStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiToggleStyle>(CAssetPath::GuiToggleStyleSystem(tu::GUITOGGLESTYLE_MENU_CHECKBOX));
		pGuiStyle->SetName("menuCheckbox");
		pGuiStyle->SetIdleTrueStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_IDLE));
		pGuiStyle->SetIdleFalseStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_IDLE));
		pGuiStyle->SetMouseOverTrueStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_MO));
		pGuiStyle->SetMouseOverFalseStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_MENU_BUTTON_MO));
		pGuiStyle->SetSwitchIcon(true);
		pGuiStyle->SetIconTruePath(CAssetPath::SpriteSystem(tu::SPRITE_MENU_CHECKBOX_TRUE));
		pGuiStyle->SetIconFalsePath(CAssetPath::SpriteSystem(tu::SPRITE_MENU_CHECKBOX_FALSE));
	}
	
	//GuiTabsStyle
	{
		CAsset_GuiTabsStyle* pGuiTabsStyle = pAssetsManager->NewAsset<CAsset_GuiTabsStyle>(CAssetPath::GuiTabsStyleSystem(tu::GUITABSSTYLE_MENU_MAIN));
		pGuiTabsStyle->SetName("menuMainTabs");
		pGuiTabsStyle->m_LayoutPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_MENU_MAINTABS_LAYOUT);
		pGuiTabsStyle->m_ButtonListPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_MENU_MAINTABS_BUTTONLIST);
		pGuiTabsStyle->m_ActiveButtonPath = CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_MENU_MAINTABS_ACTIVEBUTTON);
		pGuiTabsStyle->m_InactiveButtonPath = CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_MENU_MAINTABS_INACTIVEBUTTON);
		pGuiTabsStyle->m_ButtonListFill = true;
		pGuiTabsStyle->m_ButtonListText = true;
	}
}

void CAssetsManager::InitAssetsManager_System_Editor(CAssetsManager* pAssetsManager)
{
	//Images
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SYSTEM, IStorage::TYPE_ALL, "tu/assetseditor.png", CAssetsHistory::NO_TOKEN);
		if(pImage)
			pImage->SetGridSize(32, 16);
	}
	
	CREATE_SPRITE(SPRITE_CURSOR, "cursor", IMAGE_ASSETSEDITOR, 30, 0, 2, 3);
	CREATE_SPRITE(SPRITE_CURSOR_TRANSLATEX, "cursorTranslateX", IMAGE_ASSETSEDITOR, 30, 3, 2, 2);
	CREATE_SPRITE(SPRITE_CURSOR_TRANSLATEY, "cursorTranslateY", IMAGE_ASSETSEDITOR, 30, 5, 2, 2);
	CREATE_SPRITE(SPRITE_CURSOR_TEXT, "cursorText", IMAGE_ASSETSEDITOR, 30, 7, 2, 2);
	
	CREATE_SPRITE(SPRITE_EDITOR_DEFAULT, "iconMenuDefault", IMAGE_ASSETSEDITOR, 1, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_DECREASE, "iconMenuDecrease", IMAGE_ASSETSEDITOR, 2, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_INCREASE, "iconMenuIncrease", IMAGE_ASSETSEDITOR, 3, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_DELETE, "iconMenuDelete", IMAGE_ASSETSEDITOR, 4, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_EDIT, "iconMenuEdit", IMAGE_ASSETSEDITOR, 9, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_VIEW, "iconMenuView", IMAGE_ASSETSEDITOR, 10, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_UP, "iconMenuUp", IMAGE_ASSETSEDITOR, 11, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_DOWN, "iconMenuDown", IMAGE_ASSETSEDITOR, 12, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_DUPLICATE, "iconMenuDuplicate", IMAGE_ASSETSEDITOR, 13, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SAVE, "iconMenuSave", IMAGE_ASSETSEDITOR, 14, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_LOAD, "iconMenuLoad", IMAGE_ASSETSEDITOR, 15, 0, 1, 1);
	
	CREATE_SPRITE(SPRITE_EDITOR_FIRST_FRAME, "iconMenuFirstFrame", IMAGE_ASSETSEDITOR, 16+0, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_PREV_FRAME, "iconMenuPrevFrame", IMAGE_ASSETSEDITOR, 16+1, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_PLAY, "iconMenuPlay", IMAGE_ASSETSEDITOR, 16+2, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_PAUSE, "iconMenuPause", IMAGE_ASSETSEDITOR, 16+3, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_NEXT_FRAME, "iconMenuNextFram", IMAGE_ASSETSEDITOR, 16+4, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_LAST_FRAME, "iconMenuLastFrame", IMAGE_ASSETSEDITOR, 16+5, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_VFLIP, "iconMenuVFlip", IMAGE_ASSETSEDITOR, 16+6, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_HFLIP, "iconMenuHFlip", IMAGE_ASSETSEDITOR, 16+7, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_ROTATE_CW, "iconMenuRotateCW", IMAGE_ASSETSEDITOR, 16+8, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_ROTATE_CCW, "iconMenuRotateCCW", IMAGE_ASSETSEDITOR, 16+9, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SELECTIONSTAMP, "iconMenuSelectionStamp", IMAGE_ASSETSEDITOR, 16+10, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SELECTIONFILL, "iconMenuSelectionFill", IMAGE_ASSETSEDITOR, 16+11, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GRID, "iconMenuGrid", IMAGE_ASSETSEDITOR, 16+12, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_NONE, "iconMenuNone", IMAGE_ASSETSEDITOR, 16+13, 0, 1, 1);
	
	CREATE_SPRITE(SPRITE_EDITOR_FOLDER, "iconMenuFolder", IMAGE_ASSETSEDITOR, 0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_TILES, "iconMenuTiles", IMAGE_ASSETSEDITOR, 1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_QUAD, "iconMenuQuads", IMAGE_ASSETSEDITOR, 2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_WEAPON, "iconMenuWeapon", IMAGE_ASSETSEDITOR, 3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_MAP, "iconMenuMap", IMAGE_ASSETSEDITOR, 4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_ZONETILES, "iconMenuZoneTile", IMAGE_ASSETSEDITOR, 5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_ZONETYPE, "iconMenuZoneType", IMAGE_ASSETSEDITOR, 6, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_ENTITYTYPE, "iconMenuEntityType", IMAGE_ASSETSEDITOR, 7, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_EDITOR_ASSET, "iconMenuAsset", IMAGE_ASSETSEDITOR, 8+0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_UNIVERSE, "iconMenuUniverse", IMAGE_ASSETSEDITOR, 8+1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_WORLD, "iconMenuWorld", IMAGE_ASSETSEDITOR, 8+2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_LAND, "iconMenuLand", IMAGE_ASSETSEDITOR, 8+3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SKIN, "iconMenuSkin", IMAGE_ASSETSEDITOR, 8+4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_LAYERS, "iconMenuLayer", IMAGE_ASSETSEDITOR, 8+5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_BONE, "iconMenuBone", IMAGE_ASSETSEDITOR, 8+6, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SPRITE, "iconMenuSprite", IMAGE_ASSETSEDITOR, 8+7, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_IMAGE, "iconMenuImage", IMAGE_ASSETSEDITOR, 8+8, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SKELETON, "iconMenuSkeleton", IMAGE_ASSETSEDITOR, 8+9, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SKELETONSKIN, "iconMenuSkeletonSkin", IMAGE_ASSETSEDITOR, 8+10, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SKELETONANIMATION, "iconMenuSkeletonAnimation", IMAGE_ASSETSEDITOR, 8+11, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_LAYERANIMATION, "iconMenuLayerAnimation", IMAGE_ASSETSEDITOR, 8+12, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_CHARACTER, "iconMenuCharacter", IMAGE_ASSETSEDITOR, 8+13, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_CHARACTERPART, "iconMenuCharacterPart", IMAGE_ASSETSEDITOR, 8+14, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_EDITOR_MOVE, "iconMenuMove", IMAGE_ASSETSEDITOR, 0, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_TRANSLATE, "iconMenuTranslate", IMAGE_ASSETSEDITOR, 1, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_TRANSLATE_X, "iconMenuTranslateX", IMAGE_ASSETSEDITOR, 2, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_TRANSLATE_Y, "iconMenuTranslateY", IMAGE_ASSETSEDITOR, 3, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_ROTATE, "iconMenuRotate", IMAGE_ASSETSEDITOR, 4, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SCALE, "iconMenuScale", IMAGE_ASSETSEDITOR, 5, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SCALE_X, "iconMenuScaleX", IMAGE_ASSETSEDITOR, 6, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SCALE_Y, "iconMenuScaleY", IMAGE_ASSETSEDITOR, 7, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_BONE_LENGTH, "iconMenuBoneLength", IMAGE_ASSETSEDITOR, 8, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_BONE_ADD, "iconMenuBoneAdd", IMAGE_ASSETSEDITOR, 9, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_BONE_DELETE, "iconMenuBoneDelete", IMAGE_ASSETSEDITOR, 10, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_BONE_ATTACH, "iconMenuAttach", IMAGE_ASSETSEDITOR, 11, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GUIRECT, "iconMenuGuiRect", IMAGE_ASSETSEDITOR, 12, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GUILINE, "iconMenuGuiLine", IMAGE_ASSETSEDITOR, 13, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GUIBOX, "iconMenuGuiBox", IMAGE_ASSETSEDITOR, 14, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GUISCROLLBAR, "iconMenuGuiScrollbar", IMAGE_ASSETSEDITOR, 15, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GUITABS, "iconMenuGuiTabs", IMAGE_ASSETSEDITOR, 16, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_SYSTEM, "iconMenuSystem", IMAGE_ASSETSEDITOR, 17, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GUIBUTTON, "iconMenuButton", IMAGE_ASSETSEDITOR, 18, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GUILABEL, "iconMenuLabel", IMAGE_ASSETSEDITOR, 19, 2, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_GUITOGGLE, "iconMenuToggle", IMAGE_ASSETSEDITOR, 20, 2, 1, 1);
	
	CREATE_SPRITE(SPRITE_EDITOR_FRAME_MOVE, "iconMenuKeyFrameMove", IMAGE_ASSETSEDITOR, 23+0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_FRAME_ADD, "iconMenuKeyFrameAdd", IMAGE_ASSETSEDITOR, 23+1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_FRAME_DELETE, "iconMenuKeyFrameDelete", IMAGE_ASSETSEDITOR, 23+2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_FRAMES, "iconMenuKeyFrames", IMAGE_ASSETSEDITOR, 23+3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_FRAME_COLOR, "iconMenuKeyFrameColor", IMAGE_ASSETSEDITOR, 23+4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_KEYFRAME_BONE, "iconMenuKeyFrameBone", IMAGE_ASSETSEDITOR, 23+5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_KEYFRAME_LAYER, "iconMenuKeyFrameLayer", IMAGE_ASSETSEDITOR, 23+6, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_EDITOR_COLORPICKER_RGB, "iconMenuColorPickerRGB", IMAGE_ASSETSEDITOR, 5, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_COLORPICKER_HSV, "iconMenuColorPickerHSV", IMAGE_ASSETSEDITOR, 6, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_COLORPICKER_SQUARE, "iconMenuColorPickerSquare", IMAGE_ASSETSEDITOR, 7, 0, 1, 1);
	CREATE_SPRITE(SPRITE_EDITOR_COLORPICKER_WHEEL, "iconMenuColorPickerWheel", IMAGE_ASSETSEDITOR, 8, 0, 1, 1);
	
	CREATE_SPRITE(SPRITE_GIZMOFRAME, "gizmoFrame", IMAGE_ASSETSEDITOR, 0, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_SELECT, "gizmoFrameSelect", IMAGE_ASSETSEDITOR, 2, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_ADD, "gizmoFrameAdd", IMAGE_ASSETSEDITOR, 4, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_DELETE, "gizmoFrameDelete", IMAGE_ASSETSEDITOR, 6, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_COLOR, "gizmoFrameColor", IMAGE_ASSETSEDITOR, 8, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_HIDDEN, "gizmoFrameHidden", IMAGE_ASSETSEDITOR, 10, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_COLORED_FG, "gizmoFrameColoredFg", IMAGE_ASSETSEDITOR, 12, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_COLORED_BG, "gizmoFrameColoredBg", IMAGE_ASSETSEDITOR, 14, 8, 2, 3);
	
	CREATE_SPRITE(SPRITE_GIZMO_VERTEX_BG, "gizmoVertexBg", IMAGE_ASSETSEDITOR, 0, 11, 1, 1);
	CREATE_SPRITE(SPRITE_GIZMO_VERTEX_FG, "gizmoVertexFg", IMAGE_ASSETSEDITOR, 1, 11, 1, 1);
	CREATE_SPRITE(SPRITE_GIZMO_PIVOT, "gizmoPivot", IMAGE_ASSETSEDITOR, 2, 11, 1, 1);
	CREATE_SPRITE(SPRITE_GIZMO_SCALE, "gizmoScale", IMAGE_ASSETSEDITOR, 3, 11, 1, 1);
	CREATE_SPRITE(SPRITE_GIZMO_ROTATE, "gizmoRotate", IMAGE_ASSETSEDITOR, 4, 11, 1, 1);
	
	CREATE_SPRITE(SPRITE_GIZMODIR_AIM, "gizmoDirAim", IMAGE_ASSETSEDITOR, 0, 12, 4, 4);
	CREATE_SPRITE(SPRITE_GIZMODIR_MOVE, "gizmoDirMove", IMAGE_ASSETSEDITOR, 4, 12, 4, 4);
	CREATE_SPRITE(SPRITE_GIZMODIR_HOOK, "gizmoDirHook", IMAGE_ASSETSEDITOR, 8, 12, 4, 4);
	
	//GuiRectStyles
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON));
		pGuiRectStyle->SetName("editorButton");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(130.0f/255.0f, 130.0f/255.0f, 130.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*9)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*6)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		pGuiRectStyle->SetName("editorButtonMO");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(172.0f/255.0f, 172.0f/255.0f, 172.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*6)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*3)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_HL));
		pGuiRectStyle->SetName("editorButtonHL");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(151.0f/255.0f, 194.0f/255.0f, 201.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*3)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*0)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY));
		pGuiRectStyle->SetName("editorEntry");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*15)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*12)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY_MO));
		pGuiRectStyle->SetName("editorEntryMO");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*12)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*9)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_POPUP));
		pGuiRectStyle->SetName("editorPopup");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(57.0f/255.0f, 57.0f/255.0f, 57.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_LAYOUT));
		pGuiRectStyle->SetName("editorTabsLayout");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(57.0f/255.0f, 57.0f/255.0f, 57.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_CONTENT));
		pGuiRectStyle->SetName("editorTabsContent");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND;
		pGuiRectStyle->m_BackgroundColor = vec4(83.0f/255.0f, 83.0f/255.0f, 83.0f/255.0f, 1.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_INACTIVEBUTTON));
		pGuiRectStyle->SetName("editorTabsInactiveButton");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND;
		pGuiRectStyle->m_BackgroundColor = vec4(57.0f/255.0f, 57.0f/255.0f, 57.0f/255.0f, 1.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_ACTIVEBUTTON));
		pGuiRectStyle->SetName("editorTabsActiveButton");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND;
		pGuiRectStyle->m_BackgroundColor = vec4(83.0f/255.0f, 83.0f/255.0f, 83.0f/255.0f, 1.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_VIEW));
		pGuiRectStyle->SetName("editorView");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(57.0f/255.0f, 57.0f/255.0f, 57.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	//~ {
		//~ CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_HL));
		//~ pGuiRectStyle->SetName("buttonDefaultHL");
		//~ pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		//~ pGuiRectStyle->m_BackgroundColor = vec4(151.0f/255.0f, 194.0f/255.0f, 201.0f/255.0f, 1.0f);
		//~ pGuiRectStyle->m_BackgroundPadding = 4.0f;
		//~ pGuiRectStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		//~ pGuiRectStyle->m_ImageUV_Min = vec2((512-16*3)/512.0f, (256-16*3)/256.0f);
		//~ pGuiRectStyle->m_ImageUV_Max = vec2((512-16*0)/512.0f, (256-16*0)/256.0f);
	//~ }
	//~ {
		//~ CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_BUTTON_LINK));
		//~ pGuiRectStyle->SetName("buttonLink");
		//~ pGuiRectStyle->m_Flags = 0;
	//~ }
	//~ {
		//~ CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_BUTTON_LINK_MOUSEOVER));
		//~ pGuiRectStyle->SetName("buttonLinkMO");
		//~ pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		//~ pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 0.2f);
		//~ pGuiRectStyle->m_CornerRadius = 3.0f;
		//~ pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	//~ }
	//~ {
		//~ CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_BUTTON_LINK_HIGHLIGHT));
		//~ pGuiRectStyle->SetName("buttonLinkHL");
		//~ pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		//~ pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 0.4f);
		//~ pGuiRectStyle->m_CornerRadius = 3.0f;
		//~ pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	//~ }
	//~ {
		//~ CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_LAYOUT_DEFAULT));
		//~ pGuiRectStyle->SetName("layoutDefault");
		//~ pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		//~ pGuiRectStyle->m_BackgroundColor = vec4(57.0f/255.0f, 57.0f/255.0f, 57.0f/255.0f, 1.0f);
		//~ pGuiRectStyle->m_BorderColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		//~ pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	//~ }
	//~ {
		//~ CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_LAYOUT_TOOLBAR));
		//~ pGuiRectStyle->SetName("layoutToolBar");
		//~ pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		//~ pGuiRectStyle->m_BackgroundColor = vec4(83.0f/255.0f, 83.0f/255.0f, 83.0f/255.0f, 1.0f);
		//~ pGuiRectStyle->m_BorderColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		//~ pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	//~ }
	//~ {
		//~ CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_LAYOUT_VIEW));
		//~ pGuiRectStyle->SetName("layoutToolBar");
		//~ pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		//~ pGuiRectStyle->m_BackgroundColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		//~ pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	//~ }
	
	//GuiLineStyles
	{
		CAsset_GuiLineStyle* pGuiLineStyle = pAssetsManager->NewAsset<CAsset_GuiLineStyle>(CAssetPath::GuiLineStyleSystem(tu::GUILINESTYLE_SEPARATOR));
		pGuiLineStyle->SetName("separator");
		pGuiLineStyle->m_Flags = CAsset_GuiLineStyle::FLAG_BORDER;
		pGuiLineStyle->m_BorderColor = vec4(83.0f/255.0f, 83.0f/255.0f, 83.0f/255.0f, 1.0f);
	}
	{
		CAsset_GuiLineStyle* pGuiLineStyle = pAssetsManager->NewAsset<CAsset_GuiLineStyle>(CAssetPath::GuiLineStyleSystem(tu::GUILINESTYLE_EDITOR_SCROLLBAR_RAIL));
		pGuiLineStyle->SetName("editorScrollbarRail");
		pGuiLineStyle->m_Flags = CAsset_GuiLineStyle::FLAG_IMAGE;
		pGuiLineStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiLineStyle->m_ImageUV_Min = vec2((512-16*3)/512.0f, (256-16*7)/256.0f);
		pGuiLineStyle->m_ImageUV_Max = vec2((512-16*0)/512.0f, (256-16*6)/256.0f);
	}
	{
		CAsset_GuiLineStyle* pGuiLineStyle = pAssetsManager->NewAsset<CAsset_GuiLineStyle>(CAssetPath::GuiLineStyleSystem(tu::GUILINESTYLE_EDITOR_SCROLLBAR_SLIDER));
		pGuiLineStyle->SetName("editorScrollbarSlider");
		pGuiLineStyle->m_Flags = CAsset_GuiLineStyle::FLAG_IMAGE;
		pGuiLineStyle->m_ImagePath = CAssetPath::System(CAssetPath::TYPE_IMAGE, IMAGE_ASSETSEDITOR);
		pGuiLineStyle->m_ImageUV_Min = vec2((512-16*6)/512.0f, (256-16*7)/256.0f);
		pGuiLineStyle->m_ImageUV_Max = vec2((512-16*3)/512.0f, (256-16*6)/256.0f);
	}
	
	//GuiBoxStyles
	//~ {
		//~ CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_BUTTON_HL));
		//~ pGuiBoxStyle->SetName("editorButtonHL");
		//~ pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_HL));
		//~ pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		//~ pGuiBoxStyle->SetMargin(2);
		//~ pGuiBoxStyle->SetPadding(2);
		//~ pGuiBoxStyle->SetSpacing(4);
		//~ pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//~ pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_CENTER;
	//~ }
	//~ {
		//~ CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TEXTENTRY));
		//~ pGuiBoxStyle->SetName("editorTextEntry");
		//~ pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY));
		//~ pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY_MO));
		//~ pGuiBoxStyle->SetMargin(2);
		//~ pGuiBoxStyle->SetPadding(4);
		//~ pGuiBoxStyle->SetSpacing(4);
		//~ pGuiBoxStyle->SetMinHeight(14);
		//~ pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//~ }
	//~ {
		//~ CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_NUMERICENTRY));
		//~ pGuiBoxStyle->SetName("editorNumericEntry");
		//~ pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY));
		//~ pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY_MO));
		//~ pGuiBoxStyle->SetMargin(2);
		//~ pGuiBoxStyle->SetPadding(4);
		//~ pGuiBoxStyle->SetSpacing(4);
		//~ pGuiBoxStyle->SetMinHeight(14);
		//~ pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//~ pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_CENTER;
	//~ }
	//~ {
		//~ CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_POPUP));
		//~ pGuiBoxStyle->SetName("editorPopup");
		//~ pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_POPUP));
		//~ pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_POPUP));
		//~ pGuiBoxStyle->SetPadding(2);
		//~ pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//~ pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_CENTER;
	//~ }
	//~ {
		//~ CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_ASSETEDITBUTTON));
		//~ pGuiBoxStyle->SetName("editorAssetEditButton");
		//~ pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON));
		//~ pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		//~ pGuiBoxStyle->SetMargin(2);
		//~ pGuiBoxStyle->SetPadding(2);
		//~ pGuiBoxStyle->SetSpacing(4);
		//~ pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//~ }
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_LAYOUT));
		pGuiBoxStyle->SetName("editorTabsLayout");
		pGuiBoxStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_LAYOUT));
		pGuiBoxStyle->SetPadding(1);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_CONTENT));
		pGuiBoxStyle->SetName("editorTabsContent");
		pGuiBoxStyle->GetRectPath() = CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_CONTENT);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_BUTTONLIST));
		pGuiBoxStyle->SetName("editorTabsButtonList");
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_VIEW));
		pGuiBoxStyle->SetName("editorView");
		pGuiBoxStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_VIEW));
		pGuiBoxStyle->SetPadding(1);
	}
	
	//GuiLabelStyles
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_TEXT));
		pGuiStyle->SetName("editorText");
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->SetFontSize(11);
		pGuiStyle->m_TextColor = 1.0f;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_HEADER));
		pGuiStyle->SetName("editorHeader");
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->SetFontSize(16);
		pGuiStyle->m_TextColor = vec4(1.0f, 1.0f, 0.5f, 1.0f);
		pGuiStyle->m_TextAlignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_BUTTON_IDLE));
		pGuiStyle->SetName("editorButtonIdle");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON));
		pGuiStyle->SetMargin(2);
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->SetFontSize(11);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->m_TextAlignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_BUTTON_MO));
		pGuiStyle->SetName("editorButtonMO");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		pGuiStyle->SetMargin(2);
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->SetFontSize(11);
		pGuiStyle->m_TextColor = 1.0f;
		pGuiStyle->m_TextAlignment = CAsset_GuiLabelStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_ACTIVETAB));
		pGuiStyle->SetName("editorActiveTab");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_ACTIVEBUTTON));
		pGuiStyle->SetPadding(4);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->m_TextColor = 1.0f;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_INACTIVETAB));
		pGuiStyle->SetName("editorInactiveTab");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_INACTIVEBUTTON));
		pGuiStyle->SetPadding(4);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->m_TextColor = 1.0f;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_LISTITEM));
		pGuiStyle->SetName("editorListItem");
		pGuiStyle->SetMargin(2);
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->m_TextColor = 1.0f;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_LISTITEM_MO));
		pGuiStyle->SetName("editorListItemMO");
		pGuiStyle->SetRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		pGuiStyle->SetMargin(2);
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->m_TextColor = 1.0f;
	}
	{
		CAsset_GuiLabelStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiLabelStyle>(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_INACTIVELISTITEM));
		pGuiStyle->SetName("editorInactiveListItem");
		pGuiStyle->SetMargin(2);
		pGuiStyle->SetPadding(2);
		pGuiStyle->SetSpacing(4);
		pGuiStyle->m_TextColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	}
	
	//GuiButtonStyles
	{
		CAsset_GuiButtonStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiButtonStyle>(CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_EDITOR_DEFAULT));
		pGuiStyle->SetName("editorButton");
		pGuiStyle->SetIdleStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_BUTTON_IDLE));
		pGuiStyle->SetMouseOverStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_BUTTON_MO));
	}
	{
		CAsset_GuiButtonStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiButtonStyle>(CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_EDITOR_ACTIVETAB));
		pGuiStyle->SetName("editorActiveTab");
		pGuiStyle->SetIdleStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_ACTIVETAB));
		pGuiStyle->SetMouseOverStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_ACTIVETAB));
	}
	{
		CAsset_GuiButtonStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiButtonStyle>(CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_EDITOR_INACTIVETAB));
		pGuiStyle->SetName("editorInactiveTab");
		pGuiStyle->SetIdleStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_INACTIVETAB));
		pGuiStyle->SetMouseOverStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_INACTIVETAB));
	}
	{
		CAsset_GuiButtonStyle* pGuiStyle = pAssetsManager->NewAsset<CAsset_GuiButtonStyle>(CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_EDITOR_LISTITEM));
		pGuiStyle->SetName("editorListItem");
		pGuiStyle->SetIdleStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_LISTITEM));
		pGuiStyle->SetMouseOverStylePath(CAssetPath::GuiLabelStyleSystem(tu::GUILABELSTYLE_EDITOR_LISTITEM_MO));
	}
	
	//GuiScrollbarStyles
	{
		CAsset_GuiScrollbarStyle* pGuiScrollbarStyle = pAssetsManager->NewAsset<CAsset_GuiScrollbarStyle>(CAssetPath::GuiScrollbarStyleSystem(tu::GUISCROLLBARSTYLE_EDITOR));
		pGuiScrollbarStyle->SetName("editorScrollbar");
		pGuiScrollbarStyle->m_DefaultRailPath = CAssetPath::GuiLineStyleSystem(tu::GUILINESTYLE_EDITOR_SCROLLBAR_RAIL);
		pGuiScrollbarStyle->m_DefaultSliderPath = CAssetPath::GuiLineStyleSystem(tu::GUILINESTYLE_EDITOR_SCROLLBAR_SLIDER);
		pGuiScrollbarStyle->m_MouseOverSliderPath = CAssetPath::GuiLineStyleSystem(tu::GUILINESTYLE_EDITOR_SCROLLBAR_SLIDER);
		pGuiScrollbarStyle->m_RectPath = CAssetPath::Null();
		pGuiScrollbarStyle->m_Margin = 0;
		pGuiScrollbarStyle->m_Padding = 12;
	}
	
	//GuiTabsStyles
	{
		CAsset_GuiTabsStyle* pGuiTabsStyle = pAssetsManager->NewAsset<CAsset_GuiTabsStyle>(CAssetPath::GuiTabsStyleSystem(tu::GUITABSSTYLE_EDITOR));
		pGuiTabsStyle->SetName("editorTabs");
		pGuiTabsStyle->m_LayoutPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_LAYOUT);
		pGuiTabsStyle->m_ContentPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_CONTENT);
		pGuiTabsStyle->m_ButtonListPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_BUTTONLIST);
		pGuiTabsStyle->m_InactiveButtonPath = CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_EDITOR_INACTIVETAB);
		pGuiTabsStyle->m_ActiveButtonPath = CAssetPath::GuiButtonStyleSystem(tu::GUIBUTTONSTYLE_EDITOR_ACTIVETAB);
		pGuiTabsStyle->m_ButtonListFill = false;
		pGuiTabsStyle->m_ButtonListText = false;
	}
}

}
