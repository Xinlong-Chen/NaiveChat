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
protected:
    Dispatcher() = default;
};

#endif