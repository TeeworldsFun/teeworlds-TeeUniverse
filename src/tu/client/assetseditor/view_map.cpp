#include <tu/client/assetseditor/view_map.h>
#include <tu/client/assetseditor/assetseditor.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/assetsrenderer.h>
#include <tu/client/maprenderer.h>
#include <engine/graphics.h>

namespace tu
{

namespace assetseditor
{

/* CURSORTOOL *********************************************************/

CViewMap::CCursorTool::CCursorTool(CViewMap* pViewMap, CAssetPath IconPath) :
	CViewManager::CView::CCursorTool(pViewMap, IconPath),
	m_pViewMap(pViewMap)
{
	
}

/* VIEW IMAGE *********************************************************/

CViewMap::CViewMap(CAssetsEditor* pAssetsEditor) :
	CViewManager::CView(pAssetsEditor),
	m_CameraPos(0.0f, 0.0f),
	m_CameraZoom(1.0f),
	m_CameraDraged(false)
{
	//~ m_pToolbar->Add();
}

CAssetPath CViewMap::GetMapPath()
{
	switch(AssetsEditor()->GetEditedAssetPath().GetType())
	{
		case CAssetPath::TYPE_MAP:
			return AssetsEditor()->GetEditedAssetPath();
		case CAssetPath::TYPE_MAPZONETILES:
			for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
			{
				for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Map>(s); i++)
				{
					CAssetPath MapPath = CAssetPath::Asset(CAsset_Map::TypeId, s, i);
					const CAsset_Map* pMap = AssetsManager()->GetAsset<CAsset_Map>(MapPath);
					if(pMap)
					{
						CAsset_Map::CIteratorZoneLayer IterZoneLayer;
						for(IterZoneLayer = pMap->BeginZoneLayer(); IterZoneLayer != pMap->EndZoneLayer(); ++IterZoneLayer)
						{
							if(pMap->GetZoneLayer(*IterZoneLayer) == AssetsEditor()->GetEditedAssetPath())
								return MapPath;
						}
					}
				}
			}
			break;
		case CAssetPath::TYPE_MAPGROUP:
			for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
			{
				for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Map>(s); i++)
				{
					CAssetPath MapPath = CAssetPath::Asset(CAsset_Map::TypeId, s, i);
					const CAsset_Map* pMap = AssetsManager()->GetAsset<CAsset_Map>(MapPath);
					if(pMap)
					{
						CAsset_Map::CIteratorBgGroup IterBgGroup;
						for(IterBgGroup = pMap->BeginBgGroup(); IterBgGroup != pMap->EndBgGroup(); ++IterBgGroup)
						{
							if(pMap->GetBgGroup(*IterBgGroup) == AssetsEditor()->GetEditedAssetPath())
								return MapPath;
						}
						CAsset_Map::CIteratorFgGroup IterFgGroup;
						for(IterFgGroup = pMap->BeginFgGroup(); IterFgGroup != pMap->EndFgGroup(); ++IterFgGroup)
						{
							if(pMap->GetFgGroup(*IterFgGroup) == AssetsEditor()->GetEditedAssetPath())
								return MapPath;
						}
					}
				}
			}
			break;
		case CAssetPath::TYPE_MAPLAYERTILES:
		case CAssetPath::TYPE_MAPLAYERQUADS:
			for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
			{
				for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_Map>(s); i++)
				{
					CAssetPath MapPath = CAssetPath::Asset(CAsset_Map::TypeId, s, i);
					const CAsset_Map* pMap = AssetsManager()->GetAsset<CAsset_Map>(MapPath);
					if(pMap)
					{
						CAsset_Map::CIteratorBgGroup IterBgGroup;
						for(IterBgGroup = pMap->BeginBgGroup(); IterBgGroup != pMap->EndBgGroup(); ++IterBgGroup)
						{
							const CAsset_MapGroup* pGroup = AssetsManager()->GetAsset<CAsset_MapGroup>(pMap->GetBgGroup(*IterBgGroup));
							if(pGroup)
							{
								CAsset_MapGroup::CIteratorLayer IterLayer;
								for(IterLayer = pGroup->BeginLayer(); IterLayer != pGroup->EndLayer(); ++IterLayer)
								{
									if(pGroup->GetLayer(*IterLayer) == AssetsEditor()->GetEditedAssetPath())
										return MapPath;
								}
							}
						}
						CAsset_Map::CIteratorFgGroup IterFgGroup;
						for(IterFgGroup = pMap->BeginFgGroup(); IterFgGroup != pMap->EndFgGroup(); ++IterFgGroup)
						{
							const CAsset_MapGroup* pGroup = AssetsManager()->GetAsset<CAsset_MapGroup>(pMap->GetBgGroup(*IterFgGroup));
							if(pGroup)
							{
								CAsset_MapGroup::CIteratorLayer IterLayer;
								for(IterLayer = pGroup->BeginLayer(); IterLayer != pGroup->EndLayer(); ++IterLayer)
								{
									if(pGroup->GetLayer(*IterLayer) == AssetsEditor()->GetEditedAssetPath())
										return MapPath;
								}
							}
						}
					}
				}
			}
			break;
	}
	
	return CAssetPath::Null();
}

void CViewMap::RenderView()
{
	CAssetPath MapPath = GetMapPath();
	
	CMapRenderer MapRenderer(Kernel());
		
	MapRenderer.SetTime(0.0f);
	MapRenderer.SetLocalTime(time_get());
	MapRenderer.SetCanvas(vec2(m_ViewRect.x + m_ViewRect.w/2, m_ViewRect.y + m_ViewRect.h/2));
	MapRenderer.SetCamera(m_CameraPos, m_CameraZoom);
	
	MapRenderer.RenderMap(MapPath);
}

void CViewMap::OnButtonClick(int X, int Y, int Button, int Count)
{
	if(m_ViewRect.IsInside(X, Y))
	{
		if(Button == KEY_MOUSE_WHEEL_UP)
		{
			m_CameraZoom *= 1.1f;
		}
		if(Button == KEY_MOUSE_WHEEL_DOWN)
		{
			m_CameraZoom /= 1.1f;
		}
		else if(Button == KEY_MOUSE_3)
		{
			m_CameraDraged = true;
			return;
		}
	}
	
	CView::OnButtonClick(X, Y, Button, Count);
}

void CViewMap::OnButtonRelease(int Button)
{
	if(Button == KEY_MOUSE_3)
	{
		m_CameraDraged = false;
	}
	
	CView::OnButtonRelease(Button);
}

void CViewMap::OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
{
	if(m_CameraDraged)
	{
		m_CameraPos -= vec2(RelX, RelY)/m_CameraZoom;
		return;
	}
	
	CView::OnMouseOver(X, Y, RelX, RelY, KeyState);
}

}

}
