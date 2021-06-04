#pragma once
#include <stdlib.h>
#include <arpa/inet.h>
typedef struct _Array{
    size_t size;
    size_t capacity;
    struct sockaddr_in *buff;
}Array;

void array_init(Array* arr,size_t init_size);
void array_add(Array* arr,const struct sockaddr_in *s);
void array_remove(Array* arr,const struct sockaddr_in s);