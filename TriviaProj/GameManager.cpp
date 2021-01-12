#include "GameManager.h"

GameManager::GameManager(IDatabase* database, std::vector<Game> games)
{
	this->m_database = database;
	this->m_games = games;
}

GameManager::GameManager(IDatabase* database)
{
	std::vector<Game> games;
	this->m_games = games;
	this->m_database = database;
}

GameManager::~GameManager()
{
}


/*
function creates a new game.
*/
Game GameManager::createGame(Room* room)
{
	room->setIsActive(true);

	//getting the questions
	int numQuestions = room->getMetaData().numOfQuestionsInGame;

	SqliteDatabase* sql = (SqliteDatabase*)this->m_database;
	//opening the db, in case it's closed.
	sql->open();
	std::list<Question> questions = sql->getQuestions(numQuestions);

	//setting the players:
	std::map<LoggedUser, GameData> players;

	Game game(questions, players);
	return game;
}
