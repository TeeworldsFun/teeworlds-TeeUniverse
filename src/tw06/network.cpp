/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <base/system.h>

#include <engine/shared/config.h>
#include <tw06/network.h>

void CTW06_NetRecvUnpacker::Clear()
{
	m_Valid = false;
}

void CTW06_NetRecvUnpacker::Start(const NETADDR *pAddr, CTW06_NetConnection *pConnection, int ClientID)
{
	m_Addr = *pAddr;
	m_pConnection = pConnection;
	m_ClientID = ClientID;
	m_CurrentChunk = 0;
	m_Valid = true;
}

// TODO: rename this function
int CTW06_NetRecvUnpacker::FetchChunk(CNetChunk *pChunk)
{
	CTW06_NetChunkHeader Header;
	unsigned char *pEnd = m_Data.m_aChunkData + m_Data.m_DataSize;

	while(1)
	{
		unsigned char *pData = m_Data.m_aChunkData;

		// check for old data to unpack
		if(!m_Valid || m_CurrentChunk >= m_Data.m_NumChunks)
		{
			Clear();
			return 0;
		}

		// TODO: add checking here so we don't read too far
		for(int i = 0; i < m_CurrentChunk; i++)
		{
			pData = Header.Unpack(pData);
			pData += Header.m_Size;
		}

		// unpack the header
		pData = Header.Unpack(pData);
		m_CurrentChunk++;

		if(pData+Header.m_Size > pEnd)
		{
			Clear();
			return 0;
		}

		// handle sequence stuff
		if(m_pConnection && (Header.m_Flags&TW06_NET_CHUNKFLAG_VITAL))
		{
			if(Header.m_Sequence == (m_pConnection->m_Ack+1)%TW06_NET_MAX_SEQUENCE)
			{
				// in sequence
				m_pConnection->m_Ack = (m_pConnection->m_Ack+1)%TW06_NET_MAX_SEQUENCE;
			}
			else
			{
				// old packet that we already got
				if(CTW06_NetBase::IsSeqInBackroom(Header.m_Sequence, m_pConnection->m_Ack))
					continue;

				// out of sequence, request resend
				if(g_Config.m_Debug)
					dbg_msg("conn", "asking for resend %d %d", Header.m_Sequence, (m_pConnection->m_Ack+1)%TW06_NET_MAX_SEQUENCE);
				m_pConnection->SignalResend();
				continue; // take the next chunk in the packet
			}
		}

		// fill in the info
		pChunk->m_ClientID = m_ClientID;
		pChunk->m_Address = m_Addr;
		pChunk->m_Flags = 0;
		pChunk->m_DataSize = Header.m_Size;
		pChunk->m_pData = pData;
		return 1;
	}
}

// packs the data tight and sends it
void CTW06_NetBase::SendPacketConnless(NETSOCKET Socket, NETADDR *pAddr, const void *pData, int DataSize)
{
	unsigned char aBuffer[TW06_NET_MAX_PACKETSIZE];
	aBuffer[0] = 0xff;
	aBuffer[1] = 0xff;
	aBuffer[2] = 0xff;
	aBuffer[3] = 0xff;
	aBuffer[4] = 0xff;
	aBuffer[5] = 0xff;
	mem_copy(&aBuffer[6], pData, DataSize);
	net_udp_send(Socket, pAddr, aBuffer, 6+DataSize);
}

