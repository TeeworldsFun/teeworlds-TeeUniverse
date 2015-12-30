// Copyright (c) 2015 Henritees

#include "netgui.h"

#define GUI_BUILDRESPONSE(name, type) \
		int index = -1;\
		for(int i = 0; i < m_ModAPI_Gui##name.size(); i++)\
		{\
			if(m_ModAPI_Gui##name[i].m_ID == pMsg->m_ID)\
				index = i;\
		}\
		if(index < 0)\
			break;\
		CNetMsg_ModAPI_Cl_GuiResponse##type Reply;\
		Reply.m_ID = pMsg->m_ID;\
		Reply.m_Type = pMsg->m_Type

#define GUIRECEIVE_INIT(name) \
		CNetMsg_ModAPI_Sv_Gui##name *pMsg = (CNetMsg_ModAPI_Sv_Gui##name *)pRawMsg;\
		CNetMsg_ModAPI_Sv_Gui##name e;\
		e.m_ID = pMsg->m_ID;\
		e.m_Dimension[0] = pMsg->m_Dimension[0];\
		e.m_Dimension[1] = pMsg->m_Dimension[1];\
		e.m_Dimension[2] = pMsg->m_Dimension[2];\
		e.m_Dimension[3] = pMsg->m_Dimension[3]

#define GUIRECEIVE_FINALIZE(name) \
		if(m_ModAPI_Gui##name.size() > 1)\
		{\
			for(int i = 0; i < m_ModAPI_Gui##name.size(); i++)\
			{\
				if(m_ModAPI_Gui##name[i].m_ID == e.m_ID)\
					m_ModAPI_Gui##name.remove_index(i);\
			}\
		}\
		m_ModAPI_Gui##name.add(e);\
		SortGuiList<CNetMsg_ModAPI_Sv_Gui##name>(m_ModAPI_Gui##name)


void CModAPIGui::OnReset()
{
	// auto-generated clear's
	#define GUIDEFINE(name, netmsgname, args...) m_ModAPI_Gui##name.clear();
	#include <modapi/guidefines.h>
	#undef GUIDEFINE
}

void CModAPIGui::OnMessage(int MsgId, void *pRawMsg)
{
	if(MsgId == NETMSGTYPE_MODAPI_SV_GUIREMOVEELEMENT)
	{
		CNetMsg_ModAPI_Sv_GuiRemoveElement *pMsg = (CNetMsg_ModAPI_Sv_GuiRemoveElement *)pRawMsg;

		// remove handler; the "args..." thingy is just for compatiblity and will be dropped
		#define GUIDEFINE(name, netmsgname, args...) \
			case NETMSGTYPE_MODAPI_SV_GUI##netmsgname: \
				for(int i = 0; i < m_ModAPI_Gui##name.size(); i++) \
				{ \
					if(m_ModAPI_Gui##name[i].m_ID == pMsg->m_ID) \
						m_ModAPI_Gui##name.remove_index(i); \
				} \
				break;

		switch(pMsg->m_Type)
		{
			// auto-generated remove handlers
			#include <modapi/guidefines.h>
			#undef GUIDEFINE
		}
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUIREQUESTDATA)
	{
		CNetMsg_ModAPI_Sv_GuiRequestData *pMsg = (CNetMsg_ModAPI_Sv_GuiRequestData *)pRawMsg;
		switch(pMsg->m_Type)
		{
		case NETMSGTYPE_MODAPI_SV_GUIEDITBOX:
		{
			GUI_BUILDRESPONSE(EditBox, String);
			Reply.m_Text = m_aModAPI_GuiEditBoxContent[index];
			Client()->SendPackMsg(&Reply, MSGFLAG_VITAL);
		}
		break;
		case NETMSGTYPE_MODAPI_SV_GUICHECKBOX:
		{
			GUI_BUILDRESPONSE(CheckBox, Int);
			Reply.m_Value = m_ModAPI_GuiCheckBox[index].m_Checked;
			Client()->SendPackMsg(&Reply, MSGFLAG_VITAL);
		}
		break;
		case NETMSGTYPE_MODAPI_SV_GUICHECKBOXNUMBER:
		{
			GUI_BUILDRESPONSE(CheckBoxNumber, Int);
			Reply.m_Value = m_ModAPI_GuiCheckBoxNumber[index].m_Value;
			Client()->SendPackMsg(&Reply, MSGFLAG_VITAL);
		}
		break;
		case NETMSGTYPE_MODAPI_SV_GUISCROLLBAR:
		{
			GUI_BUILDRESPONSE(Scrollbar, Int);
			Reply.m_Value = m_ModAPI_GuiScrollbar[index].m_ValueX100;
			Client()->SendPackMsg(&Reply, MSGFLAG_VITAL);
		}
		break;
		case NETMSGTYPE_MODAPI_SV_GUISCROLLBAROPTION:
		{
			GUI_BUILDRESPONSE(ScrollbarOption, Int);
			Reply.m_Value = m_ModAPI_GuiScrollbarOption[index].m_Value;
			Client()->SendPackMsg(&Reply, MSGFLAG_VITAL);
		}
		break;

		}
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUIUIRECT)
	{
		GUIRECEIVE_INIT(UIRect);

		e.m_Color[0] = pMsg->m_Color[0];
		e.m_Color[1] = pMsg->m_Color[1];
		e.m_Color[2] = pMsg->m_Color[2];
		e.m_Color[3] = pMsg->m_Color[3];
		e.m_Corner = pMsg->m_Corner;
		e.m_RoundingX10 = pMsg->m_RoundingX10;

		GUIRECEIVE_FINALIZE(UIRect);
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUILABEL)
	{
		GUIRECEIVE_INIT(Label);

		char* aBuf = (char*)mem_alloc(512, 0);
		str_format(aBuf, 512, "%s", pMsg->m_Text);
		e.m_Text = aBuf;
		e.m_Color[0] = pMsg->m_Color[0];
		e.m_Color[1] = pMsg->m_Color[1];
		e.m_Color[2] = pMsg->m_Color[2];
		e.m_Color[3] = pMsg->m_Color[3];
		e.m_FontSize = pMsg->m_FontSize;
		e.m_FontAlign = pMsg->m_FontAlign;

		GUIRECEIVE_FINALIZE(Label);
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUIBUTTONMENU)
	{
		GUIRECEIVE_INIT(ButtonMenu);

		char* aBuf = (char*)mem_alloc(512, 0);
		str_format(aBuf, 512, "%s", pMsg->m_Text);
		e.m_Text = aBuf;
		e.m_Selected = pMsg->m_Selected;

		GUIRECEIVE_FINALIZE(ButtonMenu);
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUIEDITBOX)
	{
		GUIRECEIVE_INIT(EditBox);

		char* aBuf = (char*)mem_alloc(512, 0);
		str_format(aBuf, 512, "%s", pMsg->m_Title);
		e.m_Title = aBuf;
		e.m_SplitValue = pMsg->m_SplitValue;
		e.m_MaxTextWidth = pMsg->m_MaxTextWidth;
		e.m_Password = pMsg->m_Password;

		GUIRECEIVE_FINALIZE(EditBox);
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUICHECKBOX)
	{
		GUIRECEIVE_INIT(CheckBox);

		char* aBuf = (char*)mem_alloc(512, 0);
		str_format(aBuf, 512, "%s", pMsg->m_Text);
		e.m_Text = aBuf;
		e.m_Checked = pMsg->m_Checked;

		GUIRECEIVE_FINALIZE(CheckBox);
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUICHECKBOXNUMBER)
	{
		GUIRECEIVE_INIT(CheckBoxNumber);

		char* aBuf = (char*)mem_alloc(512, 0);
		str_format(aBuf, 512, "%s", pMsg->m_Text);
		e.m_Text = aBuf;
		e.m_Value = pMsg->m_Value;
		e.m_MinValue = pMsg->m_MinValue;
		e.m_MaxValue = pMsg->m_MaxValue;
		e.m_StepValue = pMsg->m_StepValue;

		GUIRECEIVE_FINALIZE(CheckBoxNumber);
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUISCROLLBAR)
	{
		GUIRECEIVE_INIT(Scrollbar);
		GUIRECEIVE_FINALIZE(Scrollbar);
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUISCROLLBAROPTION)
	{
		GUIRECEIVE_INIT(ScrollbarOption);

		char* aBuf = (char*)mem_alloc(512, 0);
		str_format(aBuf, 512, "%s", pMsg->m_Text);
		e.m_Text = aBuf;
		e.m_Value = pMsg->m_Value;
		e.m_MinValue = pMsg->m_MinValue;
		e.m_MaxValue = pMsg->m_MaxValue;
		e.m_VSplitValX10 = pMsg->m_VSplitValX10;

		GUIRECEIVE_FINALIZE(ScrollbarOption);
	}
	else if(MsgId == NETMSGTYPE_MODAPI_SV_GUIINFOBOX)
	{
		GUIRECEIVE_INIT(InfoBox);

		char* aBufLabel = (char*)mem_alloc(512, 0);
		str_format(aBufLabel, 512, "%s", pMsg->m_Label);
		e.m_Label = aBufLabel;
		char* aBufValue = (char*)mem_alloc(512, 0);
		str_format(aBufValue, 512, "%s", pMsg->m_Value);
		e.m_Value = aBufValue;

		GUIRECEIVE_FINALIZE(InfoBox);
	}
}
