#pragma once

#define tui_write_default(str,...) tui_write(0,str __VA_OPT__(,) __VA_ARGS__)
#define tui_write_error(str,...) tui_write(1,str __VA_OPT__(,) __VA_ARGS__)
void tui_setup();
void tui_write(int error,const char* str,...);