#include <tu/compatibility.h>
#include <tu/server/event.h>
#include <tu/graphics.h>

#include <engine/server.h>
#include <generated/server_data.h>

#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include <mod/entities/character.h>

#define TU_EVENT_CORE(TypeName) \
TypeName& TypeName::Client(int ClientID) \
{ \
	m_Mask = 0; \
	if(ClientID == -1) \
		m_Mask = -1; \
	else \
	{ \
		m_Mask |= (1 << ClientID); \
	} \
	return *this; \
} \
 \
TypeName& TypeName::Mask(int Mask) \
{ \
	m_Mask = Mask; \
	return *this; \
} \
 \
TypeName& TypeName::World(int WorldID) \
{ \
	m_Mask = 0; \
	if(WorldID == MOD_WORLD_ALL) \
		m_Mask = -1; \
	else \
	{ \
		for(int i=0; i<MAX_CLIENTS; i++) \
		{ \
			if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetWorldID() == WorldID) \
			{ \
				m_Mask |= (1 << i); \
			} \
		} \
	} \
	return *this; \
}

namespace tu
{
	
CEvent::CEvent(CGameContext* pGameServer) :
	m_pGameServer(pGameServer), m_Mask(-1)
{
	
}

IServer* CEvent::Server()
{
	return m_pGameServer->Server();
}
	
int CEvent::GetMask_TW06()
{
	int Mask = 0;
	
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if(m_Mask & (1 << i) == 1 && Server()->GetClientProtocol(i) == CLIENTPROTOCOL_TW06)
			Mask |= (1 << i);
	}
	
	return Mask;
}
	
int CEvent::GetMask_TW07()
{
	int Mask = 0;
	
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if(m_Mask & (1 << i) == 1 && Server()->GetClientProtocol(i) == CLIENTPROTOCOL_TW07)
			Mask |= (1 << i);
	}
	
	return Mask;
}
	
int CEvent::GetMask_TU07()
{
	int Mask = 0;
	
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if(m_Mask & (1 << i) == 1 && Server()->GetClientProtocol(i) == CLIENTPROTOCOL_TU07)
			Mask |= (1 << i);
	}
	
	return Mask;
}

/* WEAPONPICKUP *******************************************************/

CEvent_WeaponPickup::CEvent_WeaponPickup(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_WeaponPickup)

void CEvent_WeaponPickup::Send(int WeaponID)
{
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if(m_Mask & (1 << i) == 0 || GameServer()->m_apPlayers[i])
			continue;
		
		if(GameServer()->m_apPlayers[i])
		{
			if(Server()->GetClientProtocol(i) == CLIENTPROTOCOL_TW06)
			{
				CTW06_NetMsg_Sv_WeaponPickup Msg;
				Msg.m_Weapon = WeaponID;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
			}
			else
			{
				CNetMsg_Sv_WeaponPickup Msg;
				Msg.m_Weapon = WeaponID;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
			}
		}
	}
}

/* BROADCAST **********************************************************/

CEvent_Broadcast::CEvent_Broadcast(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_Broadcast)

void CEvent_Broadcast::Send(const char* pText, int Alternative)
{
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if(m_Mask & (1 << i) == 0 || GameServer()->m_apPlayers[i])
			continue;
		
		if(GameServer()->m_apPlayers[i])
		{
			if(Server()->GetClientProtocol(i) == CLIENTPROTOCOL_TU07)
			{
				CNetMsg_TU_Sv_Broadcast Msg;
				Msg.m_pMessage = pText;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
			}
			else if(Server()->GetClientProtocol(i) == CLIENTPROTOCOL_TW06)
			{
				CTW06_NetMsg_Sv_Broadcast Msg;
				Msg.m_pMessage = pText;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
			}
			else if(Alternative == ALT_CHAT)
			{
				CNetMsg_Sv_Chat Msg;
				Msg.m_Team = 0;
				Msg.m_ClientID = -1;
				Msg.m_pMessage = pText;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
			}
			else if(Alternative == ALT_MOTD)
			{
				CNetMsg_Sv_Motd Msg;
				Msg.m_pMessage = pText;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
			}
		}
	}
}

/* CHAT ***************************************************************/

CEvent_Chat::CEvent_Chat(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_Chat)

void CEvent_Chat::Send(int From, int Team, const char* pText)
{
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if((m_Mask & (1 << i)) == 0 || !GameServer()->m_apPlayers[i])
			continue;
		
		if(GameServer()->m_apPlayers[i])
		{
			if(Server()->GetClientProtocol(i) == CLIENTPROTOCOL_TW06)
			{
				CTW06_NetMsg_Sv_Chat Msg;
				Msg.m_Team = Team;
				Msg.m_ClientID = From;
				Msg.m_pMessage = pText;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL|MSGFLAG_NORECORD, i);
			}
			else
			{
				CNetMsg_Sv_Chat Msg;
				Msg.m_Team = Team;
				Msg.m_ClientID = From;
				Msg.m_pMessage = pText;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL|MSGFLAG_NORECORD, i);
			}
		}
	}
}

