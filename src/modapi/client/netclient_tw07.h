#ifndef TU_SERVER_NETCLIENT_TW07_H
#define TU_SERVER_NETCLIENT_TW07_H

#include <modapi/client/metanetclient.h>

namespace tu
{

class CNetClient_TW07 : public CMetaNetClient::CNetClient
{
private:
	CNetConnection m_Connection;
	CNetRecvUnpacker m_RecvUnpacker;
	
	CNetTokenCache m_TokenCache;
	CNetTokenManager m_TokenManager;
	
	NETSOCKET m_Socket;
	int m_Flags;

private:
	bool Recv(CNetChunk *pChunk, TOKEN* pResponseToken = 0);

public:
	CNetClient_TW07(CMetaNetClient* MetaNetClient, CMetaNetClient::FProcessPacket fProcessServerPacket, CMetaNetClient::FProcessPacket fProcessConnlessPacket);
	
	//NetServer
	virtual bool Open(int NetServerID, NETADDR BindAddr, int Flags);
	virtual bool Connect(NETADDR *pAddr);
	virtual bool Disconnect(const char* pReason);
	virtual bool Update();
	virtual bool RecvLoop();
	virtual bool Send(CNetChunk *pChunk, TOKEN Token = NET_TOKEN_NONE);
	virtual bool GotProblems() const;
	virtual int State() const;
	virtual NETSOCKET Socket() const;
	virtual int NetType() const;
	virtual const char* ErrorString() const;
	virtual void ResetErrorString();
};

}

#endif
