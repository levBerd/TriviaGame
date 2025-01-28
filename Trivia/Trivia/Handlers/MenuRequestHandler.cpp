#include "MenuRequestHandler.h"
#include "../Codes.h"
#include "../Requests/JsonRequestPacketDeserializer.h"
#include "../Response/JsonResponsePacketSerializer.h"
#include "../Managers/ManagerException.h"
#include "../Infastructure/RequestHandlerFactory.h"
#include "../Infastructure/ServerException.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory, std::string loggedUser, std::weak_ptr<IDatabase> database) : m_handleFactory(handlerFactory)
{
    this->currentlyLoggedUser = loggedUser;
    this->m_database = database;
}

bool MenuRequestHandler::isRequestsRelevant(const RequestInfo& requestInfo) const
{
    CodeId id = requestInfo.id;

    if (id == CodeId::SignOut || id == CodeId::GetRooms || id == CodeId::JoinRoom || id == CodeId::CreateRoom 
        || id == CodeId::GetPlayersInRoom || id == CodeId::GetPersonalStats) {
        return true;
    }

    return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    try {
        if (requestInfo.id == CodeId::SignOut) {
            return this->logout(requestInfo);
        }
        else if (requestInfo.id == CodeId::GetRooms) {
            return this->GetRooms(requestInfo);
        }
        else if (requestInfo.id == CodeId::JoinRoom) {
            return this->JoinRoom(requestInfo);
        }
        else if (requestInfo.id == CodeId::CreateRoom) {
            return this->CreateRoom(requestInfo);
        }
        else if (requestInfo.id == CodeId::GetPlayersInRoom) {
            return this->GetPlayersInRoom(requestInfo);
        }
        else if (requestInfo.id == CodeId::GetHighScore) {
            return this->getHighScore(requestInfo);
        }
        else if (requestInfo.id == CodeId::GetPersonalStats) {
            return this->GetPersonalStats(requestInfo);
        }
        else if (requestInfo.id == CodeId::AddNewQuestion) {
            return this->AddNewQuestion(requestInfo);
        }
        else if (requestInfo.id == CodeId::AddUserToOneVOneWait) {
            return this->addUserToOneVOneWait(requestInfo);
        }
        else if (requestInfo.id == CodeId::Amount) {
            return this->printUserAmount(requestInfo);
        }
        throw ManagerException("got to MenuRequestHandler with irrelevant requestInfo");

    }
    catch (ManagerException& e) {
        ErrorResponse Er;
        Er.message = e.what();
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handleFactory.createMenuRequestHandler(this->currentlyLoggedUser);
        return rr;
    }
    catch (ServerException& e) {
        std::cout << "Server Error!!!" << std::endl;
        ErrorResponse Er;
        Er.message = "Server had an error: " + std::string(e.what());
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handleFactory.createMenuRequestHandler(this->currentlyLoggedUser);
        return rr;
    }
}

void MenuRequestHandler::handleCrush()
{
    this->m_handleFactory.getLoginManager().logout(this->currentlyLoggedUser);
}

RequestResult MenuRequestHandler::logout(const RequestInfo& requestInfo) const
{
    m_handleFactory.getLoginManager().logout(currentlyLoggedUser);
    LogoutResponse logoutResponse;
    logoutResponse.status = static_cast<unsigned int>(stutusId::Success);
    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(logoutResponse);
    rr.newHandler = this->m_handleFactory.createMenuRequestHandler(this->currentlyLoggedUser);
    return rr;
}

RequestResult MenuRequestHandler::GetRooms(const RequestInfo& requestInfo) const
{
    std::vector<Room> rooms = m_handleFactory.getRoomManager().getRooms();
    GetRoomsResponse getRoomsResponse;
    getRoomsResponse.status = static_cast<unsigned int>(stutusId::Success);

    std::vector<RoomData> roomsData;
    for (auto& room : rooms) {
        roomsData.push_back(room.getRoomData());
    }
    getRoomsResponse.rooms = roomsData;

    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(getRoomsResponse);
    rr.newHandler = this->m_handleFactory.createMenuRequestHandler(this->currentlyLoggedUser);
    return rr;
}

