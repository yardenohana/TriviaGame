#pragma once
#include <iostream>
#include "IDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "SqliteDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"
#include "GameManager.h"

//forward declaration to prevent the circular dependency
class MenuRequestHandler;
class LoginRequestHandler;
class GameRequestHandler;

class RequestHandlerFactory
{
private:
	LoginManager* m_loginManager;
	IDatabase* m_database;
	RoomManager* m_roomManager;
	StatisticsManager* m_StatisticsManager;
	GameManager* m_gameManager;

public:
	RequestHandlerFactory();
	RequestHandlerFactory(RoomManager* roomManager);
	~RequestHandlerFactory();

	//the description of the functions is in the cpp file.
	LoginRequestHandler* createLoginRequestHandler(RequestHandlerFactory* handler);
	MenuRequestHandler* createMenuRequestHandler(std::string username);
	GameRequestHandler* createGameRequestHandler(Room room, RequestHandlerFactory* handler, LoggedUser user, Game game);

	RequestResult getRoomStateRequest(RequestInfo infoReq, Room* room, LoggedUser user);
	RequestResult startGameRequest(RequestInfo infoReq, Room* room, LoggedUser user);

	// getters
	GameManager* getGameManager() const;
	IDatabase* getDatabase() const;
	LoginManager* getLoginManager() const;
	StatisticsManager* getStatisticsManger() const;
	RoomManager* getRoomManager() const;

	//set:
	void setRoomManager(RoomManager* roomManager);
};
