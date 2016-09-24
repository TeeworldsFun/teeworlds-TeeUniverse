#include <tu/client/menu/settings.h>
#include <tu/client/menu/menu.h>
#include <game/client/localization.h>

namespace tu
{

namespace menu
{

/* CONFIG CHECKBOX ****************************************************/

/* SETTINGS ***********************************************************/

CSettings::CSettings(CMenu* pMenu) :
	gui::CTabs(pMenu),
	m_pMenu(pMenu)
{
	m_pGeneralTab = CreateGeneralTab();
	m_pPlayerTab = new gui::CVListLayout(pMenu);
	m_pCharacterTab = new gui::CVListLayout(pMenu);
	m_pControlTab = new gui::CVListLayout(pMenu);
	m_pGraphicsTab = new gui::CVListLayout(pMenu);
	m_pSoundTab = new gui::CVListLayout(pMenu);
	
	AddTab(m_pGeneralTab, Context()->Localize("General"), CAssetPath::Null());
	AddTab(m_pPlayerTab, Context()->Localize("Player"), CAssetPath::Null());
	AddTab(m_pCharacterTab, Context()->Localize("Character"), CAssetPath::Null());
	AddTab(m_pControlTab, Context()->Localize("Controles"), CAssetPath::Null());
	AddTab(m_pGraphicsTab, Context()->Localize("Graphics"), CAssetPath::Null());
	AddTab(m_pSoundTab, Context()->Localize("Sound"), CAssetPath::Null());
}

gui::CVListLayout* CSettings::CreateGeneralTab()
{
	gui::CVListLayout* pTab = new gui::CVListLayout(Menu());
	
	pTab->Add(new gui::CButton(Context(), Context()->Localize("Dynamic Camera")), false);
	pTab->Add(new gui::CButton(Context(), Context()->Localize("Switch weapon on pickup")), false);
	pTab->Add(new gui::CButton(Context(), Context()->Localize("Show ingame HUD")), false);
	pTab->Add(new gui::CButton(Context(), Context()->Localize("Show only chat messages from friends")), false);
	pTab->Add(new gui::CButton(Context(), Context()->Localize("Show only chat messages from friends")), false);
	
	return pTab;
}

}

}

