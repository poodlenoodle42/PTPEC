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
    while(1){
        usleep(1000000000);
    }

    /*

    char str[1024];
    while(1){
        wgetnstr(input,str,1024);
        waddstr(output,str);
        wrefresh(output);
        wclear(input);
        wmove(input, 0, 0);
        wrefresh(input);
    }
*/
}