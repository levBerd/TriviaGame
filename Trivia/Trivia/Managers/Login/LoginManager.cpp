#include "LoginManager.h"
#include <regex>
#include "../../Infastructure/ServerException.h"

LoginManager::LoginManager(std::shared_ptr<IDatabase> dataAccess)
    : m_IDatabase(dataAccess) {}

void LoginManager::printUserAmount()
{
    std::cout << "users online: " << this->m_loggedUsers.size() << std::endl;
}

void LoginManager::signup(const std::string& username, const std::string& password, const std::string& email, const std::string& addr, const std::string& birth, const std::string& phone)
{

    if (m_IDatabase.expired()) {
        throw ServerException("IDatabase ptr has expired! (LoginManager::signup)");
    }

    if ((*m_IDatabase.lock()).doesUserExists(username) > 0)
    {
        throw ManagerException("User already exists.");
    }
    
    // Password validation
    std::regex password_regex("^(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9])(?=.*[\\*\\&\\^\\%\\$\\#\\@\\!]).{8,}$");
    if (!std::regex_match(password, password_regex))
    {
        throw ManagerException("Password does not meet requirements.");
    }

    // Email validation
    std::regex email_regex("^[a-zA-Z0-9]+@[a-zA-Z0-9]+(\\.[a-zA-Z]{2,})+$");
    if (!std::regex_match(email, email_regex))
    {
        throw ManagerException("Invalid email format.");
    }

    // address validation
    std::regex addressRegex("^([a-zA-Z ]+), ([0-9]+), ([a-zA-Z ]+)$");
    if (!std::regex_match(addr, addressRegex))
    {
        throw ManagerException("Invalid residential address format.");
    }

    // Phone number validation
    std::regex phoneRegex("^0[0-9]{2,3}-[0-9]{7}$");
    if (!std::regex_match(phone, phoneRegex))
    {
        throw ManagerException("Invalid phone number format.");
    }

    // Birth date validation regex
    std::regex birthDateRegex("^\\d{2}[./]\\d{2}[./]\\d{4}$");
    if (!std::regex_match(birth, birthDateRegex))
    {
        throw ManagerException("Invalid birth date format. Use DD.MM.YYYY or DD/MM/YYYY.");
    }
    

    int result = (*m_IDatabase.lock()).addNewUser(username, password, email, addr, birth, phone);

    if (!result)
    {
        throw ManagerException("Failed to add new user.");
    }

    std::cout << "New signup, user: " << username << ", " << password << ", " << email << std::endl;
}

void LoginManager::login(const std::string& username, const std::string& passwrod)
{
    if (m_IDatabase.expired()) {
        throw ServerException("IDatabase ptr has expired! (LoginManager::login)");
    }

    if ((*m_IDatabase.lock()).doesUserExists(username) == 0)
    {
        throw ManagerException("User does not exist.");
    }

    if ((*m_IDatabase.lock()).doesPasswordMatch(username, passwrod) == 0)
    {
        throw ManagerException("Password does not match.");
    }

    std::shared_lock readLock(m_mutex);
    auto it = std::find_if(m_loggedUsers.begin(), m_loggedUsers.end(), [&username](const LoggedUser& user) {
        return user.getName() == username;
        });

    if (it != m_loggedUsers.end())
    {
        throw ManagerException("User already logged in.");
    }
    readLock.unlock();


    std::unique_lock writeLock(m_mutex);
    m_loggedUsers.emplace_back(username);
    writeLock.unlock();
    std::cout << "New login, user: " << username << ", " << passwrod << std::endl;
}

void LoginManager::logout(const std::string& username)
{
    std::shared_lock readLock(m_mutex);
    auto it = std::find_if(m_loggedUsers.begin(), m_loggedUsers.end(), [&username](const LoggedUser& user) {
        return user.getName() == username;
        });

    if (it == m_loggedUsers.end())
    {
        throw ManagerException("User not logged in.");
    }
    readLock.unlock();

    std::unique_lock writeLock(m_mutex);
    m_loggedUsers.erase(it);
}
