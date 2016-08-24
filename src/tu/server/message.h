#ifndef TU_MESSAGE_H
#define TU_MESSAGE_H

class IServer;
class CGameContext;

namespace tu
{

class CMsg
{
private:
	IServer* m_pServer;
	CGameContext* m_pGameServer;
	
public:
	CMsg(CGameContext* pGameServer);
	
	IServer* Server();
	CGameContext* GameServer();
};

/* CHAT **********************************************************/

class CMsg_Chat : public CMsg
{
protected:
	int m_Alternative;
	
public:
	CMsg_Chat(CGameContext* pGameServer, int Alternative);
	void Send(int ClientID, const char* pMessage);
};

/* BROADCAST **********************************************************/

enum
{
	TUALT_BROADCAST_NONE = 0,
	TUALT_BROADCAST_CHAT,
	TUALT_BROADCAST_MOTD
};

class CMsg_Broadcast : public CMsg
{
protected:
	int m_Alternative;
	
public:
	CMsg_Broadcast(CGameContext* pGameServer, int Alternative);
	void Send(int ClientID, const char* pMessage);
};

}

#endif
