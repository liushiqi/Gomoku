cmake_minimum_required(VERSION 3.12)
project(Cclass C)

set(CMAKE_C_STANDARD 11)

add_executable(chess src/main.c src/interface/interface.c src/interface/interface.h)