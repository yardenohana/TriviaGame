#include "Communicator.h"

Communicator::Communicator() : _serverSocket()
{
	// Initialize Winsock 
	int iResult;
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "[-] Error: WSAStartup failed: " << iResult << std::endl;
		return;
	}

	// The server uses TCP, thus we use AF_INET
	_serverSocket = socket(AF_INET,  SOCK_STREAM,  IPPROTO_TCP); 

	if (_serverSocket == INVALID_SOCKET)
	{
		DWORD err = GetLastError();
		std::cout << "[-] Exited with error number " << err << std::endl;
		throw std::exception(__FUNCTION__ " - socket");
	}

	// create a shared factory to all threads in the game
	this->m_factory = new RequestHandlerFactory();
}

Communicator::~Communicator()
{
	delete this->m_factory;

	try
	{
		closesocket(_serverSocket);
	}
	catch (std::exception e) {
		std::cout << "[-] Error (close socket): " << e.what() << std::endl;
	}
	
	std::map<SOCKET, IRequestHandler*>::iterator itr;
	// delete the handler's allocation
	for (itr = this->m_clients.begin(); itr != this->m_clients.end(); itr++)
		delete itr->second;
}

void Communicator::bindAndListen(int port)
{
	struct sockaddr_in sa = { 0 };
	
	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	
	// Start listening for incoming requests of clients
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "[+] Waiting for client connection request . . ." << std::endl;
		startHandleRequests();
	}
}


void Communicator::startHandleRequests()
{
	// this accepts the client and create a specific socket from server to this client
	SOCKET client_socket = ::accept(_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		DWORD err = GetLastError();
		std::cout << "[-] Exited with error: " << err << ", invalid socket" << std::endl;
	}

	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "[+] Client accepted. Server and client can speak" << std::endl;
	
	try
	{
		// add client into the clients map
		IRequestHandler* requestPtr = new LoginRequestHandler(this->m_factory);
		std::pair<SOCKET, IRequestHandler*> clientIter { client_socket, requestPtr };
		this->m_clients.insert(clientIter);

		// creating a thread for each client
		thread threadPerClient(&Communicator::handleNewClient, this, client_socket);

		// detach the thread in order to handle each client separately
		threadPerClient.detach(); 
	}
	catch (...)
	{
		std::cout << "[-] Exception was thrown from the main thread." << std::endl;
	}
}

/*
The function handles per client, communicates with a specific client.
Input: the conversation socket.
Output: none
--------------------------------------------------------------------------------------------------------------
the function deals with a client.
the communication is based on requests from user to server, and responses from server to user(client).
every client starts from signup/login state, and continues to menu state.
the rest of the states will be based on the different requests from the user at the rest of the communication.
--------------------------------------------------------------------------------------------------------------
*/
void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{
		// first stage: analyze the login or signup request, check it's validness
		std::pair<LoginRequestHandler*, MenuRequestHandler*> res = this->handleClientLoginOrSignup(clientSocket);

		// add the user from the manager into the communicator's list
		res.first->getLoginManager()->addUsersToList(m_connectedUsers);
		delete res.first;

		// Menu stage: handle the game rooms, statistics, logout & more.
		// from this stage, the user will get to other states according to his requests.
		this->handleClientMenu(clientSocket, res.second);

		std::cout << "[+] Finished conversation for now, close socket. \n" << std::endl;
	}
	catch (const std::exception& excpet)
	{
		excpet.what();
	}
	
	this->m_clients.erase(clientSocket); // remove client from list

	closesocket(clientSocket);
}

