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
int main(int argc, char* argv[]){
    srand(time(NULL));
    Arguments* args = parse_arguments(argc,argv);

    key_info.passwd = args->pwd_str;
    key_info.ex_key_info = random_external_key_info();
    fill_key_info(key_info.passwd,key_info.ex_key_info);

    nickname = args->nickname_str;

    tui_setup();
    existing_connections = array_new(INIT_CONNECTION_ARRAY_SIZE);
    if(args->address == 0){
        connection_setup_listen_socket(args->address_info);
    } else {
        connection_setup_external_peer(args->address_info);
    }
    while(1){
        thrd_yield();
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