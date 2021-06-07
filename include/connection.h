#pragma once
#include <arpa/inet.h>
#include "tui.h"
#include "array.h"
#define SOCKET_ERROR_DIE(val, msg, ...) \
if((val) < 0){\
    tui_write_error(msg __VA_OPT__(,) __VA_ARGS__); \
    exit(1);\
}

#define SOCKET_ERROR(val, msg, ...) \
if((val) < 0){\
    tui_write_error(msg __VA_OPT__(,) __VA_ARGS__); \
    return 0; \
}

typedef struct _Conn_Info{
    Client_Info client_info;
    int challenge_passed;
}Connection_Info;
extern char* nickname;
extern Array* existing_connections;
void connection_setup_listen_socket(struct sockaddr_in address);
int connection_setup_external_peer(struct sockaddr_in address);
int handle_connection(void* connection_info);