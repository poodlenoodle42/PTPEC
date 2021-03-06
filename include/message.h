#pragma once
#include <stdint.h>
#include "connection.h"
typedef enum Message_Type{
    Request_Username, //Buffer empty
    Send_Username, //Buffer contains username
    Request_Peers, //Buffer contains nothing
    Send_Peers, //Buffer contains struct sockaddr_in of other peers
    Request_External_Key_Info, //Buffer is empty
    Send_External_Key_Info, //Buffer contains external key info
    Challenge, //Buffer contains challenge salt
    Challenge_Response, //Buffer contains hash
    Challenge_Passed, //Buffer is empty
    Challenge_Failed, //Buffer is empty
    Text_Message, //Buffer contains text of message
    Text_Message_Encrypted, //Buffer contains encrypted text message
    Leave //Buffer is empty
}Message_Type;

typedef struct _Header{
    Message_Type message_type;
    uint32_t size;
}Message_Header;

typedef struct _Message{
    Message_Header header;
    char* buffer;
}Message;



int message_send(const Message msg, Connection_Info* conn_info);
int message_receive(Message* msg,Connection_Info* conn_info);
void message_destroy(Message* msg);