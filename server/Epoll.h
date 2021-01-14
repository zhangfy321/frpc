//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_EPOLL_H
#define FRPC_EPOLL_H


class Epoll {
private:
    int _epfd;
public:
    bool Init() {
        _epfd = epoll_create(1);
    }
};


#endif //FRPC_EPOLL_H
