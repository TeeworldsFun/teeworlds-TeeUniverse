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
	gui::CVListLayout* m_pGeneralTab;
	gui::CVListLayout* m_pPlayerTab;
	gui::CVListLayout* m_pCharacterTab;
	gui::CVListLayout* m_pControlTab;
	gui::CVListLayout* m_pGraphicsTab;
	gui::CVListLayout* m_pSoundTab;
	
protected:
	gui::CVListLayout* CreateGeneralTab();

public:
	CSettings(CMenu* pMenu);
	
	class CMenu* Menu() { return m_pMenu; }
};

}

}

#endif

