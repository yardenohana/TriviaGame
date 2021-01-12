#include "StatisticsManager.h"

/*
The function calculates the user score based on the ratio of the number of correct answers to the average time it took the user to answer it.
input: vector of string that contains the data about the user
output: the score (type-double) of this user. 
*/
double StatisticsManager::calcScore(std::vector<std::string> usersStatistics) const
{
	if (std::stod(usersStatistics[AVG_OF_TIME]) == 0)
		return 0;
	return (std::stod(usersStatistics[NUM_OF_CORRECT_ANS]) / std::stod(usersStatistics[AVG_OF_TIME]));
}


StatisticsManager::StatisticsManager()
{
	this->m_database = new SqliteDatabase();
}

StatisticsManager::StatisticsManager(IDatabase* DB)
{
	this->m_database = DB;
}

StatisticsManager::~StatisticsManager()
{
	delete this->m_database;
}

void StatisticsManager::setDB(IDatabase* db)
{
	this->m_database = db;
}


/*
the function calculates and returns the 3 (or less) users with the highest score in the game.
input: vector of all the users that are connected to the server.
output: vector of strings, contains the usernames of the users with the highest scores.
*/
std::vector<std::string> StatisticsManager::getHighScore(std::vector<LoggedUser> allUsers) const
{
	std::vector<std::string> result; //final result vector, will contain the names of the users with the highest scores.

	//with the use in multimap, we can have several users with the same score.
	std::multimap<double, std::string> scores; //map that contains the score of the user as key, and the name of the user as value.
	std::multimap<double, std::string, std::greater<double>> sortedScores; //map that is builed the same, but is sorted in decsending order.
	std::multimap<double, std::string, std::greater<double>>::iterator sortItr;

	int i = 0;
	int size = TOP_USERS; // size of the final vector. supposed to be 3, but if there are less users in the room, will be the number of users in the room.

	//get the specific statistics and score for each user in the system
	for (LoggedUser user : allUsers)
	{
		std::vector<std::string> statistics = getUserStatistics(user.getUsername());
		double score = calcScore(statistics);
		std::pair<double, std::string> p(score, user.getUsername());
		scores.insert(p);
	}

	//sorting the scores map in decsending order:
	for (std::pair<double, std::string> userScore : scores)
	{
		sortedScores.insert(std::pair<double, std::string>(userScore.first, userScore.second));
	}

	//checking if there are less than five users in the room.
	if (sortedScores.size() < TOP_USERS)
		size = sortedScores.size();

	//insert into 'result' the usernames of the users with the highest score.
	for (sortItr = sortedScores.begin(); i < size; sortItr++)
	{
		result.push_back(sortItr->second);
		i++;
	}
	return result;

}

/*
The function retrieves the user's statistics from the db.
Input: the username
Output: a list of the statistics
[0 = numberOfCorrectAnswers, 1 = NumberOfPlayerGames, 2 = NumberOfTotalAnswers, 3 = PlayerAverageAnswerTime]
*/
std::vector<std::string> StatisticsManager::getUserStatistics(std::string user) const
{

	SqliteDatabase* sql = (SqliteDatabase*)this->m_database;
	//opening the db, in case it's closed.
	sql->open();
	std::vector<std::string> finalResults;

	int correctAns = sql->getNumOfCorrectAnswers(user);
	int games = sql->getNumOfPlayerGames(user);
	int ttlAns = sql->getNumOfTotalAnswers(user);
	double avgTime = sql->getPlayerAverageAnswerTime(user);

	finalResults.push_back(std::to_string(correctAns));
	finalResults.push_back(std::to_string(games));
	finalResults.push_back(std::to_string(ttlAns));
	finalResults.push_back(std::to_string(avgTime));

	return finalResults;
}
