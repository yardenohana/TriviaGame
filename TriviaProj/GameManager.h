#pragma once
#include <iostream>
#include <vector>
#include "IDatabase.h"
#include "SqliteDatabase.h"
#include "Game.h"
#include "Room.h"

class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game> m_games;
public:
	GameManager(IDatabase* database, std::vector<Game> games);
	GameManager(IDatabase* database);
	~GameManager();

	//TODO:
	Game createGame(Room* room);
	void deleteGame();
};