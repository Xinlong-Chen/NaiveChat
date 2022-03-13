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

    // load friends
    std::vector<User> firendsVec;
    loadFriend(id, firendsVec);
    if (!firendsVec.empty()) {
        std::vector<std::string> str_vec;
        for (User &user : firendsVec) {
            json user_js;
            user_js["id"] = user.getId();
            user_js["name"] = user.getName();
            user_js["state"] = user.getState();
            str_vec.push_back(user_js.dump());
        }
        response["friends"] = str_vec;
    }
    
    // load group msg
    std::vector<Group> groupUserVec = groupService_.load_groups(id);
    if (!groupUserVec.empty()) {
        std::vector<std::string> group2strVec;
        // group:[{groupid:[xxx, xxx, xxx, xxx]}]
        for (Group &group : groupUserVec)
        {
            json grpjson;
            grpjson["id"] = group.getId();
            grpjson["groupname"] = group.getName();
            grpjson["groupdesc"] = group.getDesc();
            std::vector<std::string> userV;
            for (GroupUser &user : group.getUsers())
            {
                json js;
                js["id"] = user.getId();
                js["name"] = user.getName();
                js["state"] = user.getState();
                js["role"] = user.getRole();
                userV.push_back(js.dump());
            }
            grpjson["users"] = userV;
            group2strVec.push_back(grpjson.dump());
        }
        response["groups"] = group2strVec;
    }

    conn->send(response.dump());

}


int UserController::loadOfflineMsg(int id, std::vector<std::string>& ans) {
    ans = msgService_.query_and_remove(id);
    return 0;
}

int UserController::loadFriend(int id, std::vector<User>& ans) {
    ans = friendService_.getFriends(id);
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
        return;
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

void UserController::addFriend(const muduo::net::TcpConnectionPtr &conn, 
                        json &js, muduo::Timestamp timestamp) {
    json responce;
    int userid, friendid;
    try
    {
        userid = js["id"].get<int>();
        friendid = js["friendid"].get<int>();
    }
    catch(...)
    {
        // responce
        return;
    }
    
    friendService_.addFriend(userid, friendid);
}

void UserController::createGroup(const muduo::net::TcpConnectionPtr &conn, 
                        json &js, muduo::Timestamp timestamp) {
    int userid;
    std::string name, desc;
    try {
        userid = js["id"].get<int>();
        name = js["groupname"];
        desc = js["groupdesc"];
    } catch (...) {
        // response
        return;
    }

    Group group(-1, name, desc);
    groupService_.createGroup(userid, group);
    // response
}

void UserController::addGroup(const muduo::net::TcpConnectionPtr &conn, 
                        json &js, muduo::Timestamp timestamp) {
    int userid, groupid;
    try {
        userid = js["id"].get<int>();
        groupid = js["groupid"].get<int>();
    } catch (...) {
        // response
        return;
    }

    groupService_.addGroup(userid, groupid);
}

void UserController::groupChat(const muduo::net::TcpConnectionPtr &conn, 
                        json &js, muduo::Timestamp timestamp) {
    int userid, groupid;
    try {   
        userid = js["id"].get<int>();
        groupid = js["groupid"].get<int>();
    } catch(...) {
        return;
    }
    
    std::vector<int> userIdVec = groupService_.load_users(userid, groupid);

    std::lock_guard<std::mutex> lock(connMutex_);

    for (int id : userIdVec) {
        if (id == userid) {
            continue;
        }
        auto it = userConnMap_.find(id);
        if (it != userConnMap_.end()) {
            it->second->send(js.dump());
        } else {
            // this server not found
            msgService_.store(id, js.dump());
        }
    }
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

