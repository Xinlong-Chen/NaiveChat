#ifndef FRIENDSERVICE_H
#define FRIENDSERVICE_H

#include "dao/FriendDAO.h"

class FriendService 
{
public:
    int addFriend(int selfid, int friendid);
    std::vector<User> getFriends(int selfid);

private:
    FriendDAO friendDAO;
};

#endif