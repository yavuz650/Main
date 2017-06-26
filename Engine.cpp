#include "Engine.hpp"

void Engine::set_values()
{
	getmaxyx(stdscr,max_y,max_x);
	height=max_y*5/6;		//set the dimensions of the box.
	width=max_x*5/6;
	start_y=(max_y-height)/2;		//determine the starting point(top left) of the box.
	start_x=(max_x-width)/2;
	total_score=0;
	is_game_over=false;
	is_condition_met=false;
}

void Engine::create_new_win()
{
	
	local_win=newwin(height,width,start_y,start_x);
	box(local_win,0,0);			// this is where the magic happens.
	wrefresh(local_win);
}

void Engine::initialize_game()
{
	this->set_values();
	mvaddstr(max_y/2-3,max_x/2-6,"Select difficulty.");				//difficulty settings alter the snake's movement speed and the base food score.
	mvaddstr(max_y/2-2,max_x/2-6,"Press 'F1' for 'Easy'");
	mvaddstr(max_y/2-1,max_x/2-6,"Press 'F2' for 'Medium'");
	mvaddstr(max_y/2,max_x/2-6,"Press 'F3' for 'Hard'");
	refresh();
	int input;
	while(true)
	{
		input=getch();
		if(input==KEY_F(1))
		{
			snake_speed=120;
			base_food_score=3;
			break;
		}
		else if(input==KEY_F(2))
		{
			snake_speed=80;
			base_food_score=5;
			break;
		}
		else if(input==KEY_F(3))
		{
			snake_speed=55;
			base_food_score=8;
			break;			
		}
	}
	mvprintw(1,max_x/2,"Score: %d",total_score);
	mvprintw(0,0,"Press 'q' to end.");
	this->create_new_win();
	this->initialize_snake();
	std::thread input_thread(&Engine::get_input,this);  //create a thread an execute get_input function.
	input_thread.detach();
	this->move();
}

void Engine::end_game()
{
	is_game_over=true;
	for(int i=0;i<length;i++)
	{
		mvwchgat(local_win,node_positions[i].first,node_positions[i].second,1,A_BLINK,1,NULL);
	}
	wrefresh(local_win);
	mvprintw(0,max_x/2-6,"GAME OVER. Press 'Enter' to continue.");
	refresh();
	std::unique_lock<std::mutex> local_lock(main_mutex);
	main_cv.wait(local_lock,[&](){return is_condition_met;});
	delwin(local_win);
	clear();
	refresh();
	mvprintw(max_y/2,max_x/2-4,"Your score: %d",total_score);
	mvprintw(max_y/2+1,max_x/2-4,"Press 'R' to restart the game.");
	mvprintw(max_y/2+2,max_x/2-4,"Press any other key to quit.");
	refresh();
}

