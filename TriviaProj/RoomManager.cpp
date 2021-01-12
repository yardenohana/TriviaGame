#include "RoomManager.h"

RoomManager::RoomManager()
{
}

/*
The function creates a Room and adds it to the rooms list.
Input: the user, the room's data
Output: none
*/
void RoomManager::createRoom(LoggedUser& user, RoomData data)
{
	Room* roomToAdd = new Room(data, user);

	// Insertion of new room:
	this->m_rooms.insert(std::pair<int, Room*>(latestID, roomToAdd));

	// Update the latest room ID:
	latestID++;
}

bool RoomManager::createRoom(LoggedUser& user, std::string name, unsigned int maxPlayers,
	unsigned int numOfQuestionsInGame, unsigned int timePerQuestion)
{
	bool isNameTaken = false;
	for (RoomData data : this->getRooms())
	{
		if (data.name == name)
			isNameTaken = true;
	}
	if (!isNameTaken)
	{
		RoomData data;
		data.id = latestID;
		data.isActive = false;
		data.maxPlayers = maxPlayers;
		data.name = name;
		data.numOfQuestionsInGame = numOfQuestionsInGame;
		data.timePerQuestion = timePerQuestion;

		Room* addRoom = new Room(data, user);
		this->m_rooms.insert(std::pair<int, Room*>(latestID, addRoom));

		latestID++;
		return true;
	}
	else
	{
		//another room already took this name, can't create the room.
		return false;
	}

}

/*
The function deletes a room from the rooms list.
Input: the room ID 
output: none
*/
void RoomManager::deleteRoom(int roomID)
{
	this->m_rooms.erase(roomID);
}

bool RoomManager::getRoomState(int roomID) const
{
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		if (room.first == roomID && room.second->getMetaData().isActive)
			return true;
	}

	return false;
}

/*
Input: none
Output: A vector of all the rooms data
*/
std::vector<RoomData> RoomManager::getRooms() const
{
	std::vector<RoomData> signedRoomsData;
	for (std::pair<int, Room*> room : this->m_rooms)
		signedRoomsData.push_back(room.second->getMetaData());
	
	return signedRoomsData;
}

bool RoomManager::addUserToRoom(std::string roomName, LoggedUser& user)
{
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		if (room.second->getMetaData().name == roomName)
		{
			bool placeForUser = (room.second->getAllUsers().size() < room.second->getMetaData().maxPlayers);
			if (placeForUser == false)
				return false;
			if (room.second->findUser(user) == false)
			{
				room.second->addUser(user);
				return true;
			}

		}
	}
	return false;
}

/*
The same add user function but returns the room object.
*/
Room* RoomManager::addUserToRoomB(std::string roomName, LoggedUser& user)
{
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		if (room.second->getMetaData().name == roomName){
			room.second->addUser(user);
			return room.second;
		}
	}
}

Room* RoomManager::getRoom(int roomID)
{
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		if (room.first == roomID)
			return room.second;
	}
}

Room* RoomManager::getRoom(std::string roomName)
{
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		if (room.second->getMetaData().name == roomName)
			return room.second;
	}
}

void RoomManager::deleteUserFromRooms(LoggedUser user)
{
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		room.second->removeUser(user);
	}
}

std::string RoomManager::getAllNameRooms()
{
	std::string rooms = "";
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		rooms += room.second->getMetaData().name + ",";
	}
	std::string newRooms = rooms.substr(0, rooms.size() - 1);
	return newRooms;
}

std::vector<std::string> RoomManager::getAllUsers()
{
	std::vector<std::string> users;
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		std::vector<std::string> usersPerRoom = room.second->getAllUsers();
		for (std::string user : usersPerRoom)
			users.push_back(user);
	}
	return users;
}

std::vector<LoggedUser> RoomManager::getAllUsersObject(int roomID)
{
	for (std::pair<int, Room*> room : this->m_rooms)
	{
		if (room.first == roomID)
			return room.second->getUsers();
	}
}
