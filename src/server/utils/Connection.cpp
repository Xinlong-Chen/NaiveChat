#include "utils/Connection.h"

#include <muduo/base/Logging.h>

Connection::Connection() {
    con_ = mysql_init(nullptr);
}

Connection::~Connection() {
    if (con_ != nullptr) {
        mysql_close(con_);
    }
}

bool Connection::connect(std::string ip, unsigned short port, 
                         std::string username, std::string password, 
                         std::string dbname) {
    MYSQL *p = mysql_real_connect(con_, ip.c_str(), username.c_str(), 
                                   password.c_str(), dbname.c_str(), 
                                   port, nullptr, 0
    );
    return p != nullptr;
}

bool Connection::update(std::string sql) {
    if (mysql_query(con_, sql.c_str())) {
        LOG_ERROR << "update error: " << sql;
        return false;
    }
    return true;
}

MYSQL_RES* Connection::query(std::string sql) {
    if (mysql_query(con_, sql.c_str())) {
        LOG_ERROR << "query error: " << sql;
        return nullptr;
    }
    return mysql_use_result(con_);
}
