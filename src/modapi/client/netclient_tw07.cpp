#include <modapi/client/netclient_tw07.h>
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

CModAPI_NetClient_TW07::CModAPI_NetClient_TW07(CModAPI_MetaNetClient* MetaNetClient, CModAPI_MetaNetClient::FProcessPacket fProcessServerPacket, CModAPI_MetaNetClient::FProcessPacket fProcessConnlessPacket) :
	CModAPI_MetaNetClient::CNetClient(MetaNetClient, fProcessServerPacket, fProcessConnlessPacket)
{
	
}

//NetServer :

bool CModAPI_NetClient_TW07::Open(int NetClientID, NETADDR BindAddr, int Flags)
{
	m_NetClientID = NetClientID;
	m_Address = BindAddr;
	
	// zero out the whole structure
	mem_zero(&m_Connection, sizeof(m_Connection));
	mem_zero(&m_RecvUnpacker, sizeof(m_RecvUnpacker));
	mem_zero(&m_TokenCache, sizeof(m_TokenCache));
	mem_zero(&m_TokenManager, sizeof(m_TokenManager));
	mem_zero(&m_Socket, sizeof(m_Socket));
	mem_zero(&m_Flags, sizeof(m_Flags));
	
	// open socket
	m_Socket = net_udp_create(BindAddr, (Flags&NETCREATE_FLAG_RANDOMPORT) ? 1 : 0);
	if(!m_Socket.type)
		return false;

	m_TokenManager.Init(m_Socket);
	m_TokenCache.Init(m_Socket, &m_TokenManager);

	m_Connection.Init(m_Socket, false);

	m_Flags = Flags;

	return true;
}

bool CModAPI_NetClient_TW07::Connect(NETADDR *pAddr)
{
	m_Connection.Connect(pAddr);
	return true;
}

bool CModAPI_NetClient_TW07::Disconnect(const char* pReason)
{
	m_Connection.Disconnect(pReason);
	return true;
}

bool CModAPI_NetClient_TW07::Update()
{
	m_Connection.Update();
	if(m_Connection.State() == NET_CONNSTATE_ERROR)
		Disconnect(m_Connection.ErrorString());
	m_TokenManager.Update();
	m_TokenCache.Update();
	return true;
}

bool CModAPI_NetClient_TW07::Recv(CNetChunk *pChunk, TOKEN* pResponseToken)
{
	while(1)
	{
		// check for a chunk
		if(m_RecvUnpacker.FetchChunk(pChunk))
			return true;

		// TODO: empty the recvinfo
		NETADDR Addr;
		int Bytes = net_udp_recv(m_Socket, &Addr, m_RecvUnpacker.m_aBuffer, NET_MAX_PACKETSIZE);

		// no more packets for now
		if(Bytes <= 0)
			break;

		if(CNetBase::UnpackPacket(m_RecvUnpacker.m_aBuffer, Bytes, &m_RecvUnpacker.m_Data) == 0)
		{
			if(m_Connection.State() != NET_CONNSTATE_OFFLINE && m_Connection.State() != NET_CONNSTATE_ERROR && net_addr_comp(m_Connection.PeerAddress(), &Addr) == 0)
			{
				if(m_Connection.Feed(&m_RecvUnpacker.m_Data, &Addr))
				{
					if(!(m_RecvUnpacker.m_Data.m_Flags&NET_PACKETFLAG_CONNLESS))
						m_RecvUnpacker.Start(&Addr, &m_Connection, 0);
				}
			}
			else
			{
				int Accept = m_TokenManager.ProcessMessage(&Addr, &m_RecvUnpacker.m_Data, true);
				if(!Accept)
					continue;

				if(m_RecvUnpacker.m_Data.m_Flags&NET_PACKETFLAG_CONTROL)
				{
					if(m_RecvUnpacker.m_Data.m_aChunkData[0] == NET_CTRLMSG_TOKEN)
						m_TokenCache.AddToken(&Addr, m_RecvUnpacker.m_Data.m_ResponseToken);
				}
				else if(m_RecvUnpacker.m_Data.m_Flags&NET_PACKETFLAG_CONNLESS)
				{
					pChunk->m_Flags = NETSENDFLAG_CONNLESS;

					if(Accept < 0)
					{
						if(!(m_Flags&NETCREATE_FLAG_ALLOWSTATELESS))
							continue;
						pChunk->m_Flags |= NETSENDFLAG_STATELESS;
					}
					pChunk->m_ClientID = -1;
					pChunk->m_Address = Addr;
					pChunk->m_DataSize = m_RecvUnpacker.m_Data.m_DataSize;
					pChunk->m_pData = m_RecvUnpacker.m_Data.m_aChunkData;

					if(pResponseToken)
						*pResponseToken = m_RecvUnpacker.m_Data.m_ResponseToken;
					return true;
				}
			}
		}
	}
	return false;
}