/*
After login/signup the user is presented to the games options here in menu,
such as creating a room, joining a room and so.
Input: the conversation socket, the menu handler pointer
*/
void Communicator::handleClientMenu(SOCKET& clientSocket, MenuRequestHandler* newHandler)
{
	JsonRequestPacketDeserializer deSer;
	char clientMenuRequestMessage[MIN_MESSAGE_LEN]; // client's message buffer

	// wait, until a message being recieved.
	// recieve the message & return size of buffer recived in the socket.
	int len = recv(clientSocket, clientMenuRequestMessage, MIN_MESSAGE_LEN, 0); 
	
	clientMenuRequestMessage[len] = NULL; 
	std::cout << "[+] Client message (menu): " << clientMenuRequestMessage << std::endl;

	//the data from the user is analized and gets into a requestInfo struct - that contains the id of the request, recivel time and the request itself.
	RequestInfo reqInfo = deSer.deserializeInfo(clientMenuRequestMessage);

	if (reqInfo.id == QUIT_REQUEST) 
	{
		//if users requested to leave the convo, and close the gui tab:
		//deleting the user from the connected users's vector, and sending to the socket final massege that allows him to know that the action worked.
		
		this->m_factory->getLoginManager()->logout(newHandler->getUsername());
		JsonResponsePacketSerializer ser;
		QuitResponse res;
		res.status = SUCCESS;
		std::vector<MY_BYTE> buffer = ser.serializeResponse(res);

		//sending the response to user:
		std::string msg = JsonResponsePacketSerializer::bufferAsString(buffer);
		std::cout << "[+] response for client (menu): " << msg << std::endl;
		send(clientSocket, msg.c_str(), msg.length(), 0);

		delete newHandler;
		return;
	}

	/* if the user wnated to know the room's state - recursion into the same menu state.
	the request belong to the room's functions, but can be recived if the user returned to this state.*/
	if(reqInfo.id == GET_ROOM_STATE_REQUEST) 
		this->handleClientMenu(clientSocket, newHandler); 

	if (newHandler->isRequestRelevant(reqInfo) == false)
	{
		sendNotReleventMessage(clientSocket); // send a proper message to client
		std::cout << "[-] Request in not relevant: " << clientMenuRequestMessage << std::endl;
		delete newHandler;
	}
	else // request is relevant
	{
		RequestResult reqResult = newHandler->handleRequest(reqInfo);

		//sending the response to user:
		std::string msg = JsonResponsePacketSerializer::bufferAsString(reqResult.buffer);
		std::cout << "[+] response for client (menu): " << msg << std::endl;
		send(clientSocket, msg.c_str(), msg.length(), 0);  // last parameter: flag. for us will be 0.
		
		// move the requests into the next handler, according to the request:
		if (reqInfo.id == JOIN_ROOM_REQUEST)
		{
			if (reqResult.didActionWorked == true)
				this->handleClientRoomMember(clientSocket, (RoomMemberRequestHandler*)reqResult.newHandler);
			else if (reqResult.didActionWorked == false)
				this->handleClientMenu(clientSocket, newHandler); // recursion into the same menu state
		}
		else if (reqInfo.id == CREATE_ROOM_REQUEST)
		{
			if(reqResult.didActionWorked == true)
				this->handleClientRoomAdmin(clientSocket, (RoomAdminRequestHandler*)reqResult.newHandler);
			else if(reqResult.didActionWorked == false)
				this->handleClientMenu(clientSocket, newHandler); // recursion into the same menu state
		}
		

		// Some requests will return to the menu handler as theres no specific handler for it
		if (reqInfo.id == GET_ROOM_REQUEST || reqInfo.id == HIGHSCORE_REQUEST || 
			reqInfo.id == GET_STATISTICS || reqInfo.id == GET_PLAYERS_REQUEST  )
		{
			this->handleClientMenu(clientSocket, newHandler); // recursion into the same menu state
		}
		else
		{
			if (reqInfo.id == LOGOUT_REQUEST) // if user requested to go back to login/signup state
			{
				this->m_factory->getLoginManager()->logout(newHandler->getUsername()); //deleting the user from the list of users
				this->handleNewClient(clientSocket);
			}

			delete newHandler;
		}

	}
}

