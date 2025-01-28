#include "OneVOneRoomManager.h"
#include "../ManagerException.h"
#include <thread>
#include <iostream>
#include "../../Infastructure/RequestHandlerFactory.h"

void OneVOneRoomManager::addUserToWaitingRoom(const LoggedUser& loggedUser)
{
    userWaiting = loggedUser.getName();
}

void OneVOneRoomManager::removeUserFromOneVOneWaitRoom(const LoggedUser& loggedUser)
{
    if (this->userWaiting != loggedUser.getName()) {
        throw ManagerException("User isnt waiting!");
    }
    userWaiting = "";
}

int OneVOneRoomManager::getStateOfUser(const LoggedUser& loggedUser) const
{
    if (this->userWaiting == loggedUser.getName()) {
        return (unsigned int)OveVOneRoomState::LOOKING_FOR_OPPONENT;
    }
    else {
        return (unsigned int)OveVOneRoomState::GAME_STARTED;
    }
}

std::string OneVOneRoomManager::getNameOfWaitingUser() const
{
    return this->userWaiting;
}

void OneVOneRoomManager::startGameWithWaitingUser(int newGameId)
{
    usersGameStarted.emplace_back(std::make_pair(newGameId, this->userWaiting));
    this->userWaiting = "";
}


int OneVOneRoomManager::getGameOfUser(const LoggedUser& loggedUser)
{
    for (auto it = usersGameStarted.begin(); it != usersGameStarted.end(); ) {
        if (it->second == loggedUser.getName()) {
            int gameId = it->first;
            usersGameStarted.erase(it);
            return gameId;
        }
        else {
            ++it;
        }
    }
    throw ManagerException("User game doesn't exist");
}


