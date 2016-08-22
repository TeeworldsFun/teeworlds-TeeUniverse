#ifndef TU_SERVER_NETCLIENT_TW06_H
#define TU_SERVER_NETCLIENT_TW06_H

#include <modapi/client/metanetclient.h>
#include <tw06/network.h>

namespace tu
{

class CNetClient_TW06 : public CMetaNetClient::CNetClient
{
private:
	CTW06_NetConnection m_Connection;
	CTW06_NetRecvUnpacker m_RecvUnpacker;
	
	NETSOCKET m_Socket;
	int m_Flags;

private:
	bool Recv(CNetChunk *pChunk);

public:
	CNetClient_TW06(CMetaNetClient* MetaNetClient, CMetaNetClient::FProcessPacket fProcessServerPacket, CMetaNetClient::FProcessPacket fProcessConnlessPacket);
	
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
