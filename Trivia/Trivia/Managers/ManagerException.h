#pragma once
#include <exception>
#include <string>

class ManagerException : public std::exception
{
public:
	ManagerException(const std::string& message) : m_message(message) {}
	virtual ~ManagerException() noexcept = default;
	virtual const char* what() const noexcept { return m_message.c_str(); }

protected:
	std::string m_message;
};
