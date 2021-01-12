#pragma once
#include <iostream>

class LoggedUsers
{
private:
	std::string m_username;
public:
	LoggedUsers();
	~LoggedUsers();
	std::string getUsername();
};
