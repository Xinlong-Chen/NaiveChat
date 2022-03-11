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
