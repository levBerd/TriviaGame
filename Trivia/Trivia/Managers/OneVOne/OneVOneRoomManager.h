#pragma once
#include "../Login/LoggedUser.h"
#include <vector>
#include <utility>

enum class OveVOneRoomState {
	LOOKING_FOR_OPPONENT,
	GAME_STARTED
};

class OneVOneRoomManager
{
public:
	void addUserToWaitingRoom(const LoggedUser& loggedUser);
	void removeUserFromOneVOneWaitRoom(const LoggedUser& loggedUser);
	int getStateOfUser(const LoggedUser& loggedUser) const;
	std::string getNameOfWaitingUser() const;
	void startGameWithWaitingUser(int newGameId);
	int getGameOfUser(const LoggedUser& loggedUser);

private:
	std::string userWaiting = "";
	std::vector<std::pair<int, std::string>> usersGameStarted;
};

