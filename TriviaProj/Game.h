#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "Question.h"
#include "LoggedUser.h"

struct GameData {
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;


};

class Game
{
private:
	std::list<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
public:
	Game(std::list<Question> questions, std::map<LoggedUser, GameData> players);
	Game();
	~Game();

	//TODO:
	std::list<Question> getQuestionForUser(LoggedUser user);
	bool submitAnswer(LoggedUser user, std::string answer);
	void removePlayer(LoggedUser user);
};
