#pragma once
#include "../Requests/IRequestsHandler.h"
#include "../Infastructure/RequestHandlerFactory.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestsHandler
{
public:
	MenuRequestHandler(RequestHandlerFactory& handlerFactory, std::string loggedUser, std::weak_ptr<IDatabase> database);
	bool isRequestsRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
	void handleCrush() override;

private:
	RequestHandlerFactory& m_handleFactory;
	std::string currentlyLoggedUser;
	std::weak_ptr<IDatabase> m_database;

	RequestResult logout(const RequestInfo& requestInfo) const;
	RequestResult GetRooms(const RequestInfo& requestInfo) const;
	RequestResult JoinRoom(const RequestInfo& requestInfo) const;
	RequestResult CreateRoom(const RequestInfo& requestInfo) const;
	RequestResult GetPlayersInRoom(const RequestInfo& requestInfo) const;
	RequestResult getHighScore(const RequestInfo& requestInfo) const;
	RequestResult GetPersonalStats(const RequestInfo& requestInfo) const;
	RequestResult AddNewQuestion(const RequestInfo& requestInfo) const;
	RequestResult addUserToOneVOneWait(const RequestInfo& requestInfo) const;
	RequestResult printUserAmount(const RequestInfo& requestInfo) const;
};

