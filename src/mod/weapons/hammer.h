#ifndef MOD_WEAPON_HAMMER_H
#define MOD_WEAPON_HAMMER_H

#include <base/vmath.h>
#include <tu/server/weapon.h>

class CMod_Weapon_Hammer : public tu::CWeapon
{
protected:
	int m_ReloadTimer;

public:
	CMod_Weapon_Hammer(class CCharacter* pCharacter);
	
	virtual int GetMaxAmmo() const { return -1; }
	virtual int GetAmmoType() const { return tu::AMMOTYPE_INFINITE; }
	virtual int GetAmmo() const { return -1; }
	virtual bool AddAmmo(int Ammo) { return false; }
	virtual bool IsAutomatic() const { return false; }
	virtual bool IsWeaponSwitchLocked() const;
	
	virtual bool TickPreFire(bool IsActive);
	virtual bool OnFire(vec2 Direction);

	virtual void Snap_TW06(int Snapshot, int SnappingClient, class CTW06_NetObj_Character* pChar);
	virtual void Snap_TW07(int Snapshot, int SnappingClient, struct CNetObj_Character* pChar);
	virtual void Snap_TU07(int Snapshot, int SnappingClient, struct CNetObj_Character* pChar);
};

#endif
