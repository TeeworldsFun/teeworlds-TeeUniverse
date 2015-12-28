// Copyright (c) 2015 Henritees

#include <game/server/gamecontext.h>
#include <game/server/gamecontroller.h>
#include <engine/server.h>

#include "netgui.h"

#define PREBUILD(msgtype) \
		CNetMsg_Sv_NetGui_##msgtype Msg;	\
		Msg.m_ID = NetGuiElemID;			\
		Msg.m_Dimension[0] = Dimensions.x;	\
		Msg.m_Dimension[1] = Dimensions.y;	\
		Msg.m_Dimension[2] = Dimensions.a;	\
		Msg.m_Dimension[3] = Dimensions.b


// ----------------------------- [start of GUI managing methods] -----------------------------
void CNetGui::CreateGui_EmptyGui(int ClientID)
{

}
void CNetGui::RemoveGui_EmptyGui(int ClientID)
{

}

// ------------------------------ [end of GUI managing methods] -----------------------------

void CNetGui::OnClientEnter(int ClientID)
{
}

void CNetGui::OnClientCompatible(int ClientID)
{

}

void CNetGui::OnClientDrop(int ClientID)
{
	m_NetGuiClients[ClientID] = false;
	// auto-generated clear's
	#define GUIDEFINE(name, netmsgname, args...) m_##name[ClientID].clear();
	#include <game/netguidefines.h>
	#undef GUIDEFINE
}

void CNetGui::OnMessage(int MsgID, void *pRawMsg, int ClientID)
{
	CPlayer *pPlayer = GameServer()->m_apPlayers[ClientID];

	if (MsgID == NETMSGTYPE_CL_NETGUI_TRIGGEREVENT)
	{
		CNetMsg_Cl_NetGui_TriggerEvent *pMsg = (CNetMsg_Cl_NetGui_TriggerEvent *)pRawMsg;

		switch(pMsg->m_Type)
		{
		case NETGUIMAGICNUMBER1: // verify compatibility
		{
			if(pMsg->m_ID == NETGUIMAGICNUMBER2)
			{
				dbg_msg("netgui", "client ID:%d has proven netgui compatibility (%d)", ClientID, pMsg->m_ID);
				m_NetGuiClients[ClientID] = true;

				// this seems to be the earliest moment possible for sending the first GUI...
				OnClientCompatible(ClientID);
			}
			else
			{
				GameServer()->SendChatTarget(ClientID, "[NetGUI] Your client has only partially proven NetGUI ability, you won't receive GUIs.");
				GameServer()->SendChatTarget(ClientID, "[NetGUI] You may ask the server owner for further instructions. Maybe it's time for an update?");
				dbg_msg("netgui", "client ID:%d has only partially proven netgui ability, what went wrong? (%d != %d)", ClientID, pMsg->m_ID, NETGUIMAGICNUMBER2);
			}
		}
		break;
		case NETMSGTYPE_SV_NETGUI_BUTTONMENU:
			bool exists = false;
			for(int i = 0; i < m_ButtonMenu[ClientID].size(); i++)
			{
				if(m_ButtonMenu[ClientID][i].m_ID == pMsg->m_ID)
					exists = true;
			}
			if(exists)
			{
				switch(pMsg->m_ID)
				{
					// TODO: handle button presses
				}
			}
			break;
		}
	}
	else if(MsgID == NETMSGTYPE_CL_NETGUI_RESPONSESTRING)
	{
		CNetMsg_Cl_NetGui_ResponseString *pMsg = (CNetMsg_Cl_NetGui_ResponseString *)pRawMsg;

		switch(pMsg->m_Type)
		{
			// TODO: handle string responses
		}
	}
	else if(MsgID == NETMSGTYPE_CL_NETGUI_RESPONSEINT)
	{
		CNetMsg_Cl_NetGui_ResponseInt *pMsg = (CNetMsg_Cl_NetGui_ResponseInt *)pRawMsg;

		switch(pMsg->m_Type)
		{
			// TODO: handle integer responses
		}
	}
}

