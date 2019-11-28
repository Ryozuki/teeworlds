/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"
#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>
#include <engine/shared/config.h>
#include <sqlite3.h>
#include <cstdarg>

CGameControllerMOD::CGameControllerMOD(CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "DotW";

	m_GameFlags = GAMEFLAG_TEAMS; // GAMEFLAG_TEAMS makes it a two-team gamemode

	int err;
	
	err = sqlite3_open(g_Config.m_SvDatabaseName, &m_DB);

	if(err)
	{
		Print("database", sqlite3_errmsg(m_DB));
		exit(1);
	}
	else
	{
		Print("database", "Database connection established");
	}

	Print("database", "Creating tables...");

	char *errMsg = NULL;

	err = sqlite3_exec(m_DB,
			"CREATE TABLE IF NOT EXISTS Player ("
			"	ID INT PRIMARY KEY NOT NULL,"
			"	Username TEXT NOT NULL,"
			"	Password TEXT NOT NULL,"
			"	MMR INT NOT NULL DEFAULT 0,"
			"	RegisterDate DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,"
			"	CONSTRAINT UK_Username UNIQUE (Username)"
			");"
			"CREATE TABLE IF NOT EXISTS Match ("
			"	ID INT PRIMARY KEY NOT NULL,"
			"	TeamWin INT NOT NULL,"
			"	ScoreTeam1 INT NOT NULL,"
			"	ScoreTeam2 INT NOT NULL,"
			"	IsRanked BOOL NOT NULL,"
			"	Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL"
			");"
			"CREATE TABLE IF NOT EXISTS PlayerMatch ("
			"	Match INT NOT NULL,"
			"	Player INT NOT NULL,"
			"	Kills INT NOT NULL,"
			"	Deaths INT NOT NULL,"
			"	FOREIGN KEY(Match) REFERENCES Match(ID),"
			"	FOREIGN KEY(Player) REFERENCES Player(ID),"
			"	CONSTRAINT UK_PlayerMatch UNIQUE (Match, Player)"
			");"
			, 
			NULL, 0, &errMsg);

	if(err != SQLITE_OK) {
		Print("database", "Error creating tables");
		Print("database", errMsg);
	} 
	else {
		Print("database", "Tables created succesfully");
	}
}

CGameControllerMOD::~CGameControllerMOD() {
	sqlite3_close(m_DB);
}

void CGameControllerMOD::Print(const char *submodule, const char *buffer) {
	char aBufName[48];
	str_format(aBufName, sizeof(aBufName), "DotW/%s", submodule);
	GameServer()->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, aBufName, buffer);
}

void CGameControllerMOD::Tick()
{
	// this is the main part of the gamemode, this function is run every tick

	IGameController::Tick();
}

int CGameControllerMOD::OnCharacterDeath(CCharacter *pVictim, CPlayer *pKiller, int Weapon)
{
	IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);
	return 0;
}

void CGameControllerMOD::OnPlayerConnect(CPlayer *pPlayer) {
	IGameController::OnPlayerConnect(pPlayer);
}

void CGameControllerMOD::OnPlayerDisconnect(CPlayer *pPlayer) {
	IGameController::OnPlayerDisconnect(pPlayer);
}
