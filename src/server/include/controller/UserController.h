#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include "common/Handler.h"

#include <unordered_map>
#include <mutex>

#include "common/Singleton.h"
#include "service/UserService.h"

class UserController : public Singleton<UserController> {
public:
    void registerUser(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);
    
    void login(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);
    
    void logout(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);

    void clientCloseException(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);
                            
protected:
    UserController() {}

private:
    UserService userService_;

    std::unordered_map<int, muduo::net::TcpConnectionPtr> userConnMap_;
    std::mutex connMutex_;
};

#endif