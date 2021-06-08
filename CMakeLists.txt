cmake_minimum_required(VERSION 3.17)
project(frpc)

set(CMAKE_CXX_STANDARD 11)

include_directories(3rdparty)
include_directories(comm)

add_executable(frpc main.cpp)