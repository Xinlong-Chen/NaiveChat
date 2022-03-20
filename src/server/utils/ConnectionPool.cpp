#include "utils/ConnectionPool.h"

#include <thread>

#include <muduo/base/Logging.h>

ConnectionPool::ConnectionPool() 
    : connectionCnt_(0), start_(0), is_init_(false) {}

#define TRY_GET_CONFIG(name_, name, config)                           \
        try {                                                         \
            name_ = config[name];                                     \
        } catch(...) {                                                \
            LOG_ERROR << "connection pool - " << name << " invaild";  \
        }                                                             \
        
#define TRY_GET_CONFIG_INT(name_, name, config)                       \
        try {                                                         \
            name_ = config[name].get<int>();                          \
        } catch(...) {                                                \
            LOG_ERROR << "connection pool - " << name << " invaild";  \
        }                                                             \

void ConnectionPool::setConfig(json config) {
    TRY_GET_CONFIG(ip_, "ip", config);
    TRY_GET_CONFIG_INT(port_, "port", config);
    TRY_GET_CONFIG(username_, "username", config);
    TRY_GET_CONFIG(password_, "password", config);
    TRY_GET_CONFIG_INT(initSize_, "initSize", config);
    TRY_GET_CONFIG_INT(maxSize_, "maxSize", config);
    TRY_GET_CONFIG_INT(maxIdleTime_, "maxIdleTime", config);
    TRY_GET_CONFIG_INT(connectionTimeOut_, "connectionTimeOut", config);
    TRY_GET_CONFIG(dbname_, "dbname", config);
    LOG_INFO << "Connection pool init success";
    is_init_ = true;
}

void ConnectionPool::start() {
    if (is_init_ && start_++ == 0) {
        // start 
        for (int i = 0; i < initSize_; ++i) {
            Connection* p = new Connection();
            p->connect(ip_, port_, username_, password_, dbname_);
            p->start();
            connectionQueue_.push(p);
            ++connectionCnt_;
        }
        // other thread
        std::thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
        produce.detach();

        std::thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
        scanner.detach();

        return;
    } 
    
    LOG_ERROR << "ConnectionPool didn't init or have started!";
}

std::shared_ptr<Connection> ConnectionPool::getConnection() {
    LOG_INFO << "get a connection from ConnectionPool";
    std::unique_lock<std::mutex> lock(queueMutex_);
    while (connectionQueue_.empty()) {
        if (std::cv_status::timeout == cv_.wait_for(lock, std::chrono::milliseconds(connectionTimeOut_))) {
            if (connectionQueue_.empty()) {
                LOG_ERROR << "Connector time out";
                return nullptr;
            }
        }
    }

    std::shared_ptr<Connection> sp(connectionQueue_.front(), [&] (Connection *con) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        con->start();
        connectionQueue_.push(con);
    });

    connectionQueue_.pop();
    if (connectionQueue_.empty()) {
        cv_.notify_all();
    }
    return sp;
}

// new thread
void ConnectionPool::produceConnectionTask() {
    LOG_INFO << "create thread to produceConnectionTask";
    for (;;) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        while (!connectionQueue_.empty()) {
            cv_.wait(lock);
        }
        
        if (connectionCnt_ < maxSize_) {
            Connection* p = new Connection();
            p->connect(ip_, port_, username_, password_, dbname_);
            p->start();
            connectionQueue_.push(p);
            ++connectionCnt_;
        }
        cv_.notify_all();
    }
}
// new thread
void ConnectionPool::scannerConnectionTask() {
    LOG_INFO << "create thread to scannerConnectionTask";
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(maxIdleTime_));

        std:: unique_lock<std::mutex> lock(queueMutex_);

        while (connectionCnt_ > initSize_) {
            Connection *p = connectionQueue_.front();
            if (p->end() > (maxIdleTime_ * 1000)) {
                connectionQueue_.pop();
                delete p;
                --connectionCnt_;
                continue;
            }
            break;
        }
    }
}