#ifndef OFFLINEMSG_H
#define OFFLINEMSG_H

#include <string>

class OfflineMsg
{
public:
    OfflineMsg(int id = -1, std::string msg = "")
        : id(id), msg(msg) { }
    ~OfflineMsg();

    int getId() { return id; }
    void setId(int id) { this->id = id; }

    std::string getMsg() { return msg; }
    void setMsg(std::string msg) { this->msg = msg; }

private:
    int id;
    std::string msg;
};

#endif