#include "dao/GroupDAO.h"

#include "utils/ConnectionPool.h"

bool GroupDAO::createGroup(Group &group) {
    char sql[1024] = {0};
    sprintf(sql, "insert into allgroup(groupname, groupdesc) values('%s', '%s')",
            group.getName().c_str(), group.getDesc().c_str());

    auto connector = ConnectionPool::get_instance().getConnection();
    if (connector->update(sql)) {
        group.setId(mysql_insert_id(connector->getConnection()));
        return true;
    }
    return  false;
}

void GroupDAO::addGroup(int userid, int groupid, std::string role) {
    char sql[1024] = {0};
    sprintf(sql, "insert into groupuser values(%d, %d, '%s')",
            groupid, userid, role.c_str());

    auto connector = ConnectionPool::get_instance().getConnection();
    connector->update(sql);
}

std::vector<Group> GroupDAO::queryGroups(int userid) {
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.groupname,a.groupdesc from allgroup a inner join \
         groupuser b on a.id = b.groupid where b.userid=%d",
            userid);

    std::vector<Group> groupVec;
    auto connector = ConnectionPool::get_instance().getConnection();
    
    MYSQL_RES *res = connector->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            Group group;
            group.setId(atoi(row[0]));
            group.setName(row[1]);
            group.setDesc(row[2]);
            groupVec.push_back(group);
        }
        mysql_free_result(res);
    }

    for (Group &group : groupVec)
    {
        sprintf(sql, "select a.id,a.name,a.state,b.grouprole from user a \
            inner join groupuser b on b.userid = a.id where b.groupid=%d",
                group.getId());

        MYSQL_RES *res = connector->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                group.getUsers().push_back(user);
            }
            mysql_free_result(res);
        }
    }
    return groupVec;
}

std::vector<int> GroupDAO::queryGroupUsers(int userid, int groupid) {
    char sql[1024] = {0};
    sprintf(sql, "select userid from groupuser where groupid = %d and userid != %d", groupid, userid);

    std::vector<int> idVec;

    auto connector = ConnectionPool::get_instance().getConnection();
    MYSQL_RES *res = connector->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            idVec.push_back(atoi(row[0]));
        }
        mysql_free_result(res);
    }
    return idVec;
}
