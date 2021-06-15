#include "tui.h"
#include <ncurses.h>
#include <threads.h>
WINDOW* input;
WINDOW* output;
mtx_t output_lock;
void tui_setup(){
    mtx_init(&output_lock, mtx_plain);
    initscr();
    int h,w;
    getmaxyx(stdscr,h,w);
    input = newwin(2,w-1,h-3,0);
    output = newwin(h-3,w-1,0,0);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(4,COLOR_CYAN,COLOR_BLACK);
    wbkgd(input, COLOR_PAIR(1));
    wbkgd(output, COLOR_PAIR(2));    
    scrollok(output, TRUE);
    scrollok(input, TRUE);
    keypad(input, TRUE);
    nocbreak();
    wclear(input);
    wmove(input, 0, 0);
    wclear(output);
    wrefresh(input);
    wrefresh(output);
}


void tui_write(int error ,const char* str,...){
    mtx_lock(&output_lock);
    va_list args;
    va_start(args, str);
    if(error == 0){
        vw_printw(output,str,args);
        wrefresh(output);
    } else {
        wattron(output,A_BOLD | COLOR_PAIR(error + 2));
        vw_printw(output,str,args);
        wrefresh(output);
        wattron(output, A_NORMAL | COLOR_PAIR(2));
    }
    va_end(args);
    mtx_unlock(&output_lock);
}

void tui_read(char* buf,size_t size){
    wgetnstr(input,buf,size);
    wclear(input);
    wmove(input, 0, 0);
    wrefresh(input);
}