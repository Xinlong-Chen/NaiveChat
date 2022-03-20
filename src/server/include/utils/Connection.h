#ifndef CONNECTION_H
#define CONNECTION_H

#include <mysql/mysql.h>
#include <string>
#include <ctime>

class Connection {
public:
    Connection();
    ~Connection();

    bool connect(std::string ip, unsigned short port, 
                  std::string username, std::string password, 
                  std::string dbname);
                  
    bool update(std::string sql);
    MYSQL_RES* query(std::string sql);
    MYSQL* getConnection() {
        return con_;
    }

    void start() {
        alivetime_ = clock();
    }

    clock_t end() const {
        return clock() - alivetime_;
    }

private:
    MYSQL *con_;
    clock_t alivetime_;
};

#endif