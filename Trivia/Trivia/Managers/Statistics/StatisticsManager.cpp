#include "StatisticsManager.h"
#include <string>

StatisticsManager::StatisticsManager(std::shared_ptr<IDatabase> dataAccess) : m_IDatabase(dataAccess)
{
}

std::vector<std::string> StatisticsManager::getHighScore()
{
    return (*m_IDatabase.lock()).getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string username)
{
    std::vector<std::string> userStats;

    auto database = m_IDatabase.lock();

    if (auto databasePtr = database) { 
        float averageAnswerTime = databasePtr->getPlayerAverageAnswerTime(username);
        int correctAnswers = databasePtr->getNumOfCorrectAnswers(username);
        int totalAnswers = databasePtr->getNumOfTotalAnswers(username);
        int playerGames = databasePtr->getNumOfPlayerGames(username);
        int playerScore = databasePtr->getPlayerScore(username);

        userStats.reserve(5);

        userStats.emplace_back("Average answer time: " + std::to_string(averageAnswerTime));
        userStats.emplace_back("Correct answers: " + std::to_string(correctAnswers));
        userStats.emplace_back("Total answers: " + std::to_string(totalAnswers));
        userStats.emplace_back("Player games: " + std::to_string(playerGames));
        userStats.emplace_back("Player score: " + std::to_string(playerScore));
    }
    else {
        throw ManagerException("weak pointer has expired");
    }

    return userStats;
}