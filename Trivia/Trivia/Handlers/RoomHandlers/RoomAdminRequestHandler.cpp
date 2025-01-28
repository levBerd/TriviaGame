#include "RoomAdminRequestHandler.h"
#include "../../ErrorMessages.h"
#include "../../Requests/JsonRequestPacketDeserializer.h"
#include "../../Response/JsonResponsePacketSerializer.h"
#include "../../Infastructure/ServerException.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(const int roomid, const LoggedUser loggedUser, RoomManager& roomManageer, RequestHandlerFactory& handlerFactory) : m_roomid(roomid), m_user(loggedUser), m_roomManager(roomManageer), m_handlerFactory(handlerFactory)
{
}

bool RoomAdminRequestHandler::isRequestsRelevant(const RequestInfo& requestInfo) const
{
    return requestInfo.id == CodeId::GetRoomStateRequest || requestInfo.id == CodeId::StartGameRequest || requestInfo.id == CodeId::CloseRoomRequest;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    try {
        if (requestInfo.id == CodeId::GetRoomStateRequest) {
            return this->getRoomState(requestInfo);
        }
        if (requestInfo.id == CodeId::StartGameRequest) {
            return this->startGame(requestInfo);
        }
        if (requestInfo.id == CodeId::CloseRoomRequest) {
            return this->closeRoom(requestInfo);
        }
        throw ManagerException("got to RoomAdminRequestHandler with irrelevant requestInfo");

    }
    catch (ManagerException& e) {
        ErrorResponse Er;
        Er.message = e.what();
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(this->m_roomid, this->m_user);
        return rr;
    }
    catch (ServerException& e) {
        std::cout << "Server Error!!!" << std::endl;
        ErrorResponse Er;
        Er.message = "Server had an error: " + std::string(e.what());
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(this->m_roomid, this->m_user);
        return rr;
    }
}

void RoomAdminRequestHandler::handleCrush()
{
    this->m_roomManager.deleteRoom(m_roomid);
    this->m_handlerFactory.getLoginManager().logout(this->m_user.getName());
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& requestInfo)
{
    this->m_roomManager.deleteRoom(m_roomid);
    
    CloseRoomResponse closeRoomResponse;
    closeRoomResponse.status = static_cast<unsigned int>(stutusId::Success);

    RequestResult rr;
    rr.newHandler = this->m_handlerFactory.createMenuRequestHandler(m_user.getName());
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(closeRoomResponse);
    return rr;
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    this->m_roomManager.changeRoomState(m_roomid, RoomState::GAME_STARTED);

    StartGameResponse startGameResponse;
    startGameResponse.status = static_cast<unsigned int>(stutusId::Success);
    
    RequestResult rr;
    // initialize new game
    Game& game = this->m_handlerFactory.getGameManager().createGame(this->m_roomManager.getRoom(this->m_roomid));

    rr.newHandler = this->m_handlerFactory.createGameRequestHandler(game, m_user);
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(startGameResponse);
    return rr;
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& requestInfo) const
{
    GetRoomStateResponse getRoomSateResponse;
    RoomState r;
    getRoomSateResponse.status = static_cast<unsigned int>(stutusId::Success);
    getRoomSateResponse.questionCount = m_roomManager.getRoom(m_roomid).getRoomData().numOfQuestionsInGame;
    getRoomSateResponse.answerTimeOut = m_roomManager.getRoom(m_roomid).getRoomData().timePerQuestion;
    getRoomSateResponse.roomState = static_cast<unsigned int>(m_roomManager.getRoom(m_roomid).getState());
    getRoomSateResponse.players = m_roomManager.getRoom(m_roomid).getAllUsers();
    RequestResult rr;
    if (getRoomSateResponse.status == (unsigned int)RoomState::GAME_STARTED) {
        rr.newHandler = this->m_handlerFactory.createGameRequestHandler(this->m_handlerFactory.getGameManager().getGame(this->m_roomid), m_user);
    }
    else {
        rr.newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(m_roomid, m_user);
    }
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(getRoomSateResponse);
    return rr;
}
