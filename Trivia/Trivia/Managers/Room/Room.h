#pragma once
#include "../Login/LoggedUser.h"
#include <vector>
#include <string>

enum class RoomState {
	INACTIVE,
	ACTIVE,
	GAME_STARTED
};

struct RoomData {
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	RoomState status;
};


class Room
{
public:
	Room(const LoggedUser& loggedUser, const RoomData& data );
	~Room() = default;
	
	void addUser(const LoggedUser& loggedUser);
	void removeUser(const LoggedUser& loggedUser);
	std::vector<std::string> getAllUsers() const;
	RoomState getState() const;
	RoomData getRoomData() const;
	void setState(const RoomState newState);

private:
	std::vector<LoggedUser> m_users;
	RoomData m_metadata;

};

