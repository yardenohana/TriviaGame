#pragma once
#include "IRequestHandler.h"
#include "Request.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	Room* m_room;
	LoggedUser m_user;
	RoomManager* m_roomManager;
	RequestHandlerFactory* m_requestHandlerFactory;

public:

	RoomAdminRequestHandler(Room* room, LoggedUser& user, RequestHandlerFactory* factory);
	~RoomAdminRequestHandler();

	RoomManager* getRoomManager();
	Room getRoom();
	LoggedUser getUser();

	virtual bool isRequestRelevant(RequestInfo infoReq);
	virtual RequestResult handleRequest(RequestInfo infoReq);
	RequestResult closeRoom(RequestInfo infoReq);
	RequestResult startGame(RequestInfo infoReq);
	RequestResult getRoomState(RequestInfo infoReq);
};

