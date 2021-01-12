#include "LoggedUser.h"

LoggedUser::LoggedUser()
{
}

LoggedUser::LoggedUser(std::string username)
{
	this->m_username = username;
}

LoggedUser::~LoggedUser()
{
}

bool LoggedUser::getIsAdmin() const
{
	return this->m_isAdmin;
}

void LoggedUser::setAdmin(bool isAdmin)
{
	this->m_isAdmin = isAdmin;
}

