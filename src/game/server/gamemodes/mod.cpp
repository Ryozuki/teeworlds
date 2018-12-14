/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"
#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>
#include <game/mapitems.h>
#include <game/server/player.h>

CGameControllerMOD::CGameControllerMOD(CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "Jailbreak";

	//m_GameFlags = GAMEFLAG_TEAMS; // GAMEFLAG_TEAMS makes it a two-team gamemode
	SetGameState(IGS_GAME_RUNNING);
}

void CGameControllerMOD::Tick()
{
	// this is the main part of the gamemode, this function is run every tick

	// IGameController::Tick();
}

void CGameControllerMOD::OnCharacterSpawn(CCharacter *pChr)
{
	// IGameController::OnCharacterSpawn(pChr);
	pChr->IncreaseArmor(5);
	pChr->IncreaseHealth(10);
	pChr->GiveWeapon(WEAPON_HAMMER, -1);
	pChr->GiveWeapon(WEAPON_GUN, -1);
	pChr->Freeze(5);
}

void CGameControllerMOD::OnCharacterTick(CCharacter *pChr)
{
	if(pChr->IsFrozen()) {
		pChr->m_Input.m_Direction = 0;
		pChr->m_Input.m_Jump = 0;
		pChr->m_Input.m_Hook = 0;
		int curTick = GameServer()->Server()->Tick();
		if(curTick % GameServer()->Server()->TickSpeed() == GameServer()->Server()->TickSpeed()  - 1) {
			int freezeTick = pChr->FrozenExpireTick();
			int seconds = 1 + (freezeTick - curTick) / GameServer()->Server()->TickSpeed();
			GameServer()->CreateDamage(pChr->GetPos(), pChr->GetPlayer()->GetCID(), pChr->GetPos(), seconds, 0, true);
		}
	}
	if(!pChr->IsFrozen() && pChr->WasLastTickFrozen())
		pChr->UnFreeze();
}

void CGameControllerMOD::OnCharacterPostTick(CCharacter *pChr)
{
}
