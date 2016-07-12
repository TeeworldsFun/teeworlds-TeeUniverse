/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef TW06_SHARED_NETWORK_H
#define TW06_SHARED_NETWORK_H

#include <engine/shared/ringbuffer.h>
#include <engine/shared/huffman.h>
#include <engine/shared/network.h>

/*

CURRENT:
	packet header: 3 bytes
		unsigned char flags_ack; // 4bit flags, 4bit ack
		unsigned char ack; // 8 bit ack
		unsigned char num_chunks; // 8 bit chunks

		(unsigned char padding[3])	// 24 bit extra incase it's a connection less packet
									// this is to make sure that it's compatible with the
									// old protocol

	chunk header: 2-3 bytes
		unsigned char flags_size; // 2bit flags, 6 bit size
		unsigned char size_seq; // 4bit size, 4bit seq
		(unsigned char seq;) // 8bit seq, if vital flag is set
*/

enum
{
	TW06_NETFLAG_ALLOWSTATELESS=1,
	TW06_NETSENDFLAG_VITAL=1,
	TW06_NETSENDFLAG_CONNLESS=2,
	TW06_NETSENDFLAG_FLUSH=4,

	TW06_NETSTATE_OFFLINE=0,
	TW06_NETSTATE_CONNECTING,
	TW06_NETSTATE_ONLINE,

	TW06_NETBANTYPE_SOFT=1,
	TW06_NETBANTYPE_DROP=2
};


enum
{
	TW06_NET_VERSION = 2,

	TW06_NET_MAX_PACKETSIZE = 1400,
	TW06_NET_MAX_PAYLOAD = TW06_NET_MAX_PACKETSIZE-6,
	TW06_NET_MAX_CHUNKHEADERSIZE = 5,
	TW06_NET_PACKETHEADERSIZE = 3,
	TW06_NET_MAX_CLIENTS = 16,
	TW06_NET_MAX_CONSOLE_CLIENTS = 4,
	TW06_NET_MAX_SEQUENCE = 1<<10,
	TW06_NET_SEQUENCE_MASK = TW06_NET_MAX_SEQUENCE-1,

	TW06_NET_CONNSTATE_OFFLINE=0,
	TW06_NET_CONNSTATE_CONNECT=1,
	TW06_NET_CONNSTATE_PENDING=2,
	TW06_NET_CONNSTATE_ONLINE=3,
	TW06_NET_CONNSTATE_ERROR=4,

	TW06_NET_PACKETFLAG_CONTROL=1,
	TW06_NET_PACKETFLAG_CONNLESS=2,
	TW06_NET_PACKETFLAG_RESEND=4,
	TW06_NET_PACKETFLAG_COMPRESSION=8,

	TW06_NET_CHUNKFLAG_VITAL=1,
	TW06_NET_CHUNKFLAG_RESEND=2,

	TW06_NET_CTRLMSG_KEEPALIVE=0,
	TW06_NET_CTRLMSG_CONNECT=1,
	TW06_NET_CTRLMSG_CONNECTACCEPT=2,
	TW06_NET_CTRLMSG_ACCEPT=3,
	TW06_NET_CTRLMSG_CLOSE=4,

	TW06_NET_CONN_BUFFERSIZE=1024*32,

	TW06_NET_ENUM_TERMINATOR
};


typedef int (*TW06_NETFUNC_DELCLIENT)(int ClientID, const char* pReason, void *pUser);
typedef int (*TW06_NETFUNC_NEWCLIENT)(int ClientID, void *pUser);

class CTW06_NetChunkHeader
{
public:
	int m_Flags;
	int m_Size;
	int m_Sequence;

	unsigned char *Pack(unsigned char *pData);
	unsigned char *Unpack(unsigned char *pData);
};

class CTW06_NetChunkResend
{
public:
	int m_Flags;
	int m_DataSize;
	unsigned char *m_pData;

	int m_Sequence;
	int64 m_LastSendTime;
	int64 m_FirstSendTime;
};

class CTW06_NetPacketConstruct
{
public:
	int m_Flags;
	int m_Ack;
	int m_NumChunks;
	int m_DataSize;
	unsigned char m_aChunkData[TW06_NET_MAX_PAYLOAD];
};


class CTW06_NetConnection
{
	// TODO: is this needed because this needs to be aware of
	// the ack sequencing number and is also responible for updating
	// that. this should be fixed.
	friend class CTW06_NetRecvUnpacker;
private:
	unsigned short m_Sequence;
	unsigned short m_Ack;
	unsigned m_State;

	int m_Token;
	int m_RemoteClosed;
	bool m_BlockCloseMsg;

	TStaticRingBuffer<CTW06_NetChunkResend, TW06_NET_CONN_BUFFERSIZE> m_Buffer;

	int64 m_LastUpdateTime;
	int64 m_LastRecvTime;
	int64 m_LastSendTime;

	char m_ErrorString[256];

	CTW06_NetPacketConstruct m_Construct;

	NETADDR m_PeerAddr;
	NETSOCKET m_Socket;
	NETSTATS m_Stats;

	//
	void Reset();
	void ResetStats();
	void SetError(const char *pString);
	void AckChunks(int Ack);

