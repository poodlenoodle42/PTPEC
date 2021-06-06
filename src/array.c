#include "array.h"
#include <memory.h>
#include <stdio.h>
Array* array_new(size_t init_size){
    Array* arr = malloc(sizeof(Array));
    mtx_init(&arr->lock,mtx_plain);
    if(init_size < 10)
        init_size = 10;
    arr->capacity = init_size;
    arr->size = 0;
    arr->buff = malloc(init_size*sizeof(Client_Info));
}

void array_add(Array* arr,const Client_Info *s){
    mtx_lock(&arr->lock);
    arr->buff[arr->size] = *s;
    arr->size += 1;
    if(arr->size + 1 == arr->capacity){
        Client_Info *temp = realloc(arr->buff,arr->capacity * 2 * sizeof(Client_Info));
        if(temp == NULL){
            fputs("Failed to allocate memory \n",stderr);
            exit(1);
        }
        arr->buff = temp;
        arr->capacity *= 2;
    }
    mtx_unlock(&arr->lock);
}
void array_remove(Array* arr,const Client_Info s){
    mtx_lock(&arr->lock);
    int i;
    for(i = 0; i<arr->size;++i){
        if(memcmp(&s,&arr->buff[i],sizeof(Client_Info)) == 0)
            break;
    }
    arr->size--;
    memcpy(&arr->buff[i],&arr->buff[i+1],(arr->size-i)*sizeof(Client_Info));
    mtx_unlock(&arr->lock);
}

void array_destroy(Array* arr){
    mtx_destroy(&arr->lock);
    free(arr);
}