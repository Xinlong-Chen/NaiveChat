#include "dao/UserDAO.h"

#include "db/DB.h"

bool UserDAO::insert(User &user) {
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }

    return false;
}

User UserDAO::query(int id) {

}

bool UserDAO::updateState(User user) {

}

void UserDAO::resetState() {

}