#include "Game.h"

//the functions are not implemented, not in v4.0.0 yet.
Game::Game(std::list<Question> questions, std::map<LoggedUser, GameData> players)
{
	this->m_questions = questions;
	this->m_players = players;
}

Game::Game()
{
}

Game::~Game()
{
}

std::list<Question> Game::getQuestionForUser(LoggedUser user)
{
	return std::list<Question>();
}

bool Game::submitAnswer(LoggedUser user, std::string answer)
{
	return false;
}

void Game::removePlayer(LoggedUser user)
{
}
