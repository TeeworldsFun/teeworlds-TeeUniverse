#include <tu/client/menu/settings.h>
#include <tu/client/menu/menu.h>
#include <tu/client/gui/expand.h>
#include <tu/client/gui/toggle.h>
#include <game/client/localization.h>
#include <tu/client/localization.h>
#include <engine/shared/config.h>

namespace tu
{

namespace menu
{

/* CONFIG CHECKBOX ****************************************************/

class CConfigCheckBox : public gui::CToggle
{
protected:
	int* m_pValue;
	
	virtual bool GetValue() const { return *m_pValue; }
	virtual void SetValue(bool Value) { *m_pValue = Value; }

public:
	CConfigCheckBox(gui::CContext* pContext, int* pValue, const CLocalizableString& LocalizableString) :
		gui::CToggle(pContext, LocalizableString),
		m_pValue(pValue)
	{ }
};

/* CONFIG DYNAMIC CAMERA **********************************************/

class CConfigDynamicCamera : public gui::CToggle
{
private:
	virtual bool GetValue() const { return (g_Config.m_ClMouseDeadzone > 0); }
	virtual void SetValue(bool Value)
	{
		if(Value)
		{
			g_Config.m_ClMouseFollowfactor = 60;
			g_Config.m_ClMouseMaxDistance = 1000;
			g_Config.m_ClMouseDeadzone = 300;
		}
		else
		{
			g_Config.m_ClMouseFollowfactor = 0;
			g_Config.m_ClMouseMaxDistance = 400;
			g_Config.m_ClMouseDeadzone = 0;
		}
	}

public:
	CConfigDynamicCamera(gui::CContext* pContext, const CLocalizableString& LocalizableString) :
		gui::CToggle(pContext, LocalizableString)
	{ }
	
	virtual void Update()
	{
		if(GetValue())
			SetIcon(CAssetPath::SpriteSystem(SPRITE_MENU_CHECKBOX_TRUE));
		else
			SetIcon(CAssetPath::SpriteSystem(SPRITE_MENU_CHECKBOX_FALSE));
		
		gui::CToggle::Update();
	}
};

/* CONFIG SHOW NAMEPLATES *********************************************/

class CConfigShowNameplates : public gui::CExpand
{
public:
	CConfigShowNameplates(gui::CContext* pContext) :
		gui::CExpand(pContext)
	{
		SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_MENU_LIST));
		SetTitle(new CConfigCheckBox(Context(), &g_Config.m_ClNameplates, _("Show name plates")));
		Add(new CConfigCheckBox(Context(), &g_Config.m_ClNameplatesAlways, _("Always show name plates")));
		Add(new CConfigCheckBox(Context(), &g_Config.m_ClNameplatesTeamcolors, _("Use team colors for name plates")));
	}
	
	virtual void Update()
	{
		if(g_Config.m_ClNameplates)
			ShowContent();
		else
			HideContent();
		
		gui::CExpand::Update();
	}
};

/* LANGUAGE BUTTON ****************************************************/

class CLanguageButton : public gui::CButton
{
private:
	char m_aFilename[128];
	
	virtual void MouseClickAction()
	{
		str_copy(g_Config.m_ClLanguage, m_aFilename, sizeof(g_Config.m_ClLanguage));
	}

public:
	CLanguageButton(gui::CContext* pContext, const char* pName, const char* pFilename) :
		gui::CButton(pContext, pName)
	{
		str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
	}
};

/* SETTINGS ***********************************************************/

