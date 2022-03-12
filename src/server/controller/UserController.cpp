#include "controller/UserController.h"

#include <muduo/base/Logging.h>

#include "public.h"

void UserController::registerUser(const muduo::net::TcpConnectionPtr &conn, 
                    json &js, muduo::Timestamp timestamp) {
    json response;
    std::string name, pwd;
    try {
        name = js["name"];
        pwd  = js["pwd"];
    }
    catch(const std::exception& e) {
        LOG_ERROR << "invaild parameters";
        FIX_JSON_PACKAGE(response, REG_MSG_ACK, 1);
        conn->send(response.dump());
        return;
    }
    
    // parameters check, ought to return json.
    if (name.length() == 0 || pwd.length() == 0) {
        LOG_ERROR << "invaild parameters";
        FIX_JSON_PACKAGE(response, REG_MSG_ACK, 1);
        return;
    }

    int ret = userService_.registerUser(name, pwd);
    if (ret == -1) {
        FIX_JSON_PACKAGE(response, REG_MSG_ACK, 2);
    } else {
        FIX_JSON_PACKAGE(response, REG_MSG_ACK, 0);
        response["id"] = ret;
    }
    conn->send(response.dump());
    return;
}

void UserController::login(const muduo::net::TcpConnectionPtr &conn, 
                    json &js, muduo::Timestamp timestamp) {
    json response;
    int id;
    std::string pwd;
    try {
        id  = js["id"].get<int>();
        pwd = js["pwd"];
    } catch(...) {
        FIX_JSON_PACKAGE(response, LOGIN_MSG_ACK, 1);
        conn->send(response.dump());
        return;
    }
    //TODO password need md5
    int ret = userService_.login(id, pwd);
    // login fail
    if (ret < 0) {
        FIX_JSON_PACKAGE(response, LOGIN_MSG_ACK, std::abs(ret));
        conn->send(response.dump());
        return;
    }
    
    // login success
    {
        std::lock_guard<std::mutex> lock(connMutex_);
        userConnMap_.insert({id, conn});
    }
    FIX_JSON_PACKAGE(response, LOGIN_MSG_ACK, 0);

    // load offline msgs
    std::vector<std::string> offlineMsgs;
    loadOfflineMsg(id, offlineMsgs);
    
    if (!offlineMsgs.empty()) {
        response["offlinemsg"] = offlineMsgs;
    }


    conn->send(response.dump());

}


int UserController::loadOfflineMsg(int id, std::vector<std::string>& ans) {
    ans = msgService_.query_and_remove(id);
    return 0;
}

void UserController::logout(const muduo::net::TcpConnectionPtr &conn, 
                        json &js, muduo::Timestamp timestamp) {
    json response;
    int logout_id;
    try {
        logout_id = js["id"].get<int>();
    }
    catch(...) {
        FIX_JSON_PACKAGE(response, LOGOUT_MSG_ACK, 1);
        conn->send(js.dump());
        return;
    }
    
    // TODO other body logout?
    {
        std::lock_guard<std::mutex> lock(connMutex_);
        auto it = userConnMap_.find(logout_id);
        if (it != userConnMap_.end())
        {
            userConnMap_.erase(it);
        }
    }
    
    int ret = userService_.logout(logout_id);
    if (ret == 0) {
        FIX_JSON_PACKAGE(response, LOGOUT_MSG_ACK, 0);
    } else {
        FIX_JSON_PACKAGE(response, LOGOUT_MSG_ACK, 2);
    }
    conn->send(js.dump());

}

void UserController::oneChat(const muduo::net::TcpConnectionPtr &conn, 
                        json &js, muduo::Timestamp timestamp) {
    json response;
    int to_id;
    try {
        to_id = js["toid"].get<int>();
    } catch(...) {
        // FIX_JSON_PACKAGE()
    }
    
    {
        std::lock_guard<std::mutex> lock(connMutex_);
        auto it = userConnMap_.find(to_id);
        if (it != userConnMap_.end()) {
            // transmit from user to other user
            it->second->send(js.dump());
            return;
        }
    }

    // TODO other server?

    msgService_.store(to_id, js.dump());
}


void UserController::clientCloseException(const muduo::net::TcpConnectionPtr &conn) {
    int id = -1;
    {
        std::lock_guard<std::mutex> lock(connMutex_);
        for (auto it = userConnMap_.begin(); it != userConnMap_.end(); ++it)
        {
            if (it->second == conn)
            {
                id = it->first;
                userConnMap_.erase(it);
                break;
            }
        }
    }

    if (id != -1) {
        userService_.logout(id);
    }
}

void UserController::serverReset() {
    userService_.resetState();
}

