#pragma once
#include "IRequestHandler.h"
#include "Request.h"
#include "MenuRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestHandlerFactory.h"

//forward declaration to prevent the circular dependency
class RequestHandlerFactory;
struct RequestResult;
struct RequsetInfo;

//the first handler the client uses : the login request.
//it's next handler will be the Menu Request Handler.

class LoginRequestHandler : public IRequestHandler
{
private:
	LoginManager* m_loginManager;
	RequestHandlerFactory* m_handlerFactory;
	
	RequestResult createNewRequestResult(RequestInfo infoReq, bool didActionWorked, std::string userName);

public:
	
	LoginRequestHandler(RequestHandlerFactory* factory);
	~LoginRequestHandler();
	virtual bool isRequestRelevant(RequestInfo infoReq);
	virtual RequestResult handleRequest(RequestInfo infoReq);

	LoginManager* getLoginManager() const;
	RequestHandlerFactory* getRequestHandlerFactory() const;
	RequestResult login(RequestInfo infoReq);
	RequestResult signup(RequestInfo infoReq);
};