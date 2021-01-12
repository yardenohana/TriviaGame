#pragma once
#include <iostream>
#include <string>

class LoggedUser
{
private:
	std::string m_username;
	bool m_isAdmin;

public:
	LoggedUser();
	LoggedUser(std::string username);
	~LoggedUser();

	bool getIsAdmin() const;
	void setAdmin(bool isAdmin);
	std::string getUsername() const
	{
		return this->m_username;
	}
};