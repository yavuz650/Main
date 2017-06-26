#include <ncurses.h>
<<<<<<< HEAD
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
=======
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
>>>>>>> ab81c90c4719b570b1d1b48d3e2ac5edd6ff63df
	return 0;
}
