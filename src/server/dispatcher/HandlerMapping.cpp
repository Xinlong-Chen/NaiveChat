#include "dispatcher/Dispatcher.h"

#include "public.h"

#include "controller/UserController.h"

/**
 * @brief Construct a new Handler Mapping:: Handler Mapping object
 * register controler callback function in here
 */
HandlerMapping::HandlerMapping() {
    // register msg handle
    controllerMap_.insert({
        LOGIN_MSG, std::bind(&UserController::login, &UserController::get_instance(), std::placeholders::_1, 
                                std::placeholders::_2, std::placeholders::_3)
    });

    controllerMap_.insert({
        REG_MSG, std::bind(&UserController::registerUser, &UserController::get_instance(), std::placeholders::_1, 
                                std::placeholders::_2, std::placeholders::_3)
    });

    controllerMap_.insert({
        LOGOUT_MSG, std::bind(&UserController::logout, &UserController::get_instance(), std::placeholders::_1, 
                                std::placeholders::_2, std::placeholders::_3)
    });

    controllerMap_.insert({
        CLIENT_EXCEPTION, std::bind(&UserController::clientCloseException, &UserController::get_instance(), 
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    });
}