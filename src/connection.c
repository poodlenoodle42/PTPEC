#include "connection.h"
#include <threads.h>
#include "tui.h"
#include <sys/socket.h>
#include "common.h"
thrd_t server_thread;

void* listen_and_serve(void* v){
    int server_socket = *(int*)v;
    int client_socket;
    struct sockaddr_in client_addr;
    while(1){
        client_socket = accept(server_socket,
        (struct sockaddr*)&client_addr,sizeof(client_addr));
        if(client_socket == -1){
            tui_write_error("Failed to accept connection from to %s\n",inet_ntoa(client_addr));
        } else {
            //Make handshake and put connection in diffrent thread
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