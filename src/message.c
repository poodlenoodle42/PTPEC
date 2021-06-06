#include "message.h"

int message_send(const Message msg, Connection_Info* conn_info);
int message_receive(Message* msg,Connection_Info* conn_info);


void message_destroy(Message* msg){
    if(msg->buffer != NULL)
        free(msg->buffer);
    
    free(msg);
}