#pragma once
#include "../Question.h"
#include "../Login/LoggedUser.h"
#include <vector>
#include <map>
#include <queue>
#include <chrono>
#include "../../Response/Responses.h"
#include <mutex>
#include <shared_mutex>

struct UserData {
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int totalAnswerTime;

	// Default constructor
	UserData() : correctAnswerCount(0), wrongAnswerCount(0), totalAnswerTime(0) {}
};


enum class GameState {
	GameStarting = 1,
	WaitingForAnswers,
	WaitingForNextQuestion,
	GameEnded
};

class Game
{
public:
	Game(int gameId, const std::vector<std::string> usersInRoom, const std::vector<Question> gameQuestions, int timePerQuestion);
	

	Question getCurrentQuestion() const;
	void removePlayer(std::string user);
	int getGameId() const;
	std::map<std::string, UserData> getPlayersGameData() const;
	bool isGameEmpty();
	UserData getUserDataOfUser(const LoggedUser& user);

	GameStateResponse getGameStateResponse(const int answerid, const std::string& user); // al the game's logic

	bool isTimerExpired(const std::chrono::seconds duration) const; // check if the timer has ended

private:

	std::vector<Question> m_questions;
	std::map<std::string, UserData> m_players;
	int m_gameId;
	int m_totalQuestions;
	GameState m_state;
	std::shared_mutex m_updateMutex;
	std::shared_mutex m_userMutex;

	// timer:
	std::chrono::steady_clock::time_point m_timerStart; // start time of the timer
	std::chrono::seconds m_timerDuration; // Duration of the timer

	const std::chrono::seconds m_waitingForQuestionDuration = std::chrono::seconds(2);
	std::chrono::seconds m_gameStartingDuration;
	std::chrono::seconds timeLeft() const; // getter for how much time left for the timer 
	std::chrono::seconds elapsedTime() const; // getter for how much time elapsed
	std::chrono::seconds timeDurationWait() const;

	void updateGame();
	void submitAnswer(int answerId, std::string user);
	bool didUserAnswer(const std::string& user);
	std::vector<PlayerResult> getResults();
	std::vector<std::string> getPlayerOpponentNames(const std::string& user);
};

