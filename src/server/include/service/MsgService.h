#ifndef MsgSERVICE_H
#define MsgSERVICE_H

#include "dao/OfflineMsgDAO.h"

class MsgService
{
public:
    MsgService()  = default; 
    ~MsgService() = default;

    int store(const int toid, const std::string& msg);
    std::vector<std::string> query_and_remove(const int id);
    
private:
    OfflineMsgDAO offlineMsgDAO;
};
#endif