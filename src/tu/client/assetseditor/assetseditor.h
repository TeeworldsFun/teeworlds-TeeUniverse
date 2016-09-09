#ifndef TU_ASSETSEDITOR_H
#define TU_ASSETSEDITOR_H

#include <base/vmath.h>
#include <engine/kernel.h>
#include <game/client/render.h>
#include <tu/client/gui/popup.h>
#include <tu/client/gui/button.h>
#include <tu/client/gui/tabs.h>
#include <tu/client/gui/label.h>

class IClient;
class IInput;
class IGraphics;
class ITextRender;
class CRenderTools;
class IStorage;
class IAssetsFile;

namespace tu
{

class CGraphics;
class CAssetsManager;

namespace assetseditor
{

class IAssetsEditor : public IInterface
{
	MACRO_INTERFACE("assetseditor", 0)
public:

	virtual ~IAssetsEditor() {}
	virtual void Init(class CAssetsManager** ppAssetsManager, class CGraphics** ppTUGraphics) = 0;
	virtual void UpdateAndRender() = 0;
	virtual bool HasUnsavedData() const = 0;
};

extern IAssetsEditor *CreateAssetsEditor();

class CAssetsEditor : public IAssetsEditor
{
private:
	class IClient *m_pClient;
	class IInput *m_pInput;
	class IGraphics *m_pGraphics;
	class CAssetsManager** m_ppAssetsManager;
	class CGraphics** m_ppTUGraphics;
	class ITextRender *m_pTextRender;
	class IStorage *m_pStorage;
	class IAssetsFile *m_pAssetsFile;
	
	CRenderTools m_RenderTools;
	
	bool m_ShowCursor;
	vec2 m_MouseDelta;
	vec2 m_MousePos;
	int m_MouseButton[5];
	
	float m_Time;
	float m_LastTime;
	bool m_Paused;
	
	IGraphics::CTextureHandle m_CursorTexture;
	
	bool m_RefreshAssetsEditor;

public:
	class gui::CConfig *m_pGuiConfig;
	IGraphics::CTextureHandle m_ZoneTexture;
	
	bool m_Hint;
	bool m_Status;
	class gui::CLabel* m_pHintLabel;
	class gui::CLabel* m_pStatusLabel;
	class gui::CHListLayout* m_pGuiStatusBar;
	class gui::CVListLayout* m_pGuiAssetList[CAssetPath::NUM_SOURCES];
	class gui::CTabs* m_pGuiAssetListTabs;
	class CEditor* m_pGuiAssetsEditor;
	class CView* m_pGuiView;
	class CTimeline* m_pGuiTimeline;
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
	
	virtual void Init(class CAssetsManager** ppAssetsManager, class CGraphics** ppTUGraphics);
	virtual void UpdateAndRender();
	virtual bool HasUnsavedData() const;
	
	class IClient *Client() { return m_pClient; };
	class IInput *Input() { return m_pInput; };
	class IGraphics *Graphics() { return m_pGraphics; };
	const class IGraphics *Graphics() const { return m_pGraphics; };
	class CAssetsManager *AssetsManager(int Type = ASSETS_GAME) { return m_ppAssetsManager[Type]; };
	class CGraphics *TUGraphics(int Type = ASSETS_GAME) { return m_ppTUGraphics[Type]; };
	class ITextRender *TextRender() { return m_pTextRender; };
	class CRenderTools *RenderTools() { return &m_RenderTools; }
	class IStorage *Storage() { return m_pStorage; };
	class IAssetsFile *AssetsFile() { return m_pAssetsFile; };
	
	void SetPause(bool Pause);
	bool IsPaused();
	void SetTime(float Time);
	float GetTime();
	
	vec2 GetScreenSize() const;
	vec2 GetCursorPos() const;
	
	void LoadAssetsFile(const char* pFilename, int Source);
	void CloseEditor();
	
	void RefreshAssetsList(int Source);
	void RefreshAssetsList();
	void RefreshAssetsEditor(int Tab=-1);
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
	void DuplicateAsset(CAssetPath AssetPath);
	void DeleteAsset(CAssetPath AssetPath);
	void DeleteSubItem(CAssetPath AssetPath, int SubPath);
	void NewAsset(CAssetPath AssetPath);
	
	bool IsEditedAsset(CAssetPath AssetPath);
	bool IsEditedSubItem(CAssetPath AssetPath, int SubPath);
	bool IsDisplayedAsset(CAssetPath AssetPath);
	
	static void ShowHint(const char* pText, void* pData);
	void ShowStatus(const char* pText);
};

}

}

#endif
