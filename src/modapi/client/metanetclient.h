#ifndef MODAPI_CLIENT_METANETSERVER_H
#define MODAPI_CLIENT_METANETSERVER_H

#include <base/tl/array.h>
#include <engine/shared/network.h>
#include <engine/shared/packer.h>
#include <engine/masterserver.h>

class CModAPI_MetaNetClient
{
public:
	typedef void (*FProcessPacket)(CNetChunk *pPacket, void *pUser);

	enum
	{
		DST_SERVER07 = 0,
		DST_SERVER06,
		DST_MASTER07,
		DST_MASTER06,
		NUM_DST,
		DST_SERVER,
	};

public:
	class CNetClient
	{
	protected:
		CModAPI_MetaNetClient* m_pMetaNetClient;
		int m_NetClientID;
		NETADDR m_Address;
		
	public:
		FProcessPacket m_fProcessServerPacket;
		FProcessPacket m_fProcessConnlessPacket;
		
	public:
		CNetClient(CModAPI_MetaNetClient* pMetaNetClient, FProcessPacket fProcessServerPacket, FProcessPacket fProcessConnlessPacket);
		virtual ~CNetClient();
		
		//NetClient
		virtual bool Open(int NetClientID, NETADDR BindAddr, int Flags) = 0;
		virtual bool Connect(NETADDR *pAddr) = 0;
		virtual bool Disconnect(const char* pReason) = 0;
		virtual bool Update() = 0;
		virtual bool RecvLoop() = 0;
		virtual bool Send(CNetChunk *pChunk, TOKEN Token = NET_TOKEN_NONE) = 0;
		virtual bool GotProblems() const = 0;
		virtual int State() const = 0;
		virtual int NetType() const = 0;
		virtual const char* ErrorString() const = 0;
		virtual void ResetErrorString() = 0;
	};

public:
	CNetClient* m_apNetClient[NUM_DST];
	
private:
	class IMasterServer *m_pMasterServer;
	class IConsole *m_pConsole;

public:
	void* m_pData;
	int m_DstServerID;

public:
	CModAPI_MetaNetClient();
	~CModAPI_MetaNetClient();
	
	void Init(class IMasterServer *pMasterServer, class IConsole *pConsole);
	void SetCallbacks(void* pData);
	
	bool OpenNetClient(int Dst, CNetClient* pNetClient, NETADDR BindAddr, int Flags);
	bool Connect(int Dst, NETADDR *pAddr);
	bool Disconnect(int Dst, const char* pReason);
	bool Update();
	bool RecvLoop();
	bool Send(int Dst, CNetChunk *pChunk, TOKEN Token = NET_TOKEN_NONE);
	bool GotProblems(int Dst) const;
	int State(int Dst) const;
	int NetType(int Dst) const;
	const char* ErrorString(int Dst) const;
	void ResetErrorString(int Dst);
	
	class IMasterServer* MasterServer() { return m_pMasterServer; }
	class IConsole* Console() { return m_pConsole; }
};

#endif
