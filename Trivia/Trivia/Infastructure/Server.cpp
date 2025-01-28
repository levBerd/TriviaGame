#include "Server.h"
#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <utility>
#include "./Databse/SqliteDatabase.h"

Server::Server() : m_communicator(this->m_handlerFactory), m_database(std::make_shared<SqliteDatabase>()), m_handlerFactory(this->m_database)
{
	this->m_database.get()->open();
}

void Server::run()	
{
	
	std::jthread communicatorThread(&Server::openCommunicatorThread, this);
	communicatorThread.detach();

	this->openAdminInteface();
}

void Server::openCommunicatorThread()
{
	this->m_communicator.startHandleRequests();
}

void Server::openAdminInteface() const
{
	std::string command = "";

	std::this_thread::sleep_for(std::chrono::milliseconds(150));

	std::cout << "server is running" << std::endl;
	std::cout << "type EXIT to close server." << std::endl;
	std::cout << "type 1 to add questions in the DB" << std::endl;

	while (command != "EXIT") {
		std::cout << "Enter Command: " << std::endl;
		std::getline(std::cin, command);

		if (command == "1") {
			
			this->addQuestionsToDb();
		}
		else if (command == "EXIT") {
			break;
		}
		else {
			std::cout << "Not a command" << std::endl;
		}
	}
}

void Server::addQuestionsToDb() const
{
	std::string message("\nPlease enter question ammount: ");
	std::string numericStr("0123456789");

	std::cout << message << std::endl;
	std::string input;
	std::getline(std::cin, input);

	while (std::cin.fail() || std::cin.eof() || input.find_first_not_of(numericStr) != std::string::npos) {

		std::cout << "Please enter a number only!" << std::endl;

		if (input.find_first_not_of(numericStr) == std::string::npos) {
			std::cin.clear();
		}

		std::cout << std::endl << message << std::endl;
		std::getline(std::cin, input);
	}
	
	std::string command = std::string("python AddQuestions.py ") + input;

	system(static_cast<const char*>(command.c_str()));
}
