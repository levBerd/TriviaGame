#pragma once
#include "../../Infastructure/Databse/IDatabase.h"
#include <memory>
#include <vector>
#include <iostream>
#include "../ManagerException.h" // throw exception if something goes wrong

class StatisticsManager
{
public:
	StatisticsManager(std::shared_ptr<IDatabase> dataAccess);

	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string username);
private:
	std::weak_ptr<IDatabase> m_IDatabase;
};

