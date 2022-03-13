#ifndef GROUPDAO_H
#define GROUPDAO_H

#include "model/Group.h"

class GroupDAO
{
public:
    GroupDAO()  = default;
    ~GroupDAO() = default;

    bool createGroup(Group &group);
    void addGroup(int userid, int groupid, std::string role);
    std::vector<Group> queryGroups(int userid);
    std::vector<int> queryGroupUsers(int userid, int groupid);

};

#endif