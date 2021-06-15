#include <ncurses.h>
#include <stdlib.h>
#include "arguments.h"
#include "array.h"
#include "tui.h"
#include <crypt.h>
#include <time.h>
#include "connection.h"
#include "common.h"
#include "crypto.h"
#include <unistd.h>
#include "message.h"
#include <signal.h>
static cnd_t sigint;
static mtx_t sigint_lock;
void sigint_handler(int s){
    cnd_signal(&sigint);
}

int cleanup(void* _){
    mtx_lock(&sigint_lock);
    cnd_wait(&sigint,&sigint_lock);
    mtx_lock(&existing_connections->lock);
    Message leave_msg;
    leave_msg.buffer = NULL;
    leave_msg.header.size = 0;
    leave_msg.header.message_type = Leave;
    for(int i = 0;i<existing_connections->size;i++){
        Connection_Info conn_info;
        conn_info.challenge_passed = 1;
        conn_info.client_info = existing_connections->buff[i];
        message_send(leave_msg,&conn_info);
        close(conn_info.client_info.socket);
    }
    mtx_unlock(&existing_connections->lock);
    mtx_unlock(&sigint_lock);
    exit(0);
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    Arguments* args = parse_arguments(argc,argv);


    cnd_init(&sigint);
    signal(SIGINT,sigint_handler);
    thrd_t sigint_thread;
    thrd_create(&sigint_thread,cleanup,NULL);


    key_info.passwd = args->pwd_str;


    nickname = args->nickname_str;
    External_Key_Information ex_k;
    tui_setup();
    existing_connections = array_new(INIT_CONNECTION_ARRAY_SIZE);
    if(args->address == 0){
        random_external_key_info();
        fill_key_info();
        connection_setup_listen_socket(args->address_info);
    } else {
        connection_setup_external_peer(args->address_info);
    }

    
    Message msg;
    Connection_Info conn_info;
    char str[1024];
    while(1){
        tui_read(str,1024);
        tui_write_green("\nYou > %s\n",str);
        mtx_lock(&existing_connections->lock);
        msg.header.message_type = Text_Message_Encrypted;
        msg.header.size = strlen(str) + 1;
        msg.buffer = str;
        for(int i = 0; i<existing_connections->size;i++){
            conn_info.client_info = existing_connections->buff[i];
            if(message_send(msg,&conn_info) < 0){
               tui_write_error("Error sending message to %s\n",inet_ntoa(existing_connections->buff[i].addr.sin_addr));
            }
        }
        mtx_unlock(&existing_connections->lock);
    }

}