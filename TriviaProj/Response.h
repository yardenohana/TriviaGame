#pragma once
#include <iostream>
#include "Room.h"

// A response for a login request from a client
struct LoginResponse {

	unsigned int status;
};

// A response for a signup request from a client
struct SignupResponse {

	unsigned int status;
};

// An error response, used in every error type
struct ErrorResponse {

	std::string message;
};

struct LogoutResponse {

	unsigned int status;
};

struct QuitResponse {

	unsigned int status;
};

/****************/
/*Rooms Response*/
/****************/

struct GetRoomsResponse {

	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomsResponse {

	std::vector<std::string> players;
};

struct JoinRoomResponse {

	unsigned int status;
};

struct CreateRoomResponse {

	unsigned int status;
};

struct CloseRoomResponse {

	unsigned int status;
};

struct StartGameResponse {

	unsigned int status;
};

struct LeaveRoomResponse {

	unsigned int status;
};

struct GetRoomStateResponse {

	unsigned int status;
	bool hasGameBegun; 
	std::vector<std::string> players;
	unsigned int questionCount;
	double answerTimeout;
};


/**************/
/* statistics */
/**************/

struct GetStatisticsResponse {
	// [0 = numberOfCorrectAnswers, 1 = NumberOfPlayerGames, 2 = NumberOfTotalAnswers, 3 = PlayerAverageAnswerTime]
	unsigned int status;
	std::vector<std::string> statistics;
};

struct HighScoreResponse {

	unsigned int status;
	std::vector<std::string> names;
};


/*********/
/* Games */
/*********/

struct LeaveGameResponse {

	unsigned int status;
};

struct GetQuestionResponse {

	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
};

struct SumbitAnswerResponse {

	unsigned int status;
	unsigned int correctAnswer;
};

struct PlayerResults{

	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

struct GetGameResultsResponse {

	unsigned int status;
	std::vector<PlayerResults> results;
};