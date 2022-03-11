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