#ifndef FriendDAO_H
#define FriendDAO_H

#include <vector>

#include "model/User.h"

class FriendDAO
{
public:
    FriendDAO()  = default;
    ~FriendDAO() = default;

    void insert(int userid, int friendid);
    std::vector<User> query(int userid);
};

#endif

