#include "entity.h"

#include <engine/server.h>

namespace tu
{

CEntity_TW::CEntity_TW(CGameWorld *pGameWorld, int ObjType, vec2 Pos, int NbIdPerSnapshot, int ProximityRadius) :
	CEntityCore(pGameWorld, ObjType, Pos, ProximityRadius)
{
	m_NbIdPerSnapshot = NbIdPerSnapshot;
	m_IDList.set_size(3*m_NbIdPerSnapshot);
	
	for(int i=0; i<m_NbIdPerSnapshot; i++)
	{
		m_IDList[i] = Server()->SnapNewID(tu::SNAPSHOT_TW06);
		m_IDList[NbIdPerSnapshot+i] = Server()->SnapNewID(tu::SNAPSHOT_TW07);
		m_IDList[NbIdPerSnapshot*2+i] = Server()->SnapNewID(tu::SNAPSHOT_TU07);
	}
}

CEntity_TW::~CEntity_TW()
{
	for(int i=0; i<m_NbIdPerSnapshot; i++)
	{
		Server()->SnapFreeID(tu::SNAPSHOT_TW06, m_IDList[i]);
		Server()->SnapFreeID(tu::SNAPSHOT_TW07, m_IDList[i]);
		Server()->SnapFreeID(tu::SNAPSHOT_TU07, m_IDList[m_NbIdPerSnapshot+i]);
	}
}

void CEntity_TW::Snap_TU07(int Snapshot, int SnappingClient)
{
	Snap_TW07(Snapshot, SnappingClient);
}

int CEntity_TW::GetID(int Snapshot, int ID)
{
	int TrueID = ID;
	if(Snapshot == tu::SNAPSHOT_TU07) TrueID += m_NbIdPerSnapshot;
	return m_IDList[TrueID];
}

CEntity::CEntity(CGameWorld *pGameWorld, int ObjType, vec2 Pos, int NbId_TW06, int NbId_TW07, int NbId_TU07, int ProximityRadius) :
	CEntityCore(pGameWorld, ObjType, Pos, ProximityRadius)
{
	m_IDList_TW06.set_size(NbId_TW06);
	for(int i=0; i<m_IDList_TW06.size(); i++)
	{
		m_IDList_TW06[i] = Server()->SnapNewID(tu::SNAPSHOT_TW06);
	}
	
	m_IDList_TW07.set_size(NbId_TW07);
	for(int i=0; i<m_IDList_TW07.size(); i++)
	{
		m_IDList_TW07[i] = Server()->SnapNewID(tu::SNAPSHOT_TW07);
	}
	
	m_IDList_TU07.set_size(NbId_TU07);
	for(int i=0; i<m_IDList_TU07.size(); i++)
	{
		m_IDList_TU07[i] = Server()->SnapNewID(tu::SNAPSHOT_TU07);
	}
}

CEntity::~CEntity()
{
	for(int i=0; i<m_IDList_TW06.size(); i++)
	{
		Server()->SnapFreeID(tu::SNAPSHOT_TW06, m_IDList_TW06[i]);
	}
	
	for(int i=0; i<m_IDList_TW07.size(); i++)
	{
		Server()->SnapFreeID(tu::SNAPSHOT_TW07, m_IDList_TW07[i]);
	}
	
	for(int i=0; i<m_IDList_TU07.size(); i++)
	{
		Server()->SnapFreeID(tu::SNAPSHOT_TU07, m_IDList_TU07[i]);
	}
}

int CEntity::GetID(int Snapshot, int ID)
{
	if(Snapshot == tu::SNAPSHOT_TW06)
		return m_IDList_TW06[ID];
	else if(Snapshot == tu::SNAPSHOT_TW07)
		return m_IDList_TW07[ID];
	else
		return m_IDList_TU07[ID];
}

}