void CTW06_NetBase::SendPacket(NETSOCKET Socket, NETADDR *pAddr, CTW06_NetPacketConstruct *pPacket)
{
	unsigned char aBuffer[TW06_NET_MAX_PACKETSIZE];
	int CompressedSize = -1;
	int FinalSize = -1;

	// log the data
	if(ms_DataLogSent)
	{
		int Type = 1;
		io_write(ms_DataLogSent, &Type, sizeof(Type));
		io_write(ms_DataLogSent, &pPacket->m_DataSize, sizeof(pPacket->m_DataSize));
		io_write(ms_DataLogSent, &pPacket->m_aChunkData, pPacket->m_DataSize);
		io_flush(ms_DataLogSent);
	}

	// compress
	CompressedSize = CNetBase::ms_Huffman.Compress(pPacket->m_aChunkData, pPacket->m_DataSize, &aBuffer[3], TW06_NET_MAX_PACKETSIZE-4);

	// check if the compression was enabled, successful and good enough
	if(CompressedSize > 0 && CompressedSize < pPacket->m_DataSize)
	{
		FinalSize = CompressedSize;
		pPacket->m_Flags |= TW06_NET_PACKETFLAG_COMPRESSION;
	}
	else
	{
		// use uncompressed data
		FinalSize = pPacket->m_DataSize;
		mem_copy(&aBuffer[3], pPacket->m_aChunkData, pPacket->m_DataSize);
		pPacket->m_Flags &= ~TW06_NET_PACKETFLAG_COMPRESSION;
	}

	// set header and send the packet if all things are good
	if(FinalSize >= 0)
	{
		FinalSize += TW06_NET_PACKETHEADERSIZE;
		aBuffer[0] = ((pPacket->m_Flags<<4)&0xf0)|((pPacket->m_Ack>>8)&0xf);
		aBuffer[1] = pPacket->m_Ack&0xff;
		aBuffer[2] = pPacket->m_NumChunks;
		net_udp_send(Socket, pAddr, aBuffer, FinalSize);

		// log raw socket data
		if(ms_DataLogSent)
		{
			int Type = 0;
			io_write(ms_DataLogSent, &Type, sizeof(Type));
			io_write(ms_DataLogSent, &FinalSize, sizeof(FinalSize));
			io_write(ms_DataLogSent, aBuffer, FinalSize);
			io_flush(ms_DataLogSent);
		}
	}
}

// TODO: rename this function
int CTW06_NetBase::UnpackPacket(unsigned char *pBuffer, int Size, CTW06_NetPacketConstruct *pPacket)
{
	// check the size
	if(Size < TW06_NET_PACKETHEADERSIZE || Size > TW06_NET_MAX_PACKETSIZE)
	{
		dbg_msg("", "packet too small, %d", Size);
		return -1;
	}

	// log the data
	if(ms_DataLogRecv)
	{
		int Type = 0;
		io_write(ms_DataLogRecv, &Type, sizeof(Type));
		io_write(ms_DataLogRecv, &Size, sizeof(Size));
		io_write(ms_DataLogRecv, pBuffer, Size);
		io_flush(ms_DataLogRecv);
	}

	// read the packet
	pPacket->m_Flags = pBuffer[0]>>4;
	pPacket->m_Ack = ((pBuffer[0]&0xf)<<8) | pBuffer[1];
	pPacket->m_NumChunks = pBuffer[2];
	pPacket->m_DataSize = Size - TW06_NET_PACKETHEADERSIZE;

	if(pPacket->m_Flags&TW06_NET_PACKETFLAG_CONNLESS)
	{
		if(Size < 6)
		{
			dbg_msg("", "connection less packet too small, %d", Size);
			return -1;
		}

		pPacket->m_Flags = TW06_NET_PACKETFLAG_CONNLESS;
		pPacket->m_Ack = 0;
		pPacket->m_NumChunks = 0;
		pPacket->m_DataSize = Size - 6;
		mem_copy(pPacket->m_aChunkData, &pBuffer[6], pPacket->m_DataSize);
	}
	else
	{
		if(pPacket->m_Flags&TW06_NET_PACKETFLAG_COMPRESSION)
		{
			pPacket->m_DataSize = CNetBase::ms_Huffman.Decompress(&pBuffer[3], pPacket->m_DataSize, pPacket->m_aChunkData, sizeof(pPacket->m_aChunkData));
		}
		else
			mem_copy(pPacket->m_aChunkData, &pBuffer[3], pPacket->m_DataSize);
	}

	// check for errors
	if(pPacket->m_DataSize < 0)
	{
		if(g_Config.m_Debug)
			dbg_msg("network", "error during packet decoding");
		return -1;
	}

	// log the data
	if(ms_DataLogRecv)
	{
		int Type = 1;
		io_write(ms_DataLogRecv, &Type, sizeof(Type));
		io_write(ms_DataLogRecv, &pPacket->m_DataSize, sizeof(pPacket->m_DataSize));
		io_write(ms_DataLogRecv, pPacket->m_aChunkData, pPacket->m_DataSize);
		io_flush(ms_DataLogRecv);
	}

	// return success
	return 0;
}


