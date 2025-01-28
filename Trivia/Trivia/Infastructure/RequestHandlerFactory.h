#pragma once
#include "./Databse/IDatabase.h"
#include <iostream>
#include <memory>
#include "../Requests/IRequestsHandler.h"
#include "../Managers/Login/LoginManager.h"
#include "../Managers/Room/RoomManager.h"
#include "../Managers/OneVOne/OneVOneRoomManager.h"
#include "../Managers/Statistics/StatisticsManager.h"
#include "../Managers/Game/GameManager.h"

class LoginRequestsHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(std::shared_ptr<IDatabase> database);
	std::unique_ptr<IRequestsHandler> createLoginRequestHandler();
	std::unique_ptr<IRequestsHandler> createMenuRequestHandler(std::string loggedUser);
	std::unique_ptr<IRequestsHandler> createRoomAdminRequestHandler(const int roomid, const LoggedUser loggedUser);
	std::unique_ptr<IRequestsHandler> createRoomMemberRequestHandler(const int roomid, const LoggedUser loggedUser);
	std::unique_ptr<IRequestsHandler> createGameRequestHandler(Game& game, const LoggedUser loggedUser);
	std::unique_ptr<IRequestsHandler> createOneVOneRoomRequestHandler(const LoggedUser loggedUser);

	LoginManager& getLoginManager();
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
	GameManager& getGameManager();
	OneVOneRoomManager& getOneVOneManager();

private:
	std::weak_ptr<IDatabase> m_database;
	LoginManager m_loginManager;

	RoomManager m_roomManager;
	StatisticsManager m_StatisticsManager;
	GameManager m_gameManager;
	OneVOneRoomManager m_oneVOneRoomManager;
};

