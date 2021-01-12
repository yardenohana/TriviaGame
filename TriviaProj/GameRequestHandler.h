#pragma once
#include <iostream>
#include "LoggedUsers.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
private:
	Game m_game;
	LoggedUser m_user;
	GameManager* m_gameManager;
	RequestHandlerFactory* m_requestHandlerFactory;

public:
	GameRequestHandler(RequestHandlerFactory* factory, Game game, LoggedUser user, GameManager* manager);
	~GameRequestHandler();

	//TODO:
	virtual bool isRequestRelevant(RequestInfo infoReq);
	virtual RequestResult handleRequest(RequestInfo infoReq);

	RequestResult getQuestion(RequestInfo infoReq);
	RequestResult sumbitAnswer(RequestInfo infoReq);
	RequestResult getGameResults(RequestInfo infoReq);
	RequestResult leaveGame(RequestInfo infoReq);



};
