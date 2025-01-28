#pragma once
#include "../../Infastructure/Databse/IDatabase.h"
#include <memory>
#include <vector>
#include <iostream>
#include "LoggedUser.h"
#include "../ManagerException.h" // throw exception if something goes wrong
#include <mutex>
#include <shared_mutex>

class LoginManager
{
public:
	LoginManager(std::shared_ptr<IDatabase> dataAccess);

	void printUserAmount();
	void signup(const std::string& username, const std::string& password, const std::string& email, const std::string& addr, const std::string& birth, const std::string& phone);
	void login(const std::string& username, const std::string& password);
	void logout(const std::string& username);

private:
	std::weak_ptr<IDatabase> m_IDatabase;
	std::vector<LoggedUser> m_loggedUsers;
	std::shared_mutex m_mutex;
};
