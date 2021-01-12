#pragma once
#include "IRequestHandler.h"
#include "Request.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RequestHandlerFactory.h"

#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class RequestHandlerFactory;

/*
Handler for the menu of the game
*/
class MenuRequestHandler : public IRequestHandler
{
private:
	LoggedUser m_user;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
	RequestHandlerFactory* m_requestHandlerFactory;


public:
	MenuRequestHandler(RequestHandlerFactory& handler, LoggedUser& user, 
		RoomManager* roomManager, StatisticsManager* statistics);
	~MenuRequestHandler();

	std::string getUsername();
	
	virtual bool isRequestRelevant(RequestInfo infoReq);
	virtual RequestResult handleRequest(RequestInfo infoReq);

	void setRoomManager(RoomManager* roomManager);

	// Room related methods:
	RequestResult createRoom(RequestInfo reqInfo);
	RequestResult joinRoom(RequestInfo reqInfo);
	RequestResult getPlayersInRoom(RequestInfo reqInfo);
	RequestResult signout(RequestInfo reqInfo);
	RequestResult getRoom(RequestInfo reqInfo);
	RequestResult getStatistics(RequestInfo reqInfo);
	RequestResult getHighScore(RequestInfo reqInfo);

};