	int QueueChunkEx(int Flags, int DataSize, const void *pData, int Sequence);
	void SendControl(int ControlMsg, const void *pExtra, int ExtraSize);
	void ResendChunk(CTW06_NetChunkResend *pResend);
	void Resend();

public:
	void Init(NETSOCKET Socket, bool BlockCloseMsg);
	int Connect(NETADDR *pAddr);
	void Disconnect(const char *pReason);

	int Update();
	int Flush();

	int Feed(CTW06_NetPacketConstruct *pPacket, NETADDR *pAddr);
	int QueueChunk(int Flags, int DataSize, const void *pData);

	const char *ErrorString();
	void SignalResend();
	int State() const { return m_State; }
	const NETADDR *PeerAddress() const { return &m_PeerAddr; }

	void ResetErrorString() { m_ErrorString[0] = 0; }
	const char *ErrorString() const { return m_ErrorString; }

	// Needed for GotProblems in NetClient
	int64 LastRecvTime() const { return m_LastRecvTime; }
	int64 ConnectTime() const { return m_LastUpdateTime; }

	int AckSequence() const { return m_Ack; }
};

class CTW06_ConsoleNetConnection
{
private:
	int m_State;

	NETADDR m_PeerAddr;
	NETSOCKET m_Socket;

	char m_aBuffer[TW06_NET_MAX_PACKETSIZE];
	int m_BufferOffset;

	char m_aErrorString[256];

	bool m_LineEndingDetected;
	char m_aLineEnding[3];

public:
	void Init(NETSOCKET Socket, const NETADDR *pAddr);
	void Disconnect(const char *pReason);

	int State() const { return m_State; }
	const NETADDR *PeerAddress() const { return &m_PeerAddr; }
	const char *ErrorString() const { return m_aErrorString; }

	void Reset();
	int Update();
	int Send(const char *pLine);
	int Recv(char *pLine, int MaxLength);
};

class CTW06_NetRecvUnpacker
{
public:
	bool m_Valid;

	NETADDR m_Addr;
	CTW06_NetConnection *m_pConnection;
	int m_CurrentChunk;
	int m_ClientID;
	CTW06_NetPacketConstruct m_Data;
	unsigned char m_aBuffer[TW06_NET_MAX_PACKETSIZE];

	CTW06_NetRecvUnpacker() { Clear(); }
	void Clear();
	void Start(const NETADDR *pAddr, CTW06_NetConnection *pConnection, int ClientID);
	int FetchChunk(CNetChunk *pChunk);
};

class CTW06_NetConsole
{
	struct CSlot
	{
		CTW06_ConsoleNetConnection m_Connection;
	};

	NETSOCKET m_Socket;
	class CNetBan *m_pNetBan;
	CSlot m_aSlots[TW06_NET_MAX_CONSOLE_CLIENTS];

	TW06_NETFUNC_NEWCLIENT m_pfnNewClient;
	TW06_NETFUNC_DELCLIENT m_pfnDelClient;
	void *m_UserPtr;

	CTW06_NetRecvUnpacker m_RecvUnpacker;

public:
	void SetCallbacks(TW06_NETFUNC_NEWCLIENT pfnNewClient, TW06_NETFUNC_DELCLIENT pfnDelClient, void *pUser);

	//
	bool Open(NETADDR BindAddr, class CNetBan *pNetBan, int Flags);
	int Close();

	//
	int Recv(char *pLine, int MaxLength, int *pClientID = 0);
	int Send(int ClientID, const char *pLine);
	int Update();

	//
	int AcceptClient(NETSOCKET Socket, const NETADDR *pAddr);
	int Drop(int ClientID, const char *pReason);

	// status requests
	const NETADDR *ClientAddr(int ClientID) const { return m_aSlots[ClientID].m_Connection.PeerAddress(); }
	class CNetBan *NetBan() const { return m_pNetBan; }
};

// TODO: both, fix these. This feels like a junk class for stuff that doesn't fit anywere
class CTW06_NetBase
{
	static IOHANDLE ms_DataLogSent;
	static IOHANDLE ms_DataLogRecv;
public:
	static void OpenLog(IOHANDLE DataLogSent, IOHANDLE DataLogRecv);
	static void CloseLog();
	static int Compress(const void *pData, int DataSize, void *pOutput, int OutputSize);
	static int Decompress(const void *pData, int DataSize, void *pOutput, int OutputSize);

	static void SendControlMsg(NETSOCKET Socket, NETADDR *pAddr, int Ack, int ControlMsg, const void *pExtra, int ExtraSize);
	static void SendPacketConnless(NETSOCKET Socket, NETADDR *pAddr, const void *pData, int DataSize);
	static void SendPacket(NETSOCKET Socket, NETADDR *pAddr, CTW06_NetPacketConstruct *pPacket);
	static int UnpackPacket(unsigned char *pBuffer, int Size, CTW06_NetPacketConstruct *pPacket);

	// The backroom is ack-NET_MAX_SEQUENCE/2. Used for knowing if we acked a packet or not
	static int IsSeqInBackroom(int Seq, int Ack);
};


#endif
