#include "RoomManager.h"
#include <iostream>
#include "../ManagerException.h"

void RoomManager::createRoom(const LoggedUser& loggedUser, const RoomData& roomData)
{
	std::shared_lock readLock(m_mutex);
	if (this->m_rooms.contains(roomData.id)) {
		throw ManagerException("Room Already Exists, cannot create room");
	}
	readLock.unlock();

	Room r(loggedUser, roomData);

	std::unique_lock writeLock(m_mutex);
	this->m_rooms.insert({ roomData.id, r});
}

void RoomManager::deleteRoom(int roomId)
{
	std::shared_lock readLock(m_mutex);
	if (!this->m_rooms.contains(roomId)) {
		throw ManagerException("Room does not exists Exists, cannot delete room");
	}
	readLock.unlock();
	std::unique_lock writeLock(m_mutex);
	this->m_rooms.erase(roomId);
}

RoomState RoomManager::getRoomState(int roomId) const
{
	return this->getRoom(roomId).getState();
}

const Room RoomManager::getRoom(int roomId) const
{
	if (!this->m_rooms.contains(roomId)) {
		throw ManagerException("Room does not exsists");
	}
	
	return this->m_rooms.at(roomId);
}

std::vector<Room> RoomManager::getRooms() const
{
	std::vector<Room> rooms;

	for (const std::pair<int, Room>& room : this->m_rooms) {
		rooms.push_back(room.second);
	}

	return rooms;
}

void RoomManager::changeRoomState(int roomId, RoomState newState)
{
	std::shared_lock readLock(m_mutex);
	if (!this->m_rooms.contains(roomId)) {
		throw ManagerException("Room does not exsists");
	}

	this->m_rooms.at(roomId).setState(newState);
}

int RoomManager::getEmptyRoomId()
{
	int roomId = 0;
	std::shared_lock readLock(m_mutex);
	while (this->m_rooms.contains(roomId)) {
		roomId++;
	}
	readLock.unlock();

	return roomId;
}

void RoomManager::removeUserFromRoom(const int roomid, const LoggedUser& loggedUser)
{
	this->getRoomNotConst(roomid).removeUser(loggedUser);
}

void RoomManager::addUserToRoom(const int roomid, const LoggedUser& loggedUser)
{
	this->getRoomNotConst(roomid).addUser(loggedUser);
}

Room& RoomManager::getRoomNotConst(int roomId)
{
	std::shared_lock readLock(m_mutex);
	if (!this->m_rooms.contains(roomId)) {
		throw ManagerException("Room does not exsists");
	}

	return this->m_rooms.at(roomId);
}
