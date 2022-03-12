#include "net/ChatServer.h"

#include <iostream>
#include <functional>

#include "json/json.hpp"

#include "public.h"
#include "common/Handler.h"
#include "dispatcher/Dispatcher.h"

using json = nlohmann::json;

void resetHandler(int) {
    auto handler = Dispatcher::get_instance().getServerExecptionHandle();
    handler();
    exit(0);
}

ChatServer::ChatServer(muduo::net::EventLoop *loop,
                       const muduo::net::InetAddress &listenAddr,
                       const std::string &nameArg)
                    : loop_(loop), server_(loop, listenAddr, nameArg) {
    server_.setConnectionCallback(
        std::bind(&ChatServer::onConnection, this, std::placeholders::_1)
    );

    server_.setMessageCallback(
        std::bind(&ChatServer::onMessage, this, std::placeholders::_1, 
                         std::placeholders::_2, std::placeholders::_3)
    );

    server_.setThreadNum(4);
}

void ChatServer::start() {
    server_.start();
}

void ChatServer::onConnection(const muduo::net::TcpConnectionPtr &conn) {
    // client close TCP connection
    if (!conn->connected()) {
        auto handler = Dispatcher::get_instance().getClientExecptionHandle();
        handler(conn);
        conn->shutdown();
    }

}

void ChatServer::onMessage(const muduo::net::TcpConnectionPtr & conn,
                           muduo::net::Buffer *buffer,
                           muduo::Timestamp timestamp) {
    // TODO Add some application layer protocol
    std::string buf = buffer->retrieveAllAsString();
    // std::cout << buf << std::endl;
    json request, response;
    try {
        request = json::parse(buf);
    } catch(...) {
        FIX_JSON_PACKAGE(response, ERROR_MESSAGE, 1);
        conn->send(response.dump());
        return;
    }

    int msgid = -1;
    try {
        msgid = request["msgid"].get<int>();
    } catch(...) {
        FIX_JSON_PACKAGE(response, ERROR_MESSAGE, 2);
        conn->send(response.dump());
        return;
    }

    MsgHandler handler = Dispatcher::get_instance().getHandler(msgid);
    handler(conn, request, timestamp);
}