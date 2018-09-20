cmake_minimum_required(VERSION 3.10)
project(chess C)

set(CMAKE_C_STANDARD 11)

add_executable(chess src/main.c src/interface/interface.c src/interface/interface.h)