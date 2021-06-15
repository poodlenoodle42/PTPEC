#pragma once
#include <stddef.h>
#define DEFAULT_PORT 16103
#define MAX_CONNECTION_QUEUE 10
#define PBKDF_ITERATIONS 1000
#define INIT_CONNECTION_ARRAY_SIZE 20
#define _GNU_SOURCE
void strrnd(char * str, size_t len);