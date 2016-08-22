#include <modapi/server/netserver_tw07.h>
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

namespace tu
{

CNetServer_TW07::CNetServer_TW07(CMetaNetServer* MetaNetServer, CMetaNetServer::FProcessClientPacket fProcessClientPacket, CMetaNetServer::FGenerateServerInfo fGenerateServerInfo) :
	CMetaNetServer::CNetServer(MetaNetServer, fProcessClientPacket, fGenerateServerInfo)
{
	m_RegisterState = REGISTERSTATE_START;
	m_RegisterStateStart = 0;
	m_RegisterFirst = 1;
	m_RegisterCount = 0;

	mem_zero(m_aMasterserverInfo, sizeof(m_aMasterserverInfo));
	m_RegisterRegisteredServer = -1;
}

//NetServer :

bool CNetServer_TW07::Open(int NetServerID, NETADDR BindAddr, int Flags)
{
	m_NetServerID = NetServerID;
	m_Address = BindAddr;
	
	// zero out the whole structure
	mem_zero(&m_Socket, sizeof(m_Socket));
	mem_zero(&m_aSlots, sizeof(CSlot)*NET_MAX_CLIENTS);
	mem_zero(&m_RecvUnpacker, sizeof(m_RecvUnpacker));
	mem_zero(&m_TokenManager, sizeof(m_TokenManager));
	mem_zero(&m_Flags, sizeof(m_Flags));
	
	// open socket
	m_Socket = net_udp_create(BindAddr, 0);
	if(!m_Socket.type)
		return false;

	m_TokenManager.Init(m_Socket);
	m_TokenCache.Init(m_Socket, &m_TokenManager);

	for(int i = 0; i < NET_MAX_CLIENTS; i++)
		m_aSlots[i].m_Connection.Init(m_Socket, true);

	m_Flags = Flags;

	return true;
}

bool CNetServer_TW07::ApplyDrop(int ClientID, const char* pReason)
{
	m_aSlots[ClientID].m_Connection.Disconnect(pReason);
	return true;
}

bool CNetServer_TW07::Send(CNetChunk *pChunk, TOKEN Token)
{
	if(pChunk->m_Flags&NETSENDFLAG_CONNLESS)
	{
		if(pChunk->m_DataSize >= NET_MAX_PAYLOAD)
		{
			dbg_msg("netserver", "packet payload too big. %d. dropping packet", pChunk->m_DataSize);
			return false;
		}

		if(pChunk->m_ClientID == -1)
			for(int i = 0; i < m_pMetaNetServer->MaxClients(); i++)
				if(net_addr_comp(&pChunk->m_Address, m_aSlots[i].m_Connection.PeerAddress()) == 0)
				{
					// upgrade the packet, now that we know its recipent
					pChunk->m_Flags &= ~NETSENDFLAG_STATELESS;
					pChunk->m_ClientID = i;
					break;
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
				dbg_assert(pChunk->m_ClientID >= 0, "errornous client id");
				dbg_assert(pChunk->m_ClientID < m_pMetaNetServer->MaxClients(), "errornous client id");

				m_aSlots[pChunk->m_ClientID].m_Connection.SendPacketConnless((const char *)pChunk->m_pData, pChunk->m_DataSize);
			}
		}
	}
	else
	{
		if(pChunk->m_DataSize+NET_MAX_CHUNKHEADERSIZE >= NET_MAX_PAYLOAD)
		{
			dbg_msg("netclient", "chunk payload too big. %d. dropping chunk", pChunk->m_DataSize);
			return false;
		}

		int Flags = 0;
		dbg_assert(pChunk->m_ClientID >= 0, "errornous client id");
		dbg_assert(pChunk->m_ClientID < m_pMetaNetServer->MaxClients(), "errornous client id");

		if(pChunk->m_Flags&NETSENDFLAG_VITAL)
			Flags = NET_CHUNKFLAG_VITAL;

		if(m_aSlots[pChunk->m_ClientID].m_Connection.QueueChunk(Flags, pChunk->m_DataSize, pChunk->m_pData) == 0)
		{
			if(pChunk->m_Flags&NETSENDFLAG_FLUSH)
				m_aSlots[pChunk->m_ClientID].m_Connection.Flush();
		}
		else
		{
			m_pMetaNetServer->Drop(pChunk->m_ClientID, "Error sending data");
		}
	}
	return true;
}

