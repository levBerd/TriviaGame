#pragma once
#pragma once
#include <exception>
#include <string>

class ServerException : public std::exception
{
public:
	ServerException(const std::string& message) : m_message(message) {}
	virtual ~ServerException() noexcept = default;
	virtual const char* what() const noexcept { return m_message.c_str(); }

protected:
	std::string m_message;
};
