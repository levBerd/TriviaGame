#include "Game.h"
#include "../ManagerException.h"

bool Game::isTimerExpired(const std::chrono::seconds duration) const
{
    auto now = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - m_timerStart);
    return elapsedTime >= duration;
}

std::chrono::seconds Game::timeLeft() const
{
    auto now = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - m_timerStart);
    return std::max(std::chrono::seconds(0), this->timeDurationWait() - elapsedTime);
}

std::chrono::seconds Game::elapsedTime() const
{
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - m_timerStart);
}

std::chrono::seconds Game::timeDurationWait() const
{
    return this->m_state == GameState::WaitingForAnswers ? this->m_timerDuration : this->m_waitingForQuestionDuration;
}

Game::Game(int gameId, const std::vector<std::string> usersInRoom, const std::vector<Question> gameQuestions, int timePerQuestion)
    : m_gameId(gameId), m_questions(gameQuestions), m_totalQuestions(gameQuestions.size()), m_state(GameState::GameStarting), m_gameStartingDuration(std::chrono::seconds(2))
{
    m_timerDuration = std::chrono::seconds(timePerQuestion);
    for (const auto& user : usersInRoom) {
        UserData userData;
        m_players[user] = userData;
    }

    this->m_timerStart = std::chrono::steady_clock::now();
}

Question Game::getCurrentQuestion() const
{
    return this->m_questions.front();
}

void Game::updateGame()
{
    if (!isTimerExpired(this->timeDurationWait())) {
        return;
    }

    // time has passed
    if (this->m_state == GameState::WaitingForAnswers) {
        // submit all players that did not answer
        for (std::pair<std::string, UserData> playerPair : this->m_players) {
            if (!didUserAnswer(playerPair.first)) {
                this->submitAnswer(-2, playerPair.first);
            }
        }

        this->m_state = GameState::WaitingForNextQuestion;
    }
    else if (this->m_state == GameState::GameEnded) {
        // game ended, no need to do anything
    }
    else {
        if (this->m_state == GameState::WaitingForNextQuestion) {
            this->m_questions.erase(this->m_questions.begin());

            if (this->m_questions.empty()) {
                this->m_state = GameState::GameEnded;
            }
            else {
                this->m_state = GameState::WaitingForAnswers;
            }
        }
        else {
            this->m_state = GameState::WaitingForAnswers;
        }
    }

    this->m_timerStart = std::chrono::steady_clock::now();
}

void Game::submitAnswer(int answerId, std::string user)
{
    if (answerId == -1) { // no answer
        return;
    }
    if (didUserAnswer(user)) { // user has already answerd
        return;
    }

    if (this->m_questions.front().getCorrectAnswerId() == answerId) {
        this->m_players[user].correctAnswerCount++;
    }
    else {
        this->m_players[user].wrongAnswerCount++;
    }

    this->m_players[user].totalAnswerTime +=  elapsedTime().count();
}

bool Game::didUserAnswer(const std::string& user)
{
    std::shared_lock readLock(m_userMutex);
    return (this->m_players.at(user).correctAnswerCount + this->m_players.at(user).wrongAnswerCount == this->m_totalQuestions - this->m_questions.size() + 1); // checks if players answer amount, equals to the questions showed amount
}

std::vector<PlayerResult> Game::getResults()
{
    std::vector<PlayerResult> results;

    std::shared_lock radLock(m_userMutex);
    for (const std::pair<std::string, UserData>& resultPair : this->m_players) {
        PlayerResult pr;
        pr.username = resultPair.first;
        pr.averageAnswerTime = resultPair.second.totalAnswerTime / this->m_totalQuestions;
        pr.correctAnswerCount = resultPair.second.correctAnswerCount;
        pr.wrongAnswerCount = resultPair.second.wrongAnswerCount;

        results.push_back(pr);
    }
    radLock.unlock();

    return results;
}

std::vector<std::string> Game::getPlayerOpponentNames(const std::string& user)
{
    std::vector<std::string> names;

    for (auto player : this->m_players) {
        if (player.first != user) {
            names.push_back(player.first);
        }
    }

    return names;
}

void Game::removePlayer(std::string user)
{
    std::unique_lock writeLock(m_userMutex);
    m_players.erase(user);
}

int Game::getGameId() const
{
    return this->m_gameId;
}

std::map<std::string, UserData> Game::getPlayersGameData() const
{
    return this->m_players;
}

bool Game::isGameEmpty()
{
    std::shared_lock readLock(m_userMutex);
    return this->m_players.empty();
}

UserData Game::getUserDataOfUser(const LoggedUser& user)
{
    std::shared_lock readLock(m_userMutex);
    if (!this->m_players.contains(user.getName())) {
        throw ManagerException("user not found");
    }

    return this->m_players.at(user.getName());
}

GameStateResponse Game::getGameStateResponse(const int answerid, const std::string& user)
{
    if (this->m_state == GameState::WaitingForAnswers) {
        submitAnswer(answerid, user);
    }
    
    std::unique_lock updateLock(m_updateMutex);
    this->updateGame();
    updateLock.unlock();

    GameStateResponse gameStateResponse;
    gameStateResponse.status = (unsigned int)this->m_state;
    
    switch (this->m_state) {
    case GameState::GameStarting:
        gameStateResponse.playerNames = this->getPlayerOpponentNames(user);
        break;
    case GameState::WaitingForAnswers:
        gameStateResponse.question = this->getCurrentQuestion().getQuestion();
        gameStateResponse.answers = this->getCurrentQuestion().getAnswersMap();
        break;
    case GameState::WaitingForNextQuestion:
        gameStateResponse.correctAnswerId = this->getCurrentQuestion().getCorrectAnswerId();
        break;
    case GameState::GameEnded:
        gameStateResponse.results = this->getResults();
        break;
    }

    gameStateResponse.timeLeft = this->timeLeft().count();

    return gameStateResponse;
}
