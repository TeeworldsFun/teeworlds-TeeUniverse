// Copyright (c) 2015 Henritees

#ifndef GAME_SERVER_NETGUI_H
#define GAME_SERVER_NETGUI_H

#include <base/tl/array.h>
#include <game/server/gamecontroller.h>
#include <game/server/gamecontext.h>
#include <engine/shared/protocol.h>
#include <engine/server.h>

#include "player.h"

#define GUISET(name) void CreateGui_##name(int ClientID); void RemoveGui_##name(int ClientID);
#define GUIDEFINE(name, netmsgname, args...) \
		public: \
			void Do##name(int ClientID, int NetGuiElemID, vec4 Dimensions, args); \
		private: \
			array<CNetMsg_ModAPI_Sv_Gui##name> m_##name[MAX_CLIENTS]; public:

class CModAPIGui
{
	CGameContext *m_pGameServer;

	// methods for managing sets of GUIs
	GUISET(EmptyGui)

public:
	CModAPIGui(CGameContext *pGameServer) : m_pGameServer(pGameServer){}
	void RemoveElement(int ClientID, int Type, int NetGuiElemID);
	void RemoveAllElements(int ClientID, int Type);
	void RemoveEverything(int ClientID);

	void OnClientEnter(int ClientID);
	void OnClientDrop(int ClientID);
	void OnMessage(int MsgID, void *pRawMsg, int ClientID);

	// auto-generated declarations of functions
	#include <modapi/guidefines.h>
	#undef GUIDEFINE

protected:
	CGameContext *GameServer() const { return m_pGameServer; }

private:

	template<class T>
	void SendNetGui(int ClientID, T Msg);
	void RequestData(int ClientID, int Type, int NetGuiElemID);


};

#endif
