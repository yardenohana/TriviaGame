#include "Room.h"

Room::Room(RoomData data, LoggedUser& user)
{
	this->m_metaData = data;
	this->m_users.push_back(user);
}


/*
The function adds a user to the user's list.
Input: the user
Output: none
*/
void Room::addUser(LoggedUser& user)
{
	this->m_users.push_back(user);
}

/*
The function removes a user from the users list.
Input: the user to remove
Output: none
*/
void Room::removeUser(LoggedUser& user)
{
	std::vector<LoggedUser> newList; // target user to delete
	for (LoggedUser userInRoom : this->m_users)
	{
		if (user.getUsername() != userInRoom.getUsername())
			newList.push_back(userInRoom); // save iterator
	}
	
	this->m_users = newList;
}

/*
Input: none
Output: The function returns the names list of users.
*/
std::vector<std::string> Room::getAllUsers() const
{
	std::vector<std::string> theUsers;
	
	for (LoggedUser usr : this->m_users)
		theUsers.push_back(usr.getUsername());
	
	return theUsers;
}

std::string Room::getAllUsersString() const
{
	std::string users = "";
	for (LoggedUser usr : this->m_users)
		users += usr.getUsername() + ",";
	std::string newUsers = users.substr(0, users.size() - 1);
	return newUsers;
}

/*
function searches for the user in the list of users that are in the room.
input: 'user', LoggedUser.
output: true if the user is in the room, false if not.
*/
bool Room::findUser(LoggedUser& user)
{
	std::vector<LoggedUser>::iterator it;
	for (it = m_users.begin(); it != m_users.end(); it++)
	{
		if (it->getUsername() == user.getUsername())
			return true;
	}
	return false;
}

/*
function cleans all users from a room.
*/
void Room::removeAllUsers()
{
	this->m_users.clear();
}

//getters - setters:

void Room::setMetaData(RoomData metaData)
{
	this->m_metaData = metaData;
}

RoomData Room::getMetaData() const
{
	return this->m_metaData;
}

std::vector<LoggedUser> Room::getUsers()
{
	return this->m_users;
}

void Room::setIsActive(bool isActive)
{
	this->m_metaData.isActive = isActive;
}

bool Room::getIsActive()
{
	return this->m_metaData.isActive;
}


