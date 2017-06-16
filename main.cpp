#include <ncurses.h>
#include "Engine.h"

int main()
{
	Engine snakeEngine;
	initscr();			/* Start curses mode 		*/
	start_color();			/* Start the color functionality */
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		
	noecho();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	attron(COLOR_PAIR(1));
	refresh();
	curs_set(0);
	snakeEngine.initialize_game();
	snakeEngine.end_game();
	return 0;
}
