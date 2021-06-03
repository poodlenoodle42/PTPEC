#pragma once                                                                                                                                                                                                                                                                                                                                                                         
#include <stdio.h>                                                                                                                                                                                               
#include <stdlib.h>                                                                                                                                                                                              
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h>

typedef struct _Arguments
{
    int nickname;
    char* nickname_str;

    int pwd;
    char* pwd_str;

    int address;
    int port;
    struct sockaddr_in address_info;
} Arguments;

Arguments* parse_arguments(int argc, char* argv[]);