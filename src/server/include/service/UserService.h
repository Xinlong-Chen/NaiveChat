#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "dao/UserDAO.h"

class UserService
{
public:
    UserService()  = default; 
    ~UserService() = default;

    int registerUser(const std::string& name, const std::string& password);

private:
    UserDAO userDAO;
};
#endif