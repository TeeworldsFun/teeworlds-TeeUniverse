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
	CURSORTOOL_BONE_TRANSLATE=0,
	CURSORTOOL_BONE_TRANSLATE_X,
	CURSORTOOL_BONE_TRANSLATE_Y,
	CURSORTOOL_BONE_ROTATE,
	CURSORTOOL_BONE_SCALE,
	CURSORTOOL_BONE_SCALE_X,
	CURSORTOOL_BONE_SCALE_Y,
	CURSORTOOL_BONE_LENGTH,
	CURSORTOOL_BONE_ADD,
	CURSORTOOL_BONE_DELETE,
	CURSORTOOL_SPRITE_CREATOR,
	CURSORTOOL_SELECTIONSTAMP,
	CURSORTOOL_MAPQUADS_EDIT,
	CURSORTOOL_MAPQUADS_TRANSFORM,
	CURSORTOOL_MAPQUADS_STAMP,
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
	CGraphics* TUGraphics(int Type = ASSETS_GAME);
	CAssetsManager* AssetsManager();
	CAssetsEditor* AssetsEditor();
	CView* View();
	
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState) { };
	virtual void OnButtonClick(int X, int Y, int Button, int Count) { };
	virtual void OnButtonRelease(int Button) { };
	virtual void Render() { };
	virtual void RefreshToolBar() { };
};

#define CURSORTOOL_BUTTON(TypeName, IconID) class CButton : public CButtonCore\
	{\
	public:\
		CButton(CView* pView, int CursorToolID, bool DefaultCursorTool = false) :\
			CButtonCore(pView, CAssetPath::SpriteUniverse(IconID), CursorToolID, DefaultCursorTool)\
		{ }\
		\
		virtual CView_CursorTool* CreateCursorTool()\
		{\
			return new TypeName(m_pView, m_CursorToolID);\
		}\
	};

/* BONES **************************************************************/

class CView_CursorTool_Bone_Picker : public CView_CursorTool
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
	CView_CursorTool_Bone_Picker(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void SetMemberInfo() = 0;
};

/* BONE TRANSFORM *****************************************************/

class CView_CursorTool_Bone_Transform : public CView_CursorTool_Bone_Picker
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
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_TranslateXY, SPRITE_ICON_TRANSLATE)
	CView_CursorTool_Bone_TranslateXY(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_TranslateX : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_TranslateX, SPRITE_ICON_TRANSLATE_X)
	CView_CursorTool_Bone_TranslateX(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_TranslateY : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_TranslateY, SPRITE_ICON_TRANSLATE_Y)
	CView_CursorTool_Bone_TranslateY(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_ScaleXY : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_ScaleXY, SPRITE_ICON_SCALE)
	CView_CursorTool_Bone_ScaleXY(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_ScaleX : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_ScaleX, SPRITE_ICON_SCALE_X)
	CView_CursorTool_Bone_ScaleX(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_ScaleY : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_ScaleY, SPRITE_ICON_SCALE_Y)
	CView_CursorTool_Bone_ScaleY(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_Rotate : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_Rotate, SPRITE_ICON_ROTATE)
	CView_CursorTool_Bone_Rotate(CView* pView, int CursorToolID);
};

class CView_CursorTool_Bone_Length : public CView_CursorTool_Bone_Transform
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_Length, SPRITE_ICON_BONE_LENGTH)
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
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_Add, SPRITE_ICON_BONE_ADD)
	CView_CursorTool_Bone_Add(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
};

/* BONE DELETE ***********************************************************/

