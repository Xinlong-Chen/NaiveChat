#include "service/MsgService.h"

int MsgService::store(const int toid, const std::string& msg) {
    offlineMsgDAO.insert(toid, msg);
    return 0;
}


std::vector<std::string> MsgService::query_and_remove(const int id) {
    std::vector<std::string> ans = offlineMsgDAO.query(id);
    if (!ans.empty()) {
        offlineMsgDAO.remove(id);
    }
    return ans;
}

