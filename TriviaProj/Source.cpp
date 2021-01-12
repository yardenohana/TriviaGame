#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Communicator.h"
#include "Server.h"
#include <iostream>
#include <exception>

#define EXIT "EXIT"

int main()
{
	Server myServer;
	
	// Create a thread, in order to the Server to work 
	// simultaneously with user command input.
	thread threadserverRun(&Server::run, myServer);
	threadserverRun.detach();
	
	// allowing the user to enter commands to the 
	// console without interrupting the server.
	string command = "";
	while (command != EXIT)
	{
		cout << ">>> ";
		cin >> command;
		
		if (command == EXIT)
			system("pause");
		
		cout << "User entered: " + command << endl;
	}
	return 0;
}