#pragma once
#include <iostream>

class LoggedUser
{
public:
	LoggedUser(const std::string& name);
	std::string getName() const;
	bool operator == (const LoggedUser& user);

private:
	std::string m_name;
};
