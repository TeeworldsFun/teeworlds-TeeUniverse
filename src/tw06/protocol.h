/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef TW06_PROTOCOL_H
#define TW06_PROTOCOL_H

#include <base/system.h>

/*
	Connection diagram - How the initilization works.

	Client -> INFO -> Server
		Contains version info, name, and some other info.

	Client <- MAP <- Server
		Contains current map.

	Client -> READY -> Server
		The client has loaded the map and is ready to go,
		but the mod needs to send it's information aswell.
		modc_connected is called on the client and
		mods_connected is called on the server.
		The client should call client_entergame when the
		mod has done it's initilization.

	Client -> ENTERGAME -> Server
		Tells the server to start sending snapshots.
		client_entergame and server_client_enter is called.
*/


enum
{
	TW06_NETMSG_NULL=0,

	// the first thing sent by the client
	// contains the version info for the client
	TW06_NETMSG_INFO=1,

	// sent by server
	TW06_NETMSG_MAP_CHANGE,		// sent when client should switch map
	TW06_NETMSG_MAP_DATA,		// map transfer, contains a chunk of the map file
	TW06_NETMSG_CON_READY,		// connection is ready, client should send start info
	TW06_NETMSG_SNAP,			// normal snapshot, multiple parts
	TW06_NETMSG_SNAPEMPTY,		// empty snapshot
	TW06_NETMSG_SNAPSINGLE,		// ?
	TW06_NETMSG_SNAPSMALL,		//
	TW06_NETMSG_INPUTTIMING,		// reports how off the input was
	TW06_NETMSG_RCON_AUTH_STATUS,// result of the authentication
	TW06_NETMSG_RCON_LINE,		// line that should be printed to the remote console

	TW06_NETMSG_AUTH_CHALLANGE,	//
	TW06_NETMSG_AUTH_RESULT,		//

	// sent by client
	TW06_NETMSG_READY,			//
	TW06_NETMSG_ENTERGAME,
	TW06_NETMSG_INPUT,			// contains the inputdata from the client
	TW06_NETMSG_RCON_CMD,		//
	TW06_NETMSG_RCON_AUTH,		//
	TW06_NETMSG_REQUEST_MAP_DATA,//

	TW06_NETMSG_AUTH_START,		//
	TW06_NETMSG_AUTH_RESPONSE,	//

	// sent by both
	TW06_NETMSG_PING,
	TW06_NETMSG_PING_REPLY,
	TW06_NETMSG_ERROR,

	// sent by server (todo: move it up)
	TW06_NETMSG_RCON_CMD_ADD,
	TW06_NETMSG_RCON_CMD_REM,
};

// this should be revised
enum
{
	TW06_SERVER_TICK_SPEED=50,
	TW06_SERVER_FLAG_PASSWORD = 0x1,

	TW06_MAX_CLIENTS=16,

	TW06_MAX_INPUT_SIZE=128,
	TW06_MAX_SNAPSHOT_PACKSIZE=900,

	TW06_MAX_NAME_LENGTH=16,
	TW06_MAX_CLAN_LENGTH=12,
};

