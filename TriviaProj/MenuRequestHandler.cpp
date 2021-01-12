#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handler, LoggedUser& user, RoomManager* roomManager, StatisticsManager* statistics)
	: m_requestHandlerFactory(&handler), m_user(user), m_roomManager(roomManager), m_statisticsManager(statistics)
{
}

MenuRequestHandler::~MenuRequestHandler()
{
	//can't delete the managers
}

std::string MenuRequestHandler::getUsername()
{
	return this->m_user.getUsername();
}

/* 
checks if the message code is relevant and has a specific handle function.
Input: the request information object.
Output: true if the request id is valid, else false.
*/
bool MenuRequestHandler::isRequestRelevant(RequestInfo infoReq)
{
	if (infoReq.id == GET_PLAYERS_REQUEST || infoReq.id == JOIN_ROOM_REQUEST || infoReq.id == CREATE_ROOM_REQUEST\
		|| infoReq.id == HIGHSCORE_REQUEST || infoReq.id == GET_ROOM_REQUEST|| infoReq.id == LOGOUT_REQUEST || infoReq.id == GET_STATISTICS)
		return true;
	return false;
}

/*
Route the request into the specific handler
Input: the request information.
Output: the request result object.
*/
RequestResult MenuRequestHandler::handleRequest(RequestInfo infoReq)
{
	if (infoReq.id == GET_PLAYERS_REQUEST) {
		return getPlayersInRoom(infoReq);
	}
	else if (infoReq.id == JOIN_ROOM_REQUEST) {
		return joinRoom(infoReq);
	}
	else if (infoReq.id == CREATE_ROOM_REQUEST) {
		return createRoom(infoReq);
	}
	else if (infoReq.id == HIGHSCORE_REQUEST) {
		return getHighScore(infoReq);
	}
	else if (infoReq.id == GET_ROOM_REQUEST) {
		return getRoom(infoReq);
	}
	else if (infoReq.id == LOGOUT_REQUEST) {
		return signout(infoReq);
	}
	else if (infoReq.id == GET_STATISTICS) {
		return getStatistics(infoReq);
	}

	throw std::exception("Error (menu): request ID doesn't fit any type of request available.");
}

void MenuRequestHandler::setRoomManager(RoomManager* roomManager)
{
	this->m_roomManager = roomManager;
}

/*
the function deletes the user from all the rooms in the system, and "kills" it's thread.
after deleting it from everything, the user will receive a request reasult that contains:
buffer - the response to the user, LogoutResponse object after serialization,
newHandler - nullptr, since we destroy the thread.
*/
RequestResult MenuRequestHandler::signout(RequestInfo reqInfo)
{
	// delete user from all rooms and the whole system.
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	RequestResult reqReasult;
	LogoutResponse logOut;

	if (reqInfo.id != LOGOUT_REQUEST)
		throw std::exception("Message ID is not 'LOGOUT_REQUEST' (code: 106)\n");

	this->m_roomManager->deleteUserFromRooms(this->m_user);
	logOut.status = 1;
	reqReasult.buffer = ser.serializeResponse(logOut);
	LoginRequestHandler* newHandler = new LoginRequestHandler(this->m_requestHandlerFactory);
	reqReasult.newHandler = newHandler; // returning the user back to login/signup state
	reqReasult.didActionWorked = true;

	return reqReasult;
	
}

/*
the function return a request reasult that contains the same state the user is at right now (new Handler),
and the answer to return to user (list of the names of all the room), after serialization.
*/
RequestResult MenuRequestHandler::getRoom(RequestInfo reqInfo)
{
	// return all rooms' names
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	RequestResult reqReasult;
	GetRoomsResponse response;

	if (reqInfo.id != GET_ROOM_REQUEST)
		throw std::exception("Message ID is not 'GET_ROOM_REQUEST' (code: 107)\n");

	//get all rooms. 
	response.rooms = this->m_roomManager->getRooms();
	response.status = 1;

	// turning the answer to vector<MY_BYTE> from response object
	reqReasult.buffer = ser.serializeResponse(response);

	//the user will remain in it's current state.
	MenuRequestHandler* menuHandler = new MenuRequestHandler(*this->m_requestHandlerFactory, this->m_user, this->m_roomManager, this->m_statisticsManager);
	reqReasult.newHandler = menuHandler;
	reqReasult.didActionWorked = true;

	return reqReasult;
}

