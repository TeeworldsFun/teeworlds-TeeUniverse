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

void CAssetsManager::InitAssetsManager_System(CAssetsManager* pAssetsManager)
{
	//Images
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SYSTEM, IStorage::TYPE_ALL, "tu/assetseditor.png", CAssetsHistory::NO_TOKEN);
		if(pImage)
			pImage->SetGridSize(32, 16);
	}
	
	#define CREATE_SPRITE(id, name, image, x, y, w, h) {\
		CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(CAssetPath::SpriteSystem(id));\
		pSprite->SetName(name);\
		pSprite->Init(CAssetPath::ImageSystem(image), x, y, w, h);\
	}
	
	CREATE_SPRITE(SPRITE_CURSOR, "cursor", IMAGE_ASSETSEDITOR, 30, 0, 2, 3);
	CREATE_SPRITE(SPRITE_CURSOR_TRANSLATEX, "cursorTranslateX", IMAGE_ASSETSEDITOR, 30, 3, 2, 2);
	CREATE_SPRITE(SPRITE_CURSOR_TRANSLATEY, "cursorTranslateY", IMAGE_ASSETSEDITOR, 30, 5, 2, 2);
	CREATE_SPRITE(SPRITE_CURSOR_TEXT, "cursorText", IMAGE_ASSETSEDITOR, 30, 7, 2, 2);
	
	CREATE_SPRITE(SPRITE_ICON_DEFAULT, "iconDefault", IMAGE_ASSETSEDITOR, 1, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_DECREASE, "iconDecrease", IMAGE_ASSETSEDITOR, 2, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_INCREASE, "iconIncrease", IMAGE_ASSETSEDITOR, 3, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_DELETE, "iconDelete", IMAGE_ASSETSEDITOR, 4, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_EDIT, "iconEdit", IMAGE_ASSETSEDITOR, 9, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_VIEW, "iconView", IMAGE_ASSETSEDITOR, 10, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_UP, "iconUp", IMAGE_ASSETSEDITOR, 11, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_DOWN, "iconDown", IMAGE_ASSETSEDITOR, 12, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_DUPLICATE, "iconDuplicate", IMAGE_ASSETSEDITOR, 13, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SAVE, "iconSave", IMAGE_ASSETSEDITOR, 14, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LOAD, "iconLoad", IMAGE_ASSETSEDITOR, 15, 0, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_FIRST_FRAME, "iconFirstFrame", IMAGE_ASSETSEDITOR, 16+0, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_PREV_FRAME, "iconPrevFrame", IMAGE_ASSETSEDITOR, 16+1, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_PLAY, "iconPlay", IMAGE_ASSETSEDITOR, 16+2, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_PAUSE, "iconPause", IMAGE_ASSETSEDITOR, 16+3, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_NEXT_FRAME, "iconNextFram", IMAGE_ASSETSEDITOR, 16+4, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LAST_FRAME, "iconLastFrame", IMAGE_ASSETSEDITOR, 16+5, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_VFLIP, "iconVFlip", IMAGE_ASSETSEDITOR, 16+6, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_HFLIP, "iconHFlip", IMAGE_ASSETSEDITOR, 16+7, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ROTATE_CW, "iconRotateCW", IMAGE_ASSETSEDITOR, 16+8, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ROTATE_CCW, "iconRotateCCW", IMAGE_ASSETSEDITOR, 16+9, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SELECTIONSTAMP, "iconSelectionStamp", IMAGE_ASSETSEDITOR, 16+10, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SELECTIONFILL, "iconSelectionFill", IMAGE_ASSETSEDITOR, 16+11, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_GRID, "iconGrid", IMAGE_ASSETSEDITOR, 16+12, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_NONE, "iconNone", IMAGE_ASSETSEDITOR, 16+13, 0, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_FOLDER, "iconFolder", IMAGE_ASSETSEDITOR, 0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_TILES, "iconTiles", IMAGE_ASSETSEDITOR, 1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_QUAD, "iconQuads", IMAGE_ASSETSEDITOR, 2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_WEAPON, "iconWeapon", IMAGE_ASSETSEDITOR, 3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_MAP, "iconMap", IMAGE_ASSETSEDITOR, 4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ZONETILES, "iconZoneTile", IMAGE_ASSETSEDITOR, 5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ZONETYPE, "iconZoneType", IMAGE_ASSETSEDITOR, 6, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ENTITYTYPE, "iconEntityType", IMAGE_ASSETSEDITOR, 7, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_ASSET, "iconAsset", IMAGE_ASSETSEDITOR, 8+0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_UNIVERSE, "iconUniverse", IMAGE_ASSETSEDITOR, 8+1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_WORLD, "iconWorld", IMAGE_ASSETSEDITOR, 8+2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LAND, "iconLand", IMAGE_ASSETSEDITOR, 8+3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SKIN, "iconSkin", IMAGE_ASSETSEDITOR, 8+4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LAYERS, "iconLayer", IMAGE_ASSETSEDITOR, 8+5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE, "iconBone", IMAGE_ASSETSEDITOR, 8+6, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SPRITE, "iconSprite", IMAGE_ASSETSEDITOR, 8+7, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_IMAGE, "iconImage", IMAGE_ASSETSEDITOR, 8+8, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SKELETON, "iconSkeleton", IMAGE_ASSETSEDITOR, 8+9, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SKELETONSKIN, "iconSkeletonSkin", IMAGE_ASSETSEDITOR, 8+10, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SKELETONANIMATION, "iconSkeletonAnimation", IMAGE_ASSETSEDITOR, 8+11, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LAYERANIMATION, "iconLayerAnimation", IMAGE_ASSETSEDITOR, 8+12, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_CHARACTER, "iconCharacter", IMAGE_ASSETSEDITOR, 8+13, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_CHARACTERPART, "iconCharacterPart", IMAGE_ASSETSEDITOR, 8+14, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_MOVE, "iconMove", IMAGE_ASSETSEDITOR, 0, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_TRANSLATE, "iconTranslate", IMAGE_ASSETSEDITOR, 1, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_TRANSLATE_X, "iconTranslateX", IMAGE_ASSETSEDITOR, 2, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_TRANSLATE_Y, "iconTranslateY", IMAGE_ASSETSEDITOR, 3, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ROTATE, "iconRotate", IMAGE_ASSETSEDITOR, 4, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SCALE, "iconScale", IMAGE_ASSETSEDITOR, 5, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SCALE_X, "iconScaleX", IMAGE_ASSETSEDITOR, 6, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SCALE_Y, "iconScaleY", IMAGE_ASSETSEDITOR, 7, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE_LENGTH, "iconBoneLength", IMAGE_ASSETSEDITOR, 8, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE_ADD, "iconBoneAdd", IMAGE_ASSETSEDITOR, 9, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE_DELETE, "iconBoneDelete", IMAGE_ASSETSEDITOR, 10, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE_ATTACH, "iconAttach", IMAGE_ASSETSEDITOR, 11, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_GUIRECT, "iconGuiRect", IMAGE_ASSETSEDITOR, 12, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_GUILINE, "iconGuiLine", IMAGE_ASSETSEDITOR, 13, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_GUIBOX, "iconGuiBox", IMAGE_ASSETSEDITOR, 14, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_GUISCROLLBAR, "iconGuiScrollbar", IMAGE_ASSETSEDITOR, 15, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_GUITABS, "iconGuiTabs", IMAGE_ASSETSEDITOR, 16, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SYSTEM, "iconSystem", IMAGE_ASSETSEDITOR, 17, 2, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_FRAME_MOVE, "iconKeyFrameMove", IMAGE_ASSETSEDITOR, 23+0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_FRAME_ADD, "iconKeyFrameAdd", IMAGE_ASSETSEDITOR, 23+1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_FRAME_DELETE, "iconKeyFrameDelete", IMAGE_ASSETSEDITOR, 23+2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_FRAMES, "iconKeyFrames", IMAGE_ASSETSEDITOR, 23+3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_FRAME_COLOR, "iconKeyFrameColor", IMAGE_ASSETSEDITOR, 23+4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_KEYFRAME_BONE, "iconKeyFrameBone", IMAGE_ASSETSEDITOR, 23+5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_KEYFRAME_LAYER, "iconKeyFrameLayer", IMAGE_ASSETSEDITOR, 23+6, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_COLORPICKER_RGB, "iconColorPickerRGB", IMAGE_ASSETSEDITOR, 5, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_COLORPICKER_HSV, "iconColorPickerHSV", IMAGE_ASSETSEDITOR, 6, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_COLORPICKER_SQUARE, "iconColorPickerSquare", IMAGE_ASSETSEDITOR, 7, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_COLORPICKER_WHEEL, "iconColorPickerWheel", IMAGE_ASSETSEDITOR, 8, 0, 1, 1);
	
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
	dbg_msg("assetsmanager", "Sprites initialised");
	
	//GuiRectStyles
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_DEBUG));
		pGuiRectStyle->SetName("debug");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BorderColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_DEBUG_MOUSEOVER));
		pGuiRectStyle->SetName("debugMouseOver");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BorderColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
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
	dbg_msg("assetsmanager", "GuiRectStyles initialised");
	
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
	dbg_msg("assetsmanager", "GuiLineStyles initialised");
	
	//GuiBoxStyles
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_DEBUG));
		pGuiBoxStyle->SetName("debug");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_DEBUG));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_DEBUG_MOUSEOVER));
		pGuiBoxStyle->SetMargin(2);
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_COMPOSING));
		pGuiBoxStyle->SetName("composing");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_COMPOSING));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_COMPOSING));
		pGuiBoxStyle->SetPadding(4);
		pGuiBoxStyle->m_TextColor = vec4(15.0f/255.0f, 103.0f/255.0f, 146.0f/255.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_LABEL));
		pGuiBoxStyle->SetName("editorLabelDefault");
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_LABELHEADER));
		pGuiBoxStyle->SetName("editorLabelHeader");
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->SetFontSize(16);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 0.5f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_BUTTON));
		pGuiBoxStyle->SetName("editorButton");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		pGuiBoxStyle->SetMargin(2);
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_BUTTON_HL));
		pGuiBoxStyle->SetName("editorButtonHL");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_HL));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		pGuiBoxStyle->SetMargin(2);
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TEXTENTRY));
		pGuiBoxStyle->SetName("editorTextEntry");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY_MO));
		pGuiBoxStyle->SetMargin(2);
		pGuiBoxStyle->SetPadding(4);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->SetMinHeight(14);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_NUMERICENTRY));
		pGuiBoxStyle->SetName("editorNumericEntry");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_ENTRY_MO));
		pGuiBoxStyle->SetMargin(2);
		pGuiBoxStyle->SetPadding(4);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->SetMinHeight(14);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_POPUP));
		pGuiBoxStyle->SetName("editorPopup");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_POPUP));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_POPUP));
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_CENTER;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_ASSETEDITBUTTON));
		pGuiBoxStyle->SetName("editorAssetEditButton");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		pGuiBoxStyle->SetMargin(2);
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_LISTITEM));
		pGuiBoxStyle->SetName("editorListItem");
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_BUTTON_MO));
		pGuiBoxStyle->SetMargin(2);
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_INACTIVELISTITEM));
		pGuiBoxStyle->SetName("editorInactiveListItem");
		pGuiBoxStyle->SetMargin(2);
		pGuiBoxStyle->SetPadding(2);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_LAYOUT));
		pGuiBoxStyle->SetName("editorTabsLayout");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_LAYOUT));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_LAYOUT));
		pGuiBoxStyle->SetPadding(1);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_CONTENT));
		pGuiBoxStyle->SetName("editorTabsContent");
		pGuiBoxStyle->GetDefaultRectPath() = CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_CONTENT);
		pGuiBoxStyle->GetMouseOverRectPath() = CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_CONTENT);
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_BUTTONLIST));
		pGuiBoxStyle->SetName("editorTabsButtonList");
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_INACTIVEBUTTON));
		pGuiBoxStyle->SetName("editorTabsInactiveButton");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_INACTIVEBUTTON));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_INACTIVEBUTTON));
		pGuiBoxStyle->SetPadding(4);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_ACTIVEBUTTON));
		pGuiBoxStyle->SetName("editorTabsActiveButton");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_ACTIVEBUTTON));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_TABS_ACTIVEBUTTON));
		pGuiBoxStyle->SetPadding(4);
		pGuiBoxStyle->SetSpacing(4);
		pGuiBoxStyle->m_TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pGuiBoxStyle->m_TextAlignment = CAsset_GuiBoxStyle::TEXTALIGNMENT_LEFT;
	}
	{
		CAsset_GuiBoxStyle* pGuiBoxStyle = pAssetsManager->NewAsset<CAsset_GuiBoxStyle>(CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_VIEW));
		pGuiBoxStyle->SetName("editorView");
		pGuiBoxStyle->SetDefaultRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_VIEW));
		pGuiBoxStyle->SetMouseOverRectPath(CAssetPath::GuiRectStyleSystem(tu::GUIRECTSTYLE_EDITOR_VIEW));
		pGuiBoxStyle->SetPadding(1);
	}
	dbg_msg("assetsmanager", "GuiBoxStyles initialised (%d assets)", pAssetsManager->GetNumAssets<CAsset_GuiBoxStyle>(CAssetPath::SRC_SYSTEM));
	
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
	dbg_msg("assetsmanager", "GuiScrollbarStyles initialised (%d assets)", pAssetsManager->GetNumAssets<CAsset_GuiScrollbarStyle>(CAssetPath::SRC_SYSTEM));
	
	//GuiTabsStyles
	{
		CAsset_GuiTabsStyle* pGuiTabsStyle = pAssetsManager->NewAsset<CAsset_GuiTabsStyle>(CAssetPath::GuiTabsStyleSystem(tu::GUITABSSTYLE_EDITOR));
		pGuiTabsStyle->SetName("editorTabs");
		pGuiTabsStyle->m_LayoutPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_LAYOUT);
		pGuiTabsStyle->m_ContentPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_CONTENT);
		pGuiTabsStyle->m_ButtonListPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_BUTTONLIST);
		pGuiTabsStyle->m_InactiveButtonPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_INACTIVEBUTTON);
		pGuiTabsStyle->m_ActiveButtonPath = CAssetPath::GuiBoxStyleSystem(tu::GUIBOXSTYLE_EDITOR_TABS_ACTIVEBUTTON);
		pGuiTabsStyle->m_ButtonListFill = false;
		pGuiTabsStyle->m_ButtonListText = false;
	}
	dbg_msg("assetsmanager", "GuiTabsStyles initialised (%d assets)", pAssetsManager->GetNumAssets<CAsset_GuiTabsStyle>(CAssetPath::SRC_SYSTEM));
}

}
