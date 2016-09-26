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
	m_IconScale = 0.75f;
	
	m_ScrollbarStyle = CAssetPath::GuiScrollbarStyleSystem(GUISCROLLBARSTYLE_EDITOR);
	m_LabelStyle = CAssetPath::GuiLabelStyleSystem(GUILABELSTYLE_MENU_TEXT);
	m_LabelHeaderStyle = CAssetPath::GuiLabelStyleSystem(GUILABELSTYLE_MENU_HEADER);
	m_ButtonStyle = CAssetPath::GuiButtonStyleSystem(GUIBUTTONSTYLE_MENU_DEFAULT);
	m_ToggleStyle = CAssetPath::GuiToggleStyleSystem(GUITOGGLESTYLE_MENU_CHECKBOX);
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
	
}

}

}