class CView_CursorTool_Bone_Delete : public CView_CursorTool
{
public:
	CURSORTOOL_BUTTON(CView_CursorTool_Bone_Delete, SPRITE_ICON_BONE_DELETE)
	CView_CursorTool_Bone_Delete(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
};

/* STAMP **************************************************************/

class CView_CursorTool_MapStamp : public CView_CursorTool
{
public:
	class CPalettePopup : public gui::CPopup
	{
	protected:
		class CTexturePalette : public gui::CWidget
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
			CTexturePalette(gui::CConfig* pConfig, CPalettePopup* pPopup, CAssetPath ImagePath);
			virtual void Render();
			void OnButtonClick(int X, int Y, int Button, int Count);
			void OnButtonRelease(int Button);
			void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
		};
		
		class CListEntry : public gui::CTextButton
		{
		public:
			CPalettePopup* m_pPopup;
			int m_SubPath;
			
			CListEntry(gui::CConfig* pConfig, CPalettePopup* pPopup, int SubPath, const char* pText);
			void MouseClickAction();
		};
		
		class CZonePalette : public gui::CVListLayout
		{
		protected:
			
			CPalettePopup* m_pPopup;
			CAssetPath m_ZoneTypePath;
		
		public:
			CZonePalette(gui::CConfig* pConfig, CPalettePopup* pPopup, CAssetPath ZoneTypePath);
		};
	
	public:
		CAssetsEditor* m_pAssetsEditor;
		CView_CursorTool_MapStamp* m_pCursorTool;
		
	public:
		CPalettePopup(CAssetsEditor* pAssetsEditor, CView_CursorTool_MapStamp* pCursorTool, const gui::CRect& CreatorRect);
		virtual void Close();
		void OnTexturePaletteSelection(int X0, int Y0, int X1, int Y1);
		void OnListPaletteSelection(int SubPathInt);
	};

public:
	CPalettePopup* m_pPalette;

protected:
	virtual void OnPaletteCloseCallback();
	virtual void OnTexturePaletteSelection(int X0, int Y0, int X1, int Y1) {};
	virtual void OnListPaletteSelection(int SubPathInt) {};

public:
	CView_CursorTool_MapStamp(CView* pView, int CursorToolID);
};

/* MAPLAYERTILES ******************************************************/

class CView_CursorTool_MapTiles_SelectionStamp : public CView_CursorTool_MapStamp
{
public:	
	/*
	class CPalettePopup_Physics : public gui::CPopup
	{	
	public:
		CAssetsEditor* m_pAssetsEditor;
		CView_CursorTool_MapTiles_SelectionStamp* m_pCursorTool;
		
	public:
		CPalettePopup_Layer(CAssetsEditor* pAssetsEditor, CView_CursorTool_MapTiles_SelectionStamp* pCursorTool, const gui::CRect& CreatorRect);
		virtual void Close();
	};
	*/
	
#define MAPTILES_SELECTIONSTAMP_BUTTON(TypeName) class TypeName : public gui::CIconButton\
	{\
	protected:\
		CView_CursorTool_MapTiles_SelectionStamp* m_pCursorTool;\
	\
	protected:\
		virtual void MouseClickAction();\
	\
	public:\
		TypeName(CView_CursorTool_MapTiles_SelectionStamp* pCursorTool);\
	};\

	MAPTILES_SELECTIONSTAMP_BUTTON(CHFlipButton)
	MAPTILES_SELECTIONSTAMP_BUTTON(CVFlipButton)
	MAPTILES_SELECTIONSTAMP_BUTTON(CRotateCWButton)
	MAPTILES_SELECTIONSTAMP_BUTTON(CRotateCCWButton)

public:
	vec2 m_Pivot;
	CAsset_MapLayerTiles::CTile* m_pSelection;
	int m_SelectionWidth;
	int m_SelectionHeight;
	bool m_DragSelection;
	int m_Token;

protected:
	virtual void OnTexturePaletteSelection(int X0, int Y0, int X1, int Y1);
	virtual void OnListPaletteSelection(int SubPathInt);

public:
	CURSORTOOL_BUTTON(CView_CursorTool_MapTiles_SelectionStamp, SPRITE_ICON_SELECTIONSTAMP)
	CView_CursorTool_MapTiles_SelectionStamp(CView* pView, int CursorToolID);
	virtual ~CView_CursorTool_MapTiles_SelectionStamp();
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void Render();
	virtual void RefreshToolBar();
};

