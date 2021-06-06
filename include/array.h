#pragma once
#include <stdlib.h>
#include <arpa/inet.h>
#include <threads.h>

typedef struct _Client_Info{
    
    struct sockaddr_in addr;
    int socket;
}Client_Info;

typedef struct _Array{
    mtx_t lock;
    size_t size;
    size_t capacity;
    Client_Info *buff;
}Array;

Array* array_new(size_t init_size);
void array_add(Array* arr,const Client_Info *s);
void array_remove(Array* arr,const Client_Info s);
void array_destroy(Array* arr);