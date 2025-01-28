#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <mutex>
#include <shared_mutex>
#include <map>
#include "RequestHandlerFactory.h"
#include "./CryptoAlgorithms/ICryptoAlgorithm.h"

class Communicator
{
public:
	Communicator(RequestHandlerFactory& requestHandlerFactory);
	void startHandleRequests();

private:
	SOCKET m_serverSocket;
	std::map<SOCKET, std::unique_ptr<IRequestsHandler>> m_clients;
	std::shared_mutex m_mutex;
	RequestHandlerFactory& m_requestHandlerFactory;

	std::shared_ptr<ICryptoAlgorithm> m_cryptoAlgorithm;

	void bindAndListen();
	void handleNewClient(const SOCKET clientSoc);
};

