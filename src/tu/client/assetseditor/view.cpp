#include <tu/client/assetseditor/view.h>
#include <tu/client/assetseditor/view_image.h>
#include <tu/client/assetseditor/view_sprite.h>
#include <tu/client/assetseditor/view_map.h>
#include <tu/client/assetseditor/assetseditor.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/assetsrenderer.h>
#include <game/client/localization.h>

namespace tu
{

namespace assetseditor
{

/* CURSORTOOL *********************************************************/

CViewManager::CView::CCursorTool::CCursorTool(CView* pView, CAssetPath IconPath) :
	gui::CButton(pView->Context(), "", IconPath),
	m_pView(pView)
{
	SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_BUTTON));
}

void CViewManager::CView::CCursorTool::MouseClickAction()
{
	m_pView->SetCursorTool(this);
}

void CViewManager::CView::CCursorTool::OnUse(bool Used)
{
	if(Used)
		SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_BUTTON_HL));
	else
		SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_BUTTON));
}

/* VIEW ***************************************************************/

CViewManager::CView::CView(CAssetsEditor* pAssetsEditor) :
	gui::CWidget(pAssetsEditor),
	m_pCursorTool(0),
	m_pAssetsEditor(pAssetsEditor)
{
	m_pToolbar = new gui::CVListLayout(Context());
}

CViewManager::CView::~CView()
{
	delete m_pToolbar;
}

void CViewManager::CView::SetCursorTool(CCursorTool* pCursorTool)
{
	if(m_pCursorTool == pCursorTool)
		return;
	
	if(m_pCursorTool)
		m_pCursorTool->OnUse(false);
	
	m_pCursorTool = pCursorTool;
	
	if(m_pCursorTool)
		m_pCursorTool->OnUse(true);
}

void CViewManager::CView::Update()
{
	if(m_pCursorTool)
		m_pCursorTool->UpdateView();
	
	m_pToolbar->Update();
}

void CViewManager::CView::UpdateBoundingSize()
{
	m_pToolbar->UpdateBoundingSize();
	
	m_BoundingSizeRect.BSMinimum(200, 200);
	m_BoundingSizeRect.BSVerticalAdd(m_pToolbar->GetBS());
}

void CViewManager::CView::UpdatePosition(gui::CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	gui::CRect ToolbarRect(
		m_DrawRect.x,
		m_DrawRect.y,
		m_DrawRect.w,
		m_pToolbar->GetBS().minh
	);
	m_pToolbar->UpdatePosition(ToolbarRect);
	
	m_ViewRect = gui::CRect(
		m_DrawRect.x,
		ToolbarRect.y + ToolbarRect.h,
		m_DrawRect.w,
		m_DrawRect.h - m_pToolbar->GetBS().minh
	);
}

void CViewManager::CView::Render()
{
	Graphics()->ClipPush(m_ViewRect.x, m_ViewRect.y, m_ViewRect.w, m_ViewRect.h);
	
	RenderView();
	
	if(m_pCursorTool)
		m_pCursorTool->RenderView();
	
	Graphics()->ClipPop();
	
	m_pToolbar->Render();
}

void CViewManager::CView::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(m_pCursorTool)
		m_pCursorTool->OnViewButtonClick(X, Y, Button, Count);
		
	m_pToolbar->OnButtonClick(X, Y, Button, Count);
}

void CViewManager::CView::OnButtonRelease(int Button)
{
	if(m_pCursorTool)
		m_pCursorTool->OnViewButtonRelease(Button);
		
	m_pToolbar->OnButtonRelease(Button);
}

void CViewManager::CView::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_pCursorTool)
		m_pCursorTool->OnViewMouseOver(X, Y, RelX, RelY, KeyState);
		
	m_pToolbar->OnMouseOver(X, Y, RelX, RelY, KeyState);
}


/* VIEW MANAGER *******************************************************/

CViewManager::CViewManager(CAssetsEditor* pAssetsEditor) :
	gui::CWidget(pAssetsEditor),
	m_pAssetsEditor(pAssetsEditor),
	m_pCurrentView(0),
	m_BoxStylePath(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_VIEW))
{
	m_pViewImage = new CViewImage(AssetsEditor());
	m_pViewSprite = new CViewSprite(AssetsEditor());
	m_pViewMap = new CViewMap(AssetsEditor());
}

CViewManager::~CViewManager()
{
	delete m_pViewImage;
}

void CViewManager::Update()
{
	switch(AssetsEditor()->GetEditedAssetPath().GetType())
	{
		case CAssetPath::TYPE_IMAGE:
			m_pCurrentView = m_pViewImage;
			break;
		case CAssetPath::TYPE_SPRITE:
			m_pCurrentView = m_pViewSprite;
			break;
		case CAssetPath::TYPE_MAP:
		case CAssetPath::TYPE_MAPGROUP:
		case CAssetPath::TYPE_MAPLAYERQUADS:
		case CAssetPath::TYPE_MAPLAYERTILES:
		case CAssetPath::TYPE_MAPZONETILES:
			m_pCurrentView = m_pViewMap;
			break;
		default:
			m_pCurrentView = 0;
	}
	
	if(m_pCurrentView)
		m_pCurrentView->Update();
}

void CViewManager::UpdateBoundingSize()
{
	m_BoundingSizeRect.BSNoConstraint();
	
	if(m_pCurrentView)
	{
		m_pCurrentView->UpdateBoundingSize();
		m_BoundingSizeRect.BSVerticalAdd(m_pCurrentView->GetBS());
	}
	
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
		m_BoundingSizeRect.BSAddMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
	}
}

void CViewManager::UpdatePosition(gui::CRect BoundingRect)
{
	m_DrawRect.DRUpdatePosition(BoundingRect, m_BoundingSizeRect);
	
	gui::CRect Rect = m_DrawRect;
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
	}
	
	if(m_pCurrentView)
		m_pCurrentView->UpdatePosition(Rect);
}

void CViewManager::Render()
{
	const CAsset_GuiBoxStyle* pBoxStyle = AssetsManager()->GetAsset<CAsset_GuiBoxStyle>(m_BoxStylePath);
	if(pBoxStyle)
	{
		gui::CRect Rect = m_DrawRect;
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetMargin()));
	
		ivec2 MousePos = Context()->GetMousePos();
		if(Rect.IsInside(MousePos.x, MousePos.y))
			AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetMouseOverRectPath());
		else
			AssetsRenderer()->DrawGuiRect(&Rect, pBoxStyle->GetDefaultRectPath());
			
		Rect.RemoveMargin(Context()->ApplyGuiScale(pBoxStyle->GetPadding()));
	}
	
	if(m_pCurrentView)
		m_pCurrentView->Render();
}

void CViewManager::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_pCurrentView)
		m_pCurrentView->OnMouseOver(X, Y, RelX, RelY, KeyState);
}

void CViewManager::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(m_pCurrentView)
		m_pCurrentView->OnButtonClick(X, Y, Button, Count);
}

void CViewManager::OnButtonRelease(int Button)
{
	if(m_pCurrentView)
		m_pCurrentView->OnButtonRelease(Button);
}

void CViewManager::OnInputEvent()
{
	if(m_pCurrentView)
		m_pCurrentView->OnInputEvent();
}

}

}
