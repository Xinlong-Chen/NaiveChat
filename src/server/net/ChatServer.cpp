#include "net/ChatServer.h"

#include <iostream>
#include <functional>

#include "json/json.hpp"

using json = nlohmann::json;

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
        conn->shutdown();
    }

}

void ChatServer::onMessage(const muduo::net::TcpConnectionPtr & conn,
                           muduo::net::Buffer *buffer,
                           muduo::Timestamp timestamp) {
    // TODO Add some application layer protocol
    std::string buf = buffer->retrieveAllAsString();
    std::cout << buf << std::endl;
    // json js = json::parse(buf);

}