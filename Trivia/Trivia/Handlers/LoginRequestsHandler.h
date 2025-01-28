#pragma once
#include "../Requests/IRequestsHandler.h"
#include "../Infastructure/RequestHandlerFactory.h"

class RequestHandlerFactory;

class LoginRequestsHandler : public IRequestsHandler
{
public:
	LoginRequestsHandler(RequestHandlerFactory& handlerFactory);
	bool isRequestsRelevant(const RequestInfo& requestInfo) const override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
	void handleCrush() override;

private:
	RequestHandlerFactory& m_handleFactory;

	RequestResult login(const RequestInfo& requestInfo) const;
	RequestResult signup(const RequestInfo& requestInfo) const;
};