void CTW06_NetBase::SendControlMsg(NETSOCKET Socket, NETADDR *pAddr, int Ack, int ControlMsg, const void *pExtra, int ExtraSize)
{
	CTW06_NetPacketConstruct Construct;
	Construct.m_Flags = TW06_NET_PACKETFLAG_CONTROL;
	Construct.m_Ack = Ack;
	Construct.m_NumChunks = 0;
	Construct.m_DataSize = 1+ExtraSize;
	Construct.m_aChunkData[0] = ControlMsg;
	mem_copy(&Construct.m_aChunkData[1], pExtra, ExtraSize);

	// send the control message
	CTW06_NetBase::SendPacket(Socket, pAddr, &Construct);
}



unsigned char *CTW06_NetChunkHeader::Pack(unsigned char *pData)
{
	pData[0] = ((m_Flags&3)<<6)|((m_Size>>4)&0x3f);
	pData[1] = (m_Size&0xf);
	if(m_Flags&TW06_NET_CHUNKFLAG_VITAL)
	{
		pData[1] |= (m_Sequence>>2)&0xf0;
		pData[2] = m_Sequence&0xff;
		return pData + 3;
	}
	return pData + 2;
}

unsigned char *CTW06_NetChunkHeader::Unpack(unsigned char *pData)
{
	m_Flags = (pData[0]>>6)&3;
	m_Size = ((pData[0]&0x3f)<<4) | (pData[1]&0xf);
	m_Sequence = -1;
	if(m_Flags&TW06_NET_CHUNKFLAG_VITAL)
	{
		m_Sequence = ((pData[1]&0xf0)<<2) | pData[2];
		return pData + 3;
	}
	return pData + 2;
}


int CTW06_NetBase::IsSeqInBackroom(int Seq, int Ack)
{
	int Bottom = (Ack-TW06_NET_MAX_SEQUENCE/2);
	if(Bottom < 0)
	{
		if(Seq <= Ack)
			return 1;
		if(Seq >= (Bottom + TW06_NET_MAX_SEQUENCE))
			return 1;
	}
	else
	{
		if(Seq <= Ack && Seq >= Bottom)
			return 1;
	}

	return 0;
}

IOHANDLE CTW06_NetBase::ms_DataLogSent = 0;
IOHANDLE CTW06_NetBase::ms_DataLogRecv = 0;

void CTW06_NetBase::OpenLog(IOHANDLE DataLogSent, IOHANDLE DataLogRecv)
{
	if(DataLogSent)
	{
		ms_DataLogSent = DataLogSent;
		dbg_msg("network", "logging sent packages");
	}
	else
		dbg_msg("network", "failed to start logging sent packages");

	if(DataLogRecv)
	{
		ms_DataLogRecv = DataLogRecv;
		dbg_msg("network", "logging recv packages");
	}
	else
		dbg_msg("network", "failed to start logging recv packages");
}

void CTW06_NetBase::CloseLog()
{
	if(ms_DataLogSent)
	{
		dbg_msg("network", "stopped logging sent packages");
		io_close(ms_DataLogSent);
		ms_DataLogSent = 0;
	}

	if(ms_DataLogRecv)
	{
		dbg_msg("network", "stopped logging recv packages");
		io_close(ms_DataLogRecv);
		ms_DataLogRecv = 0;
	}
}

int CTW06_NetBase::Compress(const void *pData, int DataSize, void *pOutput, int OutputSize)
{
	return CNetBase::ms_Huffman.Compress(pData, DataSize, pOutput, OutputSize);
}

int CTW06_NetBase::Decompress(const void *pData, int DataSize, void *pOutput, int OutputSize)
{
	return CNetBase::ms_Huffman.Decompress(pData, DataSize, pOutput, OutputSize);
}
