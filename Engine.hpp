<<<<<<< HEAD
#ifndef ENGINE_HPP
#define ENGINE_HPP
=======
#ifndef ENGINE_H
#define ENGINE_H
>>>>>>> ab81c90c4719b570b1d1b48d3e2ac5edd6ff63df

#include <ncurses.h>
#include <tuple>
#include <chrono>
#include <thread>
#include <queue>
<<<<<<< HEAD
#include <utility>
#include <deque>
#include <algorithm>
#include <cstdlib>
#include <atomic>
#include <mutex>
#include <condition_variable>
=======
#include <exception>
#include <utility>
#include <deque>
#include <algorithm>
#include <atomic>
#include <cstdlib>
>>>>>>> ab81c90c4719b570b1d1b48d3e2ac5edd6ff63df

class Engine
{
private:
	WINDOW *local_win;
<<<<<<< HEAD
	void set_values();		//Sets the parameters(width,height,starting positions etc.) of the windows that's going be created next.
	void create_new_win();		//Creates a new window and boxes it.
	void initialize_snake();	//Initializes the snake by placing it in within the box and setting its values(back_dir,tip_dir,length,speed etc.).
	void move();			//Moves the snake. Also spawns food when needed.
	void get_input();		//Executed in a seperate thread. Constantly waits for input(up,right etc.) from the user.
	void grow(int);			//increases the 'grow_amount' member by the amount passed to it as argument.
	void end_game();
	std::pair<int,int> coordinate_generator();		//Generates a random coordinate and returns it.
	std::pair<int,int> spawn_food();			//Spawns food and returns its coordinates.
	std::pair<int,int> snake_tip_pos;			//Stores snake's tip position. Constantly updated as the snake travels around.
	std::pair<int,int> snake_back_pos;			//Stores snake's back position. Constantly updated as the snake travels around.
	enum directions{left,right,up,down};
	directions back_dir; 	//snake's back's direction. This will be set 'right' initially.
	directions tip_dir;	 //snake's tip's direction. This will be set 'right' initially.
	int length;			//snake's length
	int snake_speed; 	//snake's move period in milliseconds.
	int grow_amount;	//this is needed so the engine can know if the snake needs to grow. This is done by checking grow_amount every time before the snake moves.
	int height,width,start_y,start_x,max_x,max_y;		//height of the windows, width of the window, starting positions of the window, total columns of the terminal, total rows of the terminal.
	std::queue<std::tuple<int,int,directions>> turning_points; 	// this queue will store the turning points of the snake's movement. tuple's format is (y,x,direction)
	std::deque<std::pair<int,int>> node_positions; 	// stores every single node of the snake's position. this is needed for collision detection. (y,x)
	int base_food_score;
	int total_score;
	int input;	//this is the variable that'll be used when reading input. this is declared here because it'll be used in more than one thread simultaneously.
	std::atomic<bool> is_game_over;	//this is used to notify the input thread if it needs to return.
	std::mutex main_mutex;
	bool is_condition_met;
	std::condition_variable main_cv;

public:
	void initialize_game();
};

#endif
=======
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
>>>>>>> ab81c90c4719b570b1d1b48d3e2ac5edd6ff63df
