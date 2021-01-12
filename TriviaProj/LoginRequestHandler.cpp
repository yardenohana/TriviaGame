#include "LoginRequestHandler.h"

/*
The function creates 'RequestResult' struct, inserts the data to RequestResult's buffer,
and enters a new handler according to the parameter 'didActionWorked'.
*/
RequestResult LoginRequestHandler::createNewRequestResult(RequestInfo infoReq, bool didActionWorked, std::string userName)
{
	RequestResult res;
	JsonResponsePacketSerializer ser;
	if (didActionWorked)
	{
		// if the insert worked good, the next state will be the menu handler.
		MenuRequestHandler* menuHandler = m_handlerFactory->createMenuRequestHandler(userName);
		res.newHandler = menuHandler;
		if (infoReq.id == LOGIN_REQUEST)
		{
			LoginResponse response;
			response.status = SUCCESS;
			res.buffer = ser.serializeResponse(response);
			std::cout << "[+] user " << userName << " logged in successfully" << std::endl;
		}
		if (infoReq.id == SIGNUP_REQUEST)
		{
			SignupResponse response;
			response.status = SUCCESS;
			res.buffer = ser.serializeResponse(response);
			std::cout << "[+] user " << userName << " signed up successfully" << std::endl;
		}
		res.didActionWorked = true;
		
	}
	else // action didn't work
	{
		// if not, the user will remain in the same state until it will work
		LoginRequestHandler* loginHandler = new LoginRequestHandler(this->m_handlerFactory);
		res.newHandler = loginHandler;
		if (infoReq.id == LOGIN_REQUEST)
		{
			ErrorResponse response;
			response.message = "Error: Login failed.";
			res.buffer = ser.serializeResponse(response);
			std::cout << "[-] user " << userName << " didn't logged in successfully" << std::endl;
		}
		if (infoReq.id == SIGNUP_REQUEST)
		{
			ErrorResponse response;
			response.message = "Error: Sign up failed.";
			res.buffer = ser.serializeResponse(response);
			std::cout << "[-] user " << userName << " didn't signed up successfully" << std::endl;
		}
		res.didActionWorked = false;
	}
	return res;
}

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* factory)
{
	this->m_handlerFactory = factory;
	this->m_loginManager = m_handlerFactory->getLoginManager();
}


LoginRequestHandler::~LoginRequestHandler()
{
}

/*
The function checks the validness of the message, by the code (first byte of the meeage).
Input: the request information, contains the buffer
Output: true if the code of message is valid, else false
*/
bool LoginRequestHandler::isRequestRelevant(RequestInfo infoReq)
{
	if (infoReq.id == SIGNUP_REQUEST || infoReq.id == LOGIN_REQUEST)
		return true;
	return false;
}

/*
The function get a 'RequestInfo' object and insert the data 
inside it to the users vector, and creates the next
*/
RequestResult LoginRequestHandler::handleRequest(RequestInfo infoReq)
{
	if (infoReq.id == LOGIN_REQUEST)
		return login(infoReq);
	if (infoReq.id == SIGNUP_REQUEST)
		return signup(infoReq);

	throw std::exception("Error (login/signup): request ID doesn't fit any type of request available.");
}


/*
The function does 2 main things:
1. Deserialize the buffer from the client message, and insert it into the vector of the logged in users.
2. Creates a 'RequestResult' with the buffer and the next handler the user will use.
*/
LoginManager* LoginRequestHandler::getLoginManager() const
{
	return this->m_loginManager;
}

RequestHandlerFactory* LoginRequestHandler::getRequestHandlerFactory() const
{
	return this->m_handlerFactory;
}

/*
The function connects the database to check if the users exists, 
and returns the request result object.
Input: the request information
Output: the request result
*/
RequestResult LoginRequestHandler::login(RequestInfo infoReq)
{
	JsonRequestPacketDeserializer deSer;
	RequestResult res;
	std::string data = "";

	for (auto itr = infoReq.buffer.begin(); itr != infoReq.buffer.end(); itr++)
		data += *itr;

	LoginRequest loginInfo = deSer.deserializeLoginRequest(data.c_str());
	
	bool insertRes = m_loginManager->login(loginInfo.username, loginInfo.password); // insert into database, and user's vector
	res = createNewRequestResult(infoReq, insertRes, loginInfo.username);
	return res;

}

/*
The function does 2 main things:
1. The function deserialize the buffer from the client message, and insert it into the vector of the signed up users.
2. The ftunction creates a 'RequestResult' with the buffer and the next handler the user will use.
*/
RequestResult LoginRequestHandler::signup(RequestInfo infoReq)
{
	JsonRequestPacketDeserializer deSer;
	std::string data = "";
	RequestResult res;
	
	// Convert buffer into std::string
	for (auto itr = infoReq.buffer.begin(); itr != infoReq.buffer.end(); itr++)
		data += *itr;
	
	SignupRequest signupInfo = deSer.deserializeSignupRequest(data.c_str());

	// Add user to database
	bool insertRes = this->m_loginManager->signup(signupInfo.username, signupInfo.password, signupInfo.email);
	// Create the Request Result object
	res = createNewRequestResult(infoReq, insertRes, signupInfo.username);
	
	return res;
}
