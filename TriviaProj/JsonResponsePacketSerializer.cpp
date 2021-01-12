#include "JsonResponsePacketSerializer.h"

/*
Converts an integer into a byte based std::string.
Input: the integer number
Output: std::string byte-based (8 bits)
*/
std::string JsonResponsePacketSerializer::intToByteStr(const int number)
{
	return std::bitset<BYTE_SIZE>(number).to_string();
}


/*
The function inserts the length of the data, as a 
binary form, into the final buffer.
Input: the buffer of message, the length as an integer number
Output: none
*/
void JsonResponsePacketSerializer::insertDataIntoMsg(std::vector<MY_BYTE>& serializedResponse, int lengthOfData) const
{
	// Get a std::string byte-like representation (e.g: "10101")
	std::string lenStrByte = intToByteStr(lengthOfData);
	
	// Create a buffer of 4 bytes for the length
	std::bitset<BYTE_SIZE * 4> bytesList(lenStrByte);

	// Divide each byte in order to insert into each vector node
	std::string newLength = bytesList.to_string();
	std::bitset<BYTE_SIZE> firstByte(newLength.substr(0, BYTE_SIZE + 1)); // first byte
	std::bitset<BYTE_SIZE> secByte(newLength.substr(BYTE_SIZE, 2 * BYTE_SIZE + 1)); // second byte
	std::bitset<BYTE_SIZE> thirdByte(newLength.substr(2 * BYTE_SIZE, 3 * BYTE_SIZE + 1)); // third byte
	std::bitset<BYTE_SIZE> fourthByte(newLength.substr(3 * BYTE_SIZE, 4 * BYTE_SIZE + 1)); // fourth byte

	// Push the 4 bytes into the buffer, as ASCII values
	serializedResponse.push_back(firstByte.to_ulong());
	serializedResponse.push_back(secByte.to_ulong());
	serializedResponse.push_back(thirdByte.to_ulong());
	serializedResponse.push_back(fourthByte.to_ulong());
}


/*
The method returns a vector of bytes which represents the error message.
Input: the error reposnse object
Output: the byte vector, represents the error json message
*/
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeBase(json& jsonResponseData, int code) const
{
	// The buffer which contains the response
	std::vector<MY_BYTE> serializedResponse;

	// First parameter: code of message, represents the type of comment.
	std::bitset<BYTE_SIZE> codeOfMessage(intToByteStr(code));
	serializedResponse.push_back(codeOfMessage.to_ulong());

	// Second parameter: the length of the data
	std::string jsStr = jsonResponseData.dump(); // converts the json type into std::string
	this->insertDataIntoMsg(serializedResponse, jsStr.length());
	
	// Third parameter: the data itself.
	// converts into a serialized binary-json (represented in vector of bytes)
	std::vector<MY_BYTE> serializedData;

	// Move the data into the message vector
	for (unsigned char ch : jsonResponseData.dump())
		serializedResponse.push_back(ch);

	return serializedResponse;
}


// returns the buffer as a string, each character is a byte.
std::string JsonResponsePacketSerializer::bufferAsString(std::vector<MY_BYTE>& buff)
{
	std::string ans = "";
	std::vector<MY_BYTE>::iterator byte;
	
	for (byte = buff.begin(); byte != buff.end(); byte++)
		ans += *byte;

	return ans;
}


/*
The method returns a vector of bytes which represents the error message.
Input: the error response object
Output: the byte vector, represents the error json message
*/
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(ErrorResponse& err) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"message", err.message}
	};

	return this->serializeBase(jsonResponseData, TYPE_ERROR);
}

/*
The method returns a vector of bytes which represents the login message.
Input: the login response object
Output: the byte vector, represents the login json message
*/
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(LoginResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};

	return this->serializeBase(jsonResponseData, TYPE_LOGIN);
}

/*
The method returns a vector of bytes which represents the signup message.
Input: the signup response object
Output: the byte vector, represents the signup json message
*/
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(SignupResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};
	
	return this->serializeBase(jsonResponseData, TYPE_SIGNUP);
}

/*
The method returns a vector of bytes which represents the logout message.
Input: the logout response object
Output: the byte vector, represents the logout json message
*/
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(LogoutResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};

	return this->serializeBase(jsonResponseData, TYPE_LOGOUT);
}

//the method returns a vector of bytes which represents the Get Rooms Response to return to the user.
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse& response) const
{
	std::string listOfRooms = "";
	for (RoomData data : response.rooms)
		listOfRooms += data.name + " ";

	listOfRooms = listOfRooms.substr(0, listOfRooms.length() - 1);

	// Create the json (data) object
	json jsonResponseData = {
		{"Rooms", listOfRooms}
	};

	return this->serializeBase(jsonResponseData, TYPE_GET_ROOM);
}

