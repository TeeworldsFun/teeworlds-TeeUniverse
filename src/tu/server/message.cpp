#include <tu/compatibility.h>
#include <tu/server/message.h>

#include <engine/server.h>

#include <game/server/gamecontext.h>

namespace tu
{

CMsg::CMsg(CGameContext* pGameServer) :
	m_pGameServer(pGameServer)
{
	
}

IServer* CMsg::Server()
{
	return m_pGameServer->Server();
}

CGameContext* CMsg::GameServer()
{
	return m_pGameServer;
}

}
