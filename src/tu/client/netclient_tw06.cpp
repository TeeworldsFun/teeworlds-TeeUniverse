#include <tu/client/netclient_tw06.h>
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

#include <tw06/network.h>

namespace tu
{

CNetClient_TW06::CNetClient_TW06(CMetaNetClient* MetaNetClient, CMetaNetClient::FProcessPacket fProcessServerPacket, CMetaNetClient::FProcessPacket fProcessConnlessPacket) :
	CMetaNetClient::CNetClient(MetaNetClient, fProcessServerPacket, fProcessConnlessPacket)
{
	
}

//NetServer :

bool CNetClient_TW06::Open(int NetClientID, NETADDR BindAddr, int Flags)
{
	m_NetClientID = NetClientID;
	m_Address = BindAddr;
	
	// zero out the whole structure
	mem_zero(&m_Connection, sizeof(m_Connection));
	mem_zero(&m_RecvUnpacker, sizeof(m_RecvUnpacker));
	mem_zero(&m_Socket, sizeof(m_Socket));
	mem_zero(&m_Flags, sizeof(m_Flags));
	
	// open socket
	m_Socket = net_udp_create(BindAddr, (Flags&NETCREATE_FLAG_RANDOMPORT) ? 1 : 0);
	if(!m_Socket.type)
		return false;

	m_Connection.Init(m_Socket, false);

	m_Flags = Flags;

	return true;
}

bool CNetClient_TW06::Connect(NETADDR *pAddr)
{
	m_Connection.Connect(pAddr);
	return true;
}

bool CNetClient_TW06::Disconnect(const char* pReason)
{
	m_Connection.Disconnect(pReason);
	return true;
}

bool CNetClient_TW06::Update()
{
	m_Connection.Update();
	if(m_Connection.State() == TW06_NET_CONNSTATE_ERROR)
		Disconnect(m_Connection.ErrorString());
	return 0;
}

bool CNetClient_TW06::Recv(CNetChunk *pChunk)
{	
	while(1)
	{
		// check for a chunk
		if(m_RecvUnpacker.FetchChunk(pChunk))
			return 1;

		// TODO: empty the recvinfo
		NETADDR Addr;
		int Bytes = net_udp_recv(m_Socket, &Addr, m_RecvUnpacker.m_aBuffer, TW06_NET_MAX_PACKETSIZE);

		// no more packets for now
		if(Bytes <= 0)
			break;

		if(CTW06_NetBase::UnpackPacket(m_RecvUnpacker.m_aBuffer, Bytes, &m_RecvUnpacker.m_Data) == 0)
		{
			if(m_RecvUnpacker.m_Data.m_Flags&TW06_NET_PACKETFLAG_CONNLESS)
			{
				pChunk->m_Flags = TW06_NETSENDFLAG_CONNLESS;
				pChunk->m_ClientID = -1;
				pChunk->m_Address = Addr;
				pChunk->m_DataSize = m_RecvUnpacker.m_Data.m_DataSize;
				pChunk->m_pData = m_RecvUnpacker.m_Data.m_aChunkData;
				return 1;
			}
			else
			{
				if(m_Connection.Feed(&m_RecvUnpacker.m_Data, &Addr))
					m_RecvUnpacker.Start(&Addr, &m_Connection, 0);
			}
		}
	}
	return 0;
}

bool CNetClient_TW06::RecvLoop()
{
	CNetChunk Packet;
	while(Recv(&Packet))
	{
		if(Packet.m_Flags&TW06_NETSENDFLAG_CONNLESS)
		{
			if(m_fProcessConnlessPacket)
				m_fProcessConnlessPacket(&Packet, m_pMetaNetClient->m_pData);
		}
		else
		{
			if(m_fProcessServerPacket)
				m_fProcessServerPacket(&Packet, m_pMetaNetClient->m_pData);
		}
	}
}

bool CNetClient_TW06::Send(CNetChunk *pChunk, TOKEN Token)
{
	if(pChunk->m_DataSize >= TW06_NET_MAX_PAYLOAD)
	{
		dbg_msg("netclient", "chunk payload too big. %d. dropping chunk", pChunk->m_DataSize);
		return -1;
	}

	if(pChunk->m_Flags&TW06_NETSENDFLAG_CONNLESS)
	{
		// send connectionless packet
		CTW06_NetBase::SendPacketConnless(m_Socket, &pChunk->m_Address, pChunk->m_pData, pChunk->m_DataSize);
	}
	else
	{
		int Flags = 0;
		dbg_assert(pChunk->m_ClientID == 0, "errornous client id");

		if(pChunk->m_Flags&TW06_NETSENDFLAG_VITAL)
			Flags = TW06_NET_CHUNKFLAG_VITAL;

		m_Connection.QueueChunk(Flags, pChunk->m_DataSize, pChunk->m_pData);

		if(pChunk->m_Flags&TW06_NETSENDFLAG_FLUSH)
			m_Connection.Flush();
	}
	return 0;
}

bool CNetClient_TW06::GotProblems() const
{
	if(time_get() - m_Connection.LastRecvTime() > time_freq())
		return 1;
	return 0;
}

int CNetClient_TW06::State() const
{
	if(m_Connection.State() == NET_CONNSTATE_ONLINE)
		return NETSTATE_ONLINE;
	if(m_Connection.State() == NET_CONNSTATE_OFFLINE)
		return NETSTATE_OFFLINE;
	return NETSTATE_CONNECTING;
}

NETSOCKET CNetClient_TW06::Socket() const
{
	return m_Socket;
}

int CNetClient_TW06::NetType() const
{
	return (time_get() - m_Connection.LastRecvTime() > time_freq());
}

const char* CNetClient_TW06::ErrorString() const
{
	return m_Connection.ErrorString();
}

void CNetClient_TW06::ResetErrorString()
{
	m_Connection.ResetErrorString();
}

}
