#ifndef USERDAO_H
#define USERDAO_H

#include "model/User.h"

class UserDAO {
public:
    bool insert(User &user);
    User query(int id);
    bool updateState(User user);
    void resetState();
};

#endif