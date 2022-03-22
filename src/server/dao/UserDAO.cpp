#include "dao/UserDAO.h"

#include "utils/ConnectionPool.h"

bool UserDAO::insert(User &user) {
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());


    auto connector = ConnectionPool::get_instance().getConnection();
    if (connector->update(sql)) {
        user.setId(mysql_insert_id(connector->getConnection()));
        return true;
    }
    return false;
}

User UserDAO::query(int id) {
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id);

    auto connector = ConnectionPool::get_instance().getConnection();
    MYSQL_RES *res = connector->query(sql);
    if (res != nullptr) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row != nullptr) {
            User user;
            user.setId(atoi(row[0]));
            user.setName(row[1]);
            user.setPwd(row[2]);
            user.setState(row[3]);
            mysql_free_result(res);
            return user;
        }
    }

    return User();
}

bool UserDAO::updateState(User user) {
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d", user.getState().c_str(), user.getId());

    auto connector = ConnectionPool::get_instance().getConnection();
    if (connector->update(sql)) {
        return true;
    }
    return false;
}

void UserDAO::resetState() {
    char sql[1024] = "update user set state = 'offline' where state = 'online'";
    
    auto connector = ConnectionPool::get_instance().getConnection();
    connector->update(sql);
}