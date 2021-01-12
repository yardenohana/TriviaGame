#pragma once
#include "IRequestHandler.h"
#include "Request.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RequestHandlerFactory.h"
#include "GameManager.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
private:
	Room* m_room;
	LoggedUser m_user;
	RoomManager* m_roomManager;
	RequestHandlerFactory* m_requestHandlerFactory;

public:
	RoomMemberRequestHandler(Room* room, LoggedUser& user, RequestHandlerFactory* handlerFactory);
	~RoomMemberRequestHandler();

	RoomManager* getRoomManager();

	virtual bool isRequestRelevant(RequestInfo infoReq);
	virtual RequestResult handleRequest(RequestInfo infoReq);

	RequestResult getRoomState(RequestInfo infoReq);
	RequestResult leaveRoom(RequestInfo infoReq);
	RequestResult startgame(RequestInfo infoReq);

	

};
