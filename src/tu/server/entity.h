#ifndef TU_ENTITY_H
#define TU_ENTITY_H

#include <game/server/entity.h>
#include <tu/compatibility.h>

#include <base/tl/array.h>

namespace tu
{

class CEntity_TW : public CEntityCore
{
private:
	int m_NbIdPerSnapshot;
	array<int> m_IDList;
	
public:
	CEntity_TW(CGameWorld *pGameWorld, int Objtype, vec2 Pos, int ProximityRadius=0, int NbIdPerSnapshot=1);
	~CEntity_TW();
		
	virtual void Snap_TW06(int Snapshot, int SnappingClient) = 0;
	virtual void Snap_TW07(int Snapshot, int SnappingClient) = 0;
	virtual void Snap_TU07(int Snapshot, int SnappingClient);

protected:
	int GetID(int Snapshot, int ID);
};

class CEntity : public CEntityCore
{
private:
	array<int> m_IDList_TW06;
	array<int> m_IDList_TW07;
	array<int> m_IDList_TU07;
	
public:
	CEntity(CGameWorld *pGameWorld, int Objtype, vec2 Pos, int ProximityRadius=0, int NbIdTW06=1, int NbIdTW07=1, int NbIdTU07=1);
	~CEntity();
		
	virtual void Snap_TW06(int Snapshot, int SnappingClient) = 0;
	virtual void Snap_TW07(int Snapshot, int SnappingClient) = 0;
	virtual void Snap_TU07(int Snapshot, int SnappingClient) = 0;
	
protected:
	int GetID(int Snapshot, int ID);
};

}

#endif
