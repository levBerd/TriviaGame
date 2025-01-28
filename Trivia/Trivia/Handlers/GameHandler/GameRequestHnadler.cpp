#include "GameRequestHnadler.h"
#include "../../Infastructure/ServerException.h"
#include "../../ErrorMessages.h"
#include "../../Requests/JsonRequestPacketDeserializer.h"
#include "../../Response/JsonResponsePacketSerializer.h"

GameRequestHnadler::GameRequestHnadler(Game& game, LoggedUser loggedUser, GameManager& gameManager, RequestHandlerFactory& handlerFactory) : m_game(game), m_user(loggedUser), m_gameManager(gameManager), m_handlerFactory(handlerFactory)
{
}

bool GameRequestHnadler::isRequestsRelevant(const RequestInfo& requestInfo) const
{
    return requestInfo.id == CodeId::LeaveGame || requestInfo.id == CodeId::GetGameState;
}

RequestResult GameRequestHnadler::handleRequest(const RequestInfo& requestInfo)
{
    try {
        if (requestInfo.id == CodeId::LeaveGame) {
            return this->leaveGame(requestInfo);
        }
        if (requestInfo.id == CodeId::GetGameState) {
            return this->getGameState(requestInfo);
        }
        throw ManagerException("got to GameRequestHandler with irrelevant requestInfo");

    }
    catch (ManagerException& e) {
        ErrorResponse Er;
        Er.message = e.what();
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handlerFactory.createGameRequestHandler(this->m_game, this->m_user);
        return rr;
    }
    catch (ServerException& e) {
        std::cout << "Server Error!!!" << std::endl;
        ErrorResponse Er;
        Er.message = "Server had an error: " + std::string(e.what());
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handlerFactory.createGameRequestHandler(this->m_game, this->m_user);
        return rr;
    }
}

void GameRequestHnadler::handleCrush()
{
    try {
        this->m_gameManager.submitGameStatsToDB(this->m_game.getUserDataOfUser(this->m_user), this->m_user);
        this->m_game.removePlayer(m_user.getName());
        this->m_handlerFactory.getRoomManager().removeUserFromRoom(this->m_game.getGameId(), this->m_user);

        if (this->m_game.isGameEmpty()) {
            this->m_handlerFactory.getRoomManager().deleteRoom(this->m_game.getGameId());
            this->m_gameManager.deleteGame(this->m_game.getGameId());
        }
    }
    catch (ManagerException& e) {
        // game closed or not existing
        // throw user to menu handler

        std::cout << e.what() << std::endl;
    }
    
    this->m_handlerFactory.getLoginManager().logout(this->m_user.getName());
}

RequestResult GameRequestHnadler::leaveGame(const RequestInfo& requestInfo)
{
    try {
        this->m_gameManager.submitGameStatsToDB(this->m_game.getUserDataOfUser(this->m_user), this->m_user);
        this->m_game.removePlayer(m_user.getName());
        this->m_handlerFactory.getRoomManager().removeUserFromRoom(this->m_game.getGameId(), this->m_user);

        if (this->m_game.isGameEmpty()) {
            this->m_handlerFactory.getRoomManager().deleteRoom(this->m_game.getGameId());
            this->m_gameManager.deleteGame(this->m_game.getGameId());
        }
    }
    catch (ManagerException& e) {
        // game closed or not existing
        // throw user to menu handler

        std::cout << e.what() << std::endl;
    }
    
    LeaveGameResponse leaveGameResponse;
    leaveGameResponse.status = static_cast<unsigned int>(stutusId::Success);

    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(leaveGameResponse);
    rr.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user.getName());
    return rr;
}

RequestResult GameRequestHnadler::getGameState(const RequestInfo& requestInfo)
{
    GetGameStateRequest getGameStateRequest = JsonRequestPacketDeserializer::deserializeGetGameStateRequest(requestInfo.buffer);

    RequestResult rr;
    GameStateResponse gameStateResponse = this->m_game.getGameStateResponse(getGameStateRequest.answerid, this->m_user.getName());

    rr.newHandler = this->m_handlerFactory.createGameRequestHandler(this->m_game, this->m_user);
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(gameStateResponse);
    return rr;
}

