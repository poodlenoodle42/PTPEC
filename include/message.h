#pragma once
#include <stdint.h>
typedef enum Message_Type{
    Register_Username, //Buffer contains username
    Get_Peers, //Buffer contains struct sockaddr_in 's of other peers
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

int send_message(const Message msg, int socket);
int receive_message(Message* msg,int socket);