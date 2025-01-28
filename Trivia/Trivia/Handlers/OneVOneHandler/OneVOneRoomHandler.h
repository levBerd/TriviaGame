#pragma once
#include "../../Requests/IRequestsHandler.h"
#include "../../Managers/Login/LoggedUser.h"
#include "../../Managers/OneVOne/OneVOneRoomManager.h"
#include "../../Infastructure/RequestHandlerFactory.h"

class RequestHandlerFactory;

class OneVOneRoomHandler : public IRequestsHandler
{
public:
	OneVOneRoomHandler(const LoggedUser& loggedUser, OneVOneRoomManager& oneVOneRoomManager, RequestHandlerFactory& handlerFactory);
	bool isRequestsRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
	void handleCrush() override;

private:
	LoggedUser m_user;
	OneVOneRoomManager& m_oneVOneRoomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult getOneVOneRoomState(const RequestInfo& requestInfo) const;
	RequestResult leaveOneVOneRoom(const RequestInfo& requestInfo) const;
};

