#pragma once
#include <iostream>
#include <vector>
#include <bitset>
#include <exception>
#include <sstream>
#include <ctime>
#include <string>

#include "json.hpp"
#include "Request.h"
#include "JsonResponsePacketSerializer.h"

#define BYTE_SIZE 8 // Byte: 8 bits
#define START_OF_SIZE 3
#define START_OF_MSG 5
#define SIZE_OF_CHAR 1
#define SIZE_OF_TIME 80


using json = nlohmann::json;

typedef unsigned char MY_BYTE;


/*******************************************************************/
/* response messsage form: [ code - 1b | length - 4b | data - xb ] */
/*******************************************************************/

/*
Class which serializes the server's output,
in addition to it's type of response.
*/
class JsonRequestPacketDeserializer {
	
private:
	std::string getTime() const;

public:
	json getData(const char* buffer) const; // turns the string into json object
	
	LoginRequest deserializeLoginRequest(const char* buffer) const;
	SignupRequest deserializeSignupRequest(const char* buffer) const;
	RequestInfo deserializeInfo(const char* buffer) const;

	GetPlayersInRoomRequest deserializeGetPlayersRequest(const char* buffer) const;
	JoinRoomRequest deserializeJoinRoomRequest(const char* buffer) const;
	CreateRoomRequest deserializeCreateRoomRequest(const char* buffer) const;

	// help-convert functions:
	std::string turnBytesToString(std::vector<MY_BYTE> vec) const;
	static std::string charStrToString(const char* cstr);
};