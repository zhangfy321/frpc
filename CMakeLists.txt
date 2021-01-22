cmake_minimum_required(VERSION 3.16)
project(frpc)

set(CMAKE_CXX_STANDARD 11)

add_executable(frpc main.cpp server/io/base.hpp server/io/iouring.cpp)