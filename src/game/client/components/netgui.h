// Copyright (c) 2015 Henritees

#ifndef GAME_CLIENT_COMPONENTS_NETGUI_H
#define GAME_CLIENT_COMPONENTS_NETGUI_H

#include <base/tl/array.h>
#include <game/client/component.h>

class CModAPIGui : public CComponent
{
private:
	static void ConMemPrint(IConsole::IResult *pResult, void *pUserData);
	static void ConMemOptimize(IConsole::IResult *pResult, void *pUserData);

public:
	virtual void OnConsoleInit();
	virtual void OnReset();
	virtual void OnMessage(int MsgId, void *pRawMsg);

	int GetMemoryUsage();
	void OptimizeMemory();

	// automatically make a storage array for everything
	#define GUIDEFINE(name, netmsgname, args...) array<CNetMsg_ModAPI_Sv_Gui##name> m_ModAPI_Gui##name;
	#include <modapi/guidefines.h>
	#undef GUIDEFINE

	char m_aModAPI_GuiEditBoxContent[1024][1024]; // necessary because it cannot be stored into the NetMsg (CONST char...)


	// maxsort to get stuff into correct render order.
	template<class T>
	void SortGuiList(array<T> &elem)
	{
		int num = elem.size();
		if(num < 2)
			return;
		int max = 0;
		T temp;
		for(int i = 0; i < max; i++)
		{
			max = i;
			for(int j = i; j < num; j++)
			{
				if(elem[i].m_ID > elem[max].m_ID)
					max = i;
			}

			if(max == i)
				continue;
			temp = elem[i];
			elem[i] = elem[max];
			elem[max] = temp;
		}
	}

	void SendEvent(int Type, int NetGuiElementID)
	{
		CNetMsg_ModAPI_Cl_GuiTriggerEvent Msg;
		Msg.m_Type = Type;
		Msg.m_ID = NetGuiElementID;
		Client()->SendPackMsg(&Msg, MSGFLAG_VITAL);
	}

};

#endif
