#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../Managers/Room/Room.h"
#include <map>


struct ErrorResponse {
	std::string message;
};

struct LoginResponse {
	unsigned int status;
};

struct SignupResponse {
	unsigned int status;
};

struct LogoutResponse {
	unsigned int status;
};

struct GetRoomsResponse {
	unsigned int status;
	std::vector <RoomData> rooms;
};

struct JoinRoomResponse {
	unsigned int status;
};

struct CreateRoomResponse {
	unsigned int status;
};

struct GetPlayersInRoomResponse {
	unsigned int status;
	std::vector<std::string> players;
};

struct GetHighScoreResponse {
	unsigned int status;
	std::vector<std::string> scores;
};

struct GetPersonalStatsResponse {
	unsigned int status;
	std::vector<std::string> statistics;
};

struct CloseRoomResponse {
	unsigned int status;
};

struct StartGameResponse {
	unsigned int status;
};

struct GetRoomStateResponse {
	unsigned int status;
	unsigned int roomState;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeOut;
};

struct LeaveRoomResponse {
	unsigned int status;
};

struct LeaveGameResponse {
	unsigned int status;
};

struct PlayerResult {
	std::string username;
	int correctAnswerCount;
	int wrongAnswerCount;
	float averageAnswerTime;
};

struct GameStateResponse {
	//0 - waiting for game to start
	//1 - sending answers
	//2 - questions in progress, sending question, answers, time
	//3 - game ended - send resaults
	unsigned int status;
	unsigned int correctAnswerId;
	
	unsigned int timeLeft;
	std::string question;
	std::map<int, std::string> answers; // {answer id, answer string}

	std::vector<PlayerResult> results;

	std::vector<std::string> playerNames;
};

struct AddNewQuestionResponse {
	unsigned int status;
};

struct AddUserToOneVOneWaitResponse {
	unsigned int status;
	unsigned int state;
};

struct LeaveOneVOneWaitResponse {
	unsigned int status;
};

struct GetOneVOneStateResponse {
	unsigned int status;
	unsigned int roomState;
	std::string opponnentName;
};
