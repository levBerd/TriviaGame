#include "LoginRequestsHandler.h"
#include "../Codes.h"
#include "../Requests/JsonRequestPacketDeserializer.h"
#include "../Response/JsonResponsePacketSerializer.h"
#include "../Managers/ManagerException.h"
#include "../Infastructure/RequestHandlerFactory.h"
#include "../Infastructure/ServerException.h"

LoginRequestsHandler::LoginRequestsHandler(RequestHandlerFactory& handlerFactory) : m_handleFactory(handlerFactory)
{}

bool LoginRequestsHandler::isRequestsRelevant(const RequestInfo& requestInfo) const
{
    return requestInfo.id == CodeId::Login || requestInfo.id == CodeId::Signup;
}

RequestResult LoginRequestsHandler::handleRequest(const RequestInfo& requestInfo)
{
    try {
        if (requestInfo.id == CodeId::Login) {
            return this->login(requestInfo);
        }

        if (requestInfo.id == CodeId::Signup) {
            return this->signup(requestInfo);
        }
        throw ManagerException("got to loginRequestsHandler with irrelevant requestInfo");

    }
    catch (ManagerException& e) {
        ErrorResponse Er;
        Er.message = e.what();
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handleFactory.createLoginRequestHandler();
        return rr;
    }
    catch (ServerException& e) {
        std::cout << "Server Error!!!" << std::endl;
        ErrorResponse Er;
        Er.message = "Server had an error: " + std::string(e.what());
        RequestResult rr;
        rr.buffer = JsonResponsePacketSerializer::serializeResponse(Er);
        rr.newHandler = this->m_handleFactory.createLoginRequestHandler();
        return rr;
    }
}

void LoginRequestsHandler::handleCrush()
{
    std::cout << "user exited without logging in" << std::endl;
}

RequestResult LoginRequestsHandler::login(const RequestInfo& requestInfo) const
{
    LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
    
    m_handleFactory.getLoginManager().login(loginRequest.username, loginRequest.passwrod);
    LoginResponse loginResponse;
    loginResponse.status = static_cast<unsigned int>(stutusId::Success);
    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(loginResponse);
    rr.newHandler = this->m_handleFactory.createMenuRequestHandler(loginRequest.username);
    return rr;
}

RequestResult LoginRequestsHandler::signup(const RequestInfo& requestInfo) const
{
    SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);

    m_handleFactory.getLoginManager().signup(signupRequest.username, signupRequest.passwrod, signupRequest.email, signupRequest.address, signupRequest.birth, signupRequest.phone);
    SignupResponse signupResponse;
    signupResponse.status = static_cast<unsigned int>(stutusId::Success);
    RequestResult rr;
    rr.buffer = JsonResponsePacketSerializer::serializeResponse(signupResponse);
    rr.newHandler = this->m_handleFactory.createMenuRequestHandler(signupRequest.username);
    return rr;
    
}
