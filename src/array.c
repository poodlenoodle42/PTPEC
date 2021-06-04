#include "array.h"
#include <memory.h>
#include <stdio.h>
void array_init(Array* arr,size_t init_size){
    if(init_size < 10)
        init_size = 10;
    arr->capacity = init_size;
    arr->size = 0;
    arr->buff = malloc(init_size*sizeof(struct sockaddr_in));
}

void array_add(Array* arr,const struct sockaddr_in *s){
    arr->buff[arr->size] = *s;
    arr->size += 1;
    if(arr->size + 1 == arr->capacity){
        struct sockaddr_in *temp = realloc(arr->buff,arr->capacity * 2 * sizeof(struct sockaddr_in));
        if(temp == NULL){
            fputs("Failed to allocate memory \n",stderr);
            exit(1);
        }
        arr->buff = temp;
        arr->capacity *= 2;
    }
}
void array_remove(Array* arr,const struct sockaddr_in s){
    int i;
    for(i = 0; i<arr->size;++i){
        if(memcmp(&s,&arr->buff[i],sizeof(struct sockaddr_in)) == 0)
            break;
    }
    arr->size--;
    memcpy(&arr->buff[i],&arr->buff[i+1],(arr->size-i)*sizeof(struct sockaddr_in));
}