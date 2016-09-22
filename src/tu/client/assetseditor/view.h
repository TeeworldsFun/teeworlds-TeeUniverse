#ifndef TU_CLIENT_ASSETSEDITOR_VIEWMANAGER_H
#define TU_CLIENT_ASSETSEDITOR_VIEWMANAGER_H

#include <tu/client/gui/widget.h>
#include <tu/client/gui/listlayout.h>
#include <tu/client/gui/button.h>

namespace tu
{

namespace assetseditor
{

class CViewManager : public gui::CWidget
{
public:
	class CView : public gui::CWidget
	{
	public:
		class CCursorTool : public gui::CButton
		{
		private:
			CView* m_pView;
		
		protected:	
			virtual void MouseClickAction();
		
		public:
			CCursorTool(CView* pView, CAssetPath IconPath);
			
			virtual void OnUse(bool Used);
			virtual void UpdateView() {};
			virtual void RenderView() {};
			virtual void OnViewMouseOver(int X, int Y, int RelX, int RelY, int KeyState) {};
			virtual void OnViewButtonClick(int X, int Y, int Button, int Count) {};
			virtual void OnViewButtonRelease(int Button) {};
			
		};
	
	protected:
		class CAssetsEditor* m_pAssetsEditor;
		gui::CVListLayout* m_pToolbar;
		gui::CRect m_ViewRect;
		CCursorTool* m_pCursorTool;
	
	public:
		CView(CAssetsEditor* pAssetsEditor);
		~CView();
		
		virtual void Update();
		virtual void UpdateBoundingSize();
		virtual void UpdatePosition(gui::CRect BoundingRect);
		virtual void RenderView() = 0;
		virtual void Render();
		virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
		virtual void OnButtonClick(int X, int Y, int Button, int Count);
		virtual void OnButtonRelease(int Button);
		
		void SetCursorTool(CCursorTool* pCursorTool);
		
		class CAssetsEditor* AssetsEditor() { return m_pAssetsEditor; }
	};
	
private:
	class CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_BoxStylePath;
	CView* m_pCurrentView;
	CView* m_pViewImage;
	CView* m_pViewSprite;
	CView* m_pViewMap;
	
public:
	CViewManager(CAssetsEditor* pAssetsEditor);
	virtual ~CViewManager();
	
	virtual void UpdateBoundingSize();
	virtual void UpdatePosition(gui::CRect BoundingRect);
	virtual void Update();
	virtual void Render();
	virtual void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState);
	virtual void OnButtonClick(int X, int Y, int Button, int Count);
	virtual void OnButtonRelease(int Button);
	virtual void OnInputEvent();
	
	class CAssetsEditor* AssetsEditor() { return m_pAssetsEditor; }
};

}

}

#endif
