#include "GameRequestHandler.h"

GameRequestHandler::GameRequestHandler(RequestHandlerFactory* factory, Game game, LoggedUser user, GameManager* manager)
:m_requestHandlerFactory(factory), m_user(user), m_game(game), m_gameManager(manager)
{
	this->m_requestHandlerFactory = factory;
	this->m_game = game;
	this->m_user = user;
	this->m_gameManager = manager;

}

GameRequestHandler::~GameRequestHandler()
{
}

bool GameRequestHandler::isRequestRelevant(RequestInfo infoReq)
{
	if (infoReq.id == LEAVE_GAME_REQUEST || infoReq.id == GET_QUESTION_REQUEST ||
		infoReq.id == SUMBIT_ANSWER_REQUEST || infoReq.id == GET_GAME_RESULT_REQUEST)
		return true;
	return false;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo infoReq)
{
	if (infoReq.id == LEAVE_GAME_REQUEST)
		return leaveGame(infoReq);

	if (infoReq.id == GET_QUESTION_REQUEST)
		return getQuestion(infoReq);

	if (infoReq.id == SUMBIT_ANSWER_REQUEST)
		return sumbitAnswer(infoReq);

	if (infoReq.id == GET_GAME_RESULT_REQUEST)
		return getGameResults(infoReq);

	throw std::exception("Error (Game): request ID doesn't fit any type of request available.");
}

RequestResult GameRequestHandler::getQuestion(RequestInfo infoReq)
{
	return RequestResult();
}

RequestResult GameRequestHandler::sumbitAnswer(RequestInfo infoReq)
{
	return RequestResult();
}

RequestResult GameRequestHandler::getGameResults(RequestInfo infoReq)
{
	return RequestResult();
}

RequestResult GameRequestHandler::leaveGame(RequestInfo infoReq)
{
	return RequestResult();
}
