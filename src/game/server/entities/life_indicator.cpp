/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/gamecontext.h>
#include <game/server/player.h>


#include "character.h"
#include "life_indicator.h"

CLifeIndicator::CLifeIndicator(CGameWorld *pGameWorld, int Owner, int MaxLife)
	: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP, vec2(0, 0), 14)
{
	m_Owner = Owner;
	m_MaxLife = MaxLife;
	m_Life = m_MaxLife;
	m_Shown = true;
	m_IDS = (int*)mem_alloc(sizeof(int) * MaxLife, 1);

	for(int i = 0; i < MaxLife; i++)
		m_IDS[i] = Server()->SnapNewID();

	GameWorld()->InsertEntity(this);
}

void CLifeIndicator::Reset()
{
	GameWorld()->DestroyEntity(this);
	mem_free(m_IDS);
}

void CLifeIndicator::Tick()
{
	CCharacter *pOwnerChar = GameServer()->GetPlayerChar(m_Owner);

	if(!pOwnerChar)
		return;

	m_Pos = pOwnerChar->GetPos(); 
}

void CLifeIndicator::TickDefered()
{
}

void CLifeIndicator::Snap(int SnappingClient)
{
	CCharacter *pOwnerChar = GameServer()->GetPlayerChar(m_Owner);

	if(!pOwnerChar)
		return;

	if(NetworkClipped(SnappingClient))
		return;

	// TODO: Instead of all hearts, display 1 shield for every 2 life, if life is odd show 1 heart.
	
	for(int i = 0; i < m_Life; i++) {
		CNetObj_Pickup *pPickup = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_IDS[i], sizeof(CNetObj_Pickup)));

		if(!pPickup)
			return;

		pPickup->m_X = m_Pos.x - (20 * (m_Life - 1)) / 2 + i * 20;
		pPickup->m_Y = m_Pos.y - 50;
		pPickup->m_Type = PICKUP_HEALTH;

	}

}
