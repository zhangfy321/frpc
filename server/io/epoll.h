//
// Created by fy on 2021/1/22.
//

#ifndef FRPC_EPOLL_H
#define FRPC_EPOLL_H
#include <sys/epoll.h>
#include <memory>
#include "base/base.cpp"

class Epoll {

public:
    Epoll() {
        _epollfd = epoll_create1(0);
        if (_epoll_fd == -1) {
            LOG("epoll_create1");
        }
        _listen_fd = init_listen_fd();
    };

    [[noreturn]] void main_loop();

private:
    int init_listen_fd();

    bool epoll_remove(int fd);

    size_t receive_data(int fd);

    size_t send_data(int fd);

    int set_nonblocking(int fd);

    int _listen_fd = -1;
    int _epoll_fd = -1;
};


#endif //FRPC_EPOLL_H
