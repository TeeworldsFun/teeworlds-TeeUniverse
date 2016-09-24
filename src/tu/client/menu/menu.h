#ifndef TU_CLIENT_MENU_H
#define TU_CLIENT_MENU_H

#include <tu/client/gui/context.h>

namespace tu
{

namespace menu
{

class CMenu : public gui::CContext
{
protected:
	class CSettings* m_pSettings;
	
public:
	CMenu(CKernel* pKernel);
	
	virtual void CreateMainWidget();
	virtual void DoShortcuts();
	virtual void RenderBackground();
	void Close();
};

}

}

#endif
