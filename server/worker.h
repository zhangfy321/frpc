//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_WORKER_H
#define FRPC_WORKER_H

#include <iostream>
#include "io/epoll.h"

class Worker {
public:
    Worker() = default;

    void operator ()();
private:
    Epoll m_epoll;
};


#endif //FRPC_WORKER_H
