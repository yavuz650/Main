#ifndef ENGINE_H
#define ENGINE_H

#include <ncurses.h>
#include <tuple>
#include <chrono>
#include <thread>
#include <queue>
#include <exception>
#include <utility>
#include <deque>
#include <algorithm>
#include <atomic>
#include <cstdlib>

class Engine
{
private:
	WINDOW *local_win;
	void create_new_win();
	void move();
	void get_input();
	void initialize_snake();
	void set_values();
	void grow(int);
	std::pair<int,int> coordinate_generator();
	std::pair<int,int> spawn_food();
	std::pair<int,int> snake_tip_pos;
	std::pair<int,int> snake_back_pos;
	enum directions{left,right,up,down};
	directions back_dir,tip_dir;
	int length;
	std::atomic<int> grow_amount;
	int height,width,start_y,start_x,max_x,max_y;
	std::queue<std::tuple<int,int,directions>> turning_points; 	// this queue will store the turning points of the snake's movement. tuple's format is (y,x,direction)
	std::deque<std::pair<int,int>> node_positions; 	// stores every single node of the snake's position. this is needed for collision detection. (y,x)
public:
	void end_game();
	void initialize_game();
};

#endif