bool CNetServer_TW07::Recv(CNetChunk *pChunk, TOKEN *pResponseToken)
{
	while(1)
	{
		NETADDR Addr;

		// check for a chunk
		if(m_RecvUnpacker.FetchChunk(pChunk))
			return true;

		// TODO: empty the recvinfo
		int Bytes = net_udp_recv(m_Socket, &Addr, m_RecvUnpacker.m_aBuffer, NET_MAX_PACKETSIZE);

		// no more packets for now
		if(Bytes <= 0)
			break;

		if(CNetBase::UnpackPacket(m_RecvUnpacker.m_aBuffer, Bytes, &m_RecvUnpacker.m_Data) == 0)
		{
			// check for bans
			char aBuf[128];
			if(m_pMetaNetServer->NetBan() && m_pMetaNetServer->NetBan()->IsBanned(&Addr, aBuf, sizeof(aBuf)))
			{
				// banned, reply with a message
				CNetBase::SendControlMsg(m_Socket, &Addr, m_RecvUnpacker.m_Data.m_ResponseToken, 0, NET_CTRLMSG_CLOSE, aBuf, str_length(aBuf)+1);
				continue;
			}

			bool Found = false;
			// try to find matching slot
			for(int i = 0; i < m_pMetaNetServer->MaxClients(); i++)
			{
				if(net_addr_comp(m_aSlots[i].m_Connection.PeerAddress(), &Addr) == 0)
				{
					if(m_aSlots[i].m_Connection.Feed(&m_RecvUnpacker.m_Data, &Addr))
					{
						if(m_RecvUnpacker.m_Data.m_DataSize)
						{
							if(!(m_RecvUnpacker.m_Data.m_Flags&NET_PACKETFLAG_CONNLESS))
								m_RecvUnpacker.Start(&Addr, &m_aSlots[i].m_Connection, i);
							else
							{
								pChunk->m_Flags = NETSENDFLAG_CONNLESS;
								pChunk->m_Address = *m_aSlots[i].m_Connection.PeerAddress();
								pChunk->m_ClientID = i;
								pChunk->m_DataSize = m_RecvUnpacker.m_Data.m_DataSize;
								pChunk->m_pData = m_RecvUnpacker.m_Data.m_aChunkData;
								if(pResponseToken)
									*pResponseToken = NET_TOKEN_NONE;
								return true;
							}
						}
					}
					Found = true;
				}
			}

			if(Found)
				continue;

			int Accept = m_TokenManager.ProcessMessage(&Addr, &m_RecvUnpacker.m_Data, true);

			if(m_RecvUnpacker.m_Data.m_Flags&NET_PACKETFLAG_CONTROL)
			{
				if(!Accept)
					continue;
				if(m_RecvUnpacker.m_Data.m_aChunkData[0] == NET_CTRLMSG_CONNECT)
				{
					bool Found = false;

					// only allow a specific number of players with the same ip
					NETADDR ThisAddr = Addr, OtherAddr;
					int FoundAddr = 1;
					ThisAddr.port = 0;
					for(int i = 0; i < m_pMetaNetServer->MaxClients(); i++)
					{
						if(m_aSlots[i].m_Connection.State() == NET_CONNSTATE_OFFLINE)
							continue;

						OtherAddr = *m_aSlots[i].m_Connection.PeerAddress();
						OtherAddr.port = 0;
						if(!net_addr_comp(&ThisAddr, &OtherAddr))
						{
							if(FoundAddr++ >= m_pMetaNetServer->MaxClientsPerIP())
							{
								char aBuf[128];
								str_format(aBuf, sizeof(aBuf), "Only %d players with the same IP are allowed", m_pMetaNetServer->MaxClientsPerIP());
								CNetBase::SendControlMsg(m_Socket, &Addr, m_RecvUnpacker.m_Data.m_ResponseToken, 0, NET_CTRLMSG_CLOSE, aBuf, str_length(aBuf) + 1);
								return false;
							}
						}
					}

					for(int i = 0; i < m_pMetaNetServer->MaxClients(); i++)
					{
						if(m_aSlots[i].m_Connection.State() == NET_CONNSTATE_OFFLINE && m_pMetaNetServer->ClientSlotFree(i))
						{
							Found = true;
							m_aSlots[i].m_Connection.SetToken(m_RecvUnpacker.m_Data.m_Token);
							m_aSlots[i].m_Connection.Feed(&m_RecvUnpacker.m_Data, &Addr);
							m_aSlots[i].m_Connection.SetToken(m_RecvUnpacker.m_Data.m_Token); // HACK!
							
							m_pMetaNetServer->NewClient(m_NetServerID, i);
							break;
						}
					}

					if(!Found)
					{
						const char FullMsg[] = "This server is full";
						CNetBase::SendControlMsg(m_Socket, &Addr, m_RecvUnpacker.m_Data.m_ResponseToken, 0, NET_CTRLMSG_CLOSE, FullMsg, sizeof(FullMsg));
					}
				}
				else if(m_RecvUnpacker.m_Data.m_aChunkData[0] == NET_CTRLMSG_TOKEN)
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
	return false;
}

bool CNetServer_TW07::RecvLoop()
{
	CNetChunk Packet;
	TOKEN ResponseToken;

	// process packets
	while(Recv(&Packet, &ResponseToken))
	{
		if(Packet.m_Flags&NETSENDFLAG_CONNLESS)
		{
			// stateless?
			if(!(Packet.m_Flags&NETSENDFLAG_STATELESS))
				if(RegisterProcessPacket(&Packet, ResponseToken))
					continue;
			if(Packet.m_DataSize >= int(sizeof(SERVERBROWSE_GETINFO)) &&
				mem_comp(Packet.m_pData, SERVERBROWSE_GETINFO, sizeof(SERVERBROWSE_GETINFO)) == 0)
			{
				CUnpacker Unpacker;
				Unpacker.Reset((unsigned char*)Packet.m_pData+sizeof(SERVERBROWSE_GETINFO), Packet.m_DataSize-sizeof(SERVERBROWSE_GETINFO));
				int SrvBrwsToken = Unpacker.GetInt();
				if(Unpacker.Error())
					continue;

				CPacker Packer;
				CNetChunk Response;
				
				m_fGenerateServerInfo(&Packer, SrvBrwsToken, m_pMetaNetServer->m_pData, m_NetServerID);

				Response.m_ClientID = -1;
				Response.m_Address = Packet.m_Address;
				Response.m_Flags = NETSENDFLAG_CONNLESS;
				Response.m_pData = Packer.Data();
				Response.m_DataSize = Packer.Size();
				Send(&Response, ResponseToken);
			}
		}
		else
		{
			if(m_fProcessClientPacket)
				m_fProcessClientPacket(&Packet, m_pMetaNetServer->m_pData);
		}
	}
	
	return true;
}

bool CNetServer_TW07::Update()
{
	int64 Now = time_get();
	for(int i = 0; i < m_pMetaNetServer->MaxClients(); i++)
	{
		m_aSlots[i].m_Connection.Update();
		if(m_aSlots[i].m_Connection.State() == NET_CONNSTATE_ERROR)
		{
			if(Now - m_aSlots[i].m_Connection.ConnectTime() < time_freq() && m_pMetaNetServer->NetBan())
			{
				if(m_pMetaNetServer->NetBan()->BanAddr(ClientAddr(i), 60, "Stressing network") == -1)
					m_pMetaNetServer->Drop(i, m_aSlots[i].m_Connection.ErrorString());
			}
			else
				m_pMetaNetServer->Drop(i, m_aSlots[i].m_Connection.ErrorString());
		}
	}

	m_TokenManager.Update();
	m_TokenCache.Update();

	return true;
}

int CNetServer_TW07::NetType() const
{
	return m_Socket.type;
}

NETSOCKET CNetServer_TW07::Socket() const
{
	return m_Socket;
}

const NETADDR* CNetServer_TW07::ClientAddr(int ClientID) const
{
	return m_aSlots[ClientID].m_Connection.PeerAddress();
}

//Register

void CNetServer_TW07::RegisterNewState(int State)
{
	m_RegisterState = State;
	m_RegisterStateStart = time_get();
}

void CNetServer_TW07::RegisterSendFwcheckresponse(NETADDR *pAddr, TOKEN Token)
{
	CNetChunk Packet;
	Packet.m_ClientID = -1;
	Packet.m_Address = *pAddr;
	Packet.m_Flags = NETSENDFLAG_CONNLESS;
	Packet.m_DataSize = sizeof(SERVERBROWSE_FWRESPONSE);
	Packet.m_pData = SERVERBROWSE_FWRESPONSE;
	Send(&Packet, Token);
}

void CNetServer_TW07::RegisterSendHeartbeat(NETADDR Addr)
{
	static unsigned char aData[sizeof(SERVERBROWSE_HEARTBEAT) + 2];
	unsigned short Port = m_Address.port;
	CNetChunk Packet;

	mem_copy(aData, SERVERBROWSE_HEARTBEAT, sizeof(SERVERBROWSE_HEARTBEAT));

	Packet.m_ClientID = -1;
	Packet.m_Address = Addr;
	Packet.m_Flags = NETSENDFLAG_CONNLESS;
	Packet.m_DataSize = sizeof(SERVERBROWSE_HEARTBEAT) + 2;
	Packet.m_pData = &aData;

	// supply the set port that the master can use if it has problems
	if(g_Config.m_SvExternalPort)
		Port = g_Config.m_SvExternalPort;
	aData[sizeof(SERVERBROWSE_HEARTBEAT)] = Port >> 8;
	aData[sizeof(SERVERBROWSE_HEARTBEAT)+1] = Port&0xff;
	Send(&Packet);
}

void CNetServer_TW07::RegisterSendCountRequest(NETADDR Addr)
{
	CNetChunk Packet;
	Packet.m_ClientID = -1;
	Packet.m_Address = Addr;
	Packet.m_Flags = NETSENDFLAG_CONNLESS;
	Packet.m_DataSize = sizeof(SERVERBROWSE_GETCOUNT);
	Packet.m_pData = SERVERBROWSE_GETCOUNT;
	Send(&Packet);
}

void CNetServer_TW07::RegisterGotCount(struct CNetChunk *pChunk)
{
	unsigned char *pData = (unsigned char *)pChunk->m_pData;
	int Count = (pData[sizeof(SERVERBROWSE_COUNT)]<<8) | pData[sizeof(SERVERBROWSE_COUNT)+1];

	for(int i = 0; i < IMasterServer::MAX_MASTERSERVERS; i++)
	{
		if(net_addr_comp(&m_aMasterserverInfo[i].m_Addr, &pChunk->m_Address) == 0)
		{
			m_aMasterserverInfo[i].m_Count = Count;
			break;
		}
	}
}
		
void CNetServer_TW07::RegisterUpdate()
{
	int64 Now = time_get();
	int64 Freq = time_freq();

	if(m_RegisterState == REGISTERSTATE_START)
	{
		m_RegisterCount = 0;
		m_RegisterFirst = 1;
		RegisterNewState(REGISTERSTATE_UPDATE_ADDRS);
		m_pMetaNetServer->MasterServer()->RefreshAddresses(NetType());
		m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", "refreshing ip addresses");
	}
	else if(m_RegisterState == REGISTERSTATE_UPDATE_ADDRS)
	{
		m_RegisterRegisteredServer = -1;

		if(!m_pMetaNetServer->MasterServer()->IsRefreshing())
		{
			int i;
			for(i = 0; i < IMasterServer::MAX_MASTERSERVERS; i++)
			{
				if(!m_pMetaNetServer->MasterServer()->IsValid(i))
				{
					m_aMasterserverInfo[i].m_Valid = 0;
					m_aMasterserverInfo[i].m_Count = 0;
					continue;
				}

				NETADDR Addr = m_pMetaNetServer->MasterServer()->GetAddr(i);
				m_aMasterserverInfo[i].m_Addr = Addr;
				m_aMasterserverInfo[i].m_Valid = 1;
				m_aMasterserverInfo[i].m_Count = -1;
				m_aMasterserverInfo[i].m_LastSend = 0;
				
				
				char aAddrStr[NETADDR_MAXSTRSIZE];
				net_addr_str(&Addr, aAddrStr, sizeof(aAddrStr), true);
			}

			m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", "fetching server counts");
			RegisterNewState(REGISTERSTATE_QUERY_COUNT);
		}
	}
	else if(m_RegisterState == REGISTERSTATE_QUERY_COUNT)
	{
		int Left = 0;
		for(int i = 0; i < IMasterServer::MAX_MASTERSERVERS; i++)
		{
			if(!m_aMasterserverInfo[i].m_Valid)
				continue;

			if(m_aMasterserverInfo[i].m_Count == -1)
			{
				Left++;
				if(m_aMasterserverInfo[i].m_LastSend+Freq < Now)
				{
					m_aMasterserverInfo[i].m_LastSend = Now;
					RegisterSendCountRequest(m_aMasterserverInfo[i].m_Addr);
				}
			}
		}

		// check if we are done or timed out
		if(Left == 0 || Now > m_RegisterStateStart+Freq*3)
		{
			// choose server
			int Best = -1;
			int i;
			for(i = 0; i < IMasterServer::MAX_MASTERSERVERS; i++)
			{
				if(!m_aMasterserverInfo[i].m_Valid || m_aMasterserverInfo[i].m_Count == -1)
					continue;

				if(Best == -1 || m_aMasterserverInfo[i].m_Count < m_aMasterserverInfo[Best].m_Count)
					Best = i;
			}

			// server chosen
			m_RegisterRegisteredServer = Best;
			if(m_RegisterRegisteredServer == -1)
			{
				m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", "WARNING: No master servers. Retrying in 60 seconds");
				RegisterNewState(REGISTERSTATE_ERROR);
			}
			else
			{
				char aBuf[256];
				str_format(aBuf, sizeof(aBuf), "chose '%s' as master, sending heartbeats", m_pMetaNetServer->MasterServer()->GetName(m_RegisterRegisteredServer));
				m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", aBuf);
				m_aMasterserverInfo[m_RegisterRegisteredServer].m_LastSend = 0;
				RegisterNewState(REGISTERSTATE_HEARTBEAT);
			}
		}
	}
	else if(m_RegisterState == REGISTERSTATE_HEARTBEAT)
	{
		// check if we should send heartbeat
		if(Now > m_aMasterserverInfo[m_RegisterRegisteredServer].m_LastSend+Freq*15)
		{
			m_aMasterserverInfo[m_RegisterRegisteredServer].m_LastSend = Now;
			RegisterSendHeartbeat(m_aMasterserverInfo[m_RegisterRegisteredServer].m_Addr);
		}

		if(Now > m_RegisterStateStart+Freq*60)
		{
			m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", "WARNING: Master server is not responding, switching master");
			RegisterNewState(REGISTERSTATE_START);
		}
	}
	else if(m_RegisterState == REGISTERSTATE_REGISTERED)
	{
		if(m_RegisterFirst)
			m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", "server registered");

		m_RegisterFirst = 0;

		// check if we should send new heartbeat again
		if(Now > m_RegisterStateStart+Freq)
		{
			if(m_RegisterCount == 120) // redo the whole process after 60 minutes to balance out the master servers
				RegisterNewState(REGISTERSTATE_START);
			else
			{
				m_RegisterCount++;
				RegisterNewState(REGISTERSTATE_HEARTBEAT);
			}
		}
	}
	else if(m_RegisterState == REGISTERSTATE_ERROR)
	{
		// check for restart
		if(Now > m_RegisterStateStart+Freq*60)
			RegisterNewState(REGISTERSTATE_START);
	}
}
		
int CNetServer_TW07::RegisterProcessPacket(struct CNetChunk *pPacket, TOKEN Token)
{
	// check for masterserver address
	bool Valid = false;
	NETADDR Addr1 = pPacket->m_Address;
	Addr1.port = 0;
	for(int i = 0; i < IMasterServer::MAX_MASTERSERVERS; i++)
	{
		NETADDR Addr2 = m_aMasterserverInfo[i].m_Addr;
		Addr2.port = 0;
		if(net_addr_comp(&Addr1, &Addr2) == 0)
		{
			Valid = true;
			break;
		}
	}
	if(!Valid)
		return 0;

	if(pPacket->m_DataSize == sizeof(SERVERBROWSE_FWCHECK) &&
		mem_comp(pPacket->m_pData, SERVERBROWSE_FWCHECK, sizeof(SERVERBROWSE_FWCHECK)) == 0)
	{
		RegisterSendFwcheckresponse(&pPacket->m_Address, Token);
		return 1;
	}
	else if(pPacket->m_DataSize == sizeof(SERVERBROWSE_FWOK) &&
		mem_comp(pPacket->m_pData, SERVERBROWSE_FWOK, sizeof(SERVERBROWSE_FWOK)) == 0)
	{
		if(m_RegisterFirst && m_RegisterState != REGISTERSTATE_REGISTERED)
			m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", "no firewall/nat problems detected");
		RegisterNewState(REGISTERSTATE_REGISTERED);
		return 1;
	}
	else if(pPacket->m_DataSize == sizeof(SERVERBROWSE_FWERROR) &&
		mem_comp(pPacket->m_pData, SERVERBROWSE_FWERROR, sizeof(SERVERBROWSE_FWERROR)) == 0)
	{
		m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", "ERROR: the master server reports that clients can not connect to this server.");
		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "ERROR: configure your firewall/nat to let through udp on port %d.", m_Address.port);
		m_pMetaNetServer->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "register", aBuf);
		RegisterNewState(REGISTERSTATE_ERROR);
		return 1;
	}
	else if(pPacket->m_DataSize == sizeof(SERVERBROWSE_COUNT)+2 &&
		mem_comp(pPacket->m_pData, SERVERBROWSE_COUNT, sizeof(SERVERBROWSE_COUNT)) == 0)
	{
		RegisterGotCount(pPacket);
		return 1;
	}

	return 0;
}

}
