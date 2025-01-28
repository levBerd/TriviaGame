#pragma once
#include <string>
#include <vector>
#include <list>
#include "../../Managers/Question.h"
#include <queue>
#include "../../Managers/Game/Game.h"

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	
	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual bool doesUserExists(const std::string& username) = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual bool addNewUser(const std::string& username, const std::string& password, const std::string& email, const std::string& addr, const std::string& birth, const std::string& phone) = 0;

	virtual std::vector<Question> getQuestions(const int n) = 0;
	virtual float getPlayerAverageAnswerTime(const std::string& playerName) = 0;
	virtual int getNumOfCorrectAnswers(const std::string& playerName) = 0;
	virtual int getNumOfTotalAnswers(const std::string& playerName) = 0;
	virtual int getNumOfPlayerGames(const std::string& playerName) = 0;
	virtual int getPlayerScore(const std::string& playerName) = 0;
	virtual int submitGameStatistics(const std::pair<LoggedUser, UserData>& gameData) = 0;
	virtual std::vector<std::string> getHighScores() = 0;

	virtual void addNewQuestion(std::string question, std::string difficulty, std::string category,
		std::string correctAns, std::string wrongAns1, std::string wrongAns2, std::string wrongAns3) = 0;
};

