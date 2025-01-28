#pragma once
#include <string>
#include <vector>

struct LoginRequest {
	std::string username;
	std::string passwrod;
};

struct SignupRequest {
	std::string username;
	std::string passwrod;
	std::string email;
	std::string address;
	std::string birth;
	std::string phone;
};

struct GetPlayersInRoomRequest {
	unsigned int roomId;
};

struct JoinRoomRequest {
	unsigned int roomId;
};

struct CreateRoomRequest {
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct GetGameStateRequest {
	unsigned int answerid;
};

struct AddNewQuestionRequest {
	std::string question;
	std::string correctAns;
	std::string wrongAns1;
	std::string wrongAns2;
	std::string wrongAns3;
	std::string difficulty;
	std::string category;
};