void CNetGui::RemoveElement(int ClientID, int Type, int NetGuiElemID)
{
	if(!m_NetGuiClients[ClientID])
			return;

	CNetMsg_Sv_NetGui_RemoveElement Msg;
	Msg.m_Type = Type;
	Msg.m_ID = NetGuiElemID;

	// remove handler; the "args..." thingy is just for compatiblity and will be dropped
	#define GUIDEFINE(name, netmsgname, args...) \
		case NETMSGTYPE_SV_NETGUI_##netmsgname: \
			for(int i = 0; i < m_##name[ClientID].size(); i++) \
			{ \
				if(m_##name[ClientID][i].m_ID == NetGuiElemID) \
					m_##name[ClientID].remove_index(i); \
			} \
			break;


	switch(Type)
	{
		// auto-generated remove handlers
		#include <game/netguidefines.h>
		#undef GUIDEFINE
	}

	SendNetGui(ClientID, Msg);
}


void CNetGui::DoUIRect(int ClientID, int NetGuiElemID, vec4 Dimensions, vec4 Color, int Corner, float Rounding)
{
	if(!m_NetGuiClients[ClientID])
		return;

	PREBUILD(UIRect);
	Msg.m_Color[0] = Color.r;
	Msg.m_Color[1] = Color.g;
	Msg.m_Color[2] = Color.b;
	Msg.m_Color[3] = Color.a;
	Msg.m_Corner = Corner;
	Msg.m_RoundingX10 = (int)(Rounding*10.0f);

	m_UIRect[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::DoLabel(int ClientID, int NetGuiElemID, vec4 Dimensions, const char *pText, vec4 Color, int FontSize, int FontAlign, int MaxTextWidth)
{
	if(!m_NetGuiClients[ClientID])
		return;

	PREBUILD(Label);
	Msg.m_Text = pText;
	Msg.m_Color[0] = Color.r;
	Msg.m_Color[1] = Color.g;
	Msg.m_Color[2] = Color.b;
	Msg.m_Color[3] = Color.a;
	Msg.m_FontSize = FontSize;
	Msg.m_FontAlign = FontAlign;
	Msg.m_MaxTextWidth = MaxTextWidth;

	m_Label[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::DoButtonMenu(int ClientID, int NetGuiElemID, vec4 Dimensions, const char *pText, bool Selected)
{
	if(!m_NetGuiClients[ClientID])
		return;

	PREBUILD(ButtonMenu);
	Msg.m_Text = pText;
	Msg.m_Selected = Selected ? 1 : 0;

	m_ButtonMenu[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::DoEditBox(int ClientID, int NetGuiElemID, vec4 Dimensions, const char *pTitle, int SplitValue, int MaxTextWidth, bool Password)
{
	if(!m_NetGuiClients[ClientID])
		return;

	PREBUILD(EditBox);
	Msg.m_Title = pTitle;
	Msg.m_SplitValue = SplitValue;
	Msg.m_MaxTextWidth = MaxTextWidth;
	Msg.m_Password = Password ? 1 : 0;

	m_EditBox[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::DoCheckBox(int ClientID, int NetGuiElemID, vec4 Dimensions, const char *pText, bool Checked)
{
	if(!m_NetGuiClients[ClientID])
		return;

	PREBUILD(CheckBox);
	Msg.m_Text = pText;
	Msg.m_Checked = Checked ? 1 : 0;

	m_CheckBox[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::DoCheckBoxNumber(int ClientID, int NetGuiElemID, vec4 Dimensions, const char *pText, int MinValue, int MaxValue, int StepValue)
{
	if(!m_NetGuiClients[ClientID])
		return;

	PREBUILD(CheckBoxNumber);
	Msg.m_Text = pText;
	Msg.m_Value = MinValue;
	Msg.m_MinValue = MinValue;
	Msg.m_MaxValue = MaxValue;
	Msg.m_StepValue = StepValue;

	m_CheckBoxNumber[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::DoScrollbar(int ClientID, int NetGuiElemID, vec4 Dimensions, bool Vertical)
{
	if(!m_NetGuiClients[ClientID])
		return;

	PREBUILD(Scrollbar);
	Msg.m_Vertical = Vertical ? 1 : 0;

	m_Scrollbar[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::DoScrollbarOption(int ClientID, int NetGuiElemID, vec4 Dimensions, const char *pText, float VSplitVal, int Min, int Max, bool Infinite)
{
	if(!m_NetGuiClients[ClientID])
			return;

	PREBUILD(ScrollbarOption);
	Msg.m_Text = pText;
	Msg.m_VSplitValX10 = (int)(VSplitVal*10.0f);
	Msg.m_MinValue = Min;
	Msg.m_MaxValue = Max;
	Msg.m_Infinite = Infinite ? 1 : 0;

	m_ScrollbarOption[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::DoInfoBox(int ClientID, int NetGuiElemID, vec4 Dimensions, const char *pLabel, const char* pValue)
{
	if(!m_NetGuiClients[ClientID])
		return;

	PREBUILD(InfoBox);
	Msg.m_Label = pLabel;
	Msg.m_Value = pValue;

	m_InfoBox[ClientID].add(Msg);

	SendNetGui(ClientID, Msg);
}

void CNetGui::RequestData(int ClientID, int Type, int NetGuiElemID)
{
	if(!m_NetGuiClients[ClientID])
		return;

	CNetMsg_Sv_NetGui_RequestData Msg;
	Msg.m_ID = NetGuiElemID;
	Msg.m_Type = Type;
	GameServer()->Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, ClientID);
}


template<class T>
void CNetGui::SendNetGui(int ClientID, T Msg)
{
	if(ClientID < 0)
	{
		for(int i = 0; i < MAX_CLIENTS; ++i)
		{
			if(!GameServer()->m_apPlayers[i])
				continue;

			GameServer()->Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, i);
		}
	}
	else
		GameServer()->Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, ClientID);
}