/*
the function return a request reasult that contains the same state the user is at right now (new Handler),
and the answer to return to user (list of the names users in the room), after serialization.
*/
RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo reqInfo)
{
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	std::string data = "";
	RequestResult reqReasult;
	GetPlayersInRoomsResponse response;

	if (reqInfo.id != GET_PLAYERS_REQUEST)
		throw std::exception("Message ID is not 'GET_PLAYERS_REQUEST' (code: 102)\n");

	// Convert buffer into std::string
	for (auto itr = reqInfo.buffer.begin(); itr != reqInfo.buffer.end(); itr++)
		data += *itr;

	// deserialize the information in order to create a request object.
	GetPlayersInRoomRequest getPlayersReq = deSer.deserializeGetPlayersRequest(data.c_str());

	response.players = this->m_roomManager->getRoom(getPlayersReq.roomName)->getAllUsers();
	// turning the answer to vector<MY_BYTE> from string.
	reqReasult.buffer = ser.serializeResponse(response);

	//the user will remain in it's current state.
	MenuRequestHandler * menuHandler = new MenuRequestHandler(*this->m_requestHandlerFactory, this->m_user, this->m_roomManager, this->m_statisticsManager);
	reqReasult.newHandler = menuHandler;
	reqReasult.didActionWorked = true;

	return reqReasult;
}

/*
the function return a request reasult that contains the same state the user is at right now (new Handler),
and the answer to return to user (the statistics of the user), after serialization.
*/
RequestResult MenuRequestHandler::getStatistics(RequestInfo reqInfo)
{
	// return all rooms' names
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	RequestResult reqReasult;
	GetStatisticsResponse response;

	if (reqInfo.id != GET_STATISTICS)
		throw std::exception("Message ID is not 'GET_STATISTICS' (code: 108)\n");

	//get all user's statistics
	//the vector of the statictics will held the result in this way:
	// [0 = numberOfCorrectAnswers, 1 = NumberOfPlayerGames, 2 = NumberOfTotalAnswers, 3 = PlayerAverageAnswerTime]
	response.statistics = this->m_statisticsManager->getUserStatistics(this->m_user.getUsername());
	response.status = 1;

	// turning the answer to vector<MY_BYTE> from string.
	reqReasult.buffer = ser.serializeUserStatistics(response);

	//the user will remain in it's current state.
	MenuRequestHandler* menuHandler = new MenuRequestHandler(*this->m_requestHandlerFactory, this->m_user, this->m_roomManager, this->m_statisticsManager);
	reqReasult.newHandler = menuHandler;
	reqReasult.didActionWorked = true;

	return reqReasult;
}

/*
the function return a request reasult that contains the same state the user is at right now (new Handler),
and the answer to return to user (list of the names of users with the highest scores), after serialization.
*/
RequestResult MenuRequestHandler::getHighScore(RequestInfo reqInfo)
{
	// return top 3 users.
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	RequestResult reqReasult;
	HighScoreResponse response;

	if (reqInfo.id != HIGHSCORE_REQUEST)
		throw std::exception("Message ID is not 'HIGHSCORE_REQUEST' (code: 105)\n");

	//getting the users with the highest scores
	response.names = this->m_statisticsManager->getHighScore(this->m_requestHandlerFactory->getLoginManager()->getAllUsers());
	
	// turning the answer to vector<MY_BYTE> from string.
	reqReasult.buffer = ser.serializeResponse(response);

	//the user will remain in it's current state.
	MenuRequestHandler* menuHandler = new MenuRequestHandler(*this->m_requestHandlerFactory, this->m_user, this->m_roomManager, this->m_statisticsManager);
	reqReasult.newHandler = menuHandler;
	reqReasult.didActionWorked = true;

	return reqReasult;
}

