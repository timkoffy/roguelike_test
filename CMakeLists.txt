cmake_minimum_required(VERSION 4.0)
project(roguelike_test)

set(CMAKE_CXX_STANDARD 20)

add_executable(roguelike_test main.cpp
        draw.cpp
        draw.h
        game.cpp
        game.h)
