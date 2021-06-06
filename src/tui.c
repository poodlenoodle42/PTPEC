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
    wbkgd(input, COLOR_PAIR(1));
    wbkgd(output, COLOR_PAIR(2));    
    scrollok(output, TRUE);
    scrollok(input, TRUE);
    
    
    wclear(input);
    wmove(input, 0, 0);
    wclear(output);
    wrefresh(input);
    wrefresh(output);
}


void tui_write(const char* str, int error){
    mtx_lock(&output_lock);
    if(error == 0){
        waddstr(output,str);
        wrefresh(output);
    } else {
        int c;
        for(c = (int)*str;(int)c != '\0';str++){
            waddch(output,c | A_BOLD | COLOR_RED);
        }
        wrefresh(output);
    }
    mtx_unlock(&output_lock);
}