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

int UserService::login(const int id, const std::string& password) {
    User user = userDAO.query(id);
    if (user.getId() == id && user.getPwd() == password) {
        if (user.getState() == "online") {
            // re-login
            return -2;
        } else {
            user.setState("online");
            userDAO.updateState(user);
            return 0;
        }
    } else {
        return -1;
    }
}

int UserService::logout(const int id) {
    User user(id, "", "", "offline");
    return userDAO.updateState(user) == true ? 0 : -1;
}