RequestResult MenuRequestHandler::JoinRoom(const RequestInfo& requestInfo) const
{
    JoinRoomRequest joinRoomRequest = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.buffer);

    if (m_handleFactory.getRoomManager().getRoomState(joinRoomRequest.roomId) == RoomState::INACTIVE) {
        throw ManagerException("Room is inactive.");
    }

    if (m_handleFactory.getRoomManager().getRoomState(joinRoomRequest.roomId) == RoomState::GAME_STARTED) {
        throw ManagerException("Game has started.");
    }

    if (m_handleFactory.getRoomManager().getRoom(joinRoomRequest.roomId).getRoomData().maxPlayers == m_handleFactory.getRoomManager().getRoom(joinRoomRequest.roomId).getAllUsers().size()) {
        throw ManagerException("Room is Full");
    }
    
    m_handleFactory.getRoomManager().addUserToRoom(joinRoomRequest.roomId, this->currentlyLoggedUser);

    JoinRoomResponse joinRoomResponse;
    joinRoomResponse.status = static_cast<unsigned int>(stutusId::Success);

    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(joinRoomResponse);
    rr.newHandler = this->m_handleFactory.createRoomMemberRequestHandler(joinRoomRequest.roomId, this->currentlyLoggedUser);
    return rr;
}

RequestResult MenuRequestHandler::CreateRoom(const RequestInfo& requestInfo) const
{
    CreateRoomRequest createRoomRequest = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.buffer);
    RoomData newRoom;
    newRoom.maxPlayers = createRoomRequest.maxUsers;
    newRoom.name = createRoomRequest.roomName;
    newRoom.numOfQuestionsInGame = createRoomRequest.questionCount;
    newRoom.timePerQuestion = createRoomRequest.answerTimeout;
    newRoom.status = RoomState::ACTIVE;
    newRoom.id = m_handleFactory.getRoomManager().getEmptyRoomId();

    m_handleFactory.getRoomManager().createRoom(this->currentlyLoggedUser, newRoom);

    CreateRoomResponse createRoomResponse;
    createRoomResponse.status = static_cast<unsigned int>(stutusId::Success);

    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(createRoomResponse);
    rr.newHandler = this->m_handleFactory.createRoomAdminRequestHandler(newRoom.id, this->currentlyLoggedUser);
    return rr;
}

RequestResult MenuRequestHandler::GetPlayersInRoom(const RequestInfo& requestInfo) const
{
    GetPlayersInRoomRequest getPlayersRequest = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(requestInfo.buffer);
    Room room = m_handleFactory.getRoomManager().getRoom(getPlayersRequest.roomId);

    std::vector<std::string> players = room.getAllUsers();

    GetPlayersInRoomResponse getPlayersInRoomResponse;
    getPlayersInRoomResponse.status = static_cast<unsigned int>(stutusId::Success);
    getPlayersInRoomResponse.players = players;

    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(getPlayersInRoomResponse);
    rr.newHandler = this->m_handleFactory.createMenuRequestHandler(this->currentlyLoggedUser);
    return rr;
}

RequestResult MenuRequestHandler::getHighScore(const RequestInfo& requestInfo) const
{
    std::vector<std::string> scores = m_handleFactory.getStatisticsManager().getHighScore();
    GetHighScoreResponse getHighScoreResponse;
    getHighScoreResponse.status = static_cast<unsigned int>(stutusId::Success);
    getHighScoreResponse.scores = scores;

    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(getHighScoreResponse);
    rr.newHandler = this->m_handleFactory.createMenuRequestHandler(this->currentlyLoggedUser);
    return rr;
}

