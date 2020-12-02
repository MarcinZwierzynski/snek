#include "snake.h"

struct termios t;

void input_enter_off()
{
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void input_enter_on()
{
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void input_thread_work(void *arg)
{
    struct Snek *snek = (struct Snek *)arg;
    while (true)
    {
        enum User_input direction = get_user_input();
        snek->update_direction(direction);
    }
}

Snek::Snek(void)
{
    input_enter_off();
    std::thread thread (input_thread_work, this);
    thread.detach();
    std::pair<int,int> aPair;
    aPair.first = 10;
    aPair.second = 10;
    snek_parts.push_back(aPair);
    
    aPair.first = 9;
    aPair.second = 9;
    snek_parts.push_back(aPair);
    
    aPair.first = 8;
    aPair.second = 8;
    snek_parts.push_back(aPair);
    this->score = 1;

    food = std::make_pair(std::rand()%20, std::rand()%20);
}

Snek_map::Snek_map(Snek *snek)
{
    this->snek = snek;
    std::cout << "Snek_map constructor function" << std::endl;
    clear_map(this->snek_map);
    snek->food = std::make_pair(std::rand()%20, std::rand()%20);
}

enum User_input get_user_input(void)
{
    enum User_input result = East;
    char user_input = getchar();
    switch (user_input)
    {
    case 'a':
        result = West;
        break;
    case 'w':
        result = North;
        break;
    case 'd':
        result = East;
        break;
    case 's':
        result = South;
        break;
    default:
        result = Error;
        std::cout << "Incorrect button clicked (" << user_input << ")" << std::endl;
        break;
    }
    return result;
}

void Snek::update_direction(enum User_input direction)
{
    switch (direction)
    {
    case West:
        if (this->direction != East)
        {
            this->direction = direction;
        }
        break;
    case North:
        if (this->direction != South)
        {
            this->direction = direction;
        }
        break;
    case East:
        if (this->direction != West)
        {
            this->direction = direction;
        }
        break;
    case South:
        if (this->direction != North)
        {
            this->direction = direction;
        }
        break;
    }
}

void clear_map(char map_array[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            map_array[i][j] = MAP_CHAR;
        }
    }
}

void Snek_map::update_snek_food(bool request_update)
{
    if(request_update==1)
    {
        snek->food = std::make_pair(std::rand()%20, std::rand()%20);
      //  snek->snek_parts.push_back(std::make_pair(snek->snek_parts[0].first,snek->snek_parts[0].second));
    
        switch (snek->return_dir())
        { 
            case West:
                snek->snek_parts.insert(snek->snek_parts.end(), std::make_pair(snek->snek_parts[0].first,snek->snek_parts[0].second-1));
                //snek_parts[0].second -= 1;
                break;
            case North:
                snek->snek_parts.insert(snek->snek_parts.end(), std::make_pair(snek->snek_parts[0].first-1,snek->snek_parts[0].second));
                //snek_parts[0].first -= 1;
                break;
            case East:
                snek->snek_parts.insert(snek->snek_parts.end(), std::make_pair(snek->snek_parts[0].first,snek->snek_parts[0].second+1));
                //snek_parts[0].second += 1;
                break;
            case South:
                snek->snek_parts.insert(snek->snek_parts.end(), std::make_pair(snek->snek_parts[0].first+1,snek->snek_parts[0].second));
                //snek_parts[0].first += 1;
                break;
        }
    }
}

void Snek_map::redraw_map()
{
    clear_map(this->snek_map);
    snek->update_movement();
    this->update_snek_body();
    if(check_if_game_over())
    {
        std::cout<< "GAME OVER" << std::endl;
        exit(1);
    }
    for(int i = 0; i < snek->snek_parts.size(); i++)
    {
        // std::cout << snek->snek_parts[i].first << ", " << snek->snek_parts[i].second << std::endl;
        snek_map[snek->snek_parts[i].first][snek->snek_parts[i].second] = SNAKE_CHAR;
    }
    if((snek->snek_parts[0].first == snek->food.first) & (snek->snek_parts[0].second == snek->food.second))
    {
        update_snek_food(1);
        snek->score++;
    }
    enum User_input dir = snek->return_dir();
    for (int i = 0; i < MAP_END; i++)
    {
        std::cout << std::endl;
    }
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if(i == snek->snek_parts[0].first & j == snek->snek_parts[0].second)
            {
                if(dir == East)
                {
                    std::cout << SNAKE_HEAD_WEST<< ".";
                }
                else if(dir == West)
                {
                    std::cout << SNAKE_HEAD_EAST<< ".";
                }
                else if(dir == North)
                {
                    std::cout << SNAKE_HEAD_SOUTH<< ".";
                }
                else if(dir == South)
                {
                    std::cout << SNAKE_HEAD_NORTH<< ".";
                }
            }
            else if(i == snek->food.first & j == snek->food.second)
            {
                std::cout << SNAKE_FOOD <<  snek_map[i][j];
            }
            else
            {
                std::cout << snek_map[i][j] << ".";
            }
        }
       std::cout << std::endl;
    }
    std::cout << "SCORE: " << snek->score << std::endl;
}

void Snek::update_movement(void)
{
    std::pair<int, int> movement_part;
    enum User_input direction = this->return_dir();
    //snek_parts[0].first += 10;
    switch (direction)
    {
        case West:
            snek_parts[0].second -= 1;
            break;
        case North:
            snek_parts[0].first -= 1;
            break;
        case East:
            snek_parts[0].second += 1;
            break;
        case South:
            snek_parts[0].first += 1;
            break;
    }
    //snek_map[snek->snek_parts[snek_parts.size()].first][snek->snek_parts[snek_parts.size()].second] = SNAKE_CHAR;
    // for(int i = 1; i < snek_parts.size(); i++)
    // {
    //     snek_map[snek->snek_parts[i].first][snek->snek_parts[i].second] = SNAKE_CHAR;
    //   //  snek_map[snek->snek_parts[i].first][snek->snek_parts[i].second] = SNAKE_CHAR;
    //   //  std::cout << snek_parts[i].first << ", " << snek_parts[i].second << std::endl;
    // }
}

void Snek_map::update_snek_body(void)
{
    //std::cout << snek->snek_parts.size() << std::endl;
    if(snek->snek_parts.size()>=2)
    {
        snek->snek_parts.erase(snek->snek_parts.end());
        snek->snek_parts.insert((snek->snek_parts.begin()+1), std::make_pair(snek->snek_parts[0].first,snek->snek_parts[0].second) );    
    }
 
}

bool Snek_map::check_if_game_over()
{
    if((snek->snek_parts[0].first == MAP_WIDTH) || (snek->snek_parts[0].second == MAP_HEIGHT))
    {
        return 1;
    }
    else if((snek->snek_parts[0].first == -1) || (snek->snek_parts[0].second == -1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}