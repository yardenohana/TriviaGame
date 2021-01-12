#include "JsonRequestPacketDeserializer.h"

/*
function that check what is the current time, and returns it in the string form of "DD/MM/YYYY HH:MM:SS"
Will look like:  "14/05/2020 21:16:51"
*/
std::string JsonRequestPacketDeserializer::getTime() const
{
	time_t t = time(0); //time now
	struct tm timeinfo;
	char timeNow[SIZE_OF_TIME];

	time(&t);
	localtime_s(&timeinfo, &t);
	strftime(timeNow, SIZE_OF_TIME, "%d/%m/%Y %X", &timeinfo);
	return timeNow;
}


/*
the function recives a binary stream of a string that is in the form of a json object.
the function will translate the binary stream into a regular string and insert in into a json object
*/
json JsonRequestPacketDeserializer::getData(const char* buffer) const
{
	std::string translatedStr = "";

	for (int i = 0; buffer[i] != NULL; i++)
		translatedStr += buffer[i];

	// turning the string to JSON object, like the opposite of the methos 'dump' in the JSON lib.
	json jsObj = json::parse(translatedStr);
	
	return jsObj;
}


/*
function that recives the json in binary and extracting the data into 'LoginRequest' object
Input: the data of message
Output: the loginRequest object
*/
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const char* buffer) const
{
	LoginRequest loginReq;
	json Json = getData(buffer);
	std::string pass = Json["password"];
	loginReq.password = pass;

	std::string userName = Json["username"];
	loginReq.username = userName;

	return loginReq;
}


// function that recieves the json in binary and extracting the data into 'SignupRequest' object
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const char* buffer) const
{
	SignupRequest signupReq;
	json Json = getData(buffer);
	std::string pass = Json["password"];
	signupReq.password = pass;

	std::string userName = Json["username"];
	signupReq.username = userName;

	std::string email = Json["email"];
	signupReq.email = email;

	return signupReq;
}


// function that recives the whole packet in binary and extracting the data into 'RequestInfo' object
RequestInfo JsonRequestPacketDeserializer::deserializeInfo(const char* buffer) const
{
	RequestInfo infoReq;
	std::string strBuffer = this->charStrToString(buffer);
	std::string lengthOfMsg = "";

	infoReq.ReceivalTime = this->getTime();

	// getting code of msg : the first byte.
	infoReq.id = (int)strBuffer[0];

	// getting the length of the msg : second to fifth bytes.
	for (int i = START_OF_SIZE; i < START_OF_MSG; i++)
	{
		if ((int)strBuffer[i] != ' ') // remove blank spaces (represents null)
			lengthOfMsg += JsonResponsePacketSerializer::intToByteStr((int)strBuffer[i]);
	}
	std::bitset<4 * BYTE_SIZE> lengthOfData(lengthOfMsg); // used to convert bytes into long integer
	long dataLen = lengthOfData.to_ullong();

	// getting the msg : sixth until end of msg.
	// since every char represents a byte, the 'char* msg' is an array of bytes.
	// std::vector is an elaborate array, moving the bytes from the 'char*' to 'vector<MY_BYTE>' doesn't change a thing.
	std::vector<MY_BYTE> msgInByte;
	
	// get the buffer
	for (int i = START_OF_MSG; i < dataLen + START_OF_MSG; i++) 
	{
		msgInByte.push_back((int)strBuffer[i]);
	}

	infoReq.buffer = msgInByte;
	return infoReq;
}

/*
function that recieves msg the user sent, and extracting the data into 'GetPlayersInRoomRequest' object
Input: the data of message, json in binary form
Output: the GetPlayersInRoomRequest object
*/
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const char* buffer) const
{
	GetPlayersInRoomRequest playersReq;
	json Json = getData(buffer);

	std::string name = Json["roomName"];
	playersReq.roomName = name;

	return playersReq;
}

/*
function that recives the msg the user sent, and extracting the data into 'JoinRoomRequest' object
Input: the data of message, json in binary form
Output: the JoinRoomRequest object
*/
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const char* buffer) const
{
	JoinRoomRequest joinRoomReq;
	json Json = getData(buffer);
	std::string name = Json["roomName"];
	joinRoomReq.roomName = name;

	return joinRoomReq;
}

/*
function that recives the msg the user sent, and extracting the data into 'CreateRoomRequest' object
Input: the data of message, json in binary form
Output: the JoinRoomRequest object
*/
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const char* buffer) const
{
	CreateRoomRequest createReq;
	json Json = getData(buffer);

	int count = Json["questionCount"];
	createReq.questionCount = count;

	int max = Json["maxUsers"];
	createReq.maxUsers = max;

	int time = Json["answerTimeout"];
	createReq.answerTimeout = time;

	std::string name = Json["roomName"];
	createReq.roomName = Json["roomName"].get<std::string>();
	
	return createReq;
}


// The function turns the vector of bytes into a string 
std::string JsonRequestPacketDeserializer::turnBytesToString(std::vector<MY_BYTE> vec) const
{
	std::string ans = "";
	for (unsigned char ch : vec)
		ans += ch;

	return ans;
}

std::string JsonRequestPacketDeserializer::charStrToString(const char* cstr)
{
	std::string st = "";
	for (int i = 0; NULL != cstr[i]; i++)
		st += cstr[i];
	return st;
}