enum
{
	TW06_NETMSG_INVALID=0,
	TW06_NETMSGTYPE_SV_MOTD,
	TW06_NETMSGTYPE_SV_BROADCAST,
	TW06_NETMSGTYPE_SV_CHAT,
	TW06_NETMSGTYPE_SV_KILLMSG,
	TW06_NETMSGTYPE_SV_SOUNDGLOBAL,
	TW06_NETMSGTYPE_SV_TUNEPARAMS,
	TW06_NETMSGTYPE_SV_EXTRAPROJECTILE,
	TW06_NETMSGTYPE_SV_READYTOENTER,
	TW06_NETMSGTYPE_SV_WEAPONPICKUP,
	TW06_NETMSGTYPE_SV_EMOTICON,
	TW06_NETMSGTYPE_SV_VOTECLEAROPTIONS,
	TW06_NETMSGTYPE_SV_VOTEOPTIONLISTADD,
	TW06_NETMSGTYPE_SV_VOTEOPTIONADD,
	TW06_NETMSGTYPE_SV_VOTEOPTIONREMOVE,
	TW06_NETMSGTYPE_SV_VOTESET,
	TW06_NETMSGTYPE_SV_VOTESTATUS,
	TW06_NETMSGTYPE_CL_SAY,
	TW06_NETMSGTYPE_CL_SETTEAM,
	TW06_NETMSGTYPE_CL_SETSPECTATORMODE,
	TW06_NETMSGTYPE_CL_STARTINFO,
	TW06_NETMSGTYPE_CL_CHANGEINFO,
	TW06_NETMSGTYPE_CL_KILL,
	TW06_NETMSGTYPE_CL_EMOTICON,
	TW06_NETMSGTYPE_CL_VOTE,
	TW06_NETMSGTYPE_CL_CALLVOTE,
	TW06_NUM_NETMSGTYPES
};

enum
{
	TW06_TEAM_SPECTATORS=-1,
	TW06_TEAM_RED,
	TW06_TEAM_BLUE,

	TW06_FLAG_MISSING=-3,
	TW06_FLAG_ATSTAND,
	TW06_FLAG_TAKEN,

	TW06_SPEC_FREEVIEW=-1,
};

enum
{
	TW06_NETOBJ_INVALID=0,
	TW06_NETOBJTYPE_PLAYERINPUT,
	TW06_NETOBJTYPE_PROJECTILE,
	TW06_NETOBJTYPE_LASER,
	TW06_NETOBJTYPE_PICKUP,
	TW06_NETOBJTYPE_FLAG,
	TW06_NETOBJTYPE_GAMEINFO,
	TW06_NETOBJTYPE_GAMEDATA,
	TW06_NETOBJTYPE_CHARACTERCORE,
	TW06_NETOBJTYPE_CHARACTER,
	TW06_NETOBJTYPE_PLAYERINFO,
	TW06_NETOBJTYPE_CLIENTINFO,
	TW06_NETOBJTYPE_SPECTATORINFO,
	TW06_NETEVENTTYPE_COMMON,
	TW06_NETEVENTTYPE_EXPLOSION,
	TW06_NETEVENTTYPE_SPAWN,
	TW06_NETEVENTTYPE_HAMMERHIT,
	TW06_NETEVENTTYPE_DEATH,
	TW06_NETEVENTTYPE_SOUNDGLOBAL,
	TW06_NETEVENTTYPE_SOUNDWORLD,
	TW06_NETEVENTTYPE_DAMAGEIND,
	TW06_NUM_NETOBJTYPES
};

enum
{
	TW06_GAMEFLAG_TEAMS = 1<<0,
	TW06_GAMEFLAG_FLAGS = 1<<1,
};

enum
{
	TW06_GAMESTATEFLAG_GAMEOVER = 1<<0,
	TW06_GAMESTATEFLAG_SUDDENDEATH = 1<<1,
	TW06_GAMESTATEFLAG_PAUSED = 1<<2,
};

enum
{
	TW06_PLAYERFLAG_PLAYING = 1<<0,
	TW06_PLAYERFLAG_IN_MENU = 1<<1,
	TW06_PLAYERFLAG_CHATTING = 1<<2,
	TW06_PLAYERFLAG_SCOREBOARD = 1<<3,
};

enum
{
	TW06_EMOTICON_OOP=0,
	TW06_EMOTICON_EXCLAMATION,
	TW06_EMOTICON_HEARTS,
	TW06_EMOTICON_DROP,
	TW06_EMOTICON_DOTDOT,
	TW06_EMOTICON_MUSIC,
	TW06_EMOTICON_SORRY,
	TW06_EMOTICON_GHOST,
	TW06_EMOTICON_SUSHI,
	TW06_EMOTICON_SPLATTEE,
	TW06_EMOTICON_DEVILTEE,
	TW06_EMOTICON_ZOMG,
	TW06_EMOTICON_ZZZ,
	TW06_EMOTICON_WTF,
	TW06_EMOTICON_EYES,
	TW06_EMOTICON_QUESTION,
	TW06_NUM_EMOTICONS
};