bool CModAPI_NetClient_TW07::RecvLoop()
{
	CNetChunk Packet;
	while(Recv(&Packet))
	{
		if(Packet.m_Flags&NETSENDFLAG_CONNLESS)
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

bool CModAPI_NetClient_TW07::Send(CNetChunk *pChunk, TOKEN Token)
{
	if(pChunk->m_Flags&NETSENDFLAG_CONNLESS)
	{
		if(pChunk->m_DataSize >= NET_MAX_PAYLOAD)
		{
			dbg_msg("netserver", "packet payload too big. %d. dropping packet", pChunk->m_DataSize);
			return -1;
		}

		if(pChunk->m_ClientID == -1 && net_addr_comp(&pChunk->m_Address, m_Connection.PeerAddress()) == 0)
		{
			// upgrade the packet, now that we know its recipent
			pChunk->m_Flags &= ~NETSENDFLAG_STATELESS;
			pChunk->m_ClientID = 0;
		}


		if(pChunk->m_Flags&NETSENDFLAG_STATELESS || Token != NET_TOKEN_NONE)
		{
			if(pChunk->m_Flags&NETSENDFLAG_STATELESS)
			{
				dbg_assert(pChunk->m_ClientID == -1, "errornous client id, connless packets can only be sent to cid=-1");
				dbg_assert(Token == NET_TOKEN_NONE, "stateless packets can't have a token");
			}
			CNetBase::SendPacketConnless(m_Socket, &pChunk->m_Address, Token, m_TokenManager.GenerateToken(&pChunk->m_Address), pChunk->m_pData, pChunk->m_DataSize);
		}
		else
		{
			if(pChunk->m_ClientID == -1)
			{
				m_TokenCache.SendPacketConnless(&pChunk->m_Address, pChunk->m_pData, pChunk->m_DataSize);
			}
			else
			{
				dbg_assert(pChunk->m_ClientID == 0, "errornous client id");
				m_Connection.SendPacketConnless((const char *)pChunk->m_pData, pChunk->m_DataSize);
			}
		}
	}
	else
	{
		if(pChunk->m_DataSize+NET_MAX_CHUNKHEADERSIZE >= NET_MAX_PAYLOAD)
		{
			dbg_msg("netclient", "chunk payload too big. %d. dropping chunk", pChunk->m_DataSize);
			return -1;
		}

		int Flags = 0;
		dbg_assert(pChunk->m_ClientID == 0, "errornous client id");

		if(pChunk->m_Flags&NETSENDFLAG_VITAL)
			Flags = NET_CHUNKFLAG_VITAL;

		m_Connection.QueueChunk(Flags, pChunk->m_DataSize, pChunk->m_pData);

		if(pChunk->m_Flags&NETSENDFLAG_FLUSH)
			m_Connection.Flush();
	}
	return 0;
}

bool CModAPI_NetClient_TW07::GotProblems() const
{
	return (time_get() - m_Connection.LastRecvTime() > time_freq());
}

int CModAPI_NetClient_TW07::State() const
{
	if(m_Connection.State() == NET_CONNSTATE_ONLINE)
		return NETSTATE_ONLINE;
	if(m_Connection.State() == NET_CONNSTATE_OFFLINE)
		return NETSTATE_OFFLINE;
	return NETSTATE_CONNECTING;
}

NETSOCKET CModAPI_NetClient_TW07::Socket() const
{
	return m_Socket;
}

int CModAPI_NetClient_TW07::NetType() const
{
	return (time_get() - m_Connection.LastRecvTime() > time_freq());
}

const char* CModAPI_NetClient_TW07::ErrorString() const
{
	return m_Connection.ErrorString();
}

void CModAPI_NetClient_TW07::ResetErrorString()
{
	m_Connection.ResetErrorString();
}

