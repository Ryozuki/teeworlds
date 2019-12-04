/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_LIFE_INDICATOR_H
#define GAME_SERVER_ENTITIES_LIFE_INDICATOR_H

class CLifeIndicator : public CEntity
{
public:
	CLifeIndicator(CGameWorld *pGameWorld, int Owner, int MaxLife);

	int GetOwner() const { return m_Owner; }

	virtual void Reset();
	virtual void Tick();
	virtual void TickDefered();
	virtual void Snap(int SnappingClient);

private:
	int m_Owner;
	int m_Life;
	int m_MaxLife;
	bool m_Shown;
	int *m_IDS;
};

#endif
