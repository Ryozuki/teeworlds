/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <game/server/player.h>


#include "character.h"
#include "value_bar.h"

CValueBar::CValueBar(CGameWorld *pGameWorld, int Owner, ValueBarType Type, int MaxValue)
	: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP, vec2())
{
	m_pOwner = GameServer()->GetPlayerChar(Owner);
	m_Type = Type;
	m_Shown = true;
	SetMaxValue(MaxValue);
	SetValue(MaxValue);

	GameWorld()->InsertEntity(this);
}

CValueBar::~CValueBar()
{
	if(m_IDS)
		mem_free(m_IDS);
}

void CValueBar::Reset()
{
	mem_free(m_IDS);
	GameWorld()->DestroyEntity(this);
}

void CValueBar::Tick()
{
	m_Pos = m_pOwner->GetPos();
}

void CValueBar::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	int type;

	if(m_Type == VALUE_BAR_HP)
		type = PICKUP_HEALTH;
	else if(m_Type == VALUE_BAR_MANA)
		type = PICKUP_ARMOR;
	else
		type = PICKUP_HEALTH;

	int OffSet = 0;

	if(m_Type == VALUE_BAR_HP)
		OffSet = 50;
	else if(m_Type == VALUE_BAR_MANA)
		OffSet = 40;

	for(int i = 0; i < m_Value; i++) {
		CNetObj_Pickup *pPickup = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_IDS[i], sizeof(CNetObj_Pickup)));

		if(!pPickup)
			return;

		pPickup->m_X = m_Pos.x - (VALUE_MARGIN * (m_Value - 1)) / 2 + i * VALUE_MARGIN;
		pPickup->m_Y = m_Pos.y - OffSet;
		pPickup->m_Type = type;

	}

}

void CValueBar::SetValue(int Value)
{
	if(Value > m_MaxValue) {
		m_Value = m_MaxValue;
		return;
	}
	m_Value = Value;
}

void CValueBar::SetMaxValue(int MaxValue)
{
	dbg_assert(MaxValue >= 0, "valuebar max value cannot be 0");

	if(m_IDS) {
		for(int i = 0; i < m_MaxValue; i++) {
			Server()->SnapFreeID(m_IDS[i]);
		}
		mem_free(m_IDS);
	}

	m_IDS = (int*)mem_alloc(sizeof(int) * MaxValue, 1);

	for(int i = 0; i < MaxValue; i++) {
		m_IDS[i] = Server()->SnapNewID();
	}

	m_MaxValue = MaxValue;
}
