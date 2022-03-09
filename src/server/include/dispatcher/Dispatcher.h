#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <functional>

#include "dispatcher/HandlerMapping.h"

class Dispatcher {
public:
    static Dispatcher* instance() {
        static Dispatcher dispatcher;
        return &dispatcher;
    }

    MsgHandler getHandler(int msgid) {
        return HandlerMapping::instance()->getHandler(msgid);
    }
private:
    Dispatcher() = default;
};

#endif