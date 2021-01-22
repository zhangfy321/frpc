cmake_minimum_required(VERSION 3.16)
project(frpc)

set(CMAKE_CXX_STANDARD 11)

add_executable(frpc main.cpp server/io/base.cpp server/io/iouring.cpp server/io/base.h server/io/epoll.h)