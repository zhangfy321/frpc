cmake_minimum_required(VERSION 3.17)
project(frpc)

set(CMAKE_CXX_STANDARD 11)

add_executable(frpc main.cpp server/worker_thread.h server/server.cpp server/server.h config/config.cpp config/config.h server/worker.cpp server/worker.h server/Epoll.cpp server/Epoll.h)