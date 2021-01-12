#pragma once
#include <iostream>
#include <string>
#include <io.h>
#include <list>
#include <vector>
#include "IDatabase.h"
#include "sqlite3.h"
#include "Question.h"

#define FILE_NOT_EXISTS -1

class SqliteDatabase : public IDatabase {

private:
	sqlite3* m_database;
	
	static std::string charPtrToString(const char* str); // helpful function. char array to std::string.
	bool initDatabase() const; // initializes the database in case it hasn't already

public:
	const std::string m_dbName = "TriviaDB.sqlite";
	bool open(); // Open the database
	
	virtual bool doesUserExist(std::string username) const;
	virtual bool doesPasswordMatchToUser(std::string username, std::string password) const;
	virtual void addNewUser(std::string name, std::string password, std::string email) const;
	
	// Callback methods, used to retrieve data from the DB
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
	static int usersCallback(void* NotUsed, int argc, char** argv, char** azColName);
	static int numCallback(void* NotUsed, int argc, char** argv, char** azColName);
	static int questionsCallback(void* data, int argc, char** argv, char** azColName);

	// generates the questions, insert them into the database
	bool questions() const;
	bool queryQuestion(std::string question, std::string a, std::string b, std::string c, std::string d) const;
	virtual std::list<Question> getQuestions(int numOfQuestions) const;

	virtual double getPlayerAverageAnswerTime(std::string username) const;
	virtual int getNumOfCorrectAnswers(std::string username) const;
	virtual int getNumOfTotalAnswers(std::string username) const;
	virtual int getNumOfPlayerGames(std::string username) const;

};