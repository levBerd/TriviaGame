#pragma once
#include "Game.h"
#include "../Room/Room.h"
#include "../../Infastructure/Databse/IDatabase.h"
#include <mutex>
#include <shared_mutex>


class GameManager
{
public:
	GameManager(std::shared_ptr<IDatabase> dataAccess);

	Game& createGame(const Room& room);
	void deleteGame(int gameId);

	void submitGameStatsToDB(const UserData& data, const LoggedUser& userName) const;
	Game& getGame(int gameId);
private:
	std::weak_ptr<IDatabase> m_IDatabase;
	std::map<int, Game> m_games;
	std::shared_mutex m_mutex;
};