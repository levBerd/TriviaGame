#pragma once

#include "../Codes.h"
#include "Responses.h"
#include <string>

class JsonResponsePacketSerializer
{
public:
	JsonResponsePacketSerializer() = delete;

	// buffer is --> std::vector<unsigned char>
	static std::vector<unsigned char> serializeResponse(const ErrorResponse& errorResponse);
	static std::vector<unsigned char> serializeResponse(const LoginResponse& loginResponse);
	static std::vector<unsigned char> serializeResponse(const SignupResponse& signupResponse);

	static std::vector<unsigned char> serializeResponse(const LogoutResponse& logoutResponse);
	static std::vector<unsigned char> serializeResponse(const GetRoomsResponse& getRoomsResponse);
	static std::vector<unsigned char> serializeResponse(const JoinRoomResponse& joinRoomResponse);
	static std::vector<unsigned char> serializeResponse(const CreateRoomResponse& createRoomResponse);
	static std::vector<unsigned char> serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResponse);
	static std::vector<unsigned char> serializeResponse(const GetHighScoreResponse& getHighScoreResponse);
	static std::vector<unsigned char> serializeResponse(const GetPersonalStatsResponse& getPersonalStatsResponse);

	static std::vector<unsigned char> serializeResponse(const CloseRoomResponse& closeRoomResponse);
	static std::vector<unsigned char> serializeResponse(const StartGameResponse startGameResponse);
	static std::vector<unsigned char> serializeResponse(const GetRoomStateResponse& getRoomStateResponse);
	static std::vector<unsigned char> serializeResponse(const LeaveRoomResponse& leaveRoomResponse);

	static std::vector<unsigned char> serializeResponse(const LeaveGameResponse& leaveGameResponse);
	static std::vector<unsigned char> serializeResponse(const GameStateResponse& getGameResponse);

	static std::vector<unsigned char> serializeResponse(const AddNewQuestionResponse& addNewQuestionResponse);

	static std::vector<unsigned char> serializeResponse(const AddUserToOneVOneWaitResponse& addUserToOneVOneWait);
	static std::vector<unsigned char> serializeResponse(const LeaveOneVOneWaitResponse& leaveOneVOneWait);
	static std::vector<unsigned char> serializeResponse(const GetOneVOneStateResponse& getOneVOneStateResponse);

	static std::vector<unsigned char> serializeCipher(std::string cipher);
private:
	static std::vector<unsigned char> combineResponse(const CodeId code, const std::string& message);
	
};

