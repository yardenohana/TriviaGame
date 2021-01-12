#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(Room* room, LoggedUser& user, RequestHandlerFactory* factory)
{
	this->m_room = room;
	this->m_user = user;
	this->m_roomManager = factory->getRoomManager();;
	this->m_requestHandlerFactory = factory;
}

RoomAdminRequestHandler::~RoomAdminRequestHandler()
{
}

RoomManager* RoomAdminRequestHandler::getRoomManager()
{
	return this->m_roomManager;
}

Room RoomAdminRequestHandler::getRoom()
{
	return *this->m_room;
}

LoggedUser RoomAdminRequestHandler::getUser()
{
	return this->m_user;
}

/*
The function returns true if the code matches one of the following requests,
close room / get room state / start game, else it will return false
*/
bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo infoReq)
{
	if (infoReq.id == CLOSE_ROOM_REQUEST || infoReq.id == START_GAME_REQUEST || 
		infoReq.id == GET_ROOM_STATE_REQUEST)
		return true;

	return false;
}

/*
function directs the request from the user to the correct function based on it's code id
input: requestInfo - includes the user's request code, recivel time and the actual request
output: RequestResult - includes the response to the user, the next handler the user will get and a variable that indicates whether the operation worked or not.
*/
RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo infoReq)
{
	if (infoReq.id == CLOSE_ROOM_REQUEST) {

		return closeRoom(infoReq);
	}
	else if (infoReq.id == START_GAME_REQUEST) {

		return startGame(infoReq);
	}
	else if (infoReq.id == GET_ROOM_STATE_REQUEST) {

		return getRoomState(infoReq);
	}

	// Can we send a request reult obejct here intead??
	throw std::exception("Error (RoomAdminHandler): request ID doesn't fit any type of request available.");
}

/*
the function closes the room.
it removes all the users from the room - clears the vector of the user, 
and changes the user's next handler to be 'MenuRequestHandler', because the user returns to the main menu state.
the response the user will receive is returned as a json, inside it there is the field of status(1 if action worked)
*/
RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo infoReq)
{
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	RequestResult reqReasult;
	CloseRoomResponse response;

	if (infoReq.id != CLOSE_ROOM_REQUEST)
	{
		reqReasult.didActionWorked = false;
		return reqReasult;
	}

	//remove all users from the room, 
	this->m_room->removeAllUsers();
	this->m_roomManager->deleteRoom(this->m_room->getMetaData().id);

	response.status = SUCCESS;
	reqReasult.buffer = ser.serializeResponse(response);
	reqReasult.didActionWorked = true;
	//returning to the menu state:
	MenuRequestHandler* menuHandler = new MenuRequestHandler(*this->m_requestHandlerFactory, this->m_user, this->m_roomManager, this->m_requestHandlerFactory->getStatisticsManger());
	reqReasult.newHandler = menuHandler;

	return reqReasult;
}

/*
function crates a game, and changes the next handler of the user to be GameRequestHandler.
input: the RequestInfo with:
the buffer (it's empty because that's the syntax of the request), the id of the reusqet (110), and the reciving time.
output: buffer with:
the response to the client, the next handler(GameRequestHandler, because the game starts), and bool value for if the action worked.
*/
RequestResult RoomAdminRequestHandler::startGame(RequestInfo infoReq)
{
	return this->m_requestHandlerFactory->startGameRequest(infoReq, this->m_room, this->m_user);
}

/*
function returns to the user the information about the room that the user is at.
the info returns as a json object, and the handler of the user remains the same - RoomAdminRequestHandler.
the function is written in the RequestHandlerFactory, to prevent code duplication.
*/
RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo infoReq)
{
	RequestResult reqReasult = this->m_requestHandlerFactory->getRoomStateRequest(infoReq, this->m_room, this->m_user);
	//the function calls with all the parameters to the function that returns the correct answer.
	//stays in the same state
	RoomAdminRequestHandler* roomHandler = new RoomAdminRequestHandler(this->m_room, this->m_user, this->m_requestHandlerFactory);
	reqReasult.newHandler = roomHandler;
	return reqReasult;
}
