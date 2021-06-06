#pragma once

#define tui_write_default(str,...) tui_write(str,0 __VA_OPT__(,) __VA_ARGS__)
#define tui_write_error(str,...) tui_write(str,1 __VA_OPT__(,) __VA_ARGS__)
void tui_setup();
void tui_write(const char* str, int error,...);