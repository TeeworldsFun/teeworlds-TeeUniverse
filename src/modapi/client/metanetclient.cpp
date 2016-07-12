#include <modapi/client/metanetclient.h>
#include <mastersrv/mastersrv.h>

#include <engine/config.h>
#include <engine/console.h>
#include <engine/engine.h>
#include <engine/masterserver.h>

#include <engine/shared/config.h>

/* CNetClient *********************************************************/

CModAPI_MetaNetClient::CNetClient::CNetClient(CModAPI_MetaNetClient* pMetaNetClient, FProcessPacket fProcessServerPacket, FProcessPacket fProcessConnlessPacket) :
	m_pMetaNetClient(pMetaNetClient),
	m_fProcessServerPacket(fProcessServerPacket),
	m_fProcessConnlessPacket(fProcessConnlessPacket),
	m_NetClientID(-1)
{
	
}

CModAPI_MetaNetClient::CNetClient::~CNetClient()
{
	
}

/* CModAPI_MetaNetClient **********************************************/

CModAPI_MetaNetClient::CModAPI_MetaNetClient() :
	m_pData(0),
	m_ServerNetClient(-1),
	m_MasterNetClient(-1)
{
	
}

CModAPI_MetaNetClient::~CModAPI_MetaNetClient()
{
	for(int i=0; i<m_lpNetClient.size(); i++)
	{
		delete m_lpNetClient[i];
	}
}

void CModAPI_MetaNetClient::Init(IMasterServer *pMasterServer, IConsole *pConsole)
{
	m_pMasterServer = pMasterServer;
	m_pConsole = pConsole;
}

void CModAPI_MetaNetClient::SetCallbacks(void* pData)
{
	m_pData = pData;
}

bool CModAPI_MetaNetClient::OpenServerNetClient(CNetClient* pNetClient, NETADDR BindAddr, int Flags)
{
	int NetClientID = m_lpNetClient.size();
	bool res = pNetClient->Open(NetClientID, BindAddr, Flags);
	if(res)
	{
		m_lpNetClient.add(pNetClient);
	}
	m_ServerNetClient = NetClientID;
	return res;
}

bool CModAPI_MetaNetClient::OpenMasterNetClient(CNetClient* pNetClient, NETADDR BindAddr, int Flags)
{
	int NetClientID = m_lpNetClient.size();
	bool res = pNetClient->Open(NetClientID, BindAddr, Flags);
	if(res)
	{
		m_lpNetClient.add(pNetClient);
	}
	m_MasterNetClient = NetClientID;
	return res;
}

bool CModAPI_MetaNetClient::Connect(int Dst, NETADDR *pAddr)
{
	switch(Dst)
	{
		case CModAPI_MetaNetClient::DST_SERVER:
			if(m_ServerNetClient >= 0 && m_ServerNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_ServerNetClient]->Connect(pAddr);
			break;
		case CModAPI_MetaNetClient::DST_MASTER:
			if(m_MasterNetClient >= 0 && m_MasterNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_MasterNetClient]->Connect(pAddr);
			break;
	}
	
	return false;
}

bool CModAPI_MetaNetClient::Disconnect(int Dst, const char* pReason)
{
	switch(Dst)
	{
		case CModAPI_MetaNetClient::DST_SERVER:
			if(m_ServerNetClient >= 0 && m_ServerNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_ServerNetClient]->Disconnect(pReason);
			break;
		case CModAPI_MetaNetClient::DST_MASTER:
			if(m_MasterNetClient >= 0 && m_MasterNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_MasterNetClient]->Disconnect(pReason);
			break;
	}
	
	return false;
}

bool CModAPI_MetaNetClient::Update()
{
	for(int i=0; i<m_lpNetClient.size(); i++)
	{
		m_lpNetClient[i]->Update();
	}
}

bool CModAPI_MetaNetClient::RecvLoop()
{
	for(int i=0; i<m_lpNetClient.size(); i++)
	{
		m_lpNetClient[i]->RecvLoop();
	}
}

bool CModAPI_MetaNetClient::Send(int Dst, CNetChunk *pChunk, TOKEN Token)
{
	switch(Dst)
	{
		case CModAPI_MetaNetClient::DST_SERVER:
			if(m_ServerNetClient >= 0 && m_ServerNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_ServerNetClient]->Send(pChunk, Token);
			break;
		case CModAPI_MetaNetClient::DST_MASTER:
			if(m_MasterNetClient >= 0 && m_MasterNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_MasterNetClient]->Send(pChunk, Token);
			break;
	}
	
	return false;
}

bool CModAPI_MetaNetClient::GotProblems(int Dst) const
{
	switch(Dst)
	{
		case CModAPI_MetaNetClient::DST_SERVER:
			if(m_ServerNetClient >= 0 && m_ServerNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_ServerNetClient]->GotProblems();
			break;
		case CModAPI_MetaNetClient::DST_MASTER:
			if(m_MasterNetClient >= 0 && m_MasterNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_MasterNetClient]->GotProblems();
			break;
	}
	
	return false;
}

int CModAPI_MetaNetClient::State(int Dst) const
{
	switch(Dst)
	{
		case CModAPI_MetaNetClient::DST_SERVER:
			if(m_ServerNetClient >= 0 && m_ServerNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_ServerNetClient]->State();
			break;
		case CModAPI_MetaNetClient::DST_MASTER:
			if(m_MasterNetClient >= 0 && m_MasterNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_MasterNetClient]->State();
			break;
	}
	
	return NET_CONNSTATE_OFFLINE;
}

int CModAPI_MetaNetClient::NetType(int Dst) const
{
	switch(Dst)
	{
		case CModAPI_MetaNetClient::DST_SERVER:
			if(m_ServerNetClient >= 0 && m_ServerNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_ServerNetClient]->NetType();
			break;
		case CModAPI_MetaNetClient::DST_MASTER:
			if(m_MasterNetClient >= 0 && m_MasterNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_MasterNetClient]->NetType();
			break;
	}
	
	return 0;
}

const char* CModAPI_MetaNetClient::ErrorString(int Dst) const
{
	switch(Dst)
	{
		case CModAPI_MetaNetClient::DST_SERVER:
			if(m_ServerNetClient >= 0 && m_ServerNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_ServerNetClient]->ErrorString();
			break;
		case CModAPI_MetaNetClient::DST_MASTER:
			if(m_MasterNetClient >= 0 && m_MasterNetClient < m_lpNetClient.size())
				return m_lpNetClient[m_MasterNetClient]->ErrorString();
			break;
	}
	
	return 0;
}

void CModAPI_MetaNetClient::ResetErrorString(int Dst)
{
	switch(Dst)
	{
		case CModAPI_MetaNetClient::DST_SERVER:
			if(m_ServerNetClient >= 0 && m_ServerNetClient < m_lpNetClient.size())
				m_lpNetClient[m_ServerNetClient]->ResetErrorString();
			break;
		case CModAPI_MetaNetClient::DST_MASTER:
			if(m_MasterNetClient >= 0 && m_MasterNetClient < m_lpNetClient.size())
				m_lpNetClient[m_MasterNetClient]->ResetErrorString();
			break;
	}
}
