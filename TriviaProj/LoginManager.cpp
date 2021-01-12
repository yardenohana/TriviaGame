#include "LoginManager.h"

LoginManager::LoginManager()
{
	m_database = new SqliteDatabase(); // new sqlite database
	((SqliteDatabase*)m_database)->open();  // initialize database in case it hasn't already
	m_loggedUsers.clear(); // new list
}

LoginManager::~LoginManager()
{
	delete m_database;
}

/*
function adds the info as a new user in the DB.
Input: the users information.
Output: true when managed to signup, else false.
*/
bool LoginManager::signup(std::string username, std::string password, std::string email)
{
	//if user already exsist in system.

	for (LoggedUser usr : this->m_loggedUsers)
		if (usr.getUsername() == username)
			return false;

	if(this->m_database->doesUserExist(username))
		return false;
	
	LoggedUser user(username);
	this->m_database->addNewUser(username, password, email);
	this->m_loggedUsers.push_back(user);
	
	return true;
}

/*
function checks if the username exsists in the db,
and makes sure that this is indeed it's password.
if so, the function will add the user to the users vector.
*/
bool LoginManager::login(std::string username, std::string password)
{
	//if user doesn't exsist in system.
	for (LoggedUser usr : this->m_loggedUsers)
		if (usr.getUsername() == username)
			return false;

	if (!this->m_database->doesUserExist(username))
	{
		std::cout << "[-] User can't login, doesn't exist in the database." << std::endl;
		return false;
	}

	//checking if the password that belongs to the user in the db is the same as the 'password' value.
	if (this->m_database->doesPasswordMatchToUser(username, password))
	{
		LoggedUser user(username);
		this->m_loggedUsers.push_back(user);

		return true;
	}
	std::cout << "[-] User can't login, don't match the password in the database." << std::endl;
	return false;
}

/*
function checks if the user exsists in the db,
and makes sure that this is in the users vector.
if so, the function will erase the user from the users vector.
*/
bool LoginManager::logout(std::string username)
{
	//if user isn't exsist in system.
	if (!this->m_database->doesUserExist(username))
		return false;

	std::vector<LoggedUser>::iterator it;
	for (it = this->m_loggedUsers.begin(); it != this->m_loggedUsers.end(); it++)
	{
		if (it->getUsername() == username)
		{
			this->m_loggedUsers.erase(it);
			return true;
		}
	}
	return true;
}

/*
The function adds the current connected users to the global users list.
Input: the global list of connected users
Output: none
*/
void LoginManager::addUsersToList(std::vector<LoggedUser>& loggedUsers) const
{
	for (auto itr = m_loggedUsers.begin(); itr != m_loggedUsers.end(); itr++)
	{
		loggedUsers.push_back(*itr);
	}
}

//getters:
std::vector<LoggedUser> LoginManager::getAllUsers() const
{
	return this->m_loggedUsers;
}

IDatabase* LoginManager::getDB()
{
	return this->m_database;
}