CSettings::CSettings(CMenu* pMenu) :
	gui::CTabs(pMenu),
	m_pMenu(pMenu)
{
	SetTabsStyle(CAssetPath::GuiTabsStyleSystem(GUITABSSTYLE_MENU_MAIN));
	
	m_pGeneralTab = CreateGeneralTab();
	m_pPlayerTab = new gui::CVScrollLayout(pMenu);
	m_pCharacterTab = new gui::CVScrollLayout(pMenu);
	m_pControlTab = new gui::CVScrollLayout(pMenu);
	m_pGraphicsTab = new gui::CVScrollLayout(pMenu);
	m_pSoundTab = new gui::CVScrollLayout(pMenu);
	
	AddTab(m_pGeneralTab, _("General"), CAssetPath::Null());
	AddTab(m_pPlayerTab, _("Player"), CAssetPath::Null());
	AddTab(m_pCharacterTab, _("Tee"), CAssetPath::Null());
	AddTab(m_pControlTab, _("Controls"), CAssetPath::Null());
	AddTab(m_pGraphicsTab, _("Graphics"), CAssetPath::Null());
	AddTab(m_pSoundTab, _("Sound"), CAssetPath::Null());
}

gui::CWidget* CSettings::CreateGeneralTab()
{
	gui::CHListLayout* pTab = new gui::CHListLayout(Context());
	pTab->SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_MENU_COLUMNS));
	
	gui::CVScrollLayout* pLeftList = new gui::CVScrollLayout(Context());
	pLeftList->SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_MENU_SECTIONLIST));
	pTab->Add(pLeftList, true);
	
	gui::CVScrollLayout* pRightList = new gui::CVScrollLayout(Context());
	pRightList->SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_MENU_SECTIONLIST));
	pTab->Add(pRightList, true);
	
	{
		gui::CVListLayout* pSection = new gui::CVListLayout(Context());
		pLeftList->Add(pSection, false);
		
		pSection->SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_MENU_SECTION));
		pSection->Add(new gui::CLabelHeader(Context(), _("Game")), false);
		
		pSection->Add(new CConfigDynamicCamera(Context(), _("Dynamic Camera")), false);
		pSection->Add(new CConfigCheckBox(Context(), &g_Config.m_ClAutoswitchWeapons, _("Switch weapon on pickup")), false);
		pSection->Add(new CConfigCheckBox(Context(), &g_Config.m_ClShowhud, _("Show ingame HUD")), false);
		pSection->Add(new CConfigCheckBox(Context(), &g_Config.m_ClShowChatFriends, _("Show only chat messages from friends")), false);
		pSection->Add(new CConfigCheckBox(Context(), &g_Config.m_ClShowsocial, _("Show social")), false);
		pSection->Add(new CConfigShowNameplates(Context()), false);
	}
	
	{
		gui::CVListLayout* pSection = new gui::CVListLayout(Context());
		pLeftList->Add(pSection, false);
		
		pSection->SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_MENU_SECTION));
		pSection->Add(new gui::CLabelHeader(Context(), _("Client")), false);
		
		pSection->Add(new CConfigCheckBox(Context(), &g_Config.m_ClAutoDemoRecord, _("Automatically record demos")), false);
		pSection->Add(new CConfigCheckBox(Context(), &g_Config.m_ClAutoScreenshot, _("Automatically take game over screenshot")), false);
	}
	
	{
		gui::CVListLayout* pSection = new gui::CVListLayout(Context());
		pRightList->Add(pSection, true);
		
		pSection->SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_MENU_SECTION));
		pSection->Add(new gui::CLabelHeader(Context(), _("Language")), false);
		
		gui::CVScrollLayout* pLanguageList = new gui::CVScrollLayout(Context());
		pLanguageList->SetBoxStyle(CAssetPath::GuiBoxStyleSystem(GUIBOXSTYLE_MENU_LIST));
		pSection->Add(pLanguageList, true);
		
		pLanguageList->Add(new CLanguageButton(Context(), "English", ""), false);
		for(int i=0; i<Localization()->m_pLanguages.size(); i++)
		{
			pLanguageList->Add(new CLanguageButton(Context(), Localization()->m_pLanguages[i]->GetName(), Localization()->m_pLanguages[i]->GetFilename()), false);
		}
	}
	
	return pTab;
}

}

}