/* MOTD ***************************************************************/

CEvent_MOTD::CEvent_MOTD(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_MOTD)

void CEvent_MOTD::Send(const char* pText)
{
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if((m_Mask & (1 << i)) == 0 || !GameServer()->m_apPlayers[i])
			continue;
		
		if(GameServer()->m_apPlayers[i])
		{
			if(Server()->GetClientProtocol(i) == CLIENTPROTOCOL_TW06)
			{
				CTW06_NetMsg_Sv_Motd Msg;
				Msg.m_pMessage = pText;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL|MSGFLAG_NORECORD, i);
			}
			else
			{
				CTW06_NetMsg_Sv_Motd Msg;
				Msg.m_pMessage = pText;
				Server()->SendPackMsg(&Msg, MSGFLAG_VITAL|MSGFLAG_NORECORD, i);
			}
		}
	}
}

/* SOUND **************************************************************/

CEvent_Sound::CEvent_Sound(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_Sound)

void CEvent_Sound::Send(vec2 Pos, int Sound)
{
	if (Sound < 0)
		return;

	//TU
	{
		CNetEvent_SoundWorld *pEvent = (CNetEvent_SoundWorld *)GameServer()->m_Events_TU07.Create(NETEVENTTYPE_SOUNDWORLD, sizeof(CNetEvent_SoundWorld), GetMask_TU07());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
			pEvent->m_SoundID = Sound;
		}
	}
	//TW07
	{
		CNetEvent_SoundWorld *pEvent = (CNetEvent_SoundWorld *)GameServer()->m_Events_TW07.Create(NETEVENTTYPE_SOUNDWORLD, sizeof(CNetEvent_SoundWorld), GetMask_TW07());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
			pEvent->m_SoundID = Sound;
		}
	}
	//TW06
	{
		CTW06_NetEvent_SoundWorld *pEvent = (CTW06_NetEvent_SoundWorld *)GameServer()->m_Events_TW06.Create(TW06_NETEVENTTYPE_SOUNDWORLD, sizeof(CTW06_NetEvent_SoundWorld), GetMask_TW06());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
			pEvent->m_SoundID = Sound;
		}
	}
}

/* SPAWN EFFECT *******************************************************/

CEvent_SpawnEffect::CEvent_SpawnEffect(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_SpawnEffect)

void CEvent_SpawnEffect::Send(vec2 Pos)
{
	//TU
	{
		CNetEvent_Spawn *ev = (CNetEvent_Spawn *)GameServer()->m_Events_TU07.Create(NETEVENTTYPE_SPAWN, sizeof(CNetEvent_Spawn), GetMask_TU07());
		
		if(ev)
		{
			ev->m_X = (int)Pos.x;
			ev->m_Y = (int)Pos.y;
		}
	}
	//TW07
	{
		CNetEvent_Spawn *ev = (CNetEvent_Spawn *)GameServer()->m_Events_TW07.Create(NETEVENTTYPE_SPAWN, sizeof(CNetEvent_Spawn), GetMask_TW07());
		if(ev)
		{
			ev->m_X = (int)Pos.x;
			ev->m_Y = (int)Pos.y;
		}
	}
	//TW06
	{
		CTW06_NetEvent_Spawn *ev = (CTW06_NetEvent_Spawn *)GameServer()->m_Events_TW06.Create(TW06_NETEVENTTYPE_SPAWN, sizeof(CTW06_NetEvent_Spawn), GetMask_TW06());
		if(ev)
		{
			ev->m_X = (int)Pos.x;
			ev->m_Y = (int)Pos.y;
		}
	}
}

/* HAMMERHIT EFFECT ***************************************************/

CEvent_HammerHitEffect::CEvent_HammerHitEffect(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_HammerHitEffect)

void CEvent_HammerHitEffect::Send(vec2 Pos)
{
	//TU
	{
		CNetEvent_HammerHit *pEvent = (CNetEvent_HammerHit *)GameServer()->m_Events_TU07.Create(NETEVENTTYPE_HAMMERHIT, sizeof(CNetEvent_HammerHit), GetMask_TU07());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
		}
	}
	//TW07
	{
		CNetEvent_HammerHit *pEvent = (CNetEvent_HammerHit *)GameServer()->m_Events_TW07.Create(NETEVENTTYPE_HAMMERHIT, sizeof(CNetEvent_HammerHit), GetMask_TW07());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
		}
	}
	//TW06
	{
		CTW06_NetEvent_HammerHit *pEvent = (CTW06_NetEvent_HammerHit *)GameServer()->m_Events_TW06.Create(TW06_NETEVENTTYPE_HAMMERHIT, sizeof(CTW06_NetEvent_HammerHit), GetMask_TW06());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
		}
	}
}

