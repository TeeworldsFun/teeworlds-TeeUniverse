#include <engine/graphics.h>
#include <engine/demo.h>
#include <generated/protocol.h>
#include <generated/client_data.h>

#include <game/client/gameclient.h>
#include <game/client/ui.h>
#include <game/client/render.h>

#include <game/client/components/flow.h>
#include <game/client/components/effects.h>

#include <engine/textrender.h>

#include "items.h"

namespace tu
{

CComponent_Items::CComponent_Items()
{
	m_LastTime = time_get();
}

void CComponent_Items::SetLayer(int Layer)
{
	m_Layer = Layer;
}

int CComponent_Items::GetLayer() const
{
	return m_Layer;
}
	
void CComponent_Items::RenderSprite(const CNetObj_TU_Sprite *pPrev, const CNetObj_TU_Sprite *pCurrent)
{
	if(pCurrent->m_ItemLayer != GetLayer()) return;

	float Angle = mix(2.0*pi*static_cast<float>(pPrev->m_Angle)/360.0f, 2.0*pi*static_cast<float>(pCurrent->m_Angle)/360.0f, Client()->IntraGameTick());
	float Size = mix(pPrev->m_Size, pCurrent->m_Size, Client()->IntraGameTick());
	vec2 Pos = mix(vec2(pPrev->m_X, pPrev->m_Y), vec2(pCurrent->m_X, pCurrent->m_Y), Client()->IntraGameTick());
	
	TUKernel()->AssetsRenderer()->DrawSprite(pCurrent->m_SpriteId, Pos, Size, Angle, 0x0, 1.0f);
}

void CComponent_Items::RenderSpriteCharacter(const CNetObj_TU_SpriteCharacter *pPrev, const CNetObj_TU_SpriteCharacter *pCurrent)
{
	if(pCurrent->m_ItemLayer != GetLayer()) return;
	if(!m_pClient->m_Snap.m_aCharacters[pCurrent->m_ClientId].m_Active) return;
	
	float Angle = mix(2.0*pi*static_cast<float>(pPrev->m_Angle)/360.0f, 2.0*pi*static_cast<float>(pCurrent->m_Angle)/360.0f, Client()->IntraGameTick());
	float Size = mix(pPrev->m_Size, pCurrent->m_Size, Client()->IntraGameTick());
	vec2 Pos = mix(vec2(pPrev->m_X, pPrev->m_Y), vec2(pCurrent->m_X, pCurrent->m_Y), Client()->IntraGameTick());
	
	if(m_pClient->m_LocalClientID != -1 && m_pClient->m_LocalClientID == pCurrent->m_ClientId)
	{
		Pos = m_pClient->m_LocalCharacterPos + Pos;
	}
	else
	{
		CNetObj_Character PrevChar = m_pClient->m_Snap.m_aCharacters[pCurrent->m_ClientId].m_Prev;
		CNetObj_Character CurChar = m_pClient->m_Snap.m_aCharacters[pCurrent->m_ClientId].m_Cur;
		Pos = mix(vec2(PrevChar.m_X, PrevChar.m_Y), vec2(CurChar.m_X, CurChar.m_Y), Client()->IntraGameTick()) + Pos;
	}
	
	TUKernel()->AssetsRenderer()->DrawSprite(pCurrent->m_SpriteId, Pos, Size, Angle, 0x0, 1.0f);
}

void CComponent_Items::RenderText(const CNetObj_TU_Text *pPrev, const CNetObj_TU_Text *pCurrent)
{
	if(pCurrent->m_ItemLayer != GetLayer()) return;
	if(!TextRender()) return;

	vec2 Pos = mix(vec2(pPrev->m_X, pPrev->m_Y), vec2(pCurrent->m_X, pCurrent->m_Y), Client()->IntraGameTick());
	
	char aText[64];
	IntsToStr(pCurrent->m_aText, 16, &aText[0]);
	
	TUKernel()->AssetsRenderer()->DrawText(aText, Pos, tu::IntToColor(pCurrent->m_Color), pCurrent->m_Size, pCurrent->m_Alignment);
}

void CComponent_Items::RenderTextCharacter(const CNetObj_TU_TextCharacter *pPrev, const CNetObj_TU_TextCharacter *pCurrent)
{
	if(pCurrent->m_ItemLayer != GetLayer()) return;
	if(!TextRender()||!m_pClient->m_Snap.m_aCharacters[pCurrent->m_ClientId].m_Active) return;
	
	vec2 Pos = mix(vec2(pPrev->m_X, pPrev->m_Y), vec2(pCurrent->m_X, pCurrent->m_Y), Client()->IntraGameTick());
	
	if(m_pClient->m_LocalClientID != -1 && m_pClient->m_LocalClientID == pCurrent->m_ClientId)
	{
		Pos = m_pClient->m_LocalCharacterPos + Pos;
	}
	else
	{
		CNetObj_Character PrevChar = m_pClient->m_Snap.m_aCharacters[pCurrent->m_ClientId].m_Prev;
		CNetObj_Character CurChar = m_pClient->m_Snap.m_aCharacters[pCurrent->m_ClientId].m_Cur;
		Pos = mix(vec2(PrevChar.m_X, PrevChar.m_Y), vec2(CurChar.m_X, CurChar.m_Y), Client()->IntraGameTick()) + Pos;
	}
	
	char aText[64];
	IntsToStr(pCurrent->m_aText, 16, &aText[0]);
	
	TUKernel()->AssetsRenderer()->DrawText(aText, Pos, tu::IntToColor(pCurrent->m_Color), pCurrent->m_Size, pCurrent->m_Alignment);
}

void CComponent_Items::OnRender()
{
	if(Client()->State() < IClient::STATE_ONLINE)
		return;

	int Num = Client()->SnapNumItems(IClient::SNAP_CURRENT);
	for(int i = 0; i < Num; i++)
	{
		IClient::CSnapItem Item;
		const void *pData = Client()->SnapGetItem(IClient::SNAP_CURRENT, i, &Item);

		switch(Item.m_Type)
		{
			case NETOBJTYPE_TU_SPRITE:
			{
				const void *pPrev = Client()->SnapFindItem(IClient::SNAP_PREV, Item.m_Type, Item.m_ID);
				if(pPrev)
					RenderSprite((const CNetObj_TU_Sprite *)pPrev, (const CNetObj_TU_Sprite *)pData);
			}
			break;
				
			case NETOBJTYPE_TU_SPRITECHARACTER:
			{
				const void *pPrev = Client()->SnapFindItem(IClient::SNAP_PREV, Item.m_Type, Item.m_ID);
				if(pPrev)
					RenderSpriteCharacter((const CNetObj_TU_SpriteCharacter *)pPrev, (const CNetObj_TU_SpriteCharacter *)pData);
			}
			break;
				
			case NETOBJTYPE_TU_TEXT:
			{
				const void *pPrev = Client()->SnapFindItem(IClient::SNAP_PREV, Item.m_Type, Item.m_ID);
				if(pPrev)
					RenderText((const CNetObj_TU_Text *)pPrev, (const CNetObj_TU_Text *)pData);
			}
			break;
				
			case NETOBJTYPE_TU_TEXTCHARACTER:
			{
				const void *pPrev = Client()->SnapFindItem(IClient::SNAP_PREV, Item.m_Type, Item.m_ID);
				if(pPrev)
					RenderTextCharacter((const CNetObj_TU_TextCharacter *)pPrev, (const CNetObj_TU_TextCharacter *)pData);
			}
			break;
		}
	}

	int64 CurrentTime = time_get();
	int64 DeltaTime = CurrentTime - m_LastTime;
	m_LastTime = CurrentTime;

	if(Client()->State() == IClient::STATE_DEMOPLAYBACK)
	{
		const IDemoPlayer::CInfo *pInfo = DemoPlayer()->BaseInfo();
		if(!pInfo->m_Paused)
			UpdateEvents((float)((DeltaTime)/(double)time_freq())*pInfo->m_Speed);
	}
	else
	{
		if(m_pClient->m_Snap.m_pGameData && !(m_pClient->m_Snap.m_pGameData->m_GameStateFlags&GAMESTATEFLAG_PAUSED))
			UpdateEvents((float)((DeltaTime)/(double)time_freq()));
	}
}

void CComponent_Items::UpdateEvents(float DeltaTime)
{
	
}
	
bool CComponent_Items::ProcessEvent(int Type, CNetEvent_Common* pEvent)
{
	
}

}
