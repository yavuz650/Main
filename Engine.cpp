#include "Engine.h"

void Engine::set_values()
{
	getmaxyx(stdscr,max_y,max_x);
	height=max_y*5/6;		//set the dimensions of the box.
	width=max_x*5/6;
	start_y=(max_y-height)/2;		//determine the starting point(top left) of the box.
	start_x=(max_x-width)/2;
}

void Engine::create_new_win()
{
	
	local_win=newwin(height,width,start_y,start_x);
	box(local_win,0,0);			// this is where the magic happens.
	wrefresh(local_win);
}

void Engine::end_game()
{
	for(int i=0;i<length;i++)
	{
		mvwchgat(local_win,node_positions[i].first,node_positions[i].second,1,A_BLINK,1,NULL);
	}
	wrefresh(local_win);
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	delwin(local_win);
	endwin();
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
	back_dir=right,tip_dir=right;
	grow_amount=0;
	snake_back_pos=std::make_pair(snake_start_y,snake_start_x);
	snake_tip_pos=std::make_pair(snake_start_y,snake_start_x+length-1);		//coordinates of the tip.  (y,x) format.
}
void Engine::move()
{
	std::pair<int,int> food_position=this->spawn_food();
	std::thread input_thread(&Engine::get_input,this);
	while(true)
	{
		refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(75));
		//delete snake's end
		if(grow_amount==0)
		{
			mvwaddch(local_win,snake_back_pos.first,snake_back_pos.second,' ');
			if(!turning_points.empty())		//if the queue is not empty, this means there is at least one turning point.
			{
				if(snake_back_pos.first==std::get<0>(turning_points.front()) && snake_back_pos.second==std::get<1>(turning_points.front()))	//if the snake's back position is on a turning point, then change back's direction
				{
					back_dir=std::get<2>(turning_points.front());	//change direction.
					turning_points.pop();	//snake has turned. remove the turning point.
				}
			}
			//update end's position
			switch(back_dir)
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
		//extend the tip and update its position
		switch(tip_dir)
		{
			case left:
			{
				auto it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(snake_tip_pos.first,snake_tip_pos.second-1)); //before advancing the tip, we need to see if it collides with other parts of the snake.
				if(it!=node_positions.end() || snake_tip_pos.second+start_x-1==start_x) {this->end_game();}	//collision detected. end the game.
				if(std::make_pair(snake_tip_pos.first,snake_tip_pos.second-1)==food_position) {this->grow(2);food_position=this->spawn_food();} //ate a food? then grow... respawn another food.
				mvwaddch(local_win,snake_tip_pos.first,snake_tip_pos.second-1,'+');
				snake_tip_pos.second-=1;
				break;
			}
			case right:
			{
				auto it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(snake_tip_pos.first,snake_tip_pos.second+1));
				if(it!=node_positions.end() || snake_tip_pos.second+start_x+2==(start_x+width)) {this->end_game();}
				if(std::make_pair(snake_tip_pos.first,snake_tip_pos.second+1)==food_position) {this->grow(2);food_position=this->spawn_food();} 
				mvwaddch(local_win,snake_tip_pos.first,snake_tip_pos.second+1,'+');
				snake_tip_pos.second+=1;
				break;
			}
			case down:
			{
				auto it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(snake_tip_pos.first+1,snake_tip_pos.second));
				if(it!=node_positions.end() || snake_tip_pos.first+start_y+2==(start_y+height)) {this->end_game();}
				if(std::make_pair(snake_tip_pos.first+1,snake_tip_pos.second)==food_position) {this->grow(2);food_position=this->spawn_food();} 
				mvwaddch(local_win,snake_tip_pos.first+1,snake_tip_pos.second,'+');
				snake_tip_pos.first+=1;
				break;
			}
			case up:
			{
				auto it=std::find(node_positions.begin(),node_positions.end(),std::make_pair(snake_tip_pos.first-1,snake_tip_pos.second));
				if(it!=node_positions.end() || snake_tip_pos.first+start_y-1==start_y) {this->end_game();}
				if(std::make_pair(snake_tip_pos.first-1,snake_tip_pos.second)==food_position) {this->grow(2);food_position=this->spawn_food();} 
				mvwaddch(local_win,snake_tip_pos.first-1,snake_tip_pos.second,'+');
				snake_tip_pos.first-=1;
				break;
			}
		}
		node_positions.emplace_front(snake_tip_pos.first,snake_tip_pos.second);
		wrefresh(local_win);
	}
}
void Engine::get_input()
{
	int input;
	while(true)
	{	
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		input=getch();
		if(input=='q')
		{
			this->end_game();
		}
		if(tip_dir==right || tip_dir==left)		//snake is moving in a horizontal path, thus it can only turn up or down
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
		else
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

void Engine::initialize_game()
{
	this->set_values();
	this->create_new_win();
	this->initialize_snake();
	this->move();
}

