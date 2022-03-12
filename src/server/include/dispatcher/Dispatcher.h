#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <functional>

#include "common/Singleton.h"
#include "dispatcher/HandlerMapping.h"

class Dispatcher : public Singleton<Dispatcher> {
public:
    MsgHandler getHandler(int msgid) {
        return HandlerMapping::get_instance().getHandler(msgid);
    }

    std::function<void()> getServerExecptionHandle() {
        return HandlerMapping::get_instance().getServerExecptionHandle();
    }

    std::function<void(const muduo::net::TcpConnectionPtr &conn)> getClientExecptionHandle() {
        return HandlerMapping::get_instance().getClientExecptionHandle();
    }

protected:
    Dispatcher() = default;
};

#endif