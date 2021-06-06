#pragma once
#include <arpa/inet.h>
#include "tui.h"
#define SOCKET_ERROR_DIE(val, msg, ...) \
if((val) < 0){\
    tui_write_error(msg __VA_OPT__(,) __VA_ARGS__); \
    exit(1);\
}

#define SOCKET_ERROR(val, msg, ...) \
if((val) < 0){\
    tui_write_error(msg __VA_OPT__(,) __VA_ARGS__); \
}

void connection_setup_listen_socket(struct sockaddr_in address);