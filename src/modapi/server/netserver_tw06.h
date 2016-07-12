#ifndef MODAPI_SERVER_NETSERVER_TW06_H
#define MODAPI_SERVER_NETSERVER_TW06_H

#include <modapi/server/metanetserver.h>
#include <tw06/network.h>

class CModAPI_NetServer_TW06 : public CModAPI_MetaNetServer::CNetServer
{
private:
	//NetServer
	struct CSlot
	{
	public:
		CTW06_NetConnection m_Connection;
	};

	NETSOCKET m_Socket;
	CSlot m_aSlots[TW06_NET_MAX_CLIENTS];

	CTW06_NetRecvUnpacker m_RecvUnpacker;
	
	//Register
	enum
	{
		REGISTERSTATE_START=0,
		REGISTERSTATE_UPDATE_ADDRS,
		REGISTERSTATE_QUERY_COUNT,
		REGISTERSTATE_HEARTBEAT,
		REGISTERSTATE_REGISTERED,
		REGISTERSTATE_ERROR
	};

	struct CMasterserverInfo
	{
		NETADDR m_Addr;
		int m_Count;
		int m_Valid;
		int64 m_LastSend;
	};

	int m_RegisterState;
	int64 m_RegisterStateStart;
	int m_RegisterFirst;
	int m_RegisterCount;

	CMasterserverInfo m_aMasterserverInfo[IMasterServer::MAX_MASTERSERVERS];
	int m_RegisterRegisteredServer;

private:
	void RegisterNewState(int State);
	void RegisterSendFwcheckresponse(NETADDR *pAddr, TOKEN Token);
	void RegisterSendHeartbeat(NETADDR Addr);
	void RegisterSendCountRequest(NETADDR Addr);
	void RegisterGotCount(struct CNetChunk *pChunk);

public:
	CModAPI_NetServer_TW06(CModAPI_MetaNetServer* MetaNetServer, CModAPI_MetaNetServer::FProcessClientPacket fProcessClientPacket, CModAPI_MetaNetServer::FGenerateServerInfo fGenerateServerInfo);
	
	//NetServer
	virtual bool Open(int NetServerID, NETADDR BindAddr, int Flags);
	virtual bool ApplyDrop(int ClientID, const char* pReason);
	virtual bool Send(CNetChunk *pChunk, TOKEN Token = NET_TOKEN_NONE);
	virtual bool Recv(CNetChunk *pChunk, TOKEN *pResponseToken = 0);
	virtual bool RecvLoop();
	virtual bool Update();
	virtual int NetType() const;
	virtual NETSOCKET Socket() const;

	virtual const NETADDR* ClientAddr(int ClientID) const;
	
	//Register
	virtual void RegisterUpdate();
	virtual int RegisterProcessPacket(struct CNetChunk *pPacket, TOKEN Token);
};

#endif
