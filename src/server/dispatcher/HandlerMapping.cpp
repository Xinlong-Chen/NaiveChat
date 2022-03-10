#include "dispatcher/Dispatcher.h"

#include "public.h"

#include "controller/ChatController.h"

/**
 * @brief Construct a new Handler Mapping:: Handler Mapping object
 * register controler callback function in here
 */
HandlerMapping::HandlerMapping() {
    controllerMap_.insert({
        LOGIN_MSG, std::bind(&ChatController::test, ChatController(), std::placeholders::_1, 
                                std::placeholders::_2, std::placeholders::_3)
    });
}