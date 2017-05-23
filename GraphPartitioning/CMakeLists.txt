cmake_minimum_required(VERSION 3.7)
project(GraphPartitioning)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES Test.c Graph.h Graph.c Parse.h Parse.c)
add_executable(GraphPartitioning ${SOURCE_FILES})
