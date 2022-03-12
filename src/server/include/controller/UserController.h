#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include "common/Handler.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>

#include "common/Singleton.h"
#include "service/UserService.h"
#include "service/MsgService.h"

class UserController : public Singleton<UserController> {
public:
    // basic operator
    void registerUser(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);
    
    void login(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);
    
    void logout(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);

    void oneChat(const muduo::net::TcpConnectionPtr &conn, 
                            json &js, muduo::Timestamp timestamp);

    // Exception Process
    void clientCloseException(const muduo::net::TcpConnectionPtr &conn);

    void serverReset();
             
protected:
    UserController() {}

private:
    inline int loadOfflineMsg(int id, std::vector<std::string>& ans);

    UserService userService_;
    MsgService  msgService_;

    std::unordered_map<int, muduo::net::TcpConnectionPtr> userConnMap_;
    std::mutex connMutex_;
};

#endif