#ifndef TU_ASSETSEDITOR_VIEW_CURSORTOOL_H
#define TU_ASSETSEDITOR_VIEW_CURSORTOOL_H

#include <tu/client/gui/button.h>
#include <tu/client/gui/popup.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/assetseditor/assetseditor.h>

namespace tu
{

namespace assetseditor
{

class CView;
	
enum
{
	CURSORTOOL_MOVE=0,
	CURSORTOOL_TRANSLATE,
	CURSORTOOL_TRANSLATE_X,
	CURSORTOOL_TRANSLATE_Y,
	CURSORTOOL_ROTATE,
	CURSORTOOL_SCALE,
	CURSORTOOL_SCALE_X,
	CURSORTOOL_SCALE_Y,
	CURSORTOOL_BONE_LENGTH,
	CURSORTOOL_BONE_ADD,
	CURSORTOOL_BONE_DELETE,
	CURSORTOOL_SPRITE_CREATOR,
	CURSORTOOL_SELECTIONSTAMP,
	CURSORTOOL_SELECTIONFILL,
	NUM_CURSORTOOLS
};
	
class CView_CursorTool
{
public:
	class CButtonCore : public gui::CIconButton
	{
	protected:
		CView* m_pView;
		int m_CursorToolID;
		
	public:
		virtual void MouseClickAction();
		
	protected:
		CButtonCore(CView* pView, int Icon, int CursorToolID, bool DefaultCursorTool);
		virtual ~CButtonCore() {}
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
		virtual void Render();
		virtual CView_CursorTool* CreateCursorTool() = 0;
	};

public:
	int m_CursorToolID;

private:
	CView* m_pView;
	
protected:
	CView_CursorTool(CView* pView, int CursorToolID) :
		m_pView(pView),
		m_CursorToolID(CursorToolID)
	{ }

public:
	int GetCursorToolID() const { return m_CursorToolID; }
	
	IGraphics* Graphics();
	CAssetsManager* AssetsManager();
	CAssetsEditor* AssetsEditor();
	CView* View();
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState) { };
	virtual void OnButtonClick(int X, int Y, int Button, int Count) { };
	virtual void Render() { };
	virtual void RefreshToolBar() { };
};

#define CURSORTOOL_BUTTON(TypeName, IconID) class CButton : public CButtonCore\
	{\
	public:\
		CButton(CView* pView, int CursorToolID, bool DefaultCursorTool = false) :\
			CButtonCore(pView, CAssetsEditor::IconID, CursorToolID, DefaultCursorTool)\
		{ }\
		\
		virtual CView_CursorTool* CreateCursorTool()\
		{\
			return new TypeName(m_pView, m_CursorToolID);\
		}\
	};

/* BONES **************************************************************/

class CView_CursorTool_Bone_Picking : public CView_CursorTool
{
public:
	CAssetPath m_SelectedAsset;
	CAssetPath m_SelectedSqueleton;
	CAsset_Skeleton::CSubPath m_SelectedBone;
	int m_SelectedSubItem;
	int m_Member0;
	int m_Member1;
	float m_DefaultValue0;
	float m_DefaultValue1;
	float m_BackupValue0;
	float m_BackupValue1;
	
	vec2 m_Pivot;
	vec2 m_CursorLastPos;
	
public:
	CView_CursorTool_Bone_Picking(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void SetMemberInfo() = 0;
};

/* BONE TRANSFORM *****************************************************/

class CView_CursorTool_Bone_Transform : public CView_CursorTool_Bone_Picking
{
public:
	enum
	{
		TRANSFORM_TRANSLATION_X=0,
		TRANSFORM_TRANSLATION_Y,
		TRANSFORM_TRANSLATION_XY,
		TRANSFORM_SCALE_X,
		TRANSFORM_SCALE_Y,
		TRANSFORM_SCALE_XY,
		TRANSFORM_ROTATE,
		TRANSFORM_LENGTH,
	};
	