RequestResult MenuRequestHandler::GetPersonalStats(const RequestInfo& requestInfo) const
{
    std::vector<std::string> scores = m_handleFactory.getStatisticsManager().getUserStatistics(this->currentlyLoggedUser);
    GetPersonalStatsResponse getPersonalStatsResponse;
    getPersonalStatsResponse.status = static_cast<unsigned int>(stutusId::Success);
    getPersonalStatsResponse.statistics = scores;

    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(getPersonalStatsResponse);
    rr.newHandler = this->m_handleFactory.createMenuRequestHandler(this->currentlyLoggedUser);
    return rr;
}

RequestResult MenuRequestHandler::AddNewQuestion(const RequestInfo& requestInfo) const
{
    AddNewQuestionRequest addNewQuestionRequest = JsonRequestPacketDeserializer::deserializeAddNewQuestionRequest(requestInfo.buffer);

    this->m_database.lock()->addNewQuestion(addNewQuestionRequest.question, addNewQuestionRequest.difficulty,
        addNewQuestionRequest.category, addNewQuestionRequest.correctAns, addNewQuestionRequest.wrongAns1,
        addNewQuestionRequest.wrongAns2, addNewQuestionRequest.wrongAns3);

    AddNewQuestionResponse addNewQuestionResponse;
    addNewQuestionResponse.status = 1;

    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(addNewQuestionResponse);
    rr.newHandler = this->m_handleFactory.createMenuRequestHandler(this->currentlyLoggedUser);
    return rr;
}

RequestResult MenuRequestHandler::addUserToOneVOneWait(const RequestInfo& requestInfo) const
{
    RequestResult rr;
    AddUserToOneVOneWaitResponse addUserToOneVOneWaitResponse;

    std::string opponent = this->m_handleFactory.getOneVOneManager().getNameOfWaitingUser();
    if (opponent != "") {
        // There is someone waiting
        RoomData newRoom;
        newRoom.maxPlayers = 2;
        newRoom.name = "OneVOne" + m_handleFactory.getRoomManager().getEmptyRoomId();
        newRoom.numOfQuestionsInGame = 3;
        newRoom.timePerQuestion = 10; // one v one settings
        newRoom.status = RoomState::ACTIVE;
        newRoom.id = m_handleFactory.getRoomManager().getEmptyRoomId();

        m_handleFactory.getRoomManager().createRoom(LoggedUser(this->currentlyLoggedUser), newRoom); // create room
        m_handleFactory.getRoomManager().addUserToRoom(newRoom.id, opponent); // add opponent

        this->m_handleFactory.getGameManager().createGame(m_handleFactory.getRoomManager().getRoom(newRoom.id)); // create game

        this->m_handleFactory.getOneVOneManager().startGameWithWaitingUser(newRoom.id);
        rr.newHandler = this->m_handleFactory.createGameRequestHandler(this->m_handleFactory.getGameManager().getGame(newRoom.id), currentlyLoggedUser);
        addUserToOneVOneWaitResponse.state = (unsigned int)(OveVOneRoomState::GAME_STARTED);
    }
    else {
        // Waiting room empty
        this->m_handleFactory.getOneVOneManager().addUserToWaitingRoom(LoggedUser(this->currentlyLoggedUser));
        rr.newHandler = this->m_handleFactory.createOneVOneRoomRequestHandler(LoggedUser(this->currentlyLoggedUser));
        addUserToOneVOneWaitResponse.state = (unsigned int)(OveVOneRoomState::LOOKING_FOR_OPPONENT);
    }
    
    addUserToOneVOneWaitResponse.status = static_cast<unsigned int>(stutusId::Success);
    
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(addUserToOneVOneWaitResponse);
    return rr;
}

RequestResult MenuRequestHandler::printUserAmount(const RequestInfo& requestInfo) const
{
    this->m_handleFactory.getLoginManager().printUserAmount();

    return RequestResult();
}
