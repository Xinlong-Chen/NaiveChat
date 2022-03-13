#include "service/GroupService.h"

int GroupService::createGroup(int uid, Group group) {
    int ret = groupDAO.createGroup(group);
    if (ret) {
        groupDAO.addGroup(uid, group.getId(), "creator");
        return 0;
    }
    return 1;
}

int GroupService::addGroup(int uid, int groupid) {
    groupDAO.addGroup(uid, groupid, "normal");
    return 0;
}

int GroupService::load_groups(int uid) {
    return 0;
}

std::vector<User> GroupService::load_users(int uid, int groupid) {
    return {};
}
