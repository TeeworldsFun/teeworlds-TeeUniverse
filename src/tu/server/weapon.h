#ifndef TU_SERVER_WEAPON_H
#define TU_SERVER_WEAPON_H

#include <base/vmath.h>
#include <tu/weapon.h>

namespace tu
{

class CWeapon
{
private:
	int m_ID;
	class CCharacter* m_pCharacter;
	
public:
	CWeapon(int ID, class CCharacter* pCharacter);
	virtual ~CWeapon() {}

	int GetID() const { return m_ID; }
	class CCharacter* Character();
	class CPlayer* Player();
	class CGameWorld* GameWorld();
	class CGameContext* GameServer();
	class IServer* Server();
	int WorldID();
	const class IServer* Server() const;
	
	virtual int GetAmmoType() const = 0;
	virtual int GetMaxAmmo() const = 0;
	virtual int GetAmmo() const = 0;
	virtual bool AddAmmo(int Ammo) = 0;
	virtual bool IsAutomatic() const = 0;
	virtual bool IsWeaponSwitchLocked() const { return false; }
	
	virtual bool TickPaused(bool IsActive) { return !IsActive; }
	virtual bool TickPreFire(bool IsActive) = 0;
	virtual bool OnFire(vec2 Direction) = 0;
	virtual void OnActivation() {}
	
	virtual void Snap_TW06(int Snapshot, int SnappingClient, class CTW06_NetObj_Character* pChar) = 0;
	virtual void Snap_TW07(int Snapshot, int SnappingClient, struct CNetObj_Character* pChar) = 0;
	virtual void Snap_TU07(int Snapshot, int SnappingClient, struct CNetObj_Character* pChar) = 0;
};

class CWeapon_GenericGun07 : public CWeapon
{
protected:
	int m_TW07ID;
	int m_ReloadTimer;
	int m_LastNoAmmoSound;
	int m_AmmoRegenStart;
	int m_Ammo;
	
public:
	CWeapon_GenericGun07(int WID, int TW07ID, class CCharacter* pCharacter, int Ammo);
	
	virtual int GetMaxAmmo() const;
	virtual int GetAmmoType() const { return tu::AMMOTYPE_INTEGER; }
	virtual int GetAmmo() const;
	virtual bool AddAmmo(int Ammo);
	virtual bool IsWeaponSwitchLocked() const;
	
	virtual bool TickPaused(bool IsActive);
	virtual bool TickPreFire(bool IsActive);
	virtual bool OnFire(vec2 Direction);
	virtual void OnActivation();

	virtual void Snap_TW06(int Snapshot, int SnappingClient, class CTW06_NetObj_Character* pChar);
	virtual void Snap_TW07(int Snapshot, int SnappingClient, struct CNetObj_Character* pChar);
	virtual void Snap_TU07(int Snapshot, int SnappingClient, struct CNetObj_Character* pChar);
	
	virtual void CreateProjectile(vec2 Pos, vec2 Direction) = 0;
};

}

#endif
