#include <ncurses.h>
#include <stdlib.h>
#include "arguments.h"
#include "array.h"
int main(int argc, char* argv[]){
    Arguments* args = parse_arguments(argc,argv);
    Array arr;
    array_init(&arr,0);
    struct sockaddr_in s;
    for(int i = 0; i<1000;i++){
        array_add(&arr,&s);
    }
    /*
    initscr();
    int h,w;
    getmaxyx(stdscr,h,w);
    WINDOW* input = newwin(2,w-1,h-3,0);
    WINDOW* output = newwin(h-3,w-1,0,0);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    wbkgd(input, COLOR_PAIR(1));
    wbkgd(output, COLOR_PAIR(2));    
    scrollok(output, TRUE);
    scrollok(input, TRUE);
    
    
    wclear(input);
    wmove(input, 0, 0);
    wclear(output);
    wrefresh(input);
    wrefresh(output);
    char str[1024];
    while(1){
        wgetnstr(input,str,1024);
        waddstr(output,str);
        wrefresh(output);
        wclear(input);
        wmove(input, 0, 0);
        wrefresh(input);
    }
*/
}