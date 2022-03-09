#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include "common/Handler.h"

#include <iostream>

class ChatController {
public:
    void test(const muduo::net::TcpConnectionPtr &conn, 
                        json &js, muduo::Timestamp timestamp) {
        std::cout << "test" << std::endl;
    }
};

#endif