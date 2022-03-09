#ifndef HANDLER_H
#define HANDLER_H

#include <functional>

#include <muduo/net/TcpConnection.h>

#include <json/json.hpp>

using json = nlohmann::json;

using MsgHandler = std::function<void(const muduo::net::TcpConnectionPtr &conn, 
                                      json &js, 
                                      muduo::Timestamp)>;


#endif
