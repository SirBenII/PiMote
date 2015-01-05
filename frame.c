#include <curses.h>
#include "main.h"
#include "frame.h"

int main_y,main_x,sub_y,sub_x;
WINDOW *main_window;
WINDOW *stay_window;
WINDOW *sub_window;

void frame_print(const char *msg, int clear, int center){
	if(clear){
		wclear(sub_window);
		if(center){
			mvwprintw(sub_window,(main_y/4)-1,2,msg);
		}else{
			mvwprintw(sub_window,2,2,msg);
		}
	}else{
		wprintw(sub_window, msg);
	}
	frame_refresh();
}

void frame_values(char *msg, int row, int line){
	mvwprintw(sub_window,line,row,msg);
	frame_refresh();
}

void frame_stay(char *msg, char* pad_frame, int axis_frame, int buttons_frame){
	mvwprintw(stay_window,2,2,msg,pad_frame,axis_frame,buttons_frame);
	wrefresh(stay_window);
	frame_refresh();
}

void frame_refresh(){
    box(sub_window,0,0);
  	wrefresh(sub_window);
}

void frame_init(){
    initscr();
    noecho();
    curs_set(FALSE);
    getmaxyx(stdscr,main_y,main_x);
    sub_y = main_y/3;
    sub_x = main_x/4;
    main_window = newwin(main_y, main_x, 0, 0);
    stay_window = newwin(main_y/4, main_x/8, main_y/16, main_x/2-main_x/16);
    sub_window = newwin(main_y,main_x/2,sub_y,sub_x);
    start_color();    
    init_pair(1, COLOR_RED, COLOR_WHITE);
    wbkgd(main_window,COLOR_PAIR(1));
    wbkgd(stay_window,COLOR_PAIR(1));
    wbkgd(sub_window,COLOR_PAIR(1));
    box(main_window,0,0);
    box(sub_window,0,0);
    mvwprintw(main_window,0,(main_x/2)-strlen(TITLE)/2,TITLE);
    mvwprintw(main_window,main_y-1,(main_x/2)-strlen(COPY)/2,COPY);
    wrefresh(main_window);
    wrefresh(sub_window);
}