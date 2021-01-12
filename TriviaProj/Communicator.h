#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <map>
#include <exception>
#include <string>
#include <mutex>
#include <typeinfo>
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "Request.h"
#include "Response.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

#define MIN_MESSAGE_LEN 200

using std::thread;
using std::map;

class Communicator
{
private:
	SOCKET _serverSocket;
	std::vector<LoggedUser> m_connectedUsers;
	map<SOCKET, IRequestHandler*> m_clients;
	map<LoggedUser, SOCKET> m_usersMap;
	
	// shared factory to all threads in the game
	RequestHandlerFactory* m_factory;

	void startHandleRequests();
	void handleNewClient(SOCKET clientSocket);
	
	// handles the login/signup states
	std::pair<LoginRequestHandler*, MenuRequestHandler*> handleClientLoginOrSignup(SOCKET& clientSocket);
	
	// handles the menu state, after login/signup
	void handleClientMenu(SOCKET& clientSocket, MenuRequestHandler* newHandler); 

	// handles the different room handlers:
	// the user will be RoomAdmin if he created a room in the previous action, 
	// or RoomMember if he joined a room in the previous action.
	void handleClientRoomAdmin(SOCKET& clientSocket, RoomAdminRequestHandler* newHandler);
	void handleClientRoomMember(SOCKET& clientSocket, RoomMemberRequestHandler* newHandler);

	// handles the Game state
	void handleClientGame(SOCKET& clientSocket, GameRequestHandler* newHandler);

	// response for unrelevent client message
	void sendNotReleventMessage(SOCKET& clientSocket);

	
public:
	Communicator();
	~Communicator();

	void bindAndListen(int port);
};

