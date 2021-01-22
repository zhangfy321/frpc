//
// Created by fy on 2021/1/14.
//
#include "server.h"
#include "io/base.cpp"
#include <thread>
#include "worker.h"
using std::thread;
//using std::unique_lock;
//using std::mutex;
//using std::try_to_lock;

int Server::Run(){
    for (uint32_t i = 0; i < _thread_cnt; i++)
    {
        auto worker = std::make_shared<Worker>();
        thread t_epoll(worker);
        t_epoll.join();
    }
    return 0;
}

void Server::watch()
{

}