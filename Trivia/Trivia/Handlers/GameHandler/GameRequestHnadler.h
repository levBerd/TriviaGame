#pragma once
#include "../../Requests/IRequestsHandler.h"
#include "../../Infastructure/RequestHandlerFactory.h"
#include "../../Managers/Game/Game.h"
#include "../../Managers/Game/GameManager.h"


class GameRequestHnadler : public IRequestsHandler
{
public:
	GameRequestHnadler(Game& game, LoggedUser loggedUser, GameManager& gameManageer, RequestHandlerFactory& handlerFactory);
	bool isRequestsRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
	void handleCrush() override;

	RequestResult leaveGame(const RequestInfo& requestInfo);
	RequestResult getGameState(const RequestInfo& requestInfo);

private:
	Game& m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;
};

