#pragma once
#include <map>
#include "Room.h"
#include <vector>
#include <mutex>
#include <shared_mutex>

class RoomManager
{
public:
	void createRoom(const LoggedUser& loggedUser, const RoomData& roomData);
	void deleteRoom(int roomId);
	RoomState getRoomState(int roomId) const;
	const Room getRoom(int roomId) const;
	std::vector<Room> getRooms() const;
	void changeRoomState(int roomId, RoomState newState);
	int getEmptyRoomId();
	void removeUserFromRoom(const int roomid, const LoggedUser& loggedUser);
	void addUserToRoom(const int roomid, const LoggedUser& loggedUser);

private:
	std::map<int, Room> m_rooms;
	std::shared_mutex m_mutex;
	Room& getRoomNotConst(int roomId);
};

