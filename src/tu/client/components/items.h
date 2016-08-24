#ifndef TU_CLIENT_COMPONENTS_ITEMS_H
#define TU_CLIENT_COMPONENTS_ITEMS_H

#include <game/client/component.h>
#include <tu/graphics.h>
#include <list>

namespace tu
{

class CComponent_Items : public CComponent
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
	void RenderSprite(const CNetObj_TU_Sprite *pPrev, const struct CNetObj_TU_Sprite *pCurrent);
	void RenderSpriteCharacter(const struct CNetObj_TU_SpriteCharacter *pPrev, const struct CNetObj_TU_SpriteCharacter *pCurrent);
	
	void RenderText(const struct CNetObj_TU_Text *pPrev, const struct CNetObj_TU_Text *pCurrent);
	void RenderTextCharacter(const struct CNetObj_TU_TextCharacter *pPrev, const struct CNetObj_TU_TextCharacter *pCurrent);

	void UpdateEvents(float DeltaTime);

public:
	CComponent_Items();

	void SetLayer(int Layer);
	int GetLayer() const;
	virtual void OnRender();
	
	bool ProcessEvent(int Type, CNetEvent_Common* pEvent);
};

}

#endif
