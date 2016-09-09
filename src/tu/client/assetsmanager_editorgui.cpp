#include <engine/storage.h>
#include <engine/shared/config.h>
#include <engine/shared/datafile.h>
#include <generated/client_data.h>
#include <game/mapitems.h>
#include <game/gamecore.h>
#include <game/client/render.h>
#include <tu/shared/assetsfile.h>

#include <engine/external/pnglite/pnglite.h>

#include <tu/client/assetsmanager.h>

namespace tu
{

void CAssetsManager::InitAssetsManager_AssetsEditorGUI(CAssetsManager* pAssetsManager)
{
	//Images
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_UNIVERSE, IStorage::TYPE_ALL, "tu/assetseditor.png", CAssetsHistory::NO_TOKEN);
		if(pImage)
			pImage->SetGridSize(32, 16);
	}
	
	#define CREATE_SPRITE(id, name, image, x, y, w, h) {\
		CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(CAssetPath::SpriteUniverse(id));\
		pSprite->SetName(name);\
		pSprite->Init(CAssetPath::Universe(CAssetPath::TYPE_IMAGE, image), x, y, w, h);\
	}
	
	CREATE_SPRITE(SPRITE_ICON_DEFAULT, "iconDefault", IMAGE_ICONGIZMO, 1, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_DECREASE, "iconDecrease", IMAGE_ICONGIZMO, 2, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_INCREASE, "iconIncrease", IMAGE_ICONGIZMO, 3, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_DELETE, "iconDelete", IMAGE_ICONGIZMO, 4, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_EDIT, "iconEdit", IMAGE_ICONGIZMO, 9, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_VIEW, "iconView", IMAGE_ICONGIZMO, 10, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_UP, "iconUp", IMAGE_ICONGIZMO, 11, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_DOWN, "iconDown", IMAGE_ICONGIZMO, 12, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_DUPLICATE, "iconDuplicate", IMAGE_ICONGIZMO, 13, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SAVE, "iconSave", IMAGE_ICONGIZMO, 14, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LOAD, "iconLoad", IMAGE_ICONGIZMO, 15, 0, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_FIRST_FRAME, "iconFirstFrame", IMAGE_ICONGIZMO, 16+0, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_PREV_FRAME, "iconPrevFrame", IMAGE_ICONGIZMO, 16+1, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_PLAY, "iconPlay", IMAGE_ICONGIZMO, 16+2, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_PAUSE, "iconPause", IMAGE_ICONGIZMO, 16+3, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_NEXT_FRAME, "iconNextFram", IMAGE_ICONGIZMO, 16+4, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LAST_FRAME, "iconLastFrame", IMAGE_ICONGIZMO, 16+5, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_VFLIP, "iconVFlip", IMAGE_ICONGIZMO, 16+6, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_HFLIP, "iconHFlip", IMAGE_ICONGIZMO, 16+7, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ROTATE_CW, "iconRotateCW", IMAGE_ICONGIZMO, 16+8, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ROTATE_CCW, "iconRotateCCW", IMAGE_ICONGIZMO, 16+9, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SELECTIONSTAMP, "iconSelectionStamp", IMAGE_ICONGIZMO, 16+10, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SELECTIONFILL, "iconSelectionFill", IMAGE_ICONGIZMO, 16+11, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_GRID, "iconGrid", IMAGE_ICONGIZMO, 16+12, 0, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_FOLDER, "iconFolder", IMAGE_ICONGIZMO, 0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_TILES, "iconTiles", IMAGE_ICONGIZMO, 1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_QUAD, "iconQuads", IMAGE_ICONGIZMO, 2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_WEAPON, "iconWeapon", IMAGE_ICONGIZMO, 3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_MAP, "iconMap", IMAGE_ICONGIZMO, 4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ZONETILES, "iconZoneTile", IMAGE_ICONGIZMO, 5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ZONETYPE, "iconZoneType", IMAGE_ICONGIZMO, 6, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ENTITYTYPE, "iconEntityType", IMAGE_ICONGIZMO, 7, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_ASSET, "iconAsset", IMAGE_ICONGIZMO, 8+0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_UNIVERSE, "iconUniverse", IMAGE_ICONGIZMO, 8+1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_WORLD, "iconWorld", IMAGE_ICONGIZMO, 8+2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LAND, "iconLand", IMAGE_ICONGIZMO, 8+3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SKIN, "iconSkin", IMAGE_ICONGIZMO, 8+4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LAYERS, "iconLayer", IMAGE_ICONGIZMO, 8+5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE, "iconBone", IMAGE_ICONGIZMO, 8+6, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SPRITE, "iconSprite", IMAGE_ICONGIZMO, 8+7, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_IMAGE, "iconImage", IMAGE_ICONGIZMO, 8+8, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SKELETON, "iconSkeleton", IMAGE_ICONGIZMO, 8+9, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SKELETONSKIN, "iconSkeletonSkin", IMAGE_ICONGIZMO, 8+10, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SKELETONANIMATION, "iconSkeletonAnimation", IMAGE_ICONGIZMO, 8+11, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_LAYERANIMATION, "iconLayerAnimation", IMAGE_ICONGIZMO, 8+12, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_CHARACTER, "iconCharacter", IMAGE_ICONGIZMO, 8+13, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_CHARACTERPART, "iconCharacterPart", IMAGE_ICONGIZMO, 8+14, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_MOVE, "iconMove", IMAGE_ICONGIZMO, 0, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_TRANSLATE, "iconTranslate", IMAGE_ICONGIZMO, 1, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_TRANSLATE_X, "iconTranslateX", IMAGE_ICONGIZMO, 2, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_TRANSLATE_Y, "iconTranslateY", IMAGE_ICONGIZMO, 3, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_ROTATE, "iconRotate", IMAGE_ICONGIZMO, 4, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SCALE, "iconScale", IMAGE_ICONGIZMO, 5, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SCALE_X, "iconScaleX", IMAGE_ICONGIZMO, 6, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_SCALE_Y, "iconScaleY", IMAGE_ICONGIZMO, 7, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE_LENGTH, "iconBoneLength", IMAGE_ICONGIZMO, 8, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE_ADD, "iconBoneAdd", IMAGE_ICONGIZMO, 9, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE_DELETE, "iconBoneDelete", IMAGE_ICONGIZMO, 10, 2, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_BONE_ATTACH, "iconAttach", IMAGE_ICONGIZMO, 11, 2, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_FRAME_MOVE, "iconKeyFrameMove", IMAGE_ICONGIZMO, 23+0, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_FRAME_ADD, "iconKeyFrameAdd", IMAGE_ICONGIZMO, 23+1, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_FRAME_DELETE, "iconKeyFrameDelete", IMAGE_ICONGIZMO, 23+2, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_FRAMES, "iconKeyFrames", IMAGE_ICONGIZMO, 23+3, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_FRAME_COLOR, "iconKeyFrameColor", IMAGE_ICONGIZMO, 23+4, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_KEYFRAME_BONE, "iconKeyFrameBone", IMAGE_ICONGIZMO, 23+5, 1, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_KEYFRAME_LAYER, "iconKeyFrameLayer", IMAGE_ICONGIZMO, 23+6, 1, 1, 1);
	
	CREATE_SPRITE(SPRITE_ICON_COLORPICKER_RGB, "iconColorPickerRGB", IMAGE_ICONGIZMO, 5, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_COLORPICKER_HSV, "iconColorPickerHSV", IMAGE_ICONGIZMO, 6, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_COLORPICKER_SQUARE, "iconColorPickerSquare", IMAGE_ICONGIZMO, 7, 0, 1, 1);
	CREATE_SPRITE(SPRITE_ICON_COLORPICKER_WHEEL, "iconColorPickerWheel", IMAGE_ICONGIZMO, 8, 0, 1, 1);
	
	CREATE_SPRITE(SPRITE_GIZMOFRAME, "gizmoFrame", IMAGE_ICONGIZMO, 0, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_SELECT, "gizmoFrameSelect", IMAGE_ICONGIZMO, 2, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_ADD, "gizmoFrameAdd", IMAGE_ICONGIZMO, 4, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_DELETE, "gizmoFrameDelete", IMAGE_ICONGIZMO, 6, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_COLOR, "gizmoFrameColor", IMAGE_ICONGIZMO, 8, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_HIDDEN, "gizmoFrameHidden", IMAGE_ICONGIZMO, 10, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_COLORED_FG, "gizmoFrameColoredFg", IMAGE_ICONGIZMO, 12, 8, 2, 3);
	CREATE_SPRITE(SPRITE_GIZMOFRAME_COLORED_BG, "gizmoFrameColoredBg", IMAGE_ICONGIZMO, 14, 8, 2, 3);
	
	CREATE_SPRITE(SPRITE_GIZMO_VERTEX_BG, "gizmoVertexBg", IMAGE_ICONGIZMO, 0, 11, 1, 1);
	CREATE_SPRITE(SPRITE_GIZMO_VERTEX_FG, "gizmoVertexFg", IMAGE_ICONGIZMO, 1, 11, 1, 1);
	CREATE_SPRITE(SPRITE_GIZMO_PIVOT, "gizmoPivot", IMAGE_ICONGIZMO, 2, 11, 1, 1);
	CREATE_SPRITE(SPRITE_GIZMO_SCALE, "gizmoScale", IMAGE_ICONGIZMO, 3, 11, 1, 1);
	CREATE_SPRITE(SPRITE_GIZMO_ROTATE, "gizmoRotate", IMAGE_ICONGIZMO, 4, 11, 1, 1);
	
	CREATE_SPRITE(SPRITE_GIZMODIR_AIM, "gizmoDirAim", IMAGE_ICONGIZMO, 0, 12, 4, 4);
	CREATE_SPRITE(SPRITE_GIZMODIR_MOVE, "gizmoDirMove", IMAGE_ICONGIZMO, 4, 12, 4, 4);
	CREATE_SPRITE(SPRITE_GIZMODIR_HOOK, "gizmoDirHook", IMAGE_ICONGIZMO, 8, 12, 4, 4);
	dbg_msg("assetsmanager", "Sprites initialised");
	
	//GuiRectStyles
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_SELECTION));
		pGuiRectStyle->SetName("selection");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_ImagePath = CAssetPath::Universe(CAssetPath::TYPE_IMAGE, IMAGE_ICONGIZMO);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*3)/512.0f, (256-16*6)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*0)/512.0f, (256-16*3)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_STAMP));
		pGuiRectStyle->SetName("stamp");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_ImagePath = CAssetPath::Universe(CAssetPath::TYPE_IMAGE, IMAGE_ICONGIZMO);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*6)/512.0f, (256-16*6)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*3)/512.0f, (256-16*3)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_DEFAULT));
		pGuiRectStyle->SetName("buttonDefault");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(130.0f/255.0f, 130.0f/255.0f, 130.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::Universe(CAssetPath::TYPE_IMAGE, IMAGE_ICONGIZMO);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*9)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*6)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_DEFAULT_MOUSEOVER));
		pGuiRectStyle->SetName("buttonDefaultMO");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(172.0f/255.0f, 172.0f/255.0f, 172.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::Universe(CAssetPath::TYPE_IMAGE, IMAGE_ICONGIZMO);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*6)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*3)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_DEFAULT_HIGHLIGHT));
		pGuiRectStyle->SetName("buttonDefaultHL");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(151.0f/255.0f, 194.0f/255.0f, 201.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::Universe(CAssetPath::TYPE_IMAGE, IMAGE_ICONGIZMO);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*3)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*0)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_LINK));
		pGuiRectStyle->SetName("buttonLink");
		pGuiRectStyle->m_Flags = 0;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_LINK_MOUSEOVER));
		pGuiRectStyle->SetName("buttonLinkMO");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 0.2f);
		pGuiRectStyle->m_CornerRadius = 3.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_LINK_HIGHLIGHT));
		pGuiRectStyle->SetName("buttonLinkHL");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 0.4f);
		pGuiRectStyle->m_CornerRadius = 3.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_LAYOUT_DEFAULT));
		pGuiRectStyle->SetName("layoutDefault");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(57.0f/255.0f, 57.0f/255.0f, 57.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_LAYOUT_TOOLBAR));
		pGuiRectStyle->SetName("layoutToolBar");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(83.0f/255.0f, 83.0f/255.0f, 83.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_LAYOUT_VIEW));
		pGuiRectStyle->SetName("layoutToolBar");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_ENTRY_DEFAULT));
		pGuiRectStyle->SetName("entryDefault");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::Universe(CAssetPath::TYPE_IMAGE, IMAGE_ICONGIZMO);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*15)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*12)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_ENTRY_DEFAULT_MOUSEOVER));
		pGuiRectStyle->SetName("entryDefaultMouseOver");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_IMAGE;
		pGuiRectStyle->m_BackgroundColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
		pGuiRectStyle->m_BackgroundPadding = 4.0f;
		pGuiRectStyle->m_ImagePath = CAssetPath::Universe(CAssetPath::TYPE_IMAGE, IMAGE_ICONGIZMO);
		pGuiRectStyle->m_ImageUV_Min = vec2((512-16*12)/512.0f, (256-16*3)/256.0f);
		pGuiRectStyle->m_ImageUV_Max = vec2((512-16*9)/512.0f, (256-16*0)/256.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_TAB_LAYOUT));
		pGuiRectStyle->SetName("tabLayout");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(57.0f/255.0f, 57.0f/255.0f, 57.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_TAB_CONTENT));
		pGuiRectStyle->SetName("tabContent");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND;
		pGuiRectStyle->m_BackgroundColor = vec4(83.0f/255.0f, 83.0f/255.0f, 83.0f/255.0f, 1.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_TAB_BUTTON));
		pGuiRectStyle->SetName("tabButton");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND;
		pGuiRectStyle->m_BackgroundColor = vec4(57.0f/255.0f, 57.0f/255.0f, 57.0f/255.0f, 1.0f);
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = pAssetsManager->NewAsset<CAsset_GuiRectStyle>(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_TAB_BUTTON_HIGHLIGHT));
		pGuiRectStyle->SetName("tabButtonHL");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND;
		pGuiRectStyle->m_BackgroundColor = vec4(83.0f/255.0f, 83.0f/255.0f, 83.0f/255.0f, 1.0f);
	}
	dbg_msg("assetsmanager", "GuiRectStyles initialised");
}

}
