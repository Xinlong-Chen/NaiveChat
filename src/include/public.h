#ifndef PUBLIC_H
#define PUBLIC_H

enum EnMsgType
{
    LOGIN_MSG, 
    LOGIN_MSG_ACK, 
    LOGOUT_MSG,
    LOGOUT_MSG_ACK,
    REG_MSG,
    REG_MSG_ACK,
    ONE_CHAT_MSG,
    ADD_FRIEND_MSG,
    CREATE_GROUP_MSG,
    ADD_GROUP_MSG,
    GROUP_CHAT_MSG,

    CLIENT_EXCEPTION,
    SERVER_EXCEPTION,
    ERROR_MESSAGE,
};

#define FIX_JSON_PACKAGE(resp, msg_type, errno) \
    {                                           \
        resp["msgid"] = msg_type;               \
        resp["errno"] = errno;                  \
    }                                           \

#endif