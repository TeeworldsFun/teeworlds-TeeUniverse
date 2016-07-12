#include <modapi/server/metanetserver.h>
#include <mastersrv/mastersrv.h>

#include <engine/config.h>
#include <engine/console.h>
#include <engine/engine.h>
#include <engine/map.h>
#include <engine/masterserver.h>
#include <engine/server.h>
#include <engine/storage.h>

#include <engine/shared/config.h>
#include <engine/shared/netban.h>

/* CNetServer *********************************************************/

CModAPI_MetaNetServer::CNetServer::CNetServer(CModAPI_MetaNetServer* pMetaNetServer, FProcessClientPacket fProcessClientPacket, FGenerateServerInfo fGenerateServerInfo) :
	m_pMetaNetServer(pMetaNetServer),
	m_fProcessClientPacket(fProcessClientPacket),
	m_fGenerateServerInfo(fGenerateServerInfo),
	m_NetServerID(-1)
{
	
}

CModAPI_MetaNetServer::CNetServer::~CNetServer()
{
	
}

/* CModAPI_MetaNetServer **********************************************/

CModAPI_MetaNetServer::CModAPI_MetaNetServer() :
	m_fNewClient(0),
	m_fDeleteClient(0),
	m_pData(0)
{
	
}

CModAPI_MetaNetServer::~CModAPI_MetaNetServer()
{
	for(int i=0; i<m_lpNetServer.size(); i++)
	{
		delete m_lpNetServer[i];
	}
}

void CModAPI_MetaNetServer::Init(class CNetBan* pNetBan, int MaxClients, int MaxClientsPerIP, IMasterServer *pMasterServer, IConsole *pConsole)
{
	// clamp clients
	m_MaxClients = MaxClients;
	if(m_MaxClients > NET_MAX_CLIENTS)
		m_MaxClients = NET_MAX_CLIENTS;
	if(m_MaxClients < 1)
		m_MaxClients = 1;
	
	m_MaxClientsPerIP = MaxClientsPerIP;
	m_pNetBan = pNetBan;
	m_pMasterServer = pMasterServer;
	m_pConsole = pConsole;
	
	for(int i=0; i<MaxClients; i++)
	{
		m_aClientNetServer[i] = -1;
	}
}

void CModAPI_MetaNetServer::SetCallbacks(FNewClient fNewClient, FDeleteClient fDeleteClient, void* pData)
{
	m_fNewClient = fNewClient;
	m_fDeleteClient = fDeleteClient;
	m_pData = pData;
}

bool CModAPI_MetaNetServer::OpenNetServer(CNetServer* pNetServer, NETADDR BindAddr)
{
	int NetServerID = m_lpNetServer.size();
	bool res = pNetServer->Open(NetServerID, BindAddr, NETCREATE_FLAG_ALLOWSTATELESS);
	if(res)
	{
		m_lpNetServer.add(pNetServer);
	}
	return res;
}

bool CModAPI_MetaNetServer::Drop(int ClientID, const char* pReason)
{
	if(ClientID < 0 || ClientID >= NET_MAX_CLIENTS || m_aClientNetServer[ClientID] < 0)
		return false;
	
	if(m_fDeleteClient)
		m_fDeleteClient(ClientID, pReason, m_pData);
	
	int NetServerID = m_aClientNetServer[ClientID];
	m_aClientNetServer[ClientID] = -1;
	return m_lpNetServer[NetServerID]->ApplyDrop(ClientID, pReason);
}
	
void CModAPI_MetaNetServer::NewClient(int NetServerID, int ClientID)
{
	m_aClientNetServer[ClientID] = NetServerID;
	
	if(m_fNewClient)
		m_fNewClient(ClientID, m_pData);
}

bool CModAPI_MetaNetServer::ClientSlotFree(int ClientID)
{
	return m_aClientNetServer[ClientID] < 0;
}

bool CModAPI_MetaNetServer::Send(CNetChunk *pChunk, TOKEN Token)
{
	int ClientID = pChunk->m_ClientID;
	if(ClientID < 0 || ClientID >= NET_MAX_CLIENTS || m_aClientNetServer[ClientID] < 0)
		return false;
	
	int NetServerID = m_aClientNetServer[ClientID];
	if(NetServerID >= m_lpNetServer.size())
		return false;
		
	return m_lpNetServer[NetServerID]->Send(pChunk, Token);
}

bool CModAPI_MetaNetServer::Update()
{
	for(int i=0; i<m_lpNetServer.size(); i++)
	{
		m_lpNetServer[i]->Update();
	}
	return true;
}

bool CModAPI_MetaNetServer::RecvLoop()
{
	for(int i=0; i<m_lpNetServer.size(); i++)
	{
		m_lpNetServer[i]->RecvLoop();
	}
	
	return true;
}

const NETADDR* CModAPI_MetaNetServer::ClientAddr(int ClientID) const
{
	if(ClientID < 0 || ClientID >= NET_MAX_CLIENTS || m_aClientNetServer[ClientID] < 0)
		return 0;
	
	int NetServerID = m_aClientNetServer[ClientID];
	return m_lpNetServer[NetServerID]->ClientAddr(ClientID);
}

void CModAPI_MetaNetServer::RegisterUpdate()
{
	if(!g_Config.m_SvRegister)
		return;

	MasterServer()->Update();
	
	for(int i=0; i<m_lpNetServer.size(); i++)
	{
		m_lpNetServer[i]->RegisterUpdate();
	}
}
