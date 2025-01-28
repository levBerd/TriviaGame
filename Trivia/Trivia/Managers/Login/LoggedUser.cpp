#include "LoggedUser.h"


LoggedUser::LoggedUser(const std::string& name) : m_name(name)
{
}

std::string LoggedUser::getName() const
{
	return this->m_name;
}

bool LoggedUser::operator==(const LoggedUser& user)
{
	return this->m_name == user.getName();
}
