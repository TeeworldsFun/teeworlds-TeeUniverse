#include <tu/client/menu/menu.h>
#include <tu/client/menu/settings.h>
#include <engine/shared/config.h>
#include <engine/keys.h>
#include <engine/input.h>
#include <tu/client/clientmode.h>

namespace tu
{

namespace menu
{

CMenu::CMenu(CKernel* pKernel) :
	gui::CContext(pKernel)
{
	str_copy(m_aLocalizationContext, "", sizeof(m_aLocalizationContext));
	
	//~ m_LabelStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_LABEL);
	//~ m_LabelHeaderStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_LABELHEADER);
	//~ m_ButtonStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_BUTTON);
	//~ m_TextEntryStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_TEXTENTRY);
	//~ m_NumericEntryStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_NUMERICENTRY);
	//~ m_ScrollbarStyle = CAssetPath::GuiScrollbarStyleSystem(GUISCROLLBARSTYLE_EDITOR);
	m_TabsStyle = CAssetPath::GuiTabsStyleSystem(GUITABSSTYLE_MENU_MAIN);
	//~ m_PopupStyle = CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_EDITOR_POPUP);
}


void CMenu::CreateMainWidget()
{
	m_pSettings = new CSettings(this);
	
	m_pMainWidget = m_pSettings;
}

void CMenu::DoShortcuts()
{
	if(Input()->KeyIsPressed(KEY_ESCAPE))
	{
		Close();
		return;
	}
	
	//TAG_DEBUG
	if(Input()->KeyIsPressed(KEY_LCTRL) && Input()->KeyPress(KEY_B, true))
		m_GuiDirection = (m_GuiDirection+1)%NUM_DIRECTIONS;
		
	if(Input()->KeyIsPressed(KEY_LCTRL) && Input()->KeyPress(KEY_A, true))
		m_GuiScale -= 0.25f;
	
	if(Input()->KeyIsPressed(KEY_LCTRL) && Input()->KeyPress(KEY_E, true))
		m_GuiScale += 0.25f;
}

void CMenu::Close()
{
	g_Config.m_ClMode = TU_CLIENTMODE_GAME;
}

void CMenu::RenderBackground()
{
	//~ Graphics()->TextureClear();
	//~ Graphics()->QuadsBegin();
	//~ Graphics()->SetColor(0.0f/255.0f, 0.0f/255.0f, 255.0f/255.0f, 1.0f);
	
	//~ IGraphics::CQuadItem QuadItem(m_DrawRect.w/2, m_DrawRect.h/2, m_DrawRect.w/2, m_DrawRect.h/2);
	//~ Graphics()->QuadsDraw(&QuadItem, 1);
	
	//~ Graphics()->QuadsEnd();
}

}

}
