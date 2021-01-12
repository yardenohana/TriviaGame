#pragma once
#include <iostream>
#include <vector>
#include <bitset>

#include "json.hpp"
#include "Response.h"

#define BYTE_SIZE 8 // Byte: 8 bits

using json = nlohmann::json;

typedef unsigned char MY_BYTE;

// used as the specifier code of the response
enum responseId { TYPE_ERROR = 200, TYPE_LOGIN, TYPE_SIGNUP, TYPE_LOGOUT, TYPE_GET_ROOM, TYPE_GET_PLAYERS_IN_ROOM,
	TYPE_JOIN_ROOM, TYPE_CREATE_ROOM, TYPE_HIGHSCORE,TYPE_STATISTICS, TYPE_CLOSE_ROOM, TYPE_START_GAME, TYPE_GET_ROOM_STATE,
	TYPE_LEAVE_ROOM, TYPE_QUIT, TYPE_LEAVE_GAME, TYPE_SUMBIT_ANSWER, TYPE_GET_GAME_RESULTS, TYPE_GET_QUESTION };

#define SUCCESS 1 //code of action that succeeded


/*******************************************************************/
/* response messsage form: [ code - 1b | length - 4b | data - xb ] */
/*******************************************************************/


/*
Class which serializes the server's output,
in addition to it's type of response.
*/
class JsonResponsePacketSerializer {
	
private:

	// Used to insert the binary values of the data length
	void insertDataIntoMsg(std::vector<MY_BYTE>& serializedResponse, int lengthOfData) const;

	/* Serializes the json data, given a code- 
	  (error/login/signup) and returns the buffer */
	std::vector<MY_BYTE> serializeBase(json& jsonResponseData, int code) const;

public:
	// Useful method which returns a byte-like std::string from integer
	static std::string intToByteStr(const int number);
	static std::string bufferAsString(std::vector<MY_BYTE>& buff);

	std::vector<MY_BYTE> serializeResponse(ErrorResponse&) const;
	std::vector<MY_BYTE> serializeResponse(LoginResponse&) const;
	std::vector<MY_BYTE> serializeResponse(SignupResponse&) const;

	std::vector<MY_BYTE> serializeResponse(LogoutResponse&) const;
	std::vector<MY_BYTE> serializeResponse(GetRoomsResponse&) const;
	std::vector<MY_BYTE> serializeResponse(GetPlayersInRoomsResponse&) const;
	std::vector<MY_BYTE> serializeResponse(JoinRoomResponse&) const;
	std::vector<MY_BYTE> serializeResponse(CreateRoomResponse&) const;
	std::vector<MY_BYTE> serializeResponse(HighScoreResponse&) const;

	std::vector<MY_BYTE> serializeResponse(CloseRoomResponse&) const;
	std::vector<MY_BYTE> serializeResponse(StartGameResponse&) const;
	std::vector<MY_BYTE> serializeResponse(LeaveRoomResponse&) const;
	std::vector<MY_BYTE> serializeResponse(GetRoomStateResponse&) const;
	std::vector<MY_BYTE> serializeResponse(QuitResponse&) const;

	std::vector<MY_BYTE> serializeResponse(LeaveGameResponse&) const;
	std::vector<MY_BYTE> serializeResponse(GetQuestionResponse&) const;
	std::vector<MY_BYTE> serializeResponse(SumbitAnswerResponse&) const;
	std::vector<MY_BYTE> serializeResponse(GetGameResultsResponse&) const;

	std::vector<MY_BYTE> serializeUserStatistics(GetStatisticsResponse&)const;

};