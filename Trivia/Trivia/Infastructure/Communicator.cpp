#include "Communicator.h"
#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include "WSAInitializer.h"
#include "../Requests/JsonRequestPacketDeserializer.h"
#include "../Response/JsonResponsePacketSerializer.h"
#include "./CryptoAlgorithms/AESCryptoAlgorithm.h"

const short PORT = 8826;


Communicator::Communicator(RequestHandlerFactory& requestHandlerFactory) : m_serverSocket(INVALID_SOCKET), m_requestHandlerFactory(requestHandlerFactory), m_cryptoAlgorithm(std::make_shared<AESCryptoAlgorithm>())
{}

void Communicator::startHandleRequests()
{
    WSAInitializer wsaInit;
    std::cout << "start Handle Requests called!" << std::endl;

    // initialize the server socket
    // this server uses TCP, that's why SOCK_STREAM & IPPROTO_TCP
    try {
        m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_serverSocket == INVALID_SOCKET)
        {
            throw std::runtime_error(__FUNCTION__ " - socket");
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << ", windows error: " << WSAGetLastError() << std::endl;
    }

    try {
        bindAndListen();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << ", windows error: " << WSAGetLastError() << std::endl;
    }
}

void Communicator::bindAndListen()
{
    const int maxConnections = SOMAXCONN;

    struct sockaddr_in sa = { 0 };

    sa.sin_port = htons(PORT);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    std::cout << "starting..." << std::endl;

    // Bind the server socket
    if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
    {
        throw std::runtime_error(__FUNCTION__ " - bind");
    }

    std::cout << "binded!" << std::endl;

    // Start listening for incoming connections
    if (listen(this->m_serverSocket, maxConnections) == SOCKET_ERROR)
    {
        throw std::runtime_error(__FUNCTION__ " - listen");
    }

    std::cout << "Listening on port " << PORT << std::endl;

    // Accept incoming connections and handle them in a loop
    while (true)
    {
        // Accept a client socket
        SOCKET clientSocket = accept(m_serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)
        {
            throw std::runtime_error(__FUNCTION__ " - accept");
        }

        // Handle the new client in a separate thread
        std::thread clientThread(&Communicator::handleNewClient, this, clientSocket);
        clientThread.detach();
        

    }
}

void Communicator::handleNewClient(const SOCKET clientSoc)
{
    std::unique_ptr<IRequestsHandler> logInHandler = this->m_requestHandlerFactory.createLoginRequestHandler();
    std::unique_lock lockAddClient(m_mutex);
    m_clients.insert({ clientSoc, std::move(logInHandler) });
    lockAddClient.unlock();

    std::vector<unsigned char> buffer(1024);
    
    // send keys
    // buffer contains client public key
    int bytesReceived = recv(clientSoc, (char*)buffer.data(), buffer.size(), 0);
    int bytesSent = send(clientSoc, this->m_cryptoAlgorithm.get()->getPublicKey().c_str(), this->m_cryptoAlgorithm.get()->getPublicKey().size(), 0);

    std::string clientsPublicKey = JsonRequestPacketDeserializer::deserializeCipher(buffer);

    std::cout << "got public key: " << clientsPublicKey << std::endl;
    try {
        do {
            bytesReceived = recv(clientSoc, (char*)buffer.data(), buffer.size(), 0);
            if (bytesReceived > 0)
            {
                // decrypt method
                std::string recovered = this->m_cryptoAlgorithm.get()->decrypt(JsonRequestPacketDeserializer::deserializeCipher(buffer));
                std::vector<unsigned char> recoveredData(1024);
                recoveredData.insert(recoveredData.begin(), recovered.begin(), recovered.end());


                RequestInfo requestInfo;
                requestInfo.buffer = recoveredData;
                requestInfo.id = static_cast<CodeId>(recoveredData[0]);
                requestInfo.recivalTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

                std::shared_lock lockRead(m_mutex);
                IRequestsHandler* requestHandler = this->m_clients.at(clientSoc).get();
                lockRead.unlock();

                RequestResult requestResult = requestHandler->handleRequest(requestInfo);

                std::unique_lock lockWrite(m_mutex);
                this->m_clients[clientSoc] = std::move(requestResult.newHandler);
                lockWrite.unlock();
                
                
                // encrypt method
                std::string data(requestResult.buffer.begin(), requestResult.buffer.end());
                std::string cipher = m_cryptoAlgorithm.get()->encrypt(data, clientsPublicKey);

                std::vector<unsigned char> cipherBytes = JsonResponsePacketSerializer::serializeCipher(cipher);
                int bytesSent = send(clientSoc, reinterpret_cast<const char*>(cipherBytes.data()), cipherBytes.size(), 0);

                if (bytesSent == SOCKET_ERROR) {

                    throw std::runtime_error(__FUNCTION__ " - send");
                }
            }
            else if (bytesReceived == 0)
            {
                break;
            }
            else
            {
                throw std::runtime_error("crush");
            }
        } while (bytesReceived > 0);

    }
    catch (std::runtime_error e) {
        std::cerr << "user exited: " << e.what() << std::endl;
    }
    catch (std::exception e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Remove the client from the map and clean up
    std::shared_lock lockRead(m_mutex);
    IRequestsHandler* requestHandler = this->m_clients.at(clientSoc).get();
    lockRead.unlock();
    requestHandler->handleCrush();

    std::unique_lock lockeRemoveUser(m_mutex);
    m_clients.erase(clientSoc);
    lockeRemoveUser.unlock();


    closesocket(clientSoc);
}