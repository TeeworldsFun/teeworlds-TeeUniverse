#ifndef MODAPI_SERVER_METANETSERVER_H
#define MODAPI_SERVER_METANETSERVER_H

#include <base/tl/array.h>
#include <engine/shared/network.h>
#include <engine/shared/packer.h>
#include <engine/masterserver.h>

class CModAPI_MetaNetServer
{
public:
	typedef int (*FDeleteClient)(int ClientID, const char* pReason, void *pUser);
	typedef int (*FNewClient)(int ClientID, void *pUser);
	typedef void (*FProcessClientPacket)(CNetChunk *pPacket, void *pUser);
	typedef void (*FGenerateServerInfo)(CPacker *pPacker, int Token, void *pUser, int NetServer);

public:
	class CNetServer
	{
	protected:
		CModAPI_MetaNetServer* m_pMetaNetServer;
		int m_NetServerID;
		NETADDR m_Address;
		
	public:
		FProcessClientPacket m_fProcessClientPacket;
		FGenerateServerInfo m_fGenerateServerInfo;
		
	public:
		CNetServer(CModAPI_MetaNetServer* MetaNetServer, FProcessClientPacket fProcessClientPacket, FGenerateServerInfo fGenerateServerInfo);
		virtual ~CNetServer();
		
		//NetServer
		virtual bool Open(int NetServerID, NETADDR BindAddr, int Flags) = 0;
		virtual bool ApplyDrop(int ClientID, const char* pReason) = 0;
		virtual bool Send(CNetChunk *pChunk, TOKEN Token = NET_TOKEN_NONE) = 0;
		virtual bool Recv(CNetChunk *pChunk, TOKEN *pResponseToken = 0) = 0;
		virtual bool RecvLoop() = 0;
		virtual bool Update() = 0;
		virtual int NetType() const = 0;
		virtual NETSOCKET Socket() const = 0;
	
		virtual const NETADDR* ClientAddr(int ClientID) const = 0;
		
		//Register
		virtual void RegisterUpdate() = 0;
		virtual int RegisterProcessPacket(struct CNetChunk *pPacket, TOKEN Token) = 0;
	};

public:
	array<CNetServer*> m_lpNetServer;
	
private:
	int m_aClientNetServer[NET_MAX_CLIENTS]; //To associate a netserver to each client
	int m_MaxClients;
	int m_MaxClientsPerIP;
	class CNetBan *m_pNetBan;
	class IMasterServer *m_pMasterServer;
	class IConsole *m_pConsole;
	
	FNewClient m_fNewClient;
	FDeleteClient m_fDeleteClient;

public:
	void* m_pData;

public:
	CModAPI_MetaNetServer();
	~CModAPI_MetaNetServer();
	
	void Init(class CNetBan* pNetBan, int MaxClients, int MaxClientsPerIP, class IMasterServer *pMasterServer, class IConsole *pConsole);
	void SetCallbacks(FNewClient fNewClient, FDeleteClient fDeleteClient, void* pData);

	bool OpenNetServer(CNetServer* pNetServer, NETADDR BindAddr);
	bool Drop(int ClientID, const char* pReason);
	bool Send(CNetChunk *pChunk, TOKEN Token = NET_TOKEN_NONE);
	bool Update();
	bool RecvLoop();
	
	void NewClient(int NetServerID, int ClientID);
	bool ClientSlotFree(int ClientID);
	
	const NETADDR* ClientAddr(int ClientID) const;
	
	int NetType() const { return m_lpNetServer[0]->NetType(); }
	NETSOCKET Socket() const { return m_lpNetServer[0]->Socket(); }
	
	int MaxClients() const { return m_MaxClients; }
	int MaxClientsPerIP() const { return m_MaxClientsPerIP; }
	void SetMaxClientsPerIP(int MaxClientsPerIP) { m_MaxClientsPerIP = MaxClientsPerIP; }
	
	void RegisterUpdate();
	
	class CNetBan* NetBan() { return m_pNetBan; }
	class IMasterServer* MasterServer() { return m_pMasterServer; }
	class IConsole* Console() { return m_pConsole; }
};

#endif
