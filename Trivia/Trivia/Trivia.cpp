#pragma comment (lib, "ws2_32.lib")
#include "./Infastructure/Server.h"
#include "./Infastructure/WSAInitializer.h"
#include <iostream>
#include <exception>

int main(){

	try
	{
		WSAInitializer wsaInit;
		Server server;
		server.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}
	system("PAUSE");

	return 0;
}