#ifndef GROUPSERVICE_H
#define GROUPSERVICE_H

#include "dao/GroupDAO.h"

class GroupService 
{
public:
    int createGroup(int uid, Group group);
    int addGroup(int uid, int groupid);
    int load_groups(int uid);
    std::vector<User> load_users(int uid, int groupid);

private:
    GroupDAO groupDAO;
};

#endif