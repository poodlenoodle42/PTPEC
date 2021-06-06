#include "common.h"
#include <stdlib.h>
void strrnd(char * str, size_t len){
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        

    if (len) {
        if (str) {            
            for (int n = 0;n < len - 1;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                str[n] = charset[key];
            }
            str[len] = '\0';
        }
    }
}
