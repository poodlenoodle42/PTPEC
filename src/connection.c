#include "connection.h"
#include "message.h"
#include <threads.h>
#include "tui.h"
#include <sys/socket.h>
#include <crypt.h>
#include "common.h"
#include <string.h>
#include "crypto.h"
#include <unistd.h>
#include "array.h"
#include <errno.h>
thrd_t server_thread;
Array* existing_connections;
char* nickname;
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
    if(challenge.header.message_type != Challenge){
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
    free(v);
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t size = sizeof(client_addr);
    while(1){
        client_socket = accept(server_socket,
        (struct sockaddr*)&client_addr,&size);
        if(client_socket == -1){
            tui_write_error("Failed to accept connection from to %s Errorcode: %d\n",inet_ntoa(client_addr.sin_addr),errno);
        } else {
            Connection_Info* conn_info = malloc(sizeof(Connection_Info));
            conn_info->challenge_passed = 0;
            conn_info->client_info.addr = client_addr;
            conn_info->client_info.socket = client_socket;
            if(validate_pwd_server(conn_info) == Challenge_Passed){
                conn_info->challenge_passed = 1;
                thrd_t client_thread;
                thrd_create(&client_thread,handle_connection,conn_info);
            } else{
                close(client_socket);
            }
        }
    }
}

int connection_setup_external_peer(struct sockaddr_in address){
    int peer_socket;
    Connection_Info conn_info;
    Message rq_peers;
    Message rq_external_key_info;
    conn_info.challenge_passed = 0;
    conn_info.client_info.addr = address;
    SOCKET_ERROR_DIE(peer_socket = 
    socket(AF_INET,SOCK_STREAM,0),"Failed to create socket\n");
    conn_info.client_info.socket = peer_socket;
    SOCKET_ERROR_DIE(connect(peer_socket,(struct sockaddr*)&address,sizeof(struct sockaddr_in)),
    "Error connecting to peer\n")

    if(validate_pwd_client(&conn_info) != Challenge_Passed){
        close(peer_socket);
        return 0;
    }
    rq_peers.buffer = NULL;
    rq_peers.header.message_type = Request_Peers;
    rq_peers.header.size = 0; 
    SOCKET_ERROR(message_send(rq_peers,&conn_info),
    "Error requesting peers from %s\n",inet_ntoa(conn_info.client_info.addr.sin_addr));

    rq_external_key_info.buffer = NULL;
    rq_external_key_info.header.message_type = Request_External_Key_Info;
    rq_external_key_info.header.size = 0; 
    SOCKET_ERROR(message_send(rq_external_key_info,&conn_info),
    "Error requesting external key info from %s\n",inet_ntoa(conn_info.client_info.addr.sin_addr));
    Connection_Info* con_p = malloc(sizeof(Connection_Info));
    *con_p = conn_info;
    thrd_t client_thread;
    thrd_create(&client_thread,handle_connection,con_p);
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
    int* s = malloc(sizeof(int));
    *s = server_socket;
    thrd_create(&server_thread,listen_and_serve,s);
}

void connect_to_all(const struct sockaddr_in* buf,int len){

}

int handle_connection(void* connection_info){
    Connection_Info* conn_info = (Connection_Info*)connection_info;
    Message msg;
    array_add(existing_connections,&conn_info->client_info);
    msg.header.message_type = Request_Username;
    msg.header.size = 0;

    SOCKET_ERROR(message_send(msg,conn_info),
    "Error requesting username from %s\n",inet_ntoa(conn_info->client_info.addr.sin_addr))
    char* username = NULL;
    while(message_receive(&msg,conn_info) != -1){
        switch (msg.header.message_type){
            case Send_Username:
                username = malloc(msg.header.size);
                strcpy(username,msg.buffer);
                message_destroy(&msg);
                break;
            case Request_Username:
                msg.header.message_type = Send_Username;
                msg.header.size = strlen(nickname) +1;
                msg.buffer = nickname;
                SOCKET_ERROR(message_send(msg,conn_info),
                "Error sending username to %s\n",inet_ntoa(conn_info->client_info.addr.sin_addr))
                break;
            case Text_Message_Encrypted:
                if(username == NULL)
                    tui_write_default("Unknown User > %s \n",msg.buffer);
                else
                    tui_write_default("%s > %s \n",username,msg.buffer);
                break;
            case Request_Peers:
                if(conn_info->challenge_passed == 0)
                    break;
                struct sockaddr_in* peers_buffer = malloc((existing_connections->size-1) * sizeof(struct sockaddr_in));
                int add_i;
                for(int i = 0; i<existing_connections->size;++i){
                    if(existing_connections->buff[i].socket != conn_info->client_info.socket){
                        add_i = i;
                        peers_buffer[add_i] = existing_connections->buff[i].addr;
                    } 

                }
                msg.header.message_type = Send_Peers;
                msg.header.size = existing_connections->size - 1;
                msg.buffer = (char*)peers_buffer;
                SOCKET_ERROR(message_send(msg,conn_info),"Error sending peers to %s\n",inet_ntoa(conn_info->client_info.addr.sin_addr))
                free(peers_buffer);
                break;
            case Request_External_Key_Info:
                msg.header.message_type = Send_External_Key_Info;
                msg.header.size = sizeof(External_Key_Information);
                msg.buffer = (char*)&key_info.ex_key_info;
                SOCKET_ERROR(message_send(msg,conn_info),
                "Error sending external key info to %s\n",inet_ntoa(conn_info->client_info.addr.sin_addr));
                break;
            case Send_External_Key_Info:
                memcpy(&key_info.ex_key_info,msg.buffer,msg.header.size);
                key_info.external_init = 1;
                message_destroy(&msg);
                fill_key_info();
                break;
            case Send_Peers:
                if(existing_connections->size > 1)
                     break;
                for(int i = 0; i < msg.header.size; i++){
                    Client_Info conn;
                    conn.socket = 0;
                    conn.addr = ((struct sockaddr_in*)msg.buffer)[i];
                    array_add(existing_connections,&conn);
                }
                message_destroy(&msg);
                break;
            default:
                if(username == NULL)
                    tui_write_error("Unknown message type from %s\n",inet_ntoa(conn_info->client_info.addr.sin_addr));
                else 
                    tui_write_error("Unknown message type from %s\n",username);
        }
    }
}