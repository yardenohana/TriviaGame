#include "Server.h"

Server::Server() : m_communicator()
{

}

Server::~Server() {}

void Server::run()
{
	try
	{
		WSAInitializer wsaInit;
		Communicator myCommunicator;

		myCommunicator.bindAndListen(PORT_NUMBER);
	}
	catch (std::exception & e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}
}
