#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include "io.h"
#include "../../Managers/ManagerException.h" 
#include "../../Managers/Question.h"
#include <mutex>
#include <shared_mutex>


class SqliteDatabase : public IDatabase
{
public:
	bool open() override;
	bool close() override;

	bool doesUserExists(const std::string& username) override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) override;
	bool addNewUser(const std::string& username, const std::string& password, const std::string& email, const std::string& addr, const std::string& birth, const std::string& phone) override;

	std::vector<Question> getQuestions(const int n) override;
	float getPlayerAverageAnswerTime(const std::string& playerName) override;
	int getNumOfCorrectAnswers(const std::string& playerName) override;
	int getNumOfTotalAnswers(const std::string& playerName) override;
	int getNumOfPlayerGames(const std::string& playerName) override;
	int getPlayerScore(const std::string& playerName) override;
	std::vector<std::string> getHighScores() override;
	int submitGameStatistics(const std::pair<LoggedUser, UserData>& gameData) override;

	void addNewQuestion(std::string question, std::string difficulty, std::string category,
		std::string correctAns, std::string wrongAns1, std::string wrongAns2, std::string wrongAns3) override;
private:
	sqlite3* db;
	std::shared_mutex m_mutex;
	std::string dbFileName = "TriviaDB.sqlite";

	std::vector<Answer> getAnswersOfQuestionId(const int id);
	std::vector<std::string> getPlayers();
};