class CTW06_NetObjHandler
{
	const char *m_pMsgFailedOn;
	const char *m_pObjCorrectedOn;
	char m_aMsgData[1024];
	int m_NumObjCorrections;
	int ClampInt(const char *pErrorMsg, int Value, int Min, int Max);

	static const char *ms_apObjNames[];
	static int ms_aObjSizes[];
	static const char *ms_apMsgNames[];

public:
	CTW06_NetObjHandler();

	int ValidateObj(int Type, void *pData, int Size);
	const char *GetObjName(int Type);
	int GetObjSize(int Type);
	int NumObjCorrections();
	const char *CorrectedObjOn();

	const char *GetMsgName(int Type);
	void *SecureUnpackMsg(int Type, CUnpacker *pUnpacker);
	const char *FailedMsgOn();
};


struct CTW06_NetMsg_Sv_Motd
{
	const char *m_pMessage;
	int MsgID() const { return TW06_NETMSGTYPE_SV_MOTD; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddString(m_pMessage, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_Broadcast
{
	const char *m_pMessage;
	int MsgID() const { return TW06_NETMSGTYPE_SV_BROADCAST; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddString(m_pMessage, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_Chat
{
	int m_Team;
	int m_ClientID;
	const char *m_pMessage;
	int MsgID() const { return TW06_NETMSGTYPE_SV_CHAT; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Team);
		pPacker->AddInt(m_ClientID);
		pPacker->AddString(m_pMessage, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_KillMsg
{
	int m_Killer;
	int m_Victim;
	int m_Weapon;
	int m_ModeSpecial;
	int MsgID() const { return TW06_NETMSGTYPE_SV_KILLMSG; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Killer);
		pPacker->AddInt(m_Victim);
		pPacker->AddInt(m_Weapon);
		pPacker->AddInt(m_ModeSpecial);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_SoundGlobal
{
	int m_SoundID;
	int MsgID() const { return TW06_NETMSGTYPE_SV_SOUNDGLOBAL; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_SoundID);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_TuneParams
{
	int MsgID() const { return TW06_NETMSGTYPE_SV_TUNEPARAMS; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_ExtraProjectile
{
	int MsgID() const { return TW06_NETMSGTYPE_SV_EXTRAPROJECTILE; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_ReadyToEnter
{
	int MsgID() const { return TW06_NETMSGTYPE_SV_READYTOENTER; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_WeaponPickup
{
	int m_Weapon;
	int MsgID() const { return TW06_NETMSGTYPE_SV_WEAPONPICKUP; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Weapon);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_Emoticon
{
	int m_ClientID;
	int m_Emoticon;
	int MsgID() const { return TW06_NETMSGTYPE_SV_EMOTICON; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_ClientID);
		pPacker->AddInt(m_Emoticon);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_VoteClearOptions
{
	int MsgID() const { return TW06_NETMSGTYPE_SV_VOTECLEAROPTIONS; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_VoteOptionListAdd
{
	int m_NumOptions;
	const char *m_pDescription0;
	const char *m_pDescription1;
	const char *m_pDescription2;
	const char *m_pDescription3;
	const char *m_pDescription4;
	const char *m_pDescription5;
	const char *m_pDescription6;
	const char *m_pDescription7;
	const char *m_pDescription8;
	const char *m_pDescription9;
	const char *m_pDescription10;
	const char *m_pDescription11;
	const char *m_pDescription12;
	const char *m_pDescription13;
	const char *m_pDescription14;
	int MsgID() const { return TW06_NETMSGTYPE_SV_VOTEOPTIONLISTADD; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_NumOptions);
		pPacker->AddString(m_pDescription0, -1);
		pPacker->AddString(m_pDescription1, -1);
		pPacker->AddString(m_pDescription2, -1);
		pPacker->AddString(m_pDescription3, -1);
		pPacker->AddString(m_pDescription4, -1);
		pPacker->AddString(m_pDescription5, -1);
		pPacker->AddString(m_pDescription6, -1);
		pPacker->AddString(m_pDescription7, -1);
		pPacker->AddString(m_pDescription8, -1);
		pPacker->AddString(m_pDescription9, -1);
		pPacker->AddString(m_pDescription10, -1);
		pPacker->AddString(m_pDescription11, -1);
		pPacker->AddString(m_pDescription12, -1);
		pPacker->AddString(m_pDescription13, -1);
		pPacker->AddString(m_pDescription14, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_VoteOptionAdd
{
	const char *m_pDescription;
	int MsgID() const { return TW06_NETMSGTYPE_SV_VOTEOPTIONADD; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddString(m_pDescription, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_VoteOptionRemove
{
	const char *m_pDescription;
	int MsgID() const { return TW06_NETMSGTYPE_SV_VOTEOPTIONREMOVE; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddString(m_pDescription, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_VoteSet
{
	int m_Timeout;
	const char *m_pDescription;
	const char *m_pReason;
	int MsgID() const { return TW06_NETMSGTYPE_SV_VOTESET; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Timeout);
		pPacker->AddString(m_pDescription, -1);
		pPacker->AddString(m_pReason, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Sv_VoteStatus
{
	int m_Yes;
	int m_No;
	int m_Pass;
	int m_Total;
	int MsgID() const { return TW06_NETMSGTYPE_SV_VOTESTATUS; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Yes);
		pPacker->AddInt(m_No);
		pPacker->AddInt(m_Pass);
		pPacker->AddInt(m_Total);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_Say
{
	int m_Team;
	const char *m_pMessage;
	int MsgID() const { return TW06_NETMSGTYPE_CL_SAY; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Team);
		pPacker->AddString(m_pMessage, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_SetTeam
{
	int m_Team;
	int MsgID() const { return TW06_NETMSGTYPE_CL_SETTEAM; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Team);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_SetSpectatorMode
{
	int m_SpectatorID;
	int MsgID() const { return TW06_NETMSGTYPE_CL_SETSPECTATORMODE; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_SpectatorID);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_StartInfo
{
	const char *m_pName;
	const char *m_pClan;
	int m_Country;
	const char *m_pSkin;
	int m_UseCustomColor;
	int m_ColorBody;
	int m_ColorFeet;
	int MsgID() const { return TW06_NETMSGTYPE_CL_STARTINFO; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddString(m_pName, -1);
		pPacker->AddString(m_pClan, -1);
		pPacker->AddInt(m_Country);
		pPacker->AddString(m_pSkin, -1);
		pPacker->AddInt(m_UseCustomColor);
		pPacker->AddInt(m_ColorBody);
		pPacker->AddInt(m_ColorFeet);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_ChangeInfo
{
	const char *m_pName;
	const char *m_pClan;
	int m_Country;
	const char *m_pSkin;
	int m_UseCustomColor;
	int m_ColorBody;
	int m_ColorFeet;
	int MsgID() const { return TW06_NETMSGTYPE_CL_CHANGEINFO; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddString(m_pName, -1);
		pPacker->AddString(m_pClan, -1);
		pPacker->AddInt(m_Country);
		pPacker->AddString(m_pSkin, -1);
		pPacker->AddInt(m_UseCustomColor);
		pPacker->AddInt(m_ColorBody);
		pPacker->AddInt(m_ColorFeet);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_Kill
{
	int MsgID() const { return TW06_NETMSGTYPE_CL_KILL; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_Emoticon
{
	int m_Emoticon;
	int MsgID() const { return TW06_NETMSGTYPE_CL_EMOTICON; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Emoticon);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_Vote
{
	int m_Vote;
	int MsgID() const { return TW06_NETMSGTYPE_CL_VOTE; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddInt(m_Vote);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetMsg_Cl_CallVote
{
	const char *m_Type;
	const char *m_Value;
	const char *m_Reason;
	int MsgID() const { return TW06_NETMSGTYPE_CL_CALLVOTE; }
	
	bool Pack(CMsgPacker *pPacker)
	{
		pPacker->AddString(m_Type, -1);
		pPacker->AddString(m_Value, -1);
		pPacker->AddString(m_Reason, -1);
		return pPacker->Error() != 0;
	}
};

struct CTW06_NetObj_PlayerInput
{
	int m_Direction;
	int m_TargetX;
	int m_TargetY;
	int m_Jump;
	int m_Fire;
	int m_Hook;
	int m_PlayerFlags;
	int m_WantedWeapon;
	int m_NextWeapon;
	int m_PrevWeapon;
};

struct CTW06_NetObj_Projectile
{
	int m_X;
	int m_Y;
	int m_VelX;
	int m_VelY;
	int m_Type;
	int m_StartTick;
};

struct CTW06_NetObj_Laser
{
	int m_X;
	int m_Y;
	int m_FromX;
	int m_FromY;
	int m_StartTick;
};

struct CTW06_NetObj_Pickup
{
	int m_X;
	int m_Y;
	int m_Type;
	int m_Subtype;
};

struct CTW06_NetObj_Flag
{
	int m_X;
	int m_Y;
	int m_Team;
};

struct CTW06_NetObj_GameInfo
{
	int m_GameFlags;
	int m_GameStateFlags;
	int m_RoundStartTick;
	int m_WarmupTimer;
	int m_ScoreLimit;
	int m_TimeLimit;
	int m_RoundNum;
	int m_RoundCurrent;
};

struct CTW06_NetObj_GameData
{
	int m_TeamscoreRed;
	int m_TeamscoreBlue;
	int m_FlagCarrierRed;
	int m_FlagCarrierBlue;
};

struct CTW06_NetObj_CharacterCore
{
	int m_Tick;
	int m_X;
	int m_Y;
	int m_VelX;
	int m_VelY;
	int m_Angle;
	int m_Direction;
	int m_Jumped;
	int m_HookedPlayer;
	int m_HookState;
	int m_HookTick;
	int m_HookX;
	int m_HookY;
	int m_HookDx;
	int m_HookDy;
};

struct CTW06_NetObj_Character : public CTW06_NetObj_CharacterCore
{
	int m_PlayerFlags;
	int m_Health;
	int m_Armor;
	int m_AmmoCount;
	int m_Weapon;
	int m_Emote;
	int m_AttackTick;
};

struct CTW06_NetObj_PlayerInfo
{
	int m_Local;
	int m_ClientID;
	int m_Team;
	int m_Score;
	int m_Latency;
};

struct CTW06_NetObj_ClientInfo
{
	int m_Name0;
	int m_Name1;
	int m_Name2;
	int m_Name3;
	int m_Clan0;
	int m_Clan1;
	int m_Clan2;
	int m_Country;
	int m_Skin0;
	int m_Skin1;
	int m_Skin2;
	int m_Skin3;
	int m_Skin4;
	int m_Skin5;
	int m_UseCustomColor;
	int m_ColorBody;
	int m_ColorFeet;
};

struct CTW06_NetObj_SpectatorInfo
{
	int m_SpectatorID;
	int m_X;
	int m_Y;
};

struct CTW06_NetEvent_Common
{
	int m_X;
	int m_Y;
};

struct CTW06_NetEvent_Explosion : public CTW06_NetEvent_Common
{
};

struct CTW06_NetEvent_Spawn : public CTW06_NetEvent_Common
{
};

struct CTW06_NetEvent_HammerHit : public CTW06_NetEvent_Common
{
};

struct CTW06_NetEvent_Death : public CTW06_NetEvent_Common
{
	int m_ClientID;
};

struct CTW06_NetEvent_SoundGlobal : public CTW06_NetEvent_Common
{
	int m_SoundID;
};

struct CTW06_NetEvent_SoundWorld : public CTW06_NetEvent_Common
{
	int m_SoundID;
};

struct CTW06_NetEvent_DamageInd : public CTW06_NetEvent_Common
{
	int m_Angle;
};

enum
{
	TW06_SOUND_GUN_FIRE=0,
	TW06_SOUND_SHOTGUN_FIRE,
	TW06_SOUND_GRENADE_FIRE,
	TW06_SOUND_HAMMER_FIRE,
	TW06_SOUND_HAMMER_HIT,
	TW06_SOUND_NINJA_FIRE,
	TW06_SOUND_GRENADE_EXPLODE,
	TW06_SOUND_NINJA_HIT,
	TW06_SOUND_RIFLE_FIRE,
	TW06_SOUND_RIFLE_BOUNCE,
	TW06_SOUND_WEAPON_SWITCH,
	TW06_SOUND_PLAYER_PAIN_SHORT,
	TW06_SOUND_PLAYER_PAIN_LONG,
	TW06_SOUND_BODY_LAND,
	TW06_SOUND_PLAYER_AIRJUMP,
	TW06_SOUND_PLAYER_JUMP,
	TW06_SOUND_PLAYER_DIE,
	TW06_SOUND_PLAYER_SPAWN,
	TW06_SOUND_PLAYER_SKID,
	TW06_SOUND_TEE_CRY,
	TW06_SOUND_HOOK_LOOP,
	TW06_SOUND_HOOK_ATTACH_GROUND,
	TW06_SOUND_HOOK_ATTACH_PLAYER,
	TW06_SOUND_HOOK_NOATTACH,
	TW06_SOUND_PICKUP_HEALTH,
	TW06_SOUND_PICKUP_ARMOR,
	TW06_SOUND_PICKUP_GRENADE,
	TW06_SOUND_PICKUP_SHOTGUN,
	TW06_SOUND_PICKUP_NINJA,
	TW06_SOUND_WEAPON_SPAWN,
	TW06_SOUND_WEAPON_NOAMMO,
	TW06_SOUND_HIT,
	TW06_SOUND_CHAT_SERVER,
	TW06_SOUND_CHAT_CLIENT,
	TW06_SOUND_CHAT_HIGHLIGHT,
	TW06_SOUND_CTF_DROP,
	TW06_SOUND_CTF_RETURN,
	TW06_SOUND_CTF_GRAB_PL,
	TW06_SOUND_CTF_GRAB_EN,
	TW06_SOUND_CTF_CAPTURE,
	TW06_SOUND_MENU,
	TW06_NUM_SOUNDS
};

enum
{
	TW06_WEAPON_HAMMER=0,
	TW06_WEAPON_GUN,
	TW06_WEAPON_SHOTGUN,
	TW06_WEAPON_GRENADE,
	TW06_WEAPON_RIFLE,
	TW06_WEAPON_NINJA,
	TW06_NUM_WEAPONS
};

enum
{
	TW06_POWERUP_HEALTH=0,
	TW06_POWERUP_ARMOR,
	TW06_POWERUP_WEAPON,
	TW06_POWERUP_NINJA,
	NUM_POWERUPS
};

enum
{
	TW06_HOOK_RETRACTED=-1,
	TW06_HOOK_IDLE=0,
	TW06_HOOK_RETRACT_START=1,
	TW06_HOOK_RETRACT_END=3,
	TW06_HOOK_FLYING,
	TW06_HOOK_GRABBED,

	TW06_COREEVENT_GROUND_JUMP=0x01,
	TW06_COREEVENT_AIR_JUMP=0x02,
	TW06_COREEVENT_HOOK_LAUNCH=0x04,
	TW06_COREEVENT_HOOK_ATTACH_PLAYER=0x08,
	TW06_COREEVENT_HOOK_ATTACH_GROUND=0x10,
	TW06_COREEVENT_HOOK_HIT_NOHOOK=0x20,
	TW06_COREEVENT_HOOK_RETRACT=0x40,
};

#endif
