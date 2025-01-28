#include "JsonRequestPacketDeserializer.h"
#include "../json.hpp"
#include "../Infastructure/CryptoAlgorithms/OTPCryptoAlgorithm.h"
using json = nlohmann::json;

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char> buffer)
{
    std::pair<int, std::string> splitRequest = JsonRequestPacketDeserializer::splitRequest(buffer);

    json j = json::parse(splitRequest.second);

    LoginRequest loginRequest;
    loginRequest.passwrod = j["password"];
    loginRequest.username = j["username"];

    return loginRequest;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const std::vector<unsigned char> buffer)
{
    std::pair<int, std::string> splitRequest = JsonRequestPacketDeserializer::splitRequest(buffer);

    json j = json::parse(splitRequest.second);

    SignupRequest signupRequest;
    signupRequest.passwrod = j["password"];
    signupRequest.username = j["username"];
    signupRequest.email = j["email"];
    std::string addrStr =  j["addr"]["street"].get<std::string>() + ", " + std::to_string(j["addr"]["apt"].get<int>()) +
        ", " + j["addr"]["city"].get<std::string>();
    signupRequest.address = addrStr;
    signupRequest.birth = j["birth"];
    signupRequest.phone = j["phone"];

    return signupRequest;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const std::vector<unsigned char>& buffer)
{
    std::pair<int, std::string> splitRequest = JsonRequestPacketDeserializer::splitRequest(buffer);

    json j = json::parse(splitRequest.second);

    GetPlayersInRoomRequest playersRequest;
    playersRequest.roomId = j["roomId"];

    return playersRequest;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer)
{
    std::pair<int, std::string> splitRequest = JsonRequestPacketDeserializer::splitRequest(buffer);

    json j = json::parse(splitRequest.second);

    JoinRoomRequest joinRoomRequest;
    joinRoomRequest.roomId = j["roomId"];

    return joinRoomRequest;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer)
{
    std::pair<int, std::string> splitRequest = JsonRequestPacketDeserializer::splitRequest(buffer);

    json j = json::parse(splitRequest.second);

    CreateRoomRequest createRoomRequest;
    createRoomRequest.roomName = j["name"];
    createRoomRequest.maxUsers = j["maxUsers"];
    createRoomRequest.questionCount = j["questionCount"];
    createRoomRequest.answerTimeout = j["answerTimeout"];

    return createRoomRequest;
}

GetGameStateRequest JsonRequestPacketDeserializer::deserializeGetGameStateRequest(const std::vector<unsigned char>& buffer)
{
    std::pair<int, std::string> splitRequest = JsonRequestPacketDeserializer::splitRequest(buffer);

    json j = json::parse(splitRequest.second);

    GetGameStateRequest getGameStateRequest;
    getGameStateRequest.answerid = j["answerId"];

    return getGameStateRequest;
}

AddNewQuestionRequest JsonRequestPacketDeserializer::deserializeAddNewQuestionRequest(const std::vector<unsigned char>& buffer)
{
    std::pair<int, std::string> splitRequest = JsonRequestPacketDeserializer::splitRequest(buffer);

    json j = json::parse(splitRequest.second);

    AddNewQuestionRequest addNewQuestionRequest;
    
    addNewQuestionRequest.question = j["question"];
    addNewQuestionRequest.correctAns = j["correctAns"];
    addNewQuestionRequest.wrongAns1 = j["wrongAns1"];
    addNewQuestionRequest.wrongAns2 = j["wrongAns2"];
    addNewQuestionRequest.wrongAns3 = j["wrongAns3"];
    addNewQuestionRequest.category = j["category"];
    addNewQuestionRequest.difficulty = j["difficulty"];

    return addNewQuestionRequest;
}

std::string JsonRequestPacketDeserializer::deserializeCipher(const std::vector<unsigned char>& buffer)
{
    // get size
    unsigned int size = 0;
    for (int i = 0; i < sizeof(unsigned int); ++i) {
        size |= (static_cast<unsigned int>(buffer[i]) << (8 * (sizeof(unsigned int) - 1 - i)));
    }

    std::string cipher(buffer.begin() + sizeof(unsigned int), buffer.begin() + sizeof(unsigned int) + size);
    return cipher;
}


std::pair<int, std::string> JsonRequestPacketDeserializer::splitRequest(const std::vector<unsigned char>& buffer)
{
    std::pair<int, std::string> request;

    unsigned char id = buffer[0];
    request.first = static_cast<int>(id);

    // get size
    unsigned int size = 0;
    for (int i = 0; i < sizeof(unsigned int); ++i) {
        size |= (static_cast<unsigned int>(buffer[i + 1]) << (8 * (sizeof(unsigned int) - 1 - i)));
    }

    std::string data(buffer.begin() + 1 + sizeof(unsigned int), buffer.begin() + 1 + sizeof(unsigned int) + size);

    request.second = data;

    return request;
}
