#include "RequestHandlerFactory.h"
#include <memory>
#include "../Requests/IRequestsHandler.h"
#include "../Handlers/MenuRequestHandler.h"
#include "../Handlers/LoginRequestsHandler.h"
#include "../Handlers/RoomHandlers/RoomAdminRequestHandler.h"
#include "../Handlers/RoomHandlers/RoomMemberRequestHandler.h"
#include "../Handlers/GameHandler/GameRequestHnadler.h"
#include "../Handlers/OneVOneHandler/OneVOneRoomHandler.h"

RequestHandlerFactory::RequestHandlerFactory(std::shared_ptr<IDatabase> database) : m_database(database), m_loginManager(database), m_StatisticsManager(database), m_roomManager(), m_gameManager(database)
{
}

std::unique_ptr<IRequestsHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::make_unique<LoginRequestsHandler>(*this);
}

std::unique_ptr<IRequestsHandler> RequestHandlerFactory::createMenuRequestHandler(std::string loggedUser)
{
    return std::make_unique<MenuRequestHandler>(*this, loggedUser, this->m_database);
}

std::unique_ptr<IRequestsHandler> RequestHandlerFactory::createRoomAdminRequestHandler(const int roomid, const LoggedUser loggedUser)
{
    return std::make_unique<RoomAdminRequestHandler>(roomid, loggedUser, this->getRoomManager(), *this);
}

std::unique_ptr<IRequestsHandler> RequestHandlerFactory::createRoomMemberRequestHandler(const int roomid, const LoggedUser loggedUser)
{
    return std::make_unique<RoomMemberRequestHandler>(roomid, loggedUser, this->getRoomManager(), *this);
}

std::unique_ptr<IRequestsHandler> RequestHandlerFactory::createGameRequestHandler(Game& game, const LoggedUser loggedUser)
{
    return std::make_unique<GameRequestHnadler>(game, loggedUser, this->getGameManager(), *this);
}

std::unique_ptr<IRequestsHandler> RequestHandlerFactory::createOneVOneRoomRequestHandler(const LoggedUser loggedUser)
{
    return std::make_unique<OneVOneRoomHandler>(loggedUser, this->getOneVOneManager(), *this);
}


LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return this->m_StatisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return this->m_roomManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return this->m_gameManager;
}

OneVOneRoomManager& RequestHandlerFactory::getOneVOneManager()
{
    return this->m_oneVOneRoomManager;
}
