cmake_minimum_required(VERSION 3.10)
project(hw5)

set(CMAKE_CXX_STANDARD 11)

add_executable(hw5 main.cpp Player.cpp Weapon.cpp Game.cpp Game.h Weapon.h
        Player.h mtm_array.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall -pedantic-errors -Werror")

add_executable(main main.cpp Player.h Player.cpp Weapon.h Weapon.cpp Game.h
        Game.cpp mtm_array.h)

