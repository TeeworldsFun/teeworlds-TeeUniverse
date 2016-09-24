#ifndef MOD_WEAPON_NINJA_H
#define MOD_WEAPON_NINJA_H

#include <base/vmath.h>
#include <tu/server/weapon.h>

class CMod_Weapon_Ninja : public tu::CWeapon
{
protected:
	int m_ReloadTimer;
	
	class CEntityCore* m_apHitObjects[10];
	int m_NumObjectsHit;
	
	vec2 m_ActivationDir;
	int m_ActivationTick;
	int m_CurrentMoveTime;
	int m_OldVelAmount;

public:
	CMod_Weapon_Ninja(class CCharacter* pCharacter);
	
	virtual int GetMaxAmmo() const;
	virtual int GetAmmoType() const { return tu::AMMOTYPE_TIME; }
	virtual int GetAmmo() const;
	virtual bool AddAmmo(int Ammo) { return true; }
	virtual bool IsAutomatic() const { return false; }
	virtual bool IsWeaponSwitchLocked() const { return true; }
	
	virtual bool TickPaused(bool IsActive);
	virtual bool TickPreFire(bool IsActiveWeapon);
	virtual bool OnFire(vec2 Direction);
	
	virtual void Snap_TW06(int Snapshot, int SnappingClient, class CTW06_NetObj_Character* pChar);
	virtual void Snap_TW07(int Snapshot, int SnappingClient, struct CNetObj_Character* pChar);
	virtual void Snap_TU07(int Snapshot, int SnappingClient, struct CNetObj_Character* pChar);
};

#endif
