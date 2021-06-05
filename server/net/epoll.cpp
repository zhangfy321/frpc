//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_EPOLL_CPP
#define FRPC_EPOLL_CPP
#include <iostream>
#include <vector>
#include "base/base.cpp"
#include "epoll.h"


int Epoll::init_listen_fd()
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLLT;
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
        int nfds = epoll_wait(_epoll_fd, es, 10, 0); //注意这里的超时时间最低为几毫秒（轮转频率）
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
                    //调研之后，出于性能要求，实现一个定长块或变长块buffer池，用链表和原子变量指针来实现单读多写队列（入包）
                    // 和多写单读队列（回包）*自定义智能指针的删除器，在析构时将内存块放回内存池。
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
                struct sockaddr_in addr;
                e.events = EPOLLIN | EPOLLLT;
                e.data.fd = accept(_listen_fd, (struct sockaddr *) &addr, sizeof(addr));
                set_nonblocking(e.data.fd); //todo
                epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, e.data.fd, ev);
            }
        }
    }
}


size_t Epoll::receive_data(int fd)
{
    char * buf = new char[65535]; //tmp
    for (;;)
    {
        if (recv(fd, buf, 1024, 0) == -1)
        {
            if (errno == EAGAIN or errno == EWOULDBLOCK)
            {
                printf("recv finish detected, quit...\n");
                break;
            }
        }
    }

    for (size_t i = 0; i < 65535; i++)
    {
        if
    }

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

void setnonblocking(int fd)
{
    int  opts;
    opts = fcntl(fd, F_GETFL);
    if (opts < 0 )
    {
        perror("fcntl(sock, GETFL)");
        exit(1);
    }
    opts  =  opts | O_NONBLOCK;
    if (fcntl(fd, F_SETFL,opts) < 0 )
    {
        perror( " fcntl(sock,SETFL,opts) " );
        exit( 1 );
    }
}


#endif //FRPC_EPOLL_CPP
