#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include "common/Handler.h"

#include <iostream>

#include "common/Singleton.h"
#include "service/UserService.h"

class UserController : public Singleton<UserController> {
public:
    void registerUser(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);
    
protected:
    UserController() {}

private:
    UserService userService_;
};

#endif