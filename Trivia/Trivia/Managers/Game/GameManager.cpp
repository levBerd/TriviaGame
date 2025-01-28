#include "GameManager.h"
#include "../ManagerException.h"
#include "../../Infastructure./ServerException.h"
#include <mutex>
#include <shared_mutex>

GameManager::GameManager(std::shared_ptr<IDatabase> dataAccess) : m_IDatabase(dataAccess) {}

Game& GameManager::createGame(const Room& room)
{
	if (m_IDatabase.expired()) {
		throw ServerException("IDatabase ptr has expired! (LoginManager::signup)");
	}
	
	std::unique_lock writeLock(m_mutex);
	m_games.try_emplace(room.getRoomData().id, room.getRoomData().id, room.getAllUsers(), (*this->m_IDatabase.lock()).getQuestions(room.getRoomData().numOfQuestionsInGame), room.getRoomData().timePerQuestion);
	writeLock.unlock();
	return this->getGame(room.getRoomData().id);
}

void GameManager::deleteGame(int gameId)
{
	std::shared_lock readLock(m_mutex);
	if (!this->m_games.contains(gameId)) {
		throw ManagerException("Game not found.");
	}
	readLock.unlock();
	
	std::unique_lock writeLock(m_mutex);
	this->m_games.erase(gameId);
}

void GameManager::submitGameStatsToDB(const UserData& data, const LoggedUser& user) const
{
	if (m_IDatabase.expired()) {
		throw ServerException("IDatabase ptr has expired! (LoginManager::signup)");
	}

	(*m_IDatabase.lock()).submitGameStatistics(std::pair<LoggedUser, UserData>(user, data));
}

Game& GameManager::getGame(int gameId)
{
	std::shared_lock readLock(m_mutex);
	if (!this->m_games.contains(gameId)) {
		throw ManagerException("Game not found.");
	}

	return this->m_games.at(gameId);
}
