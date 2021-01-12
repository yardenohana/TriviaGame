#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include "IDatabase.h"
#include "LoggedUser.h"
#include "SqliteDatabase.h"

class LoginManager
{
private:
	IDatabase* m_database; // the database saves the users info & the game components
	std::vector<LoggedUser> m_loggedUsers; // logged users list

public:

	LoginManager();
	~LoginManager();

	// Basic functions used to insert the info into the database, and manage the users list
	bool signup(std::string username, std::string password, std::string email);
	bool login(std::string username, std::string password);
	bool logout(std::string username);
	void addUsersToList(std::vector<LoggedUser>& loggedUsers) const;
	std::vector<LoggedUser> getAllUsers() const;

	IDatabase* getDB();
};
