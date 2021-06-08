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
int main(int argc, char* argv[]){
    srand(time(NULL));
    Arguments* args = parse_arguments(argc,argv);

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