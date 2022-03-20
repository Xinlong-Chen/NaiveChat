#include <iostream>
#include <fstream>
#include <signal.h>

#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include "net/ChatServer.h"
#include "utils/ConnectionPool.h"


#include "json/json.hpp"

using namespace std;
using json = nlohmann::json;

extern void resetHandler(int);

void initConnectionPool() {
    json config;
    ifstream("/home/xinlong/NaiveChat/config/config.json") >> config;
    ConnectionPool::get_instance().setConfig(config["MySQL"]);
    ConnectionPool::get_instance().start();
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatServer 127.0.0.1 6000 config_path" << endl;
        exit(-1);
    }
    
    initConnectionPool();

    // 解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    signal(SIGINT, resetHandler);

    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();

    return 0;
}