#include "RoomMemberRequestHandler.h"
#include "../../ErrorMessages.h"
#include "../../Requests/JsonRequestPacketDeserializer.h"
#include "../../Response/JsonResponsePacketSerializer.h"
#include "../../Infastructure/ServerException.h"


RoomMemberRequestHandler::RoomMemberRequestHandler(const int roomid, const LoggedUser loggedUser, RoomManager& roomManageer, RequestHandlerFactory& handlerFactory) : m_roomid(roomid), m_user(loggedUser), m_roomManager(roomManageer), m_handlerFactory(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestsRelevant(const RequestInfo& requestInfo) const
{
    return requestInfo.id == CodeId::GetRoomStateRequest || requestInfo.id == CodeId::LeaveRoom;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    try {
        if (requestInfo.id == CodeId::GetRoomStateRequest) {
            return this->getRoomState(requestInfo);
        }

        if (requestInfo.id == CodeId::LeaveRoom) {
            return this->leaveRoom(requestInfo);
        }
        throw ManagerException("got to RoomMemberRequestHandler with irrelevant requestInfo");

    }
    catch (ManagerException& e) {
        ErrorResponse Er;
        Er.message = e.what();
        std::cout << this->m_user.getName() + " Had an error: " << e.what() << std::endl;
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handlerFactory.createRoomMemberRequestHandler(this->m_roomid, this->m_user);
        return rr;
    }
    catch (ServerException& e) {
        std::cout << "Server Error!!!" << std::endl;
        ErrorResponse Er;
        Er.message = "Server had an error: " + std::string(e.what());
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handlerFactory.createRoomMemberRequestHandler(this->m_roomid, this->m_user);
        return rr;
    }
}

void RoomMemberRequestHandler::handleCrush()
{
    try {
        this->m_roomManager.removeUserFromRoom(this->m_roomid, this->m_user);
    }
    catch (ManagerException& e) {}
    this->m_handlerFactory.getLoginManager().logout(this->m_user.getName());

}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo) const
{
    GetRoomStateResponse getRoomSateResponse;
    RoomState r;
    getRoomSateResponse.status = static_cast<unsigned int>(stutusId::Success);
    getRoomSateResponse.questionCount = m_roomManager.getRoom(m_roomid).getRoomData().numOfQuestionsInGame;
    getRoomSateResponse.answerTimeOut = m_roomManager.getRoom(m_roomid).getRoomData().timePerQuestion;
    getRoomSateResponse.roomState = static_cast<unsigned int>(m_roomManager.getRoom(m_roomid).getState());
    getRoomSateResponse.players = m_roomManager.getRoom(m_roomid).getAllUsers();
    RequestResult rr;
    if (getRoomSateResponse.roomState == (unsigned int)RoomState::GAME_STARTED) {
        rr.newHandler = this->m_handlerFactory.createGameRequestHandler(this->m_handlerFactory.getGameManager().getGame(this->m_roomid), m_user);
    }
    else {
        rr.newHandler = this->m_handlerFactory.createRoomMemberRequestHandler(m_roomid, m_user);
    }
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(getRoomSateResponse);
    return rr;
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& requestInfo) const
{
    try {
        this->m_roomManager.removeUserFromRoom(this->m_roomid, this->m_user);
    }
    catch (ManagerException& e) {
        // room closed or not existing
        // throw user to menu handler
    }
   
    LeaveRoomResponse leaveRoomResposne;
    leaveRoomResposne.status = static_cast<unsigned int>(stutusId::Success);

    RequestResult rr;
    rr.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user.getName());
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(leaveRoomResposne);
    return rr;
}
