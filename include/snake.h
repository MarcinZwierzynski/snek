#ifndef _SNEK_H
#define _SNEK_H

#include <thread>
#include <vector>
#include <stdio.h>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <termios.h>


const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;
const char MAP_CHAR = '.';
const int MAP_END = 50;
const char SNAKE_HEAD_WEST ='>'; 
const char SNAKE_HEAD_NORTH = 'v';
const char SNAKE_HEAD_EAST = '<';
const char SNAKE_HEAD_SOUTH = '^';
const char SNAKE_CHAR = 'o';
const char SNAKE_FOOD = 'x';
const int INIT_SNEK_X = 10;
const int INIT_SNEK_Y = 10;
 
enum User_input {
    Error = -1,
    West = 0,
    North = 1,
    East = 2,
    South = 3
};

class Snek
{
    public:
        Snek(); //Class sontructor called when the class instantiated
        std::vector<std::pair<int, int>> snek_vector; //Vector of snake pair coordinates
        void update_direction(enum User_input direction);
        bool food_eaten;
        std::vector<std::pair<int, int>> snek_parts;
        std::pair<int, int> food;
        enum User_input return_dir() {return direction;};
        void update_movement();
        uint16_t score;
    private:
        enum User_input direction;
};

class Snek_map
{
    public:
        Snek_map(Snek *snek);
        void redraw_map();
     //   std::pair<int,int> snek_food;
     //   void update_food();
     //   void update_score();
        void update_snek_food(bool request_update);
        void update_snek_body();
        bool check_if_game_over();
    private:
        char snek_map[MAP_HEIGHT][MAP_WIDTH];
        Snek* snek;
};

enum User_input get_user_input();
void clear_map(char map_array[MAP_HEIGHT][MAP_WIDTH]);

#endif