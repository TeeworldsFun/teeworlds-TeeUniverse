#ifndef TU_ASSETSEDITOR_H
#define TU_ASSETSEDITOR_H

#include <base/vmath.h>
#include <engine/kernel.h>
#include <game/client/render.h>
#include <modapi/client/gui/popup.h>
#include <modapi/client/gui/button.h>
#include <modapi/client/gui/tabs.h>
#include <modapi/client/gui/label.h>

class IClient;
class IInput;
class IGraphics;
class ITextRender;
class CRenderTools;
class IStorage;
class IAssetsFile;

namespace tu
{

class CClient_Graphics;
class CAssetManager;

class IAssetsEditor : public IInterface
{
	MACRO_INTERFACE("assetseditor", 0)
public:

	virtual ~IAssetsEditor() {}
	virtual void Init(class CAssetManager* pAssetManager, class CClient_Graphics* pTUGraphics) = 0;
	virtual void UpdateAndRender() = 0;
	virtual bool HasUnsavedData() const = 0;
};

extern IAssetsEditor *CreateAssetsEditor();

class CAssetsEditor : public IAssetsEditor
{
public:
	enum
	{
		ICON_DECREASE=2,
		ICON_INCREASE,
		ICON_DELETE,
		ICON_ROTATION,
		ICON_OPACITY,
		ICON_TRANSLATE_X,
		ICON_TRANSLATE_Y,
		ICON_EDIT,
		ICON_VIEW,
		ICON_UP,
		ICON_DOWN,
		ICON_DUPLICATE,
		
		ICON_FIRST_FRAME=16,
		ICON_PREV_FRAME,
		ICON_PLAY,
		ICON_PAUSE,
		ICON_NEXT_FRAME,
		ICON_LAST_FRAME,
		
		
		ICON_MAGNET_CIRCLE = 32,
		ICON_MAGNET_TRIANGLE,
		ICON_MAGNET_ROTATION,
		
		ICON_ASSET = 48,
		ICON_INTERNAL_ASSET,
		ICON_EXTERNAL_ASSET,
		ICON_MAP_ASSET,
		ICON_SKIN_ASSET,
		ICON_LAYERS,
		ICON_BONE,
		ICON_SPRITE,
		ICON_IMAGE,
		ICON_SKELETON,
		ICON_SKELETONSKIN,
		ICON_SKELETONANIMATION,
		ICON_LAYERANIMATION,
		ICON_CHARACTER,
		ICON_CHARACTERPART,
		
		ICON_CURSORTOOL_MOVE = 64,
		ICON_CURSORTOOL_TRANSLATE,
		ICON_CURSORTOOL_TRANSLATE_X,
		ICON_CURSORTOOL_TRANSLATE_Y,
		ICON_CURSORTOOL_ROTATE,
		ICON_CURSORTOOL_SCALE,
		ICON_CURSORTOOL_SCALE_X,
		ICON_CURSORTOOL_SCALE_Y,
		ICON_CURSORTOOL_BONE_LENGTH,
		ICON_CURSORTOOL_BONE_ADD,
		ICON_CURSORTOOL_BONE_DELETE,
		ICON_CURSORTOOL_BONE_ATTACH,
		
		ICON_CURSORTOOL_FRAME_MOVE = 80,
		ICON_CURSORTOOL_FRAME_ADD,
		ICON_CURSORTOOL_FRAME_DELETE,
		ICON_FRAMES,
		ICON_CURSORTOOL_FRAME_COLOR,
		ICON_KEYFRAME_BONE,
		ICON_KEYFRAME_LAYER,
		
		ICON_COLORPICKER_RGB = 96,
		ICON_COLORPICKER_HSV,
		ICON_COLORPICKER_SQUARE,
		ICON_COLORPICKER_WHEEL,
	};

private:
	class IClient *m_pClient;
	class IInput *m_pInput;
	class IGraphics *m_pGraphics;
	class CClient_Graphics *m_pTUGraphics;
	class CAssetManager *m_pAssetManager;
	class ITextRender *m_pTextRender;
	class IStorage *m_pStorage;
	class IAssetsFile *m_pAssetsFile;
	
	CRenderTools m_RenderTools;
	
	bool m_ShowCursor;
	vec2 m_MouseDelta;
	vec2 m_MousePos;
	int m_MouseButton[3];
	
	float m_Time;
	float m_LastTime;
	bool m_Paused;
	
	IGraphics::CTextureHandle m_CursorTexture;
	
	bool m_RefreshAssetEditor;

public:
	class gui::CConfig *m_pGuiConfig;
	IGraphics::CTextureHandle m_ModEditorTexture;
	
	bool m_Hint;
	class gui::CLabel* m_pHintLabel;
	class gui::CHListLayout* m_pGuiToolbar;
	class gui::CVListLayout* m_pGuiAssetList[CAssetPath::NUM_SOURCES];
	class gui::CTabs* m_pGuiAssetListTabs;
	class CAssetsEditorGui_Editor* m_pGuiAssetEditor;
	class CAssetsEditorGui_View* m_pGuiView;
	class CAssetsEditorGui_Timeline* m_pGuiTimeline;
	array<class gui::CPopup*> m_GuiPopups;
	
	int m_AssetsListSource;
	CAssetPath m_EditedAssetPath;
	int m_EditedAssetSubPath;
	int m_EditorTab;
	
	CAssetPath m_ViewedAssetPath;
	
private:
	void Render();

public:
	CAssetsEditor();
	virtual ~CAssetsEditor();
	
	virtual void Init(class CAssetManager* pAssetManager, class CClient_Graphics* pTUGraphics);
	virtual void UpdateAndRender();
	virtual bool HasUnsavedData() const;
	
	class IClient *Client() { return m_pClient; };
	class IInput *Input() { return m_pInput; };
	class IGraphics *Graphics() { return m_pGraphics; };
	class CClient_Graphics *TUGraphics() { return m_pTUGraphics; };
	class CAssetManager *AssetManager() { return m_pAssetManager; };
	class ITextRender *TextRender() { return m_pTextRender; };
	class CRenderTools *RenderTools() { return &m_RenderTools; }
	class IStorage *Storage() { return m_pStorage; };
	class IAssetsFile *AssetsFile() { return m_pAssetsFile; };
	
	void SetPause(bool Pause);
	bool IsPaused();
	void SetTime(float Time);
	float GetTime();
	
	void LoadAssetsFile(const char* pFilename);
	void CloseEditor();
	
	void RefreshAssetList(int Source);
	void RefreshAssetList();
	void RefreshAssetEditor(int Tab=-1);
	void DisplayPopup();
	void ShowCursor();
	void HideCursor();
	void DisplayPopup(gui::CPopup* pWidget);
	void EditAsset(CAssetPath AssetPath);
	void EditAssetSubItem(CAssetPath AssetPath, int ItemPath, int Tab=-1);
	void EditAssetFirstFrame();
	void EditAssetLastFrame();
	void EditAssetPrevFrame();
	void EditAssetNextFrame();
	void DisplayAsset(CAssetPath AssetPath);
	void DeleteAsset(CAssetPath AssetPath);
	void NewAsset(CAssetPath AssetPath);
	
	bool IsEditedAsset(CAssetPath AssetPath);
	bool IsDisplayedAsset(CAssetPath AssetPath);
	
	static void ShowHint(const char* pText, void* pData);
};

}

#endif
