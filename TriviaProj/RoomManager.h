#pragma once
#include <iostream>
#include <map>
#include "Room.h"

class RoomManager {

private:
	std::map<int, Room*> m_rooms; // map of roomID & room pairs

public:
	static unsigned int latestID; // latest room id
	
	RoomManager();
	~RoomManager() = default;

	void createRoom(LoggedUser& user, RoomData data);
	bool createRoom(LoggedUser& user, std::string name, unsigned int maxPlayers, unsigned int numOfQuestionsInGame, unsigned int timePerQuestion);

	void deleteRoom(int roomID); // deletes a room based on it's ID.
	bool getRoomState(int roomID) const;
	std::vector<RoomData> getRooms() const; // returns vector of all rooms that are currently connected.
	Room* getRoom(int roomID); // return the room based on it's ID
	Room* getRoom(std::string roomName); // return the room based on it's name
	std::string getAllNameRooms(); // return one long string that contains the name of the current rooms.
	
	bool addUserToRoom(std::string roomName, LoggedUser& user); // adds user to room based on the room's name.
	Room* addUserToRoomB(std::string roomName, LoggedUser& user); // adds user to room based on the room's name. returns the room.


	void deleteUserFromRooms(LoggedUser user); // removes a user from all the rooms it appears in.
	std::vector<std::string> getAllUsers(); // return vector of all the users that are connected to any room.
	std::vector<LoggedUser> getAllUsersObject(int roomID); // return vector of all the LoggedUser that are connected to any room.
};
