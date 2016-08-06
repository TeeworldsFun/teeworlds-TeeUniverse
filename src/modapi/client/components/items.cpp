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

CModAPI_Component_Items::CModAPI_Component_Items()
{
	m_LastTime = time_get();
}

void CModAPI_Component_Items::SetLayer(int Layer)
{
	m_Layer = Layer;
}

int CModAPI_Component_Items::GetLayer() const
{
	return m_Layer;
}
	
void CModAPI_Component_Items::RenderModAPISprite(const CNetObj_ModAPI_Sprite *pPrev, const CNetObj_ModAPI_Sprite *pCurrent)
{
	if(pCurrent->m_ItemLayer != GetLayer()) return;
	if(!ModAPIGraphics()) return;

	float Angle = mix(2.0*pi*static_cast<float>(pPrev->m_Angle)/360.0f, 2.0*pi*static_cast<float>(pCurrent->m_Angle)/360.0f, Client()->IntraGameTick());
	float Size = mix(pPrev->m_Size, pCurrent->m_Size, Client()->IntraGameTick());
	vec2 Pos = mix(vec2(pPrev->m_X, pPrev->m_Y), vec2(pCurrent->m_X, pCurrent->m_Y), Client()->IntraGameTick());
	
	ModAPIGraphics()->DrawSprite(pCurrent->m_SpriteId, Pos, Size, Angle, 0);
}

void CModAPI_Component_Items::RenderModAPISpriteCharacter(const CNetObj_ModAPI_SpriteCharacter *pPrev, const CNetObj_ModAPI_SpriteCharacter *pCurrent)
{
	if(pCurrent->m_ItemLayer != GetLayer()) return;
	if(!ModAPIGraphics()||!m_pClient->m_Snap.m_aCharacters[pCurrent->m_ClientId].m_Active) return;
	
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
	
	ModAPIGraphics()->DrawSprite(pCurrent->m_SpriteId, Pos, Size, Angle, 0);
}

void CModAPI_Component_Items::RenderModAPIText(const CNetObj_ModAPI_Text *pPrev, const CNetObj_ModAPI_Text *pCurrent)
{
	if(pCurrent->m_ItemLayer != GetLayer()) return;
	if(!TextRender()) return;

	vec2 Pos = mix(vec2(pPrev->m_X, pPrev->m_Y), vec2(pCurrent->m_X, pCurrent->m_Y), Client()->IntraGameTick());
	
	char aText[64];
	IntsToStr(pCurrent->m_aText, 16, &aText[0]);
	
	ModAPIGraphics()->DrawText(TextRender(), aText, Pos, ModAPI_IntToColor(pCurrent->m_Color), pCurrent->m_Size, pCurrent->m_Alignment);
}

void CModAPI_Component_Items::RenderModAPITextCharacter(const CNetObj_ModAPI_TextCharacter *pPrev, const CNetObj_ModAPI_TextCharacter *pCurrent)
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
	
	ModAPIGraphics()->DrawText(TextRender(), aText, Pos, ModAPI_IntToColor(pCurrent->m_Color), pCurrent->m_Size, pCurrent->m_Alignment);
}

void CModAPI_Component_Items::OnRender()
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
			case NETOBJTYPE_MODAPI_SPRITE:
			{
				const void *pPrev = Client()->SnapFindItem(IClient::SNAP_PREV, Item.m_Type, Item.m_ID);
				if(pPrev)
					RenderModAPISprite((const CNetObj_ModAPI_Sprite *)pPrev, (const CNetObj_ModAPI_Sprite *)pData);
			}
			break;
				
			case NETOBJTYPE_MODAPI_SPRITECHARACTER:
			{
				const void *pPrev = Client()->SnapFindItem(IClient::SNAP_PREV, Item.m_Type, Item.m_ID);
				if(pPrev)
					RenderModAPISpriteCharacter((const CNetObj_ModAPI_SpriteCharacter *)pPrev, (const CNetObj_ModAPI_SpriteCharacter *)pData);
			}
			break;
				
			case NETOBJTYPE_MODAPI_TEXT:
			{
				const void *pPrev = Client()->SnapFindItem(IClient::SNAP_PREV, Item.m_Type, Item.m_ID);
				if(pPrev)
					RenderModAPIText((const CNetObj_ModAPI_Text *)pPrev, (const CNetObj_ModAPI_Text *)pData);
			}
			break;
				
			case NETOBJTYPE_MODAPI_TEXTCHARACTER:
			{
				const void *pPrev = Client()->SnapFindItem(IClient::SNAP_PREV, Item.m_Type, Item.m_ID);
				if(pPrev)
					RenderModAPITextCharacter((const CNetObj_ModAPI_TextCharacter *)pPrev, (const CNetObj_ModAPI_TextCharacter *)pData);
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

void CModAPI_Component_Items::UpdateEvents(float DeltaTime)
{
	
}
	
bool CModAPI_Component_Items::ProcessEvent(int Type, CNetEvent_Common* pEvent)
{
	
}
