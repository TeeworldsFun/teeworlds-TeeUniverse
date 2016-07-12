#include <engine/message.h>
#include "protocol.h"
CTW06_NetObjHandler::CTW06_NetObjHandler()
{
	m_pMsgFailedOn = "";
	m_pObjCorrectedOn = "";
	m_NumObjCorrections = 0;
}

int CTW06_NetObjHandler::NumObjCorrections() { return m_NumObjCorrections; }
const char *CTW06_NetObjHandler::CorrectedObjOn() { return m_pObjCorrectedOn; }
const char *CTW06_NetObjHandler::FailedMsgOn() { return m_pMsgFailedOn; }





static const int max_int = 0x7fffffff;
int CTW06_NetObjHandler::ClampInt(const char *pErrorMsg, int Value, int Min, int Max)
{
	if(Value < Min) { m_pObjCorrectedOn = pErrorMsg; m_NumObjCorrections++; return Min; }
	if(Value > Max) { m_pObjCorrectedOn = pErrorMsg; m_NumObjCorrections++; return Max; }
	return Value;
}
const char *CTW06_NetObjHandler::ms_apObjNames[] = {
	"invalid",
	"PlayerInput",
	"Projectile",
	"Laser",
	"Pickup",
	"Flag",
	"GameInfo",
	"GameData",
	"CharacterCore",
	"Character",
	"PlayerInfo",
	"ClientInfo",
	"SpectatorInfo",
	"Common",
	"Explosion",
	"Spawn",
	"HammerHit",
	"Death",
	"SoundGlobal",
	"SoundWorld",
	"DamageInd",
	""
};

int CTW06_NetObjHandler::ms_aObjSizes[] = {
	0,
	sizeof(CTW06_NetObj_PlayerInput),
	sizeof(CTW06_NetObj_Projectile),
	sizeof(CTW06_NetObj_Laser),
	sizeof(CTW06_NetObj_Pickup),
	sizeof(CTW06_NetObj_Flag),
	sizeof(CTW06_NetObj_GameInfo),
	sizeof(CTW06_NetObj_GameData),
	sizeof(CTW06_NetObj_CharacterCore),
	sizeof(CTW06_NetObj_Character),
	sizeof(CTW06_NetObj_PlayerInfo),
	sizeof(CTW06_NetObj_ClientInfo),
	sizeof(CTW06_NetObj_SpectatorInfo),
	sizeof(CTW06_NetEvent_Common),
	sizeof(CTW06_NetEvent_Explosion),
	sizeof(CTW06_NetEvent_Spawn),
	sizeof(CTW06_NetEvent_HammerHit),
	sizeof(CTW06_NetEvent_Death),
	sizeof(CTW06_NetEvent_SoundGlobal),
	sizeof(CTW06_NetEvent_SoundWorld),
	sizeof(CTW06_NetEvent_DamageInd),
	0
};

const char *CTW06_NetObjHandler::ms_apMsgNames[] = {
	"invalid",
	"Sv_Motd",
	"Sv_Broadcast",
	"Sv_Chat",
	"Sv_KillMsg",
	"Sv_SoundGlobal",
	"Sv_TuneParams",
	"Sv_ExtraProjectile",
	"Sv_ReadyToEnter",
	"Sv_WeaponPickup",
	"Sv_Emoticon",
	"Sv_VoteClearOptions",
	"Sv_VoteOptionListAdd",
	"Sv_VoteOptionAdd",
	"Sv_VoteOptionRemove",
	"Sv_VoteSet",
	"Sv_VoteStatus",
	"Cl_Say",
	"Cl_SetTeam",
	"Cl_SetSpectatorMode",
	"Cl_StartInfo",
	"Cl_ChangeInfo",
	"Cl_Kill",
	"Cl_Emoticon",
	"Cl_Vote",
	"Cl_CallVote",
	""
};

const char *CTW06_NetObjHandler::GetObjName(int Type)
{
	if(Type < 0 || Type >= TW06_NUM_NETOBJTYPES) return "(out of range)";
	return ms_apObjNames[Type];
};

int CTW06_NetObjHandler::GetObjSize(int Type)
{
	if(Type < 0 || Type >= TW06_NUM_NETOBJTYPES) return 0;
	return ms_aObjSizes[Type];
};

