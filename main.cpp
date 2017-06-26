#include <ncurses.h>
#include "Engine.hpp"

int main()
{
	initscr();			/* Start curses mode 		*/
	start_color();			/* Start the color functionality */
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	curs_set(0);


	int input;
	Engine *engine_pointer;
	do
	{
		clear();
		refresh();
		engine_pointer=new Engine;
		engine_pointer->initialize_game();
		delete engine_pointer;
		input=getch();
	}while(input=='r');

	endwin();
	return 0;
}