//the method returns a vector of bytes which represents the Get Players In Rooms Response to return to the user.
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomsResponse& response) const
{
	std::string listOfUsers = "";
	for (std::string user : response.players)
		listOfUsers += user + " ";

	listOfUsers = listOfUsers.substr(0, listOfUsers.length() - 1);

	// Create the json (data) object
	json jsonResponseData = {
		{"PlayersInRoom", listOfUsers}
	};

	return this->serializeBase(jsonResponseData, TYPE_GET_PLAYERS_IN_ROOM);
}

//the method returns a vector of bytes which represents the Join Room Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};

	return this->serializeBase(jsonResponseData, TYPE_JOIN_ROOM);
}

//the method returns a vector of bytes which represents the Create Room Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};

	return this->serializeBase(jsonResponseData, TYPE_CREATE_ROOM);
}

//the method returns a vector of bytes which represents the High Score Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(HighScoreResponse& response) const
{
	std::string listOfUsers = "";
	for (std::string user : response.names)
		listOfUsers += user + " ";

	// Create the json (data) object
	json jsonResponseData = {
		{"users", listOfUsers}
	};

	return this->serializeBase(jsonResponseData, TYPE_HIGHSCORE);
}

//the method returns a vector of bytes which represents the Close Room Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};

	return this->serializeBase(jsonResponseData, TYPE_CLOSE_ROOM);
}

//the method returns a vector of bytes which represents the Start Game Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(StartGameResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};

	return this->serializeBase(jsonResponseData, TYPE_START_GAME);
}

//the method returns a vector of bytes which represents the Leave Room Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};

	return this->serializeBase(jsonResponseData, TYPE_LEAVE_ROOM);
}

//the method returns a vector of bytes which represents the Get Room State Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse& response) const
{
	std::string listOfUsers = "";
	for (std::string user : response.players)
		listOfUsers += user + " ";

	listOfUsers = listOfUsers.substr(0, listOfUsers.length() - 1);

	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status},
		{"hasGameBegun", response.hasGameBegun},
		{"players", listOfUsers},
		{"questionCount", response.questionCount},
		{"AnswerTimeout", response.answerTimeout}
	};

	return this->serializeBase(jsonResponseData, TYPE_GET_ROOM_STATE);
}

//the method returns a vector of bytes which represents the Quit Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(QuitResponse& response) const
{
	// Create the json (data) object
	json jsonResponseData = {
		{"status", response.status}
	};

	return this->serializeBase(jsonResponseData, TYPE_QUIT);
}

//the method returns a vector of bytes which represents the Leave Game Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse& response) const
{
	json data = {
		{"status", response.status}
	};
	return this->serializeBase(data,TYPE_LEAVE_GAME );
}

//the method returns a vector of bytes which represents the Get Question Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse& response) const
{
	std::map<int, std::string> ans;
	for(int i = 0; i <4;i++)
		ans.insert(std::pair<int, std::string>(i+1, response.answers[i]));
	json data = {
		{"status", response.status},
		{"question", response.question},
		{"answers", ans},
	};
	return this->serializeBase(data, TYPE_GET_QUESTION);

}

//the method returns a vector of bytes which represents the Sumbit Answer Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(SumbitAnswerResponse& response) const
{
	json data = {
		{"status", response.status}
	};
	return this->serializeBase(data,TYPE_SUMBIT_ANSWER );
}

//the method returns a vector of bytes which represents the Get Game Results Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse& response) const
{
	json data = {
		{"status", response.status},
		{"Results", response.status},
	};
	return this->serializeBase(data, TYPE_GET_GAME_RESULTS);
}

//the method returns a vector of bytes which represents the Get Statistics Response to return to the user
std::vector<MY_BYTE> JsonResponsePacketSerializer::serializeUserStatistics(GetStatisticsResponse& response) const
{
	// [0 = numberOfCorrectAnswers, 1 = NumberOfPlayerGames, 2 = NumberOfTotalAnswers, 3 = PlayerAverageAnswerTime]
	json jsonUserStatistics = {
		{"numberOfCorrectAnswers", response.statistics[0]},
		{"NumberOfPlayerGames", response.statistics[1]},
		{"NumberOfTotalAnswers", response.statistics[2]},
		{"PlayerAverageAnswerTime", response.statistics[3]}
	};
	return this->serializeBase(jsonUserStatistics, TYPE_STATISTICS);
}