#ifndef MODAPI_CLIENT_COMPONENTS_ITEMS_H
#define MODAPI_CLIENT_COMPONENTS_ITEMS_H
#include <game/client/component.h>
#include <modapi/graphics.h>
#include <list>

class CModAPI_Component_Items : public CComponent
{
	struct CTextEventState
	{
		vec2 m_Pos;
		float m_Size;
		vec4 m_Color;
		int m_Alignment;
		char m_aText[64];
		
		int m_AnimationId;
		vec2 m_Offset;
		float m_Duration;
		float m_Time;
	};
	
private:
	int64 m_LastTime;
	int m_Layer;
	std::list<CTextEventState> m_TextEvent;
	
private:
	void RenderModAPISprite(const CNetObj_ModAPI_Sprite *pPrev, const struct CNetObj_ModAPI_Sprite *pCurrent);
	void RenderModAPISpriteCharacter(const struct CNetObj_ModAPI_SpriteCharacter *pPrev, const struct CNetObj_ModAPI_SpriteCharacter *pCurrent);
	
	void RenderModAPIText(const struct CNetObj_ModAPI_Text *pPrev, const struct CNetObj_ModAPI_Text *pCurrent);
	void RenderModAPITextCharacter(const struct CNetObj_ModAPI_TextCharacter *pPrev, const struct CNetObj_ModAPI_TextCharacter *pCurrent);

	void UpdateEvents(float DeltaTime);

public:
	CModAPI_Component_Items();

	void SetLayer(int Layer);
	int GetLayer() const;
	virtual void OnRender();
	
	bool ProcessEvent(int Type, CNetEvent_Common* pEvent);
};

#endif
