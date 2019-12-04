/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_LIFE_INDICATOR_H
#define GAME_SERVER_ENTITIES_LIFE_INDICATOR_H

#define VALUE_MARGIN 10

enum ValueBarType {
	VALUE_BAR_HP,
	VALUE_BAR_MANA
} ;

class CValueBar : public CEntity
{
public:
	CValueBar(CGameWorld *pGameWorld, int Owner, ValueBarType Type, int MaxValue);
	~CValueBar();

	virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);
	void SetValue(int Value);
	void SetMaxValue(int MaxValue);

private:
	CCharacter *m_pOwner;
	int m_Type;
	int m_Value;
	int m_MaxValue;
	bool m_Shown;
	int *m_IDS;
};

#endif