void Engine::initialize_snake()
{
	int snake_start_y=max_y/2;		//determine the starting point of the snake.
	int snake_start_x=start_x+width/5;
	char snake[]= "+++++++++++++++++++++";
	mvwprintw(local_win,snake_start_y,snake_start_x,"%s",snake);
	length=sizeof(snake)/sizeof(snake[0])-1;
	for(int x=snake_start_x,y=snake_start_y,i=0;i<length;i++,x++)
	{
		node_positions.emplace_front(std::make_pair(y,x));	//initialize node positions.
	}
	wrefresh(local_win);
	back_dir=right;
	tip_dir=right;
	grow_amount=0;
	snake_back_pos=std::make_pair(snake_start_y,snake_start_x);
	snake_tip_pos=std::make_pair(snake_start_y,snake_start_x+length-1);		//coordinates of the tip.  (y,x) format.
}
void Engine::move()
{
	std::pair<int,int> food_position=this->spawn_food();
	while(!is_game_over)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(snake_speed));

		if(grow_amount==0)
		{
			mvwaddch(local_win,snake_back_pos.first,snake_back_pos.second,' '); //delete snake's end
			if(!turning_points.empty())		//if the queue is not empty, this means there is at least one turning point.
			{
				if(snake_back_pos.first==std::get<0>(turning_points.front()) && snake_back_pos.second==std::get<1>(turning_points.front()))	//if the snake's back position is on a turning point, then change back's direction
				{
					back_dir=std::get<2>(turning_points.front());	//change direction.
					turning_points.pop();	//snake has turned. remove the turning point.
				}
			}
			switch(back_dir) //update end's position
			{
				case left:
				snake_back_pos.second-=1;
				break;
				case right:
				snake_back_pos.second+=1;
				break;
				case up:
				snake_back_pos.first-=1;
				break;
				case down:
				snake_back_pos.first+=1;
				break;
			}
			node_positions.pop_back();
		}
		else
		{ 
			grow_amount--,length++;
		}

		switch(tip_dir)
		{
			case left:
			{
				auto it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(snake_tip_pos.first,snake_tip_pos.second-1)); //before advancing the tip, we need to see if it collides with other parts of the snake.
				if(it!=node_positions.end() || snake_tip_pos.second+start_x-1==start_x) 
				{
					this->end_game();	//collision detected. end the game.
					return;
				}	
				if(std::make_pair(snake_tip_pos.first,snake_tip_pos.second-1)==food_position) 
				{
					this->grow(2);
					total_score+=base_food_score;	//ate food, increase score.
					mvprintw(1,max_x/2,"Score: %d",total_score); //re-print score.
					food_position=this->spawn_food();	//respawn another food.
				} 
				mvwaddch(local_win,snake_tip_pos.first,snake_tip_pos.second-1,'+');	//extend the tip
				snake_tip_pos.second-=1;
				break;
			}
			case right:
			{
				auto it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(snake_tip_pos.first,snake_tip_pos.second+1));
				if(it!=node_positions.end() || snake_tip_pos.second+start_x+2==(start_x+width)) 
				{
					this->end_game();
					return;
				}
				if(std::make_pair(snake_tip_pos.first,snake_tip_pos.second+1)==food_position) 
				{
					this->grow(2);
					total_score+=base_food_score;
					mvprintw(1,max_x/2,"Score: %d",total_score);
					food_position=this->spawn_food();
				} 
				mvwaddch(local_win,snake_tip_pos.first,snake_tip_pos.second+1,'+');
				snake_tip_pos.second+=1;
				break;
			}
			case down:
			{
				auto it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(snake_tip_pos.first+1,snake_tip_pos.second));
				if(it!=node_positions.end() || snake_tip_pos.first+start_y+2==(start_y+height)) 
				{
					this->end_game();
					return;
				}
				if(std::make_pair(snake_tip_pos.first+1,snake_tip_pos.second)==food_position) 
				{
					this->grow(2);
					total_score+=base_food_score;
					mvprintw(1,max_x/2,"Score: %d",total_score);
					food_position=this->spawn_food();
				} 
				mvwaddch(local_win,snake_tip_pos.first+1,snake_tip_pos.second,'+');
				snake_tip_pos.first+=1;
				break;
			}
			case up:
			{
				auto it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(snake_tip_pos.first-1,snake_tip_pos.second));
				if(it!=node_positions.end() || snake_tip_pos.first+start_y-1==start_y) 
				{
					this->end_game();
					return;
				}
				if(std::make_pair(snake_tip_pos.first-1,snake_tip_pos.second)==food_position) 
				{
					this->grow(2);
					total_score+=base_food_score;
					mvprintw(1,max_x/2,"Score: %d",total_score);
					food_position=this->spawn_food();
				} 
				mvwaddch(local_win,snake_tip_pos.first-1,snake_tip_pos.second,'+');
				snake_tip_pos.first-=1;
				break;
			}
		}
		node_positions.emplace_front(snake_tip_pos.first,snake_tip_pos.second);
		refresh();
		wrefresh(local_win);
	}
}
void Engine::get_input()
{
	while(true)
	{	
		std::this_thread::sleep_for(std::chrono::milliseconds(55));
		input=getch();

		if(is_game_over)
		{
			if(input==10) //10==KEY_ENTER
			{
				std::lock_guard<std::mutex> local_lock(main_mutex);
				is_condition_met=true;
				main_cv.notify_all();
				return;
			}
		}
		else if(input=='q')
		{
			this->end_game();
		}
		else if(tip_dir==right || tip_dir==left)		//snake is moving in a horizontal path, thus it can only turn up or down
		{
			switch(input)
			{
				case KEY_UP:
				turning_points.emplace(std::make_tuple(snake_tip_pos.first,snake_tip_pos.second,up));
				tip_dir=up;
				break;
				case KEY_DOWN:
				turning_points.emplace(std::make_tuple(snake_tip_pos.first,snake_tip_pos.second,down));
				tip_dir=down;
				break;
			}
		}
		else if(tip_dir==up || tip_dir==down)
		{
			switch(input)
			{
				case KEY_RIGHT:
				turning_points.emplace(std::make_tuple(snake_tip_pos.first,snake_tip_pos.second,right));
				tip_dir=right;
				break;
				case KEY_LEFT:
				turning_points.emplace(std::make_tuple(snake_tip_pos.first,snake_tip_pos.second,left));
				tip_dir=left;
				break;
			}
		}
	}
} 

void Engine::grow(int num)
{
	grow_amount+=num;
}

std::pair<int,int> Engine::coordinate_generator()
{
	int coor_x,coor_y;
	std::deque<std::pair<int,int>>::iterator it;
	do
	{
		coor_y=rand()%(height-2)+1;
		coor_x=rand()%(width-2)+1;
		it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(coor_y,coor_x));
	}while(it!=node_positions.end());  //checks if the point(coor_x,coor_y) is overlapped by the snake. if so, re-generates new numbers.

	return std::make_pair(coor_y,coor_x);
}
std::pair<int,int>  Engine::spawn_food()
{
	std::pair<int,int> food_coordinates=this->coordinate_generator();
	mvwaddch(local_win,food_coordinates.first,food_coordinates.second,'$');

	return food_coordinates;
} 