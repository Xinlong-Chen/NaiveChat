#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <memory>
#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "common/Singleton.h"
#include "utils/Connection.h"

#include "json/json.hpp"

using json = nlohmann::json;

class ConnectionPool : public Singleton<ConnectionPool> {
public:
    std::shared_ptr<Connection> getConnection();
    void setConfig(json config);
    void start();

protected:
    // used as Singleton
    ConnectionPool();

private:
    void produceConnectionTask();
	void scannerConnectionTask();

    std::string ip_;
    unsigned short port_;
    std::string username_;
    std::string password_;
    std::string dbname_;

    int initSize_;
    int maxSize_;
    int maxIdleTime_; // s
    int connectionTimeOut_; // ms

    std::queue<Connection*> connectionQueue_;
    std::mutex queueMutex_;
    std::atomic_int connectionCnt_;
    std::condition_variable cv_;

    std::atomic_int start_;
    std::atomic_bool is_init_;
};

#endif