/*
The function handles the connection between the server and the room admin client.
Input: the connection socket, the new handler pointer
Output: none
*/
void Communicator::handleClientRoomAdmin(SOCKET& clientSocket, RoomAdminRequestHandler* newHandler)
{
	JsonRequestPacketDeserializer deSer;
	char clientMenuRequestMessage[MIN_MESSAGE_LEN]; // client's message buffer

	// wait, until a message being recieved
	// recieve the message & return size of buffer recived in the socket.
	int len = recv(clientSocket, clientMenuRequestMessage, MIN_MESSAGE_LEN, 0); 
	clientMenuRequestMessage[len] = NULL; // end the message string
	std::cout << "[+] Client message (RoomAdmin): " << clientMenuRequestMessage << std::endl;

	RequestInfo reqInfo = deSer.deserializeInfo(clientMenuRequestMessage); //exstracting the data from the request

	if (newHandler->isRequestRelevant(reqInfo) == false) // request not relevant.
	{
		sendNotReleventMessage(clientSocket); // send a proper message to client
		throw std::exception("[-] Error (RoomAdmin): client's message irrelevant.\n");
	}

	RequestResult reqResult = newHandler->handleRequest(reqInfo); //handeling the request, and assembling response for the user. will be held in field reqResult.buffer.

	//sending the response to user:
	std::string msg = JsonResponsePacketSerializer::bufferAsString(reqResult.buffer);
	std::cout << "[+] response for client (RoomAdmin): " << msg << std::endl;
	send(clientSocket, msg.c_str(), msg.length(), 0);  // last parameter: flag. for us will be 0.

	//according to the different actions, navigating the user to his next state
	if (reqInfo.id == CLOSE_ROOM_REQUEST)
	{
		std::cout << "[+] closing room " << newHandler->getRoom().getMetaData().name << std::endl;
		// move back to the menu
		handleClientMenu(clientSocket, (MenuRequestHandler*)reqResult.newHandler);
	}
	if (reqInfo.id == START_GAME_REQUEST)
	{
		// start game
		std::cout << "[+] game should start now!" << std::endl;
		handleClientGame(clientSocket, (GameRequestHandler*) reqResult.newHandler);
	}
	if (reqInfo.id == GET_ROOM_STATE_REQUEST)
	{
		// whenever the user wants an update he will stay in the same handler
		handleClientRoomAdmin(clientSocket, (RoomAdminRequestHandler*)reqResult.newHandler);
	}
}

/*
The function handles the connection between the server and the room member client.
Input: the connection socket, the new handler pointer
Output: none
*/
void Communicator::handleClientRoomMember(SOCKET& clientSocket, RoomMemberRequestHandler* newHandler)
{
	JsonRequestPacketDeserializer deSer;

	// Room Member handler:
	RoomMemberRequestHandler* memberHandle = newHandler;
	char clientMenuRequestMessage[MIN_MESSAGE_LEN]; // client's message buffer

	// wait, until a message being recieved
	int len = recv(clientSocket, clientMenuRequestMessage, MIN_MESSAGE_LEN, 0); // recieve the message & return size of buffer recived in the socket.
	clientMenuRequestMessage[len] = NULL; // end the message string
	std::cout << "[+] Client message (RoomMember): " << clientMenuRequestMessage << std::endl;

	RequestInfo reqInfo = deSer.deserializeInfo(clientMenuRequestMessage);

	if (memberHandle->isRequestRelevant(reqInfo) == false) // request not relevant.
	{
		sendNotReleventMessage(clientSocket); // send a proper message to client
		throw std::exception("[-] Error (RoomMember): client's message irrelevant.\n");
	}

	RequestResult reqResult = memberHandle->handleRequest(reqInfo);

	//sending the response to user:
	std::string msg = JsonResponsePacketSerializer::bufferAsString(reqResult.buffer);
	std::cout << "[+] response for client (RoomMember): " << msg << std::endl;
	send(clientSocket, msg.c_str(), msg.length(), 0);  // last parameter: flag. for us will be 0.

	//user will contine to game handler
	if (reqInfo.id == START_GAME_REQUEST)
		handleClientGame(clientSocket, (GameRequestHandler*)reqResult.newHandler);

	// whenever the user wants an update he will stay in the same handler
	if (reqInfo.id == GET_ROOM_STATE_REQUEST)
		handleClientRoomMember(clientSocket, (RoomMemberRequestHandler*)reqResult.newHandler);

	// navigate back to the menu in case the client wants to leave the room
	if (reqInfo.id == LEAVE_ROOM_REQUEST)
		handleClientMenu(clientSocket, (MenuRequestHandler*)reqResult.newHandler);
}

