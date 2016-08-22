#ifndef TU_SERVER_EVENT_H
#define TU_SERVER_EVENT_H

#include <base/vmath.h>
#include <mod/defines.h>

class IServer;
class CGameContext;

#define TU_EVENT_HEADER(TypeName) public:\
	TypeName& Client(int WorldID); \
	TypeName& Mask(int Mask); \
	TypeName& World(int WorldID);

namespace tu
{

class CEvent
{
protected:
	IServer* m_pServer;
	CGameContext* m_pGameServer;

protected:
	int m_Mask;
	
	int GetMask_TW06();
	int GetMask_TW07();
	int GetMask_TU07();

public:
	CEvent(CGameContext* pGameServer);
	
	IServer* Server();
	CGameContext* GameServer() { return m_pGameServer; }
};

/* WEAPONPICKUP *******************************************************/

class CEvent_WeaponPickup : public CEvent
{
TU_EVENT_HEADER(CEvent_WeaponPickup)

public:
	CEvent_WeaponPickup(CGameContext* pGameServer);
	void Send(int WeaponID);
};

/* BROADCAST **********************************************************/

class CEvent_Broadcast : public CEvent
{
TU_EVENT_HEADER(CEvent_Broadcast)

public:
	enum
	{
		ALT_NONE = 0,
		ALT_CHAT,
		ALT_MOTD
	};

public:
	CEvent_Broadcast(CGameContext* pGameServer);
	void Send(const char* pText, int Alternative);
};

/* CHAT **********************************************************/

class CEvent_Chat : public CEvent
{
TU_EVENT_HEADER(CEvent_Chat)

public:
	CEvent_Chat(CGameContext* pGameServer);
	void Send(int From, int Team, const char* pText);
};

/* MOTD ***************************************************************/

class CEvent_MOTD : public CEvent
{
TU_EVENT_HEADER(CEvent_MOTD)

public:
	CEvent_MOTD(CGameContext* pGameServer);
	void Send(const char* pText);
};

/* SOUND **************************************************************/

class CEvent_Sound : public CEvent
{
TU_EVENT_HEADER(CEvent_Sound)

public:
	CEvent_Sound(CGameContext* pGameServer);
	void Send(vec2 Pos, int Sound);
};

/* SPAWN EFFECT *******************************************************/

class CEvent_SpawnEffect : public CEvent
{
TU_EVENT_HEADER(CEvent_SpawnEffect)

public:
	CEvent_SpawnEffect(CGameContext* pGameServer);
	void Send(vec2 Pos);
};

/* HAMMERHIT EFFECT ***************************************************/

class CEvent_HammerHitEffect : public CEvent
{
TU_EVENT_HEADER(CEvent_HammerHitEffect)

public:
	CEvent_HammerHitEffect(CGameContext* pGameServer);
	void Send(vec2 Pos);
};

/* DAMAGE INDICATOR ***************************************************/

class CEvent_DamageIndicator : public CEvent
{
TU_EVENT_HEADER(CEvent_DamageIndicator)

public:
	CEvent_DamageIndicator(CGameContext* pGameServer);
	void Send(vec2 Pos, float Angle, int Amount);
};

/* DEATH EFFECT *******************************************************/

class CEvent_DeathEffect : public CEvent
{
TU_EVENT_HEADER(CEvent_DeathEffect)

public:
	CEvent_DeathEffect(CGameContext* pGameServer);
	void Send(vec2 Pos, int ClientID);
};

/* EXPLOSION EFFECT ***************************************************/

class CEvent_ExplosionEffect : public CEvent
{
TU_EVENT_HEADER(CEvent_ExplosionEffect)

public:
	CEvent_ExplosionEffect(CGameContext* pGameServer);
	void Send(vec2 Pos);
};

}

#endif
