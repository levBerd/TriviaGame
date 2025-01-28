#pragma once
#include "../../Requests/IRequestsHandler.h"
#include "../../Managers/Room/Room.h"
#include "../../Managers/Login/LoggedUser.h"
#include "../../Managers/Room/RoomManager.h"
#include "../../Infastructure/RequestHandlerFactory.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestsHandler
{
public:
	RoomAdminRequestHandler(const int roomid, const LoggedUser loggedUser, RoomManager& roomManageer, RequestHandlerFactory& handlerFactory);
	bool isRequestsRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
	void handleCrush() override;

private:
	int m_roomid;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult closeRoom(const RequestInfo& requestInfo);
	RequestResult startGame(const RequestInfo& requestInfo);
	RequestResult getRoomState(const RequestInfo& requestInfo) const;
};

