#include <modapi/client/metanetclient.h>
#include <mastersrv/mastersrv.h>

#include <engine/config.h>
#include <engine/console.h>
#include <engine/engine.h>
#include <engine/masterserver.h>

#include <engine/shared/config.h>

namespace tu
{

/* CNetClient *********************************************************/

CMetaNetClient::CNetClient::CNetClient(CMetaNetClient* pMetaNetClient, FProcessPacket fProcessServerPacket, FProcessPacket fProcessConnlessPacket) :
	m_pMetaNetClient(pMetaNetClient),
	m_fProcessServerPacket(fProcessServerPacket),
	m_fProcessConnlessPacket(fProcessConnlessPacket),
	m_NetClientID(DST_SERVER07)
{
	
}

CMetaNetClient::CNetClient::~CNetClient()
{
	
}

/* CMetaNetClient **********************************************/

CMetaNetClient::CMetaNetClient() :
	m_pData(0),
	m_DstServerID(-1)
{
	
}

CMetaNetClient::~CMetaNetClient()
{
	for(int i=0; i<NUM_DST; i++)
	{
		if(m_apNetClient[i])
			delete m_apNetClient[i];
	}
}

void CMetaNetClient::Init(IMasterServer *pMasterServer, IConsole *pConsole)
{
	m_pMasterServer = pMasterServer;
	m_pConsole = pConsole;
}

void CMetaNetClient::SetCallbacks(void* pData)
{
	m_pData = pData;
}

bool CMetaNetClient::OpenNetClient(int Dst, CNetClient* pNetClient, NETADDR BindAddr, int Flags)
{
	if(Dst >= NUM_DST || Dst < 0)
		return false;
	
	bool res = pNetClient->Open(Dst, BindAddr, Flags);
	if(res)
	{
		if(m_apNetClient[Dst])
			delete m_apNetClient[Dst];
		
		m_apNetClient[Dst] = pNetClient;
	}
	
	return res;
}

bool CMetaNetClient::Connect(int Dst, NETADDR *pAddr)
{
	if(m_apNetClient[Dst])
	{
		if(m_apNetClient[Dst]->Connect(pAddr))
		{
			m_DstServerID = Dst;
			return true;
		}
	}
	
	return false;
}

bool CMetaNetClient::Disconnect(int Dst, const char* pReason)
{
	if(Dst == DST_SERVER)
		Dst = m_DstServerID;
	
	if(m_apNetClient[Dst])
		return m_apNetClient[Dst]->Disconnect(pReason);
	
	return false;
}

bool CMetaNetClient::Update()
{
	for(int i=0; i<NUM_DST; i++)
	{
		m_apNetClient[i]->Update();
	}
}

bool CMetaNetClient::RecvLoop()
{
	for(int i=0; i<NUM_DST; i++)
	{
		m_apNetClient[i]->RecvLoop();
	}
}

bool CMetaNetClient::Send(int Dst, CNetChunk *pChunk, TOKEN Token)
{
	if(Dst == DST_SERVER)
		Dst = m_DstServerID;
	
	if(m_apNetClient[Dst])
		return m_apNetClient[Dst]->Send(pChunk, Token);
	
	return false;
}

bool CMetaNetClient::GotProblems(int Dst) const
{
	if(Dst == DST_SERVER)
		Dst = m_DstServerID;
	
	if(m_apNetClient[Dst])
		return m_apNetClient[Dst]->GotProblems();
	
	return false;
}

int CMetaNetClient::State(int Dst) const
{
	if(Dst == DST_SERVER)
		Dst = m_DstServerID;
	
	if(m_apNetClient[Dst])
		return m_apNetClient[Dst]->State();
	
	return NET_CONNSTATE_OFFLINE;
}

int CMetaNetClient::NetType(int Dst) const
{
	if(Dst == DST_SERVER)
		Dst = m_DstServerID;
	
	if(m_apNetClient[Dst])
		return m_apNetClient[Dst]->NetType();
	
	return 0;
}

const char* CMetaNetClient::ErrorString(int Dst) const
{
	if(Dst == DST_SERVER)
		Dst = m_DstServerID;
	
	if(m_apNetClient[Dst])
		return m_apNetClient[Dst]->ErrorString();
	
	return 0;
}

void CMetaNetClient::ResetErrorString(int Dst)
{
	if(Dst == DST_SERVER)
		Dst = m_DstServerID;
	
	if(m_apNetClient[Dst])
		m_apNetClient[Dst]->ResetErrorString();
}

}
