#pragma once
#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "./Databse/IDatabase.h"
#include <memory>

class Server
{
public:
	Server();
	void run();
	
private:
	Communicator m_communicator;
	std::shared_ptr<IDatabase> m_database;
	RequestHandlerFactory m_handlerFactory;

	void openCommunicatorThread();
	void openAdminInteface() const;
	void addQuestionsToDb() const;
};

