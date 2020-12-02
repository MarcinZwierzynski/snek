cmake_minimum_required(VERSION 3.5.1)

project(snek VERSION 0.1)

include_directories(include ../include)

add_executable(snek
                    src/main.cpp 
                    src/snake.cpp)

target_link_libraries(snek pthread)
#set(CMAKE_BUILD_TYPE Debug)