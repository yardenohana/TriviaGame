#pragma once
#include <iostream>
#include <vector>
#include <bitset>

#include "IRequestHandler.h"

#define BYTE_SIZE 8 // Byte == 8 bits

typedef unsigned char MY_BYTE; // Byte structure, represented by a bitset of 8 bits.
enum requestId { SIGNUP_REQUEST = 100, LOGIN_REQUEST, GET_PLAYERS_REQUEST, JOIN_ROOM_REQUEST, CREATE_ROOM_REQUEST,\
				 HIGHSCORE_REQUEST, LOGOUT_REQUEST, GET_ROOM_REQUEST, GET_STATISTICS, CLOSE_ROOM_REQUEST, START_GAME_REQUEST,\
				 GET_ROOM_STATE_REQUEST, LEAVE_ROOM_REQUEST, QUIT_REQUEST, LEAVE_GAME_REQUEST,GET_QUESTION_REQUEST, 
				  SUMBIT_ANSWER_REQUEST, GET_GAME_RESULT_REQUEST}; // Codes used to define a request

//forward declaration to prevent the circular dependency
class IRequestHandler;

struct RequestInfo
{
	std::vector<MY_BYTE> buffer; //the buffer of the json (still in binary)
	std::string ReceivalTime;
	unsigned int id; //code of request recived from client
};

struct RequestResult 
{
	std::vector<MY_BYTE> buffer; // the answer that the user requested
	IRequestHandler* newHandler; //the next state the user will be in after this one - the next handler that needs to be in service
	bool didActionWorked; // will be true if the action (or the quary) the user requested worked, false if something went wrong during the communication.
};

//struct to represent the login request
struct LoginRequest 
{
	std::string username;
	std::string password;
};

//struct to represent the signup request
struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

// struct to represent a request to get all players in room
struct GetPlayersInRoomRequest
{
	std::string roomName;
};

// struct to represent a request to join a room.
struct JoinRoomRequest
{
	std::string roomName;
};

// struct to represent a request to create a room.
struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};