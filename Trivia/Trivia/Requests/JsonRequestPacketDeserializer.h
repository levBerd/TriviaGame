#pragma once
#include "Requests.h"
#include <string>

class JsonRequestPacketDeserializer
{
public:
	JsonRequestPacketDeserializer() = delete;
	static LoginRequest deserializeLoginRequest(const std::vector<unsigned char> buffer);
	static SignupRequest deserializeSignupRequest(const std::vector<unsigned char> buffer);

	static GetPlayersInRoomRequest deserializeGetPlayersRequest(const std::vector<unsigned char>& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer);
	
	static GetGameStateRequest deserializeGetGameStateRequest(const std::vector<unsigned char>& buffer);
	
	static AddNewQuestionRequest deserializeAddNewQuestionRequest(const std::vector<unsigned char>& buffer);

	static std::string deserializeCipher(const std::vector<unsigned char>& buffer);

private:
	static std::pair<int, std::string> splitRequest(const std::vector<unsigned char>& buffer);
};

