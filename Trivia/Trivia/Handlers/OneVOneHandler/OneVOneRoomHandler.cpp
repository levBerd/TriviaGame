#include "OneVOneRoomHandler.h"
#include "../../ErrorMessages.h"
#include "../../Requests/JsonRequestPacketDeserializer.h"
#include "../../Response/JsonResponsePacketSerializer.h"
#include "../../Infastructure/ServerException.h"

OneVOneRoomHandler::OneVOneRoomHandler(const LoggedUser& loggedUser, OneVOneRoomManager& oneVOneRoomManager, 
	RequestHandlerFactory& handlerFactory) : m_user(loggedUser), m_oneVOneRoomManager(oneVOneRoomManager), m_handlerFactory(handlerFactory)
{
}

bool OneVOneRoomHandler::isRequestsRelevant(const RequestInfo& requestInfo) const
{
	return requestInfo.id == CodeId::GetOneVOneState || requestInfo.id == CodeId::LeaveOneVOneWait;
}

RequestResult OneVOneRoomHandler::handleRequest(const RequestInfo& requestInfo)
{
    try {
        if (requestInfo.id == CodeId::GetOneVOneState) {
            return this->getOneVOneRoomState(requestInfo);
        }

        if (requestInfo.id == CodeId::LeaveOneVOneWait) {
            return this->leaveOneVOneRoom(requestInfo);
        }
        throw ManagerException("got to RoomMemberRequestHandler with irrelevant requestInfo");

    }
    catch (ManagerException& e) {
        ErrorResponse Er;
        Er.message = e.what();
        std::cout << this->m_user.getName() + " Had an error: " << e.what() << std::endl;
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handlerFactory.createOneVOneRoomRequestHandler(this->m_user);
        return rr;
    }
    catch (ServerException& e) {
        std::cout << "Server Error!!!" << std::endl;
        ErrorResponse Er;
        Er.message = "Server had an error: " + std::string(e.what());
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handlerFactory.createOneVOneRoomRequestHandler(this->m_user);
        return rr;
    }
}

void OneVOneRoomHandler::handleCrush()
{
    try {
        this->m_oneVOneRoomManager.removeUserFromOneVOneWaitRoom(this->m_user);
    }
    catch (ManagerException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    this->m_handlerFactory.getLoginManager().logout(this->m_user.getName());
}

RequestResult OneVOneRoomHandler::getOneVOneRoomState(const RequestInfo& requestInfo) const
{
    GetOneVOneStateResponse getOneVOneStateResponse;
    getOneVOneStateResponse.roomState = this->m_oneVOneRoomManager.getStateOfUser(this->m_user);
    getOneVOneStateResponse.status = static_cast<unsigned int>(stutusId::Success);

    RequestResult rr;
    
    if (getOneVOneStateResponse.roomState == (unsigned int)(OveVOneRoomState::LOOKING_FOR_OPPONENT)) {
        rr.newHandler = this->m_handlerFactory.createOneVOneRoomRequestHandler(this->m_user);
    }
    else if(getOneVOneStateResponse.roomState == (unsigned int)(OveVOneRoomState::GAME_STARTED)) {
        rr.newHandler = this->m_handlerFactory.createGameRequestHandler(this->m_handlerFactory.getGameManager().getGame(
            this->m_oneVOneRoomManager.getGameOfUser(this->m_user)), m_user);
    }
    
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(getOneVOneStateResponse);
    return rr;
}

RequestResult OneVOneRoomHandler::leaveOneVOneRoom(const RequestInfo& requestInfo) const
{
    this->m_oneVOneRoomManager.removeUserFromOneVOneWaitRoom(this->m_user);

    LeaveOneVOneWaitResponse leaveOneVOneWaitResponse;
    leaveOneVOneWaitResponse.status = static_cast<unsigned int>(stutusId::Success);

    RequestResult rr;
    rr.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user.getName());
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(leaveOneVOneWaitResponse);
    return rr;
}
