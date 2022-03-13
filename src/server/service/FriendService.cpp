#include "service/FriendService.h"

int FriendService::addFriend(int selfid, int friendid) {
    friendDAO.insert(selfid, friendid);
    return 0;
}

std::vector<User> FriendService::getFriends(int selfid) {
    return friendDAO.query(selfid);
}
