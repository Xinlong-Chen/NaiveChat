#include <iostream>
#include <signal.h>

#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include "net/ChatServer.h"

using namespace std;

extern void resetHandler(int);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatServer 127.0.0.1 6000" << endl;
        exit(-1);
    }

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