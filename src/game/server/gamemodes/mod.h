/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_MOD_H
#define GAME_SERVER_GAMEMODES_MOD_H
#include <game/server/gamecontroller.h>

// you can subclass GAMECONTROLLER_CTF, GAMECONTROLLER_TDM etc if you want
// todo a modification with their base as well.
class CGameControllerMOD : public IGameController
{
public:
	explicit CGameControllerMOD(class CGameContext *pGameServer);

	void Tick() override;



	void OnCharacterSpawn(class CCharacter *pChr) override;

	void OnCharacterTick(class CCharacter *pChr) override;

	void OnCharacterPostTick(class CCharacter *pChr) override;

	// add more virtual functions here if you wish
};
#endif
