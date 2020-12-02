#include "snake.h"

class Snek snek;
class Snek_map snek_map(&snek);

int main()
{
    //std::cout << "Hello world" << std::endl;
    snek.snek_vector.push_back(std::make_pair(INIT_SNEK_X, INIT_SNEK_Y));

    while(true){
        snek_map.redraw_map();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
       // snek_map.redraw_map();
    }
}