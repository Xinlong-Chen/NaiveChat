#include "utils/redis.h"

#include <iostream>

Redis::Redis()
    : _publish_context(nullptr), _subcribe_context(nullptr) {}

Redis::~Redis()
{
    if (_publish_context != nullptr)
    {
        redisFree(_publish_context);
    }

    if (_subcribe_context != nullptr)
    {
        redisFree(_subcribe_context);
    }
}

bool Redis::connect()
{
    // publish context
    _publish_context = redisConnect("127.0.0.1", 6379);
    if (nullptr == _publish_context)
    {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }

    // subscribe context
    _subcribe_context = redisConnect("127.0.0.1", 6379);
    if (nullptr == _subcribe_context)
    {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }

    // other thread listen publish context
    std::thread t([&]() {
        observer_channel_message();
    });
    t.detach();

    std::cout << "connect redis-server success!" << std::endl;

    return true;
}

bool Redis::publish(int channel, std::string message)
{
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBLISH %d %s", channel, message.c_str());
    if (nullptr == reply)
    {
        std::cerr << "publish command failed!" << std::endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

// 向redis指定的通道subscribe订阅消息
bool Redis::subscribe(int channel)
{
    // redisCommand will block this thread, only send command
    if (REDIS_ERR == redisAppendCommand(this->_subcribe_context, "SUBSCRIBE %d", channel))
    {
        std::cerr << "subscribe command failed!" << std::endl;
        return false;
    }
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(this->_subcribe_context, &done))
        {
            std::cerr << "subscribe command failed!" << std::endl;
            return false;
        }
    }
    // redisGetReply, in function observer_channel_message(other thread)

    return true;
}

bool Redis::unsubscribe(int channel)
{
    if (REDIS_ERR == redisAppendCommand(this->_subcribe_context, "UNSUBSCRIBE %d", channel))
    {
        std::cerr << "unsubscribe command failed!" << std::endl;
        return false;
    }

    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(this->_subcribe_context, &done))
        {
            std::cerr << "unsubscribe command failed!" << std::endl;
            return false;
        }
    }
    return true;
}

void Redis::observer_channel_message()
{
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(this->_subcribe_context, (void **)&reply))
    {
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr)
        {
            _notify_message_handler(atoi(reply->element[1]->str) , reply->element[2]->str);
        }

        freeReplyObject(reply);
    }

    std::cerr << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<" << std::endl;
}

void Redis::init_notify_handler(std::function<void(int, std::string)> cb)
{
    this->_notify_message_handler = cb;
}