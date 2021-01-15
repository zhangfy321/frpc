//
// Created by fy on 2021/1/14.
//
#include "server.h"
#include <thread>
using std::thread;
using std::unique_lock;
using std::mutex;
using std::try_to_lock;

int Server::Run(){
    return 0;
};

void Server::slave(){
    int conn_fd;
    void handler(int);
    struct sockaddr *cli_addr;
    cli_addr = Malloc(addrlen);

    for ( ; ; ){
        {
            unique_lock<mutex> m_lock(g_mutex, try_to_lock);
            if(m_lock.owns_lock()){
                conn_fd = accept()
            }
            else{
                continue;
            }
        }

    }
}

int Server::master(){
    for(uint32_t i = 0; i < work_num; i++){
        thread t(Server::slave);
    }


    return 0;
}


