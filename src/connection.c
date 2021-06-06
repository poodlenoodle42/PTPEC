#include "connection.h"
#include "message.h"
#include <threads.h>
#include "tui.h"
#include <sys/socket.h>
#include <crypt.h>
#include "common.h"
#include <string.h>
#include "crypto.h"
thrd_t server_thread;

Message_Type validate_pwd_server(Connection_Info* conn_info){
    char* salt = crypt_gensalt("$6$",0,NULL,0);
    Message challenge;
    Message challenge_response;
    Message challenge_valid;
    challenge_valid.header.size = 0;
    challenge_valid.header.message_type = Challenge_Failed;
    challenge_valid.buffer = NULL;
    challenge.header.message_type = Challenge;
    challenge.header.size = strlen(salt) + 1;
    challenge.buffer = salt;
    message_send(challenge,conn_info);
    message_receive(&challenge_response,conn_info);
    if(challenge_response.header.message_type != Challenge_Response){
        message_send(challenge_valid,conn_info);
        return Challenge_Failed;
    }
    char* hash = crypt(key_info.passwd,salt);
    if(strcmp(hash,challenge_response.buffer) == 0){
        challenge_valid.header.message_type = Challenge_Passed;
        message_send(challenge_valid,conn_info);
        message_destroy(&challenge_response);
        return Challenge_Passed;
    } else {
        message_send(challenge_valid,conn_info);
        message_destroy(&challenge_response);
        return Challenge_Failed;
    } 
}

Message_Type validate_pwd_client(Connection_Info* conn_info){
    Message challenge;
    Message challenge_response;
    Message challenge_valid;
    char * salt = NULL;
    message_receive(&challenge,conn_info);
    if(challenge_response.header.message_type != Challenge){
        return Challenge_Failed;
    }
    salt = challenge.buffer;
    char* hash = crypt(key_info.passwd,salt);
    challenge_response.header.size = strlen(hash) +1;
    challenge_response.header.message_type = Challenge_Response;
    challenge_response.buffer = hash;
    message_send(challenge_response,conn_info);
    message_receive(&challenge_valid,conn_info);
    return challenge_valid.header.message_type;
}

int listen_and_serve(void* v){
    int server_socket = *(int*)v;
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t size = sizeof(client_addr);
    while(1){
        client_socket = accept(server_socket,
        (struct sockaddr*)&client_addr,&size);
        if(client_socket == -1){
            tui_write_error("Failed to accept connection from to %s\n",inet_ntoa(client_addr.sin_addr));
        } else {
            Connection_Info* conn_info = malloc(sizeof(Connection_Info));
            conn_info->challenge_passed = 0;
            conn_info->client_info.addr = client_addr;
            conn_info->client_info.socket = client_socket;
            if(validate_pwd_server(conn_info) == Challenge_Passed){
                thrd_t client_thread;
                thrd_create(&client_thread,handle_connection,conn_info);
            } else{
                close(client_socket);
            }
        }
    }
}

void connection_setup_listen_socket(struct sockaddr_in address){
    int server_socket;
    address.sin_addr.s_addr = INADDR_ANY;
    SOCKET_ERROR_DIE(server_socket = 
    socket(AF_INET,SOCK_STREAM,0),"Failed to create server socket\n");

    SOCKET_ERROR_DIE(bind(server_socket,(struct sockaddr*)&address,sizeof(struct sockaddr_in)),
    "Failed to bind server socket\n");

    SOCKET_ERROR_DIE(listen(server_socket,MAX_CONNECTION_QUEUE),
    "Failed to listen on server socket\n");

    thrd_create(&server_thread,listen_and_serve,&server_socket);
}

int handle_connection(void* connection_info){
    Connection_Info* conn_info = (Connection_Info*)connection_info;
    Message msg;
    char* username = NULL;
    while(message_receive(&msg,conn_info) != -1){
        switch (msg.header.message_type){
            case Send_Username:
                username = malloc(msg.header.size);
                strcpy(username,msg.buffer);
                break;
            case Text_Message_Encrypted:
                if(username == NULL)
                    tui_write_default("Unknown User > %s \n",msg.buffer);
                else
                    tui_write_default("%s > %s \n",username,msg.buffer);
                break;
            default:
                if(username == NULL)
                    tui_write_error("Unknown message type from %s",inet_ntoa(conn_info->client_info.addr.sin_addr));
                else 
                    tui_write_error("Unknown message type from %s",username);
        }
    }
}