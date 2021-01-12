#pragma once
#include "Request.h"

// An abstract class used as a basic pattern to
// handle a request from a client

//forward declaration to prevent the circular dependency
struct RequestInfo;
struct RequestResult;

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo infoReq) = 0;
	virtual RequestResult handleRequest(RequestInfo infoReq) = 0;
};