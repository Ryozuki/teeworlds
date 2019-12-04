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
			"	ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"	Username TEXT NOT NULL,"
			"	Password TEXT NOT NULL,"
			"	MMR INT NOT NULL DEFAULT 0,"
			"	RegisterDate DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,"
			"	CONSTRAINT UK_Username UNIQUE (Username)"
			");"
			"CREATE TABLE IF NOT EXISTS Match ("
			"	ID INTEGER PRIMARY KEY AUTOINCREMENT,"
			"	TeamWin INT NOT NULL,"
			"	ScoreTeam1 INT NOT NULL,"
			"	ScoreTeam2 INT NOT NULL,"
			"	IsRanked BOOL NOT NULL,"
			"	Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL"
			");"
			"CREATE TABLE IF NOT EXISTS PlayerMatch ("
			"	Match INTEGER NOT NULL,"
			"	Player INTEGER NOT NULL,"
			"	Kills INT NOT NULL,"
			"	Deaths INT NOT NULL,"
			"	FOREIGN KEY(Match) REFERENCES Match(ID),"
			"	FOREIGN KEY(Player) REFERENCES Player(ID),"
			"	CONSTRAINT UK_PlayerMatch UNIQUE (Match, Player)"
			");"
			,
			NULL, 0, &errMsg);

	if(err != SQLITE_OK) {
		dbg_msg("database", "error creating tables: %s", errMsg);
	}
	else {
		dbg_msg("database", "tables created succesfully.");
	}

	RegisterPlayer("Ryozuki", "test");
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

bool CGameControllerMOD::UsernameExists(const char *Username)
{
	sqlite3_stmt *pStmt;
	sqlite3_prepare_v2(DB(),
			"SELECT COUNT(*) FROM Player WHERE Username = ?;",
			-1, &pStmt, NULL);
	sqlite3_bind_text(pStmt, 1, Username, -1, SQLITE_TRANSIENT);

	int code = sqlite3_step(pStmt);


	if(code != SQLITE_ROW || sqlite3_column_count(pStmt) == 0) {
		Print("database", "No rows returned when using COUNT(*).");
		sqlite3_finalize(pStmt);
		return false;
	}

	int count = sqlite3_column_int(pStmt, 0);

	sqlite3_finalize(pStmt);
	return count;
}

bool CGameControllerMOD::RegisterPlayer(const char *Username, const char *Password)
{
	sqlite3_stmt *pStmt;
	sqlite3_prepare_v2(DB(),
			"INSERT INTO Player (Username, Password) VALUES (?,?);",
			-1, &pStmt, NULL);

	sqlite3_bind_text(pStmt, 1, Username, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(pStmt, 2, Password, -1, SQLITE_TRANSIENT);

	int code = sqlite3_step(pStmt);
	sqlite3_finalize(pStmt);

	if(code != SQLITE_DONE)
	{
		dbg_msg("database", "Error inserting new player");
		return false;
	}

	return true;
}