/* MAPLAYERQUADS ******************************************************/

class CView_CursorTool_MapQuads_Picker : public CView_CursorTool
{
public:
	CView_CursorTool_MapQuads_Picker(CView* pView, int CursorToolID);
	CAsset_MapLayerQuads::CSubPath PickQuad(vec2 CursorPos);
	void RenderPivots();
};

class CView_CursorTool_MapQuads_Edit : public CView_CursorTool_MapQuads_Picker
{
public:
	CAsset_MapLayerQuads::CSubPath m_SelectedQuad;
	vec2 m_ClickDiff;
	int m_Token;

public:
	CURSORTOOL_BUTTON(CView_CursorTool_MapQuads_Edit, SPRITE_ICON_QUAD)
	CView_CursorTool_MapQuads_Edit(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void Render();
};

class CView_CursorTool_MapQuads_Transform : public CView_CursorTool_MapQuads_Picker
{
#define MAPQUADS_TRANSFORM_TOGGLE(TypeName) class TypeName : public gui::CIconToggle\
	{\
	protected:\
		CView_CursorTool_MapQuads_Transform* m_pCursorTool;\
	\
	protected:\
		virtual void OnToggle(bool Toggle);\
	\
	public:\
		TypeName(CView_CursorTool_MapQuads_Transform* pCursorTool);\
	};\

	MAPQUADS_TRANSFORM_TOGGLE(CFrameTransformToggle)
	
public:
	enum
	{
		GIZMOTYPE_NONE=0,
		GIZMOTYPE_ROTATION,
		GIZMOTYPE_SCALE,
	};
	
	enum
	{
		DRAGTYPE_NONE=0,
		DRAGTYPE_GIZMO,
		DRAGTYPE_TRANSLATION,
	};

public:
	CAsset_MapLayerQuads::CSubPath m_SelectedQuad;
	int m_SelectedGizmo;
	int m_GizmoType;
	int m_DragType;
	vec2 m_ClickDiff;
	bool m_Transformed;
	bool m_FrameTransform;
	int m_Token;

public:
	CURSORTOOL_BUTTON(CView_CursorTool_MapQuads_Transform, SPRITE_ICON_MOVE)
	CView_CursorTool_MapQuads_Transform(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void Render();
	virtual void RefreshToolBar();
};

class CView_CursorTool_MapQuads_Stamp : public CView_CursorTool_MapStamp
{
#define MAPQUADS_SELECTIONSTAMP_BUTTON(TypeName) class TypeName : public gui::CIconButton\
	{\
	protected:\
		CView_CursorTool_MapQuads_Stamp* m_pCursorTool;\
	\
	protected:\
		virtual void MouseClickAction();\
	\
	public:\
		TypeName(CView_CursorTool_MapQuads_Stamp* pCursorTool);\
	};\

	MAPQUADS_SELECTIONSTAMP_BUTTON(CHFlipButton)
	MAPQUADS_SELECTIONSTAMP_BUTTON(CVFlipButton)
	MAPQUADS_SELECTIONSTAMP_BUTTON(CRotateCWButton)
	MAPQUADS_SELECTIONSTAMP_BUTTON(CRotateCCWButton)
	
public:
	array<CAsset_MapLayerQuads::CQuad> m_Selection;

protected:
	virtual void OnTexturePaletteSelection(int X0, int Y0, int X1, int Y1);
	
public:
	CURSORTOOL_BUTTON(CView_CursorTool_MapQuads_Stamp, SPRITE_ICON_SELECTIONSTAMP)
	CView_CursorTool_MapQuads_Stamp(CView* pView, int CursorToolID);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void Render();
	virtual void RefreshToolBar();
};

}

}

#endif
