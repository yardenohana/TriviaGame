#pragma once
#include <iostream>
#include <vector>
#include "LoggedUser.h"

// Spicified data, for each room
struct RoomData {

	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	bool isActive;
};


// Represents a room in the Trivia game
class Room {

private:
	RoomData m_metaData;
	std::vector<LoggedUser> m_users;

public:
	Room() = default;
	Room(RoomData data, LoggedUser& user);
	~Room() = default;

	void addUser(LoggedUser& user); // Add a user to the list of people in room
	void removeUser(LoggedUser& user); // Remove the user from the list
	std::vector<std::string> getAllUsers() const; // Get the names of all people in room
	std::string getAllUsersString() const; // Get the names of all people in room
	bool findUser(LoggedUser& user); // search the user in the users in the room
	void removeAllUsers();
	// Setters & Getters
	void setMetaData(RoomData metaData);
	RoomData getMetaData() const;
	std::vector<LoggedUser> getUsers();
	void setIsActive(bool isActive);
	bool getIsActive();
};