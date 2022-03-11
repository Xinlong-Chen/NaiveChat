#ifndef HANDLER_MAPPING_H
#define HANDLER_MAPPING_H

#include <functional>
#include <unordered_map>

#include <muduo/base/Logging.h>

#include "common/Singleton.h"
#include "common/Handler.h"

class HandlerMapping : public Singleton<HandlerMapping> {
public:
    ~HandlerMapping() = default;

    MsgHandler getHandler(int msgid) {
        auto iter = controllerMap_.find(msgid);
        if (iter == controllerMap_.end()) {
            return [=](const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp) {
                LOG_ERROR << "msgid:" << msgid << " can not find handler!";
            }; 
        }
        return iter->second;
    }

protected:
    HandlerMapping();

private:
    std::unordered_map<int, MsgHandler> controllerMap_;
};


#endif