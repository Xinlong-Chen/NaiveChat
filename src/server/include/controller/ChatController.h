#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include "common/Handler.h"

#include <iostream>

#include "model/User.h"
#include "dao/UserDAO.h"

class ChatController {
public:
    void test(const muduo::net::TcpConnectionPtr &conn, 
                        json &js, muduo::Timestamp timestamp) {
        User user;
        std::string name;
        try {
            name = js["name"];
        } catch(...) {
            return;
        }
        user.setName(name);

        UserDAO dao;
        dao.insert(user);
    }
};

#endif