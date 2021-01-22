//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_EPOLL_CPP
#define FRPC_EPOLL_CPP
#include <iostream>
#include <vector>
#include "base.cpp"
#include "epoll.h"


int Epoll::init_listen_fd()
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    uint16_t port = 8000;
    ev.data.fd = init_server_socket(&port);

    //epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
    if ( epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, ev.data.fd, ev) < 0 )
    {
        LOG("epoll ctrl error");
        return -1;
    }
    return ev.data.fd;
}

[[noreturn]] void Epoll::main_loop()  //非常关键的代码，每一步都要保证高效
{
    struct epoll_event ev, es[10];
    for( ; ; )
    {
//        memset(es, 0, sizeof(es));
        int nfds = epoll_wait(_epoll_fd, es, 10, 3000);
        if (nfds <= 0) {
            LOG("epoll wait timeout or error");
            continue;
        }

        for (auto &e : es) {
            int fd = e.data.fd;
            if (fd != _listen_fd)  //注意if条件的顺序也会影响cpu预测性能
            {

                if (e.events & EPOLLIN)
                {
                    receive_data(fd);
                    //todo
                }
                else if (e.events & EPOLLERR || e.events & EPOLLHUP
                {
                    close(eve.data.fd);
                }
                else if (e.events & EPOLLOUT)
                {

                }
                else if (e.e)
            }
            else
            {
                struct sockaddr_in addr {};
                e.events = EPOLLIN | EPOLLET;
                e.data.fd = accept(_epoll_fd, (struct sockaddr *) &addr, sizeof(addr));
                setnonblocking(new_cli_fd); //todo
                epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, e.data.fd, ev);
            }
        }
    }
}


size_t Epoll::receive_data(int fd)
{

}

size_t Epoll::send_data(int fd)
{

}

bool Epoll::epoll_remove(int fd)
{
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0) {
        LOG("epoll ctrl error");
        return false;
    }
    return true;
}

#endif //FRPC_EPOLL_CPP
