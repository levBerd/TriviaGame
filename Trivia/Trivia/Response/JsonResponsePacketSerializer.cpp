#include "JsonResponsePacketSerializer.h"
#include "../json.hpp"
#include "../Infastructure/CryptoAlgorithms/OTPCryptoAlgorithm.h"
using json = nlohmann::json;

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
    json j;
    j["message"] = errorResponse.message;
    std::string dump = j.dump();

    return combineResponse(CodeId::Error, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse& loginResponse)
{
    json j;
    j["status"] = loginResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::Login, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignupResponse& signupResponse)
{
    json j;
    j["status"] = signupResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::Signup, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& logoutResponse)
{
    json j;
    j["status"] = logoutResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::SignOut, dump);
}

json serializeRoom(const RoomData& room) {
    json roomJson;
    roomJson["id"] = room.id;
    roomJson["name"] = room.name;
    roomJson["maxPlayers"] = room.maxPlayers;
    roomJson["numOfQuestionsInGame"] = room.numOfQuestionsInGame;
    roomJson["timePerQuestion"] = room.timePerQuestion;
    roomJson["status"] = (unsigned int) room.status;
    return roomJson;
}

json SerializePlayerResault(const PlayerResult& playerResult) {
    json j;

    j["username"] = playerResult.username;
    j["correctAnswerCount"] = playerResult.correctAnswerCount;
    j["wrongAnswerCount"] = playerResult.wrongAnswerCount;
    j["averageAnswerTime"] = playerResult.averageAnswerTime;

    return j;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& getRoomsResponse)
{
    json j;
    j["status"] = getRoomsResponse.status;

    json roomsJson = json::array();
    for (const auto& room : getRoomsResponse.rooms) {
        roomsJson.push_back(serializeRoom(room)); // Use the helper function
    }

    j["rooms"] = roomsJson;

    std::string dump = j.dump();

    return combineResponse(CodeId::GetRooms, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& joinRoomResponse)
{
    json j;
    j["status"] = joinRoomResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::JoinRoom, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& createRoomResponse)
{
    json j;
    j["status"] = createRoomResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::CreateRoom, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResponse)
{
    json j;
    j["status"] = getPlayersInRoomResponse.status;
    j["players"] = getPlayersInRoomResponse.players;

    std::string dump = j.dump();

    return combineResponse(CodeId::GetPlayersInRoom, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& getHighScoreResponse)
{
    json j;
    j["status"] = getHighScoreResponse.status;
    j["statistics"] = getHighScoreResponse.scores;

    std::string dump = j.dump();

    return combineResponse(CodeId::GetHighScore, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& getPersonalStatsResponse)
{
    json j;
    j["status"] = getPersonalStatsResponse.status;
    j["statistics"] = getPersonalStatsResponse.statistics;

    std::string dump = j.dump();

    return combineResponse(CodeId::GetPersonalStats, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& closeRoomResponse)
{
    json j;
    j["status"] = closeRoomResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::CloseRoomRequest, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const StartGameResponse startGameResponse)
{
    json j;
    j["status"] = startGameResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::StartGameRequest, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& getRoomStateResponse)
{
    json j;
    j["status"] = getRoomStateResponse.status;
    j["roomState"] = getRoomStateResponse.roomState;
    j["questionCount"] = getRoomStateResponse.questionCount;
    j["answerTimeOut"] = getRoomStateResponse.answerTimeOut;
    j["players"] = getRoomStateResponse.players;

    std::string dump = j.dump();
    return combineResponse(CodeId::GetRoomStateRequest, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& leaveRoomResponse)
{
    json j;
    j["status"] = leaveRoomResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::LeaveRoom, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& leaveGameResponse)
{
    json j;
    j["status"] = leaveGameResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::LeaveGame, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GameStateResponse& getGameResponse)
{
    json j;
    
    j["status"] = getGameResponse.status;
    j["correctAnswerId"] = getGameResponse.correctAnswerId;
    j["timeLeft"] = getGameResponse.timeLeft;
    j["question"] = getGameResponse.question;

    json answers;
    for (const std::pair<int, std::string>& pair : getGameResponse.answers) {
        answers[std::to_string(pair.first)] = pair.second;
    }
    j["answers"] = answers;

    json results = json::array();
    for (const PlayerResult& playerResult : getGameResponse.results) {
        results.push_back(SerializePlayerResault(playerResult)); // Use the helper function
    }

    j["results"] = results;

    j["playerNames"] = getGameResponse.playerNames;

    std::string dump = j.dump();
    
    return combineResponse(CodeId::GetGameState, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const AddNewQuestionResponse& addNewQuestionResponse)
{
    json j;
    j["status"] = addNewQuestionResponse.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::LeaveGame, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const AddUserToOneVOneWaitResponse& addUserToOneVOneWait)
{
    json j;
    j["status"] = addUserToOneVOneWait.status;
    j["state"] = addUserToOneVOneWait.state;
    std::string dump = j.dump();

    return combineResponse(CodeId::AddUserToOneVOneWait, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveOneVOneWaitResponse& leaveOneVOneWait)
{
    json j;
    j["status"] = leaveOneVOneWait.status;
    std::string dump = j.dump();

    return combineResponse(CodeId::LeaveOneVOneWait, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetOneVOneStateResponse& getOneVOneStateResponse)
{
    json j;
    j["status"] = getOneVOneStateResponse.status;
    j["roomState"] = getOneVOneStateResponse.roomState;
    j["opponentName"] = getOneVOneStateResponse.opponnentName;
    std::string dump = j.dump();

    return combineResponse(CodeId::LeaveGame, dump);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeCipher(std::string cipher)
{
    std::vector<unsigned char> buffer;

    int len = cipher.size();
    char* numberStr = static_cast<char*>(static_cast<void*>(&len));

    buffer.push_back(numberStr[3]);
    buffer.push_back(numberStr[2]);
    buffer.push_back(numberStr[1]);
    buffer.push_back(numberStr[0]);

    // add message to buffer
    buffer.insert(buffer.end(), cipher.begin(), cipher.end());

    return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::combineResponse(const CodeId code, const std::string& message)
{
    std::vector<unsigned char> buffer;

    // add message code to start of buffer
    buffer.push_back((unsigned char)code);

    int len = message.size();
    char* numberStr = static_cast<char*>(static_cast<void*>(&len));

    buffer.push_back(numberStr[3]);
    buffer.push_back(numberStr[2]);
    buffer.push_back(numberStr[1]);
    buffer.push_back(numberStr[0]);

    // add message to buffer
    buffer.insert(buffer.end(), message.begin(), message.end());

    return buffer;
}
