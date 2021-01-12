#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(Room* room, LoggedUser& user, RequestHandlerFactory* handlerFactory)
{
	this->m_room = room;
	this->m_user = user;
	this->m_requestHandlerFactory = handlerFactory;
	this->m_roomManager = handlerFactory->getRoomManager();
}

RoomMemberRequestHandler::~RoomMemberRequestHandler()
{
}

RoomManager* RoomMemberRequestHandler::getRoomManager()
{
	return this->m_roomManager;
}

/* 
check if the message code is relevant and has a specific handle function.
Input: the request information object.
Output: true if the request id is valid, else false.
*/
bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo infoReq)
{
	if (infoReq.id == LEAVE_ROOM_REQUEST || infoReq.id == START_GAME_REQUEST || infoReq.id == GET_ROOM_STATE_REQUEST)
		return true;
	return false;
}

/*
function directs the request from the user to the correct function based on it's code id
input: requestInfo - includes the user's request code, recivel time and the actual request
output: RequestResult - includes the response to the user, the next handler the user will get and a variable that indicates whether the operation worked or not.
*/
RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo infoReq)
{
	if (infoReq.id == LEAVE_ROOM_REQUEST) 
		return leaveRoom(infoReq);

	else if (infoReq.id == START_GAME_REQUEST) 
		return startgame(infoReq);

	else if (infoReq.id == GET_ROOM_STATE_REQUEST) 
		return getRoomState(infoReq);

	throw std::exception("Error (Room Member): request ID doesn't fit any type of request available.");
}


/*
the function deletes the user from the room,
and changes the user's next handler to be 'MenuRequestHandler' because the user returns to the main menu state.
the response the user will receive is returned as a json, inside it there is the field of status(1 if action worked)
*/
RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo infoReq)
{
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	RequestResult reqReasult;
	LeaveRoomResponse response;

	if (infoReq.id != LEAVE_ROOM_REQUEST)
	{
		reqReasult.didActionWorked = false;
		return reqReasult;
	}

	//removing the user from the room:
	this->m_room->removeUser(this->m_user);

	//assemble  the request result:
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
RequestResult RoomMemberRequestHandler::startgame(RequestInfo infoReq)
{
	return this->m_requestHandlerFactory->startGameRequest(infoReq, this->m_room, this->m_user);
}

/*
function returns to the user the information about the room that the user is at.
the info returns as a json object, and the handler of the user remains the same - RoomAdminRequestHandler.
the function is written in the RequestHandlerFactory, to prevent code duplication.
*/
RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo infoReq)
{
	RequestResult reqReasult =  this->m_requestHandlerFactory->getRoomStateRequest(infoReq, this->m_room, this->m_user);
	//the function calls with all the parameters to the function that returns the correct answer.
	//stays in the same state
	RoomMemberRequestHandler* roomHandler = new RoomMemberRequestHandler(this->m_room, this->m_user, this->m_requestHandlerFactory);
	reqReasult.newHandler = roomHandler;
	return reqReasult;
}
