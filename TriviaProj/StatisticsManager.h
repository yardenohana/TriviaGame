#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm> // for sort function
#include "SqliteDatabase.h"
#include "RoomManager.h"

#define NUM_OF_CORRECT_ANS 0
#define NUM_OF_GAMES 1
#define NUM_OF_ANS 2
#define AVG_OF_TIME 3
#define TOP_USERS 3

class StatisticsManager {

private:
	IDatabase* m_database;
	double calcScore(std::vector<std::string> usersStatistics) const;

public:
	StatisticsManager();
	StatisticsManager(IDatabase* DB);
	~StatisticsManager();

	void setDB(IDatabase* db);

	std::vector<std::string> getHighScore(std::vector<LoggedUser> allUsers) const; // get 5 top scored users 
	std::vector<std::string> getUserStatistics(std::string user) const; // specific user statistics
};