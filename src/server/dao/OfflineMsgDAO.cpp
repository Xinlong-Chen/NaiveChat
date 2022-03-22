#include "dao/OfflineMsgDAO.h"

#include "utils/ConnectionPool.h"

void OfflineMsgDAO::insert(int userid, std::string msg) {
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage values(%d, '%s')", userid, msg.c_str());

    auto connector = ConnectionPool::get_instance().getConnection();
    connector->update(sql);
}

void OfflineMsgDAO::insert(OfflineMsg msg) {
    insert(msg.getId(), msg.getMsg());
}

void OfflineMsgDAO::remove(int userid) {
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userid=%d", userid);

    auto connector = ConnectionPool::get_instance().getConnection();
    connector->update(sql);
}

std::vector<std::string> OfflineMsgDAO::query(int userid) {
    char sql[1024] = {0};
    sprintf(sql, "select message from offlinemessage where userid = %d", userid);

    std::vector<std::string> vec;
    
    auto connector = ConnectionPool::get_instance().getConnection();
    MYSQL_RES *res = connector->query(sql);
    if (res != nullptr)
    {
        // 把userid用户的所有离线消息放入vec中返回
        MYSQL_ROW row;
        while((row = mysql_fetch_row(res)) != nullptr)
        {
            vec.push_back(row[0]);
        }
        mysql_free_result(res);
        return vec;
    }
    return vec;
}