// game handler
void Communicator::handleClientGame(SOCKET& clientSocket, GameRequestHandler* newHandler)
{
	//printing the msg and then sleeping because wer'e not in v4.0.0 yet.
	std::cout << "handle client game!!!" << std::endl;
	Sleep(100000);

	JsonRequestPacketDeserializer deSer;

	// creating game handler from the parameter:
	GameRequestHandler* gameHandle = newHandler;
	char clientMenuRequestMessage[MIN_MESSAGE_LEN]; // client's message buffer
	int len = recv(clientSocket, clientMenuRequestMessage, MIN_MESSAGE_LEN, 0); // recieve the message & return size of buffer recived in the socket.
	clientMenuRequestMessage[len] = NULL; // end the message string

	std::cout << "[+] Client message (Game): " << clientMenuRequestMessage << std::endl;

	RequestInfo reqInfo = deSer.deserializeInfo(clientMenuRequestMessage);

	if (gameHandle->isRequestRelevant(reqInfo) == false) // request not relevant.
	{
		sendNotReleventMessage(clientSocket); // send a proper message to client
		return;
	}

	//currently on comment because there's nothing to send.
	//RequestResult reqResult = gameHandle->handleRequest(reqInfo);


}

/*
A login/signup state: the request is being checked and identified as
a login request, or a signup request.
Input: the client socket connection, the next handler pointer (to be returned)
Output: a pointer to the handler used to login.
*/
std::pair<LoginRequestHandler*, MenuRequestHandler*> Communicator::handleClientLoginOrSignup(SOCKET& clientSocket)
{
	char clientMessage[MIN_MESSAGE_LEN]; // message buffer

	// wait, until a message being recieved
	int len = recv(clientSocket, clientMessage, MIN_MESSAGE_LEN, 0); // recieve the message & return size of buffer recived in the socket.
	
	if (len == -1)
		throw std::exception("[-] Can't connect to GUI. check your gui please...");
	
	clientMessage[len] = NULL; // end the message string

	std::cout << "[+] Client message (Login // Signup): " << clientMessage << std::endl;

	if (len == 0)
		handleClientLoginOrSignup(clientSocket);
	else
	{
		JsonRequestPacketDeserializer deSer; // deserialization tool
		IRequestHandler* handler = new LoginRequestHandler(this->m_factory);

		RequestInfo reqInfo = deSer.deserializeInfo(clientMessage); // retrieve request information from client message

		if (handler->isRequestRelevant(reqInfo) == false) // request not relevant.
		{
			sendNotReleventMessage(clientSocket); // send a proper message to client
			throw std::exception("Error (Login // Signup): client's message irrelevant.\n");
		}

		RequestResult reqResult = handler->handleRequest(reqInfo);
		if (reqResult.newHandler == nullptr) // client's message structure don't match
		{
			this->sendNotReleventMessage(clientSocket);
			throw std::exception("[-] Request result was empty/currupted (Login // Signup)\n");
		}

		//sending the response to user:
		std::string msg = JsonResponsePacketSerializer::bufferAsString(reqResult.buffer);
		std::cout << "message to send: " << msg << std::endl;
		send(clientSocket, msg.c_str(), msg.length(), 0);

		// use a pair structure in order to return more than one value
		std::pair<LoginRequestHandler*, MenuRequestHandler*> result;
		result.first = (LoginRequestHandler*)handler;

		if (reqResult.didActionWorked == false) // if the action (login/signup) didn't worked, letting the user try again.
		{
			return handleClientLoginOrSignup(clientSocket);
		}
		else
		{ 
			MenuRequestHandler* newHandler = (MenuRequestHandler*)reqResult.newHandler;
			result.second = newHandler;
		}
	
		return result;
	}

	throw std::exception("[-] Client message as empty");
}


/*
The function sends the client a proper message saying his request isn't relevant.
Input: the converstion socket.
Output: none
*/
void Communicator::sendNotReleventMessage(SOCKET& clientSocket)
{
	ErrorResponse err; //sending an error response to the client, because the request is not relevant in this state.
	err.message = "Request is not relevent - Code doesn't match the state of user.";

	JsonResponsePacketSerializer serialize;
	std::vector<MY_BYTE> msgBuffer = serialize.serializeResponse(err);

	std::string msg = JsonResponsePacketSerializer::bufferAsString(msgBuffer);
	std::cout << "[-] Response for client: " << msg << std::endl;
	send(clientSocket, msg.c_str(), msg.length(), 0);  // last parameter: flag. for us will be 0.
}
