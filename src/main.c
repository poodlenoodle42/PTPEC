#include <ncurses.h>
#include <stdlib.h>
#include "arguments.h"
#include "array.h"
#include "tui.h"
#include <crypt.h>
#include <time.h>
#include "connection.h"
#include "common.h"
int main(int argc, char* argv[]){
    srand(time(NULL));
    Arguments* args = parse_arguments(argc,argv);
    tui_setup();
    existing_connections = array_new(INIT_CONNECTION_ARRAY_SIZE);
    if(args->address == 0){
        connection_setup_listen_socket(args->address_info);
    } else {
        
    }
    const char password[] = "password123";
    char* salt = crypt_gensalt("$5$",0,NULL,0);
    printf("Password: %s\n",password);
    printf("Salt: %s\n",salt);
    char* encrypted = crypt(password,salt);
    printf("Encrypted: %s\n",encrypted);

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