/* DAMAGE INDICATOR EFFECT ********************************************/

CEvent_DamageIndicator::CEvent_DamageIndicator(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_DamageIndicator)

void CEvent_DamageIndicator::Send(vec2 Pos, float Angle, int Amount)
{
	float a = 3*pi/2 + Angle;
	//float a = get_angle(dir);
	float s = a-pi/3;
	float e = a+pi/3;
	
	for(int i = 0; i < Amount; i++)
	{
		float f = mix(s, e, float(i+1)/float(Amount+2));
		
		//TU
		{
			CNetEvent_DamageInd *pEvent = (CNetEvent_DamageInd *)GameServer()->m_Events_TU07.Create(NETEVENTTYPE_DAMAGEIND, sizeof(CNetEvent_DamageInd), GetMask_TU07());
			if(pEvent)
			{
				pEvent->m_X = (int)Pos.x;
				pEvent->m_Y = (int)Pos.y;
				pEvent->m_Angle = (int)(f*256.0f);
			}
		}
		//TW07
		{
			CNetEvent_DamageInd *pEvent = (CNetEvent_DamageInd *)GameServer()->m_Events_TW07.Create(NETEVENTTYPE_DAMAGEIND, sizeof(CNetEvent_DamageInd), GetMask_TW07());
			if(pEvent)
			{
				pEvent->m_X = (int)Pos.x;
				pEvent->m_Y = (int)Pos.y;
				pEvent->m_Angle = (int)(f*256.0f);
			}
		}
		//TW06
		{
			CTW06_NetEvent_DamageInd *pEvent = (CTW06_NetEvent_DamageInd *)GameServer()->m_Events_TW06.Create(TW06_NETEVENTTYPE_DAMAGEIND, sizeof(CTW06_NetEvent_DamageInd), GetMask_TW06());
			if(pEvent)
			{
				pEvent->m_X = (int)Pos.x;
				pEvent->m_Y = (int)Pos.y;
				pEvent->m_Angle = (int)(f*256.0f);
			}
		}
	}
}

/* DEATH EFFECT *******************************************************/

CEvent_DeathEffect::CEvent_DeathEffect(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_DeathEffect)

void CEvent_DeathEffect::Send(vec2 Pos, int ClientID)
{
	//TU
	{
		CNetEvent_Death *pEvent = (CNetEvent_Death *)GameServer()->m_Events_TU07.Create(NETEVENTTYPE_DEATH, sizeof(CNetEvent_Death), GetMask_TU07());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
			pEvent->m_ClientID = ClientID;
		}
	}
	//TW07
	{
		CNetEvent_Death *pEvent = (CNetEvent_Death *)GameServer()->m_Events_TW07.Create(NETEVENTTYPE_DEATH, sizeof(CNetEvent_Death), GetMask_TW07());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
			pEvent->m_ClientID = ClientID;
		}
	}
	//TW06
	{
		CTW06_NetEvent_Death *pEvent = (CTW06_NetEvent_Death *)GameServer()->m_Events_TW06.Create(TW06_NETEVENTTYPE_DEATH, sizeof(CTW06_NetEvent_Death), GetMask_TW06());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
			pEvent->m_ClientID = ClientID;
		}
	}
}

/* EXPLOSION EFFECT ***************************************************/

CEvent_ExplosionEffect::CEvent_ExplosionEffect(CGameContext* pGameServer) :
	CEvent(pGameServer)
{
	
}

TU_EVENT_CORE(CEvent_ExplosionEffect)

void CEvent_ExplosionEffect::Send(vec2 Pos)
{
	//TU
	{
		CNetEvent_Explosion *pEvent = (CNetEvent_Explosion *)GameServer()->m_Events_TU07.Create(NETEVENTTYPE_EXPLOSION, sizeof(CNetEvent_Explosion), GetMask_TU07());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
		}
	}
	//TW07
	{
		CNetEvent_Explosion *pEvent = (CNetEvent_Explosion *)GameServer()->m_Events_TW07.Create(NETEVENTTYPE_EXPLOSION, sizeof(CNetEvent_Explosion), GetMask_TW07());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
		}
	}
	//TW06
	{
		CTW06_NetEvent_Explosion *pEvent = (CTW06_NetEvent_Explosion *)GameServer()->m_Events_TW06.Create(TW06_NETEVENTTYPE_EXPLOSION, sizeof(CTW06_NetEvent_Explosion), GetMask_TW06());
		if(pEvent)
		{
			pEvent->m_X = (int)Pos.x;
			pEvent->m_Y = (int)Pos.y;
		}
	}
}

}
