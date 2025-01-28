#include "Room.h"
#include "../ManagerException.h"
#include <algorithm>
#include <iostream>
#include <vector>

Room::Room(const LoggedUser& loggedUser, const RoomData& data) : m_metadata(data)
{
    this->m_users.push_back(loggedUser);
}

void Room::addUser(const LoggedUser& loggedUser)
{
    if (std::find(this->m_users.begin(), this->m_users.end(), loggedUser) != this->m_users.end()) {
        throw ManagerException("user Already in Room, cannot add user");
    }

    this->m_users.push_back(loggedUser);
}

void Room::removeUser(const LoggedUser& loggedUser)
{
    if (std::find(this->m_users.begin(), this->m_users.end(), loggedUser) == this->m_users.end()) {
        throw ManagerException("can't remove user in Room, user does not exists");
    }

    this->m_users.erase(std::remove(this->m_users.begin(), this->m_users.end(), loggedUser), this->m_users.end());
}

std::vector<std::string> Room::getAllUsers() const
{
    std::vector<std::string> userNames;

    for (const LoggedUser& loggedUser : this->m_users) {
        userNames.push_back(loggedUser.getName());
    }

    return userNames;
}

RoomState Room::getState() const
{
    return this->m_metadata.status;
}

RoomData Room::getRoomData() const
{
    return this->m_metadata;
}

void Room::setState(const RoomState newState)
{
    this->m_metadata.status = newState;
}
