#include "service/UserService.h"

int UserService::registerUser(const std::string& name, const std::string& password) {
    User user(-1, name, password);
    bool state = userDAO.insert(user);
    if (state) {
        return user.getId();
    } else {
        return -1;
    }
}

bool UserService::isOnline(const int id) {
    User user = userDAO.query(id);
    if (user.getId() == id && user.getState() == "online") {
        return true;
    }
    return false;
}

std::pair<int, User> UserService::login(const int id, const std::string& password) {
    User user = userDAO.query(id);
    if (user.getId() == id && user.getPwd() == password) {
        if (user.getState() == "online") {
            // re-login
            return {-2, User()};
        } else {
            user.setState("online");
            userDAO.updateState(user);
            return {0, user};
        }
    } else {
        return {-1, user};
    }
}

int UserService::logout(const int id) {
    User user(id, "", "", "offline");
    return userDAO.updateState(user) == true ? 0 : -1;
}


int UserService::resetState() {
    userDAO.resetState();
    return 0;
}