/*
the function deals with a request to join a room.
the function does 2 things: executing the request - joining the room, and creating a RequestReasult that will be returned to the user.
in the request result there's the new state of the user - 'RoomMemberHandler', if the insert was successful.
if not, the state will remain the same - 'MenuRequestHandler'.
also, and there's the buffer that was sent in the begining.
input: a RequestInfo with the code of the request, the buffer that contain the actual request and the time of the receiving.
output: a RequestResult with the next state and the buffer.
*/
RequestResult MenuRequestHandler::joinRoom(RequestInfo reqInfo)
{
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	std::string data = "";
	RequestResult reqReasult;
	if (reqInfo.id != JOIN_ROOM_REQUEST)
		throw std::exception("Message ID is not 'JOIN_ROOM_REQUEST' (code: 103)\n");

	// Convert buffer into std::string
	for (auto itr = reqInfo.buffer.begin(); itr != reqInfo.buffer.end(); itr++)
		data += *itr;
	// extracting the data from the buffer recived from the user's request
	JoinRoomRequest reqJoin = deSer.deserializeJoinRoomRequest(data.c_str());
    
	Room* room = this->m_roomManager->getRoom(reqJoin.roomName);
	
	// if there's place in the room to join...
	if (room->getAllUsers().size() < room->getMetaData().maxPlayers)
	{
		Room* ro = this->m_roomManager->addUserToRoomB(reqJoin.roomName, this->m_user);
	
		std::cout << "[+] JOIN ROOM: adds user: " << this->m_user.getUsername() << ", to room: " << reqJoin.roomName << std::endl;

		Room* room = this->m_roomManager->getRoom(reqJoin.roomName);
		RoomMemberRequestHandler* roomHandler = new RoomMemberRequestHandler(room, this->m_user, this->m_requestHandlerFactory);
		reqReasult.newHandler = roomHandler;

		JoinRoomResponse joinResponse;
		joinResponse.status = SUCCESS;
		reqReasult.buffer = ser.serializeResponse(joinResponse);
		reqReasult.didActionWorked = true;
	}
	else
	{
		// if the user failed in his request, the user will remain in it's current state.
		MenuRequestHandler* menuHandler = new MenuRequestHandler(*this->m_requestHandlerFactory, this->m_user, this->m_roomManager, this->m_statisticsManager);
		reqReasult.newHandler = menuHandler;

		//sending an error response to the client, because the request canceled.
		ErrorResponse err; 
		err.message = "joining room didn't work.";
		std::vector<MY_BYTE> msgBuffer = ser.serializeResponse(err);
		reqReasult.buffer = msgBuffer;
		reqReasult.didActionWorked = false;
	}

	return reqReasult;

}

/*
the function deals with a request to create a room.
the function does 2 things: executing the request - creating the room, and creating a RequestReasult that will be returned to the user.
in the request result there's the new state of the user - 'RoomAdminHandler' and there's the buffer that was sent in the begining.
input: a RequestInfo with the code of the request, the buffer that contain the actual request and the time of the receiving.
output: a RequestResult with the next state and the buffer.
*/
RequestResult MenuRequestHandler::createRoom(RequestInfo reqInfo)
{
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	std::string data = "";
	RequestResult reqReasult;
	CreateRoomResponse response;

	if (reqInfo.id != CREATE_ROOM_REQUEST)
		throw std::exception("Message ID is not 'CREATE_ROOM_REQUEST' (code: 104)\n");

	// Convert buffer into std::string
	for (auto itr = reqInfo.buffer.begin(); itr != reqInfo.buffer.end(); itr++)
		data += *itr;

	// deserialize the information in order to create a request object.
	CreateRoomRequest createRoomReq = deSer.deserializeCreateRoomRequest(data.c_str());
	
	this->m_user.setAdmin(true); // if a user creates a room, he is an admin
	bool res = this->m_roomManager->createRoom(this->m_user, createRoomReq.roomName, 
		createRoomReq.maxUsers, createRoomReq.questionCount, createRoomReq.answerTimeout);

	if (res)
	{
		Room* room = nullptr;

		room = this->m_roomManager->getRoom((this->m_roomManager->latestID)-1); // in the creation of the room, the latest id had been increased. decreasing it by 1 will give us the room id of the new room/
		//changing the user to admin
		this->m_user.setAdmin(true);
		std::cout << "[+] created room number " << room->getMetaData().id << ", name: " << room->getMetaData().name << std::endl;
	
		// creating the request result, which is the second purpose of the function 
		RoomAdminRequestHandler* roomHandler = new RoomAdminRequestHandler(room, this->m_user, this->m_requestHandlerFactory);
		reqReasult.newHandler = roomHandler;

		response.status = 1;
		reqReasult.buffer = ser.serializeResponse(response);
		reqReasult.didActionWorked = true;

	}
	else
	{
		// if the user failed in his request, the user will remain in it's current state.
		MenuRequestHandler* menuHandler = new MenuRequestHandler(*this->m_requestHandlerFactory, this->m_user, this->m_roomManager, this->m_statisticsManager);
		reqReasult.newHandler = menuHandler;

		//sending an error response to the client, because the request canceled.
		ErrorResponse err;
		err.message = "Creating room didn't work. the name of the room is already taken.";
		std::vector<MY_BYTE> msgBuffer = ser.serializeResponse(err);
		reqReasult.buffer = msgBuffer;
		reqReasult.didActionWorked = false;
	}
	return reqReasult;
}