	int m_Transform;

public:
	CView_CursorTool_Bone_Transform(CView* pView, int CursorToolID, int Transform);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void SetMemberInfo();
};

class CView_CursorTool_Bone_TranslateXY : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_TranslateXY, ICON_CURSORTOOL_TRANSLATE)
	CView_CursorTool_Bone_TranslateXY(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_TranslateX : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_TranslateX, ICON_CURSORTOOL_TRANSLATE_X)
	CView_CursorTool_Bone_TranslateX(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_TranslateY : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_TranslateY, ICON_CURSORTOOL_TRANSLATE_Y)
	CView_CursorTool_Bone_TranslateY(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_ScaleXY : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_ScaleXY, ICON_CURSORTOOL_SCALE)
	CView_CursorTool_Bone_ScaleXY(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_ScaleX : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_ScaleX, ICON_CURSORTOOL_SCALE_X)
	CView_CursorTool_Bone_ScaleX(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_ScaleY : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_ScaleY, ICON_CURSORTOOL_SCALE_Y)
	CView_CursorTool_Bone_ScaleY(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_Rotate : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_Rotate, ICON_CURSORTOOL_ROTATE)
	CView_CursorTool_Bone_Rotate(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_Length : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_Length, ICON_CURSORTOOL_BONE_LENGTH)
	CView_CursorTool_Bone_Length(CView* pView, int CursorToolID);
};

/* BONE ADD ***********************************************************/

class CView_CursorTool_Bone_Add : public CView_CursorTool
{
public:
	CAssetPath m_SelectedSqueleton;
	CAsset_Skeleton::CSubPath m_SelectedBone;
	vec2 m_Pivot;
	vec2 m_CursorLastPos;
	
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_Add, ICON_CURSORTOOL_BONE_ADD)
	CView_CursorTool_Bone_Add(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
};

/* BONE DELETE ***********************************************************/

class CView_CursorTool_Bone_Delete : public CView_CursorTool
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_Delete, ICON_CURSORTOOL_BONE_DELETE)
	CView_CursorTool_Bone_Delete(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
};

/* MAPLAYERTILES SELECTIONSTAMP ***************************************/

class CView_CursorTool_MapLayerTiles_SelectionStamp : public CView_CursorTool
{
public:
	class CPalettePopup : public gui::CPopup
	{
	protected:
		class CTilePalette : public gui::CWidget
		{
		protected:
			CPalettePopup* m_pPopup;
			CAssetPath m_ImagePath;
			gui::CRect m_ImageRect;
			bool m_Clicked;
			int m_FirstX;
			int m_FirstY;
			int m_CurrX;
			int m_CurrY;
		
		public:
			CTilePalette(gui::CConfig* pConfig, CPalettePopup* pPopup, CAssetPath ImagePath);
			virtual void Render();
			void OnButtonClick(int X, int Y, int Button, int Count);
			void OnButtonRelease(int Button);
			void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
		};
	
	public:
		CAssetsEditor* m_pAssetsEditor;
		CView_CursorTool_MapLayerTiles_SelectionStamp* m_pCursorTool;
		
	public:
		CPalettePopup(CAssetsEditor* pAssetsEditor, CView_CursorTool_MapLayerTiles_SelectionStamp* pCursorTool, const gui::CRect& CreatorRect);
		virtual void Close();
		void SelectTile(int Index, int W, int H);
	};
	
#define MAPLAYERTILES_SELECTIONSTAMP_BUTTON(TypeName) class TypeName : public gui::CIconButton\
	{\
	protected:\
		CView_CursorTool_MapLayerTiles_SelectionStamp* m_pCursorTool;\
	\
	protected:\
		virtual void MouseClickAction();\
	\
	public:\
		TypeName(CView_CursorTool_MapLayerTiles_SelectionStamp* pCursorTool);\
	};\

	MAPLAYERTILES_SELECTIONSTAMP_BUTTON(CHFlipButton)
	MAPLAYERTILES_SELECTIONSTAMP_BUTTON(CVFlipButton)
	MAPLAYERTILES_SELECTIONSTAMP_BUTTON(CRotateCWButton)
	MAPLAYERTILES_SELECTIONSTAMP_BUTTON(CRotateCCWButton)

public:
	vec2 m_Pivot;
	CAsset_MapLayerTiles::CTile* m_pSelection;
	int m_SelectionWidth;
	int m_SelectionHeight;
	CPalettePopup* m_pPalette;

protected:
	void OnPaletteCloseCallback();
	void OnPaletteSelection_Tile(int Index, int W, int H);

public:
	CURSORTOOL_BUTTON(CView_CursorTool_MapLayerTiles_SelectionStamp, ICON_CURSORTOOL_SELECTIONSTAMP)
	CView_CursorTool_MapLayerTiles_SelectionStamp(CView* pView, int CursorToolID);
	virtual ~CView_CursorTool_MapLayerTiles_SelectionStamp();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void Render();
	virtual void RefreshToolBar();
};

}

}

#endif
