#pragma once
#include <iostream>
#include <list>
#include "Question.h"

/*
An interface used as a basic database communicate with the server.
*/
class IDatabase {

public:
	virtual bool doesUserExist(std::string username) const = 0;
	//function checks if the password in the db is the same as password recived as a parameter:
	virtual bool doesPasswordMatchToUser(std::string username, std::string password) const = 0;
	virtual void addNewUser(std::string name, std::string password, std::string email) const = 0;

	virtual std::list<Question> getQuestions(int numOfQuestions) const = 0;
	virtual double getPlayerAverageAnswerTime(std::string username) const = 0;
	virtual int getNumOfCorrectAnswers(std::string username) const = 0;
	virtual int getNumOfTotalAnswers(std::string username) const = 0;
	virtual int getNumOfPlayerGames(std::string username) const = 0;
};


