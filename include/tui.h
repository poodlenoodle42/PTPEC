#pragma once
#include <stddef.h>
#define tui_write_default(str,...) tui_write(0,str __VA_OPT__(,) __VA_ARGS__)
#define tui_write_green(str,...) tui_write(2,str __VA_OPT__(,) __VA_ARGS__)
#define tui_write_error(str,...) tui_write(1,str __VA_OPT__(,) __VA_ARGS__)
void tui_setup();
void tui_write(int error,const char* str,...);
void tui_read(char* buf,size_t size);