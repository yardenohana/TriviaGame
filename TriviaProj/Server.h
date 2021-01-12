#pragma once
#pragma comment (lib, "ws2_32.lib")
#include "Communicator.h"
#include "WSAInitializer.h"

#define PORT_NUMBER 8200 // the port number the server listens in 

using std::string;
using std::cout;
using std::cin;
using std::endl;

class Server
{
private:
	Communicator m_communicator;

public:
	Server();
	~Server();
	void run();
};