const char *CTW06_NetObjHandler::GetMsgName(int Type)
{
	if(Type < 0 || Type >= TW06_NUM_NETMSGTYPES) return "(out of range)";
	return ms_apMsgNames[Type];
};

int CTW06_NetObjHandler::ValidateObj(int Type, void *pData, int Size)
{
	switch(Type)
	{
	case TW06_NETOBJTYPE_PLAYERINPUT:
	{
		CTW06_NetObj_PlayerInput *pObj = (CTW06_NetObj_PlayerInput *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_PlayerFlags", pObj->m_PlayerFlags, 0, 256);
		return 0;
	}
	
	case TW06_NETOBJTYPE_PROJECTILE:
	{
		CTW06_NetObj_Projectile *pObj = (CTW06_NetObj_Projectile *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_Type", pObj->m_Type, 0, TW06_NUM_WEAPONS-1);
		ClampInt("m_StartTick", pObj->m_StartTick, 0, max_int);
		return 0;
	}
	
	case TW06_NETOBJTYPE_LASER:
	{
		CTW06_NetObj_Laser *pObj = (CTW06_NetObj_Laser *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_StartTick", pObj->m_StartTick, 0, max_int);
		return 0;
	}
	
	case TW06_NETOBJTYPE_PICKUP:
	{
		CTW06_NetObj_Pickup *pObj = (CTW06_NetObj_Pickup *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_Type", pObj->m_Type, 0, max_int);
		ClampInt("m_Subtype", pObj->m_Subtype, 0, max_int);
		return 0;
	}
	
	case TW06_NETOBJTYPE_FLAG:
	{
		CTW06_NetObj_Flag *pObj = (CTW06_NetObj_Flag *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_Team", pObj->m_Team, TW06_TEAM_RED, TW06_TEAM_BLUE);
		return 0;
	}
	
	case TW06_NETOBJTYPE_GAMEINFO:
	{
		CTW06_NetObj_GameInfo *pObj = (CTW06_NetObj_GameInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_GameFlags", pObj->m_GameFlags, 0, 256);
		ClampInt("m_GameStateFlags", pObj->m_GameStateFlags, 0, 256);
		ClampInt("m_RoundStartTick", pObj->m_RoundStartTick, 0, max_int);
		ClampInt("m_WarmupTimer", pObj->m_WarmupTimer, 0, max_int);
		ClampInt("m_ScoreLimit", pObj->m_ScoreLimit, 0, max_int);
		ClampInt("m_TimeLimit", pObj->m_TimeLimit, 0, max_int);
		ClampInt("m_RoundNum", pObj->m_RoundNum, 0, max_int);
		ClampInt("m_RoundCurrent", pObj->m_RoundCurrent, 0, max_int);
		return 0;
	}
	
	case TW06_NETOBJTYPE_GAMEDATA:
	{
		CTW06_NetObj_GameData *pObj = (CTW06_NetObj_GameData *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_FlagCarrierRed", pObj->m_FlagCarrierRed, TW06_FLAG_MISSING, TW06_MAX_CLIENTS-1);
		ClampInt("m_FlagCarrierBlue", pObj->m_FlagCarrierBlue, TW06_FLAG_MISSING, TW06_MAX_CLIENTS-1);
		return 0;
	}
	
	case TW06_NETOBJTYPE_CHARACTERCORE:
	{
		CTW06_NetObj_CharacterCore *pObj = (CTW06_NetObj_CharacterCore *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_Direction", pObj->m_Direction, -1, 1);
		ClampInt("m_Jumped", pObj->m_Jumped, 0, 3);
		ClampInt("m_HookedPlayer", pObj->m_HookedPlayer, 0, TW06_MAX_CLIENTS-1);
		ClampInt("m_HookState", pObj->m_HookState, -1, 5);
		ClampInt("m_HookTick", pObj->m_HookTick, 0, max_int);
		return 0;
	}
	
	case TW06_NETOBJTYPE_CHARACTER:
	{
		CTW06_NetObj_Character *pObj = (CTW06_NetObj_Character *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_PlayerFlags", pObj->m_PlayerFlags, 0, 256);
		ClampInt("m_Health", pObj->m_Health, 0, 10);
		ClampInt("m_Armor", pObj->m_Armor, 0, 10);
		ClampInt("m_AmmoCount", pObj->m_AmmoCount, 0, 10);
		ClampInt("m_Weapon", pObj->m_Weapon, 0, TW06_NUM_WEAPONS-1);
		ClampInt("m_Emote", pObj->m_Emote, 0, 6);
		ClampInt("m_AttackTick", pObj->m_AttackTick, 0, max_int);
		return 0;
	}
	
	case TW06_NETOBJTYPE_PLAYERINFO:
	{
		CTW06_NetObj_PlayerInfo *pObj = (CTW06_NetObj_PlayerInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_Local", pObj->m_Local, 0, 1);
		ClampInt("m_ClientID", pObj->m_ClientID, 0, TW06_MAX_CLIENTS-1);
		ClampInt("m_Team", pObj->m_Team, TW06_TEAM_SPECTATORS, TW06_TEAM_BLUE);
		return 0;
	}
	
	case TW06_NETOBJTYPE_CLIENTINFO:
	{
		CTW06_NetObj_ClientInfo *pObj = (CTW06_NetObj_ClientInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_UseCustomColor", pObj->m_UseCustomColor, 0, 1);
		return 0;
	}
	
	case TW06_NETOBJTYPE_SPECTATORINFO:
	{
		CTW06_NetObj_SpectatorInfo *pObj = (CTW06_NetObj_SpectatorInfo *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_SpectatorID", pObj->m_SpectatorID, TW06_SPEC_FREEVIEW, TW06_MAX_CLIENTS-1);
		return 0;
	}
	
	case TW06_NETEVENTTYPE_COMMON:
	{
		CTW06_NetEvent_Common *pObj = (CTW06_NetEvent_Common *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}
	
	case TW06_NETEVENTTYPE_EXPLOSION:
	{
		CTW06_NetEvent_Explosion *pObj = (CTW06_NetEvent_Explosion *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}
	
	case TW06_NETEVENTTYPE_SPAWN:
	{
		CTW06_NetEvent_Spawn *pObj = (CTW06_NetEvent_Spawn *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}
	
	case TW06_NETEVENTTYPE_HAMMERHIT:
	{
		CTW06_NetEvent_HammerHit *pObj = (CTW06_NetEvent_HammerHit *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}
	
	case TW06_NETEVENTTYPE_DEATH:
	{
		CTW06_NetEvent_Death *pObj = (CTW06_NetEvent_Death *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_ClientID", pObj->m_ClientID, 0, TW06_MAX_CLIENTS-1);
		return 0;
	}
	
	case TW06_NETEVENTTYPE_SOUNDGLOBAL:
	{
		CTW06_NetEvent_SoundGlobal *pObj = (CTW06_NetEvent_SoundGlobal *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_SoundID", pObj->m_SoundID, 0, TW06_NUM_SOUNDS-1);
		return 0;
	}
	
	case TW06_NETEVENTTYPE_SOUNDWORLD:
	{
		CTW06_NetEvent_SoundWorld *pObj = (CTW06_NetEvent_SoundWorld *)pData;
		if(sizeof(*pObj) != Size) return -1;
		ClampInt("m_SoundID", pObj->m_SoundID, 0, TW06_NUM_SOUNDS-1);
		return 0;
	}
	
	case TW06_NETEVENTTYPE_DAMAGEIND:
	{
		CTW06_NetEvent_DamageInd *pObj = (CTW06_NetEvent_DamageInd *)pData;
		if(sizeof(*pObj) != Size) return -1;
		return 0;
	}
	
	}
	return -1;
};

void *CTW06_NetObjHandler::SecureUnpackMsg(int Type, CUnpacker *pUnpacker)
{
	m_pMsgFailedOn = 0;
	switch(Type)
	{
	case TW06_NETMSGTYPE_SV_MOTD:
	{
		CTW06_NetMsg_Sv_Motd *pMsg = (CTW06_NetMsg_Sv_Motd *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pMessage = pUnpacker->GetString();
	} break;
	
	case TW06_NETMSGTYPE_SV_BROADCAST:
	{
		CTW06_NetMsg_Sv_Broadcast *pMsg = (CTW06_NetMsg_Sv_Broadcast *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pMessage = pUnpacker->GetString();
	} break;
	
	case TW06_NETMSGTYPE_SV_CHAT:
	{
		CTW06_NetMsg_Sv_Chat *pMsg = (CTW06_NetMsg_Sv_Chat *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Team = pUnpacker->GetInt();
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_pMessage = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(pMsg->m_Team < TW06_TEAM_SPECTATORS || pMsg->m_Team > TW06_TEAM_BLUE) { m_pMsgFailedOn = "m_Team"; break; }
		if(pMsg->m_ClientID < -1 || pMsg->m_ClientID > TW06_MAX_CLIENTS-1) { m_pMsgFailedOn = "m_ClientID"; break; }
	} break;
	
	case TW06_NETMSGTYPE_SV_KILLMSG:
	{
		CTW06_NetMsg_Sv_KillMsg *pMsg = (CTW06_NetMsg_Sv_KillMsg *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Killer = pUnpacker->GetInt();
		pMsg->m_Victim = pUnpacker->GetInt();
		pMsg->m_Weapon = pUnpacker->GetInt();
		pMsg->m_ModeSpecial = pUnpacker->GetInt();
		if(pMsg->m_Killer < 0 || pMsg->m_Killer > TW06_MAX_CLIENTS-1) { m_pMsgFailedOn = "m_Killer"; break; }
		if(pMsg->m_Victim < 0 || pMsg->m_Victim > TW06_MAX_CLIENTS-1) { m_pMsgFailedOn = "m_Victim"; break; }
		if(pMsg->m_Weapon < -3 || pMsg->m_Weapon > TW06_NUM_WEAPONS-1) { m_pMsgFailedOn = "m_Weapon"; break; }
	} break;
	
	case TW06_NETMSGTYPE_SV_SOUNDGLOBAL:
	{
		CTW06_NetMsg_Sv_SoundGlobal *pMsg = (CTW06_NetMsg_Sv_SoundGlobal *)m_aMsgData;
		(void)pMsg;
		pMsg->m_SoundID = pUnpacker->GetInt();
		if(pMsg->m_SoundID < 0 || pMsg->m_SoundID > TW06_NUM_SOUNDS-1) { m_pMsgFailedOn = "m_SoundID"; break; }
	} break;
	
	case TW06_NETMSGTYPE_SV_TUNEPARAMS:
	{
		CTW06_NetMsg_Sv_TuneParams *pMsg = (CTW06_NetMsg_Sv_TuneParams *)m_aMsgData;
		(void)pMsg;
	} break;
	
	case TW06_NETMSGTYPE_SV_EXTRAPROJECTILE:
	{
		CTW06_NetMsg_Sv_ExtraProjectile *pMsg = (CTW06_NetMsg_Sv_ExtraProjectile *)m_aMsgData;
		(void)pMsg;
	} break;
	
	case TW06_NETMSGTYPE_SV_READYTOENTER:
	{
		CTW06_NetMsg_Sv_ReadyToEnter *pMsg = (CTW06_NetMsg_Sv_ReadyToEnter *)m_aMsgData;
		(void)pMsg;
	} break;
	
	case TW06_NETMSGTYPE_SV_WEAPONPICKUP:
	{
		CTW06_NetMsg_Sv_WeaponPickup *pMsg = (CTW06_NetMsg_Sv_WeaponPickup *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Weapon = pUnpacker->GetInt();
		if(pMsg->m_Weapon < 0 || pMsg->m_Weapon > TW06_NUM_WEAPONS-1) { m_pMsgFailedOn = "m_Weapon"; break; }
	} break;
	
	case TW06_NETMSGTYPE_SV_EMOTICON:
	{
		CTW06_NetMsg_Sv_Emoticon *pMsg = (CTW06_NetMsg_Sv_Emoticon *)m_aMsgData;
		(void)pMsg;
		pMsg->m_ClientID = pUnpacker->GetInt();
		pMsg->m_Emoticon = pUnpacker->GetInt();
		if(pMsg->m_ClientID < 0 || pMsg->m_ClientID > TW06_MAX_CLIENTS-1) { m_pMsgFailedOn = "m_ClientID"; break; }
		if(pMsg->m_Emoticon < 0 || pMsg->m_Emoticon > TW06_NUM_EMOTICONS-1) { m_pMsgFailedOn = "m_Emoticon"; break; }
	} break;
	
	case TW06_NETMSGTYPE_SV_VOTECLEAROPTIONS:
	{
		CTW06_NetMsg_Sv_VoteClearOptions *pMsg = (CTW06_NetMsg_Sv_VoteClearOptions *)m_aMsgData;
		(void)pMsg;
	} break;
	
	case TW06_NETMSGTYPE_SV_VOTEOPTIONLISTADD:
	{
		CTW06_NetMsg_Sv_VoteOptionListAdd *pMsg = (CTW06_NetMsg_Sv_VoteOptionListAdd *)m_aMsgData;
		(void)pMsg;
		pMsg->m_NumOptions = pUnpacker->GetInt();
		pMsg->m_pDescription0 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription1 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription2 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription3 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription4 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription5 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription6 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription7 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription8 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription9 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription10 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription11 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription12 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription13 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pDescription14 = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(pMsg->m_NumOptions < 1 || pMsg->m_NumOptions > 15) { m_pMsgFailedOn = "m_NumOptions"; break; }
	} break;
	
	case TW06_NETMSGTYPE_SV_VOTEOPTIONADD:
	{
		CTW06_NetMsg_Sv_VoteOptionAdd *pMsg = (CTW06_NetMsg_Sv_VoteOptionAdd *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pDescription = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;
	
	case TW06_NETMSGTYPE_SV_VOTEOPTIONREMOVE:
	{
		CTW06_NetMsg_Sv_VoteOptionRemove *pMsg = (CTW06_NetMsg_Sv_VoteOptionRemove *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pDescription = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;
	
	case TW06_NETMSGTYPE_SV_VOTESET:
	{
		CTW06_NetMsg_Sv_VoteSet *pMsg = (CTW06_NetMsg_Sv_VoteSet *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Timeout = pUnpacker->GetInt();
		pMsg->m_pDescription = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pReason = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(pMsg->m_Timeout < 0 || pMsg->m_Timeout > 60) { m_pMsgFailedOn = "m_Timeout"; break; }
	} break;
	
	case TW06_NETMSGTYPE_SV_VOTESTATUS:
	{
		CTW06_NetMsg_Sv_VoteStatus *pMsg = (CTW06_NetMsg_Sv_VoteStatus *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Yes = pUnpacker->GetInt();
		pMsg->m_No = pUnpacker->GetInt();
		pMsg->m_Pass = pUnpacker->GetInt();
		pMsg->m_Total = pUnpacker->GetInt();
		if(pMsg->m_Yes < 0 || pMsg->m_Yes > TW06_MAX_CLIENTS) { m_pMsgFailedOn = "m_Yes"; break; }
		if(pMsg->m_No < 0 || pMsg->m_No > TW06_MAX_CLIENTS) { m_pMsgFailedOn = "m_No"; break; }
		if(pMsg->m_Pass < 0 || pMsg->m_Pass > TW06_MAX_CLIENTS) { m_pMsgFailedOn = "m_Pass"; break; }
		if(pMsg->m_Total < 0 || pMsg->m_Total > TW06_MAX_CLIENTS) { m_pMsgFailedOn = "m_Total"; break; }
	} break;
	
	case TW06_NETMSGTYPE_CL_SAY:
	{
		CTW06_NetMsg_Cl_Say *pMsg = (CTW06_NetMsg_Cl_Say *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Team = pUnpacker->GetInt();
		pMsg->m_pMessage = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		if(pMsg->m_Team < 0 || pMsg->m_Team > 1) { m_pMsgFailedOn = "m_Team"; break; }
	} break;
	
	case TW06_NETMSGTYPE_CL_SETTEAM:
	{
		CTW06_NetMsg_Cl_SetTeam *pMsg = (CTW06_NetMsg_Cl_SetTeam *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Team = pUnpacker->GetInt();
		if(pMsg->m_Team < TW06_TEAM_SPECTATORS || pMsg->m_Team > TW06_TEAM_BLUE) { m_pMsgFailedOn = "m_Team"; break; }
	} break;
	
	case TW06_NETMSGTYPE_CL_SETSPECTATORMODE:
	{
		CTW06_NetMsg_Cl_SetSpectatorMode *pMsg = (CTW06_NetMsg_Cl_SetSpectatorMode *)m_aMsgData;
		(void)pMsg;
		pMsg->m_SpectatorID = pUnpacker->GetInt();
		if(pMsg->m_SpectatorID < TW06_SPEC_FREEVIEW || pMsg->m_SpectatorID > TW06_MAX_CLIENTS-1) { m_pMsgFailedOn = "m_SpectatorID"; break; }
	} break;
	
	case TW06_NETMSGTYPE_CL_STARTINFO:
	{
		CTW06_NetMsg_Cl_StartInfo *pMsg = (CTW06_NetMsg_Cl_StartInfo *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pName = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pClan = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Country = pUnpacker->GetInt();
		pMsg->m_pSkin = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_UseCustomColor = pUnpacker->GetInt();
		pMsg->m_ColorBody = pUnpacker->GetInt();
		pMsg->m_ColorFeet = pUnpacker->GetInt();
		if(pMsg->m_UseCustomColor < 0 || pMsg->m_UseCustomColor > 1) { m_pMsgFailedOn = "m_UseCustomColor"; break; }
	} break;
	
	case TW06_NETMSGTYPE_CL_CHANGEINFO:
	{
		CTW06_NetMsg_Cl_ChangeInfo *pMsg = (CTW06_NetMsg_Cl_ChangeInfo *)m_aMsgData;
		(void)pMsg;
		pMsg->m_pName = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_pClan = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Country = pUnpacker->GetInt();
		pMsg->m_pSkin = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_UseCustomColor = pUnpacker->GetInt();
		pMsg->m_ColorBody = pUnpacker->GetInt();
		pMsg->m_ColorFeet = pUnpacker->GetInt();
		if(pMsg->m_UseCustomColor < 0 || pMsg->m_UseCustomColor > 1) { m_pMsgFailedOn = "m_UseCustomColor"; break; }
	} break;
	
	case TW06_NETMSGTYPE_CL_KILL:
	{
		CTW06_NetMsg_Cl_Kill *pMsg = (CTW06_NetMsg_Cl_Kill *)m_aMsgData;
		(void)pMsg;
	} break;
	
	case TW06_NETMSGTYPE_CL_EMOTICON:
	{
		CTW06_NetMsg_Cl_Emoticon *pMsg = (CTW06_NetMsg_Cl_Emoticon *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Emoticon = pUnpacker->GetInt();
		if(pMsg->m_Emoticon < 0 || pMsg->m_Emoticon > TW06_NUM_EMOTICONS-1) { m_pMsgFailedOn = "m_Emoticon"; break; }
	} break;
	
	case TW06_NETMSGTYPE_CL_VOTE:
	{
		CTW06_NetMsg_Cl_Vote *pMsg = (CTW06_NetMsg_Cl_Vote *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Vote = pUnpacker->GetInt();
		if(pMsg->m_Vote < -1 || pMsg->m_Vote > 1) { m_pMsgFailedOn = "m_Vote"; break; }
	} break;
	
	case TW06_NETMSGTYPE_CL_CALLVOTE:
	{
		CTW06_NetMsg_Cl_CallVote *pMsg = (CTW06_NetMsg_Cl_CallVote *)m_aMsgData;
		(void)pMsg;
		pMsg->m_Type = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Value = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
		pMsg->m_Reason = pUnpacker->GetString(CUnpacker::SANITIZE_CC|CUnpacker::SKIP_START_WHITESPACES);
	} break;
	
	default:
		m_pMsgFailedOn = "(type out of range)";
		break;
	}
	
	if(pUnpacker->Error())
		m_pMsgFailedOn = "(unpack error)";
	
	if(m_pMsgFailedOn)
		return 0;
	m_pMsgFailedOn = "";
	return m_aMsgData;
};
