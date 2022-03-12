#ifndef OFFLINEMSGDAO_H
#define OFFLINEMSGDAO_H

#include <vector>
#include <string>

#include "model/OfflineMsg.h"

class OfflineMsgDAO {
public:
    void insert(int userid, std::string msg);
    void insert(OfflineMsg msg);
    void remove(int userid);
    std::vector<std::string> query(int userid);

    OfflineMsgDAO()  = default;
    ~OfflineMsgDAO() = default;
};

#endif