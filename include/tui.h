#pragma once

#define tui_write_default(str) tui_write(str,0)
#define tui_write_error(str) tui_write(str,1)
void tui_setup();
void tui_write(const char* str, int error);