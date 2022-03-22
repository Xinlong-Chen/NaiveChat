#include "dao/FriendDAO.h"

#include "utils/ConnectionPool.h"

void FriendDAO::insert(int userid, int friendid) {
    char sql[1024] = {0};
    sprintf(sql, "insert into friend values(%d, %d)", userid, friendid);

    auto connector = ConnectionPool::get_instance().getConnection();
    connector->update(sql);
}

std::vector<User> FriendDAO::query(int userid) {
    char sql[1024] = {0};

    sprintf(sql, "select a.id,a.name,a.state from user a inner join friend b on b.friendid = a.id where b.userid=%d", userid);

    std::vector<User> vec;

    
    auto connector = ConnectionPool::get_instance().getConnection();
    MYSQL_RES *res = connector->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res)) != nullptr)
        {
            User user;
            user.setId(atoi(row[0]));
            user.setName(row[1]);
            user.setState(row[2]);
            vec.push_back(user);
        }
        mysql_free_result(res);
        return vec;
    }
    return vec;
}