cmake_minimum_required(VERSION 3.16)
project(frpc)

set(CMAKE_CXX_STANDARD 11)
add_executable(frpc main.cpp server/io/inqueue.cpp server/io/inqueue.h server/io/outqueue.cpp server/io/outqueue.h server/io/base/list.cpp server/io/base/list.h)
#add_executable(frpc main.cpp server/io/base/base.cpp server/io/base/iouring.cpp server/io/base/base.h server/io/epoll.h server/codec.cpp server/codec.h server/io/bufferpoll.cpp server/io/bufferpoll.h)