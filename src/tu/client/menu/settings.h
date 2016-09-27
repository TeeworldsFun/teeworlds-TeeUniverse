#ifndef TU_CLIENT_MENU_SETTINGS_H
#define TU_CLIENT_MENU_SETTINGS_H

#include <tu/client/gui/tabs.h>
#include <tu/client/gui/listlayout.h>

namespace tu
{

namespace menu
{

class CSettings : public gui::CTabs
{
private:
	class CMenu* m_pMenu;
	gui::CWidget* m_pGeneralTab;
	gui::CWidget* m_pPlayerTab;
	gui::CWidget* m_pCharacterTab;
	gui::CWidget* m_pControlTab;
	gui::CWidget* m_pGraphicsTab;
	gui::CWidget* m_pSoundTab;
	
protected:
	gui::CWidget* CreateGeneralTab();
	gui::CWidget* CreatePlayerTab();

public:
	CSettings(CMenu* pMenu);
	
	class CMenu* Menu() { return m_pMenu; }
};

}

}

#endif

