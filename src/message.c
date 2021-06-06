#include "message.h"
#include "crypto.h"
#define MESSAGE_SOCKET_ERROR(val) \
if(val == -1){return -1;}

int message_send(const Message msg, Connection_Info* conn_info){
    Message_Header header = msg.header;
    char* buff = msg.buffer;
    if(header.message_type == Text_Message_Encrypted){
        char* encrypted = malloc(header.size + 128);
        int encrypted_length = encrypt(buff,header.size,key_info.key,key_info.ex_key_info.iv,encrypted);
        buff = encrypted;
        header.size = encrypted_length;
    }
    int final_size = header.size;
    header.message_type = htonl(header.message_type);
    header.size = htonl(header.size);
    MESSAGE_SOCKET_ERROR(send(conn_info->client_info.socket,
    &header,sizeof(Message_Header),0))
    MESSAGE_SOCKET_ERROR(send(conn_info->client_info.socket,
    buff,final_size,0))
    if(header.message_type == Text_Message_Encrypted)
        free(buff);
}
int message_receive(Message* msg,Connection_Info* conn_info){
    Message_Header header;
    MESSAGE_SOCKET_ERROR(recv(conn_info->client_info.socket,&header,sizeof(Message_Header),0));
    header.message_type = ntohl(header.message_type);
    header.size = ntohl(header.size);
    if(header.size > 0){
        char* buff = malloc(header.size);
        MESSAGE_SOCKET_ERROR(recv(conn_info->client_info.socket,buff,header.size,0));
        if(header.message_type == Text_Message_Encrypted){
            char* decrypted = malloc(header.size);
            int decrypted_size = decrypt(buff,header.size,key_info.key,key_info.ex_key_info.iv,decrypted);
            free(buff);
            buff = decrypted;
            header.size = decrypted_size;
        }
        msg->buffer = buff;
    } else
        msg->buffer = NULL;
    msg->header = header;
    
}


void message_destroy(Message* msg){
    if(msg->buffer != NULL)
        free(msg->buffer);
    msg->buffer = NULL;
}