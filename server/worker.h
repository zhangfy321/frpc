//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_WORKER_H
#define FRPC_WORKER_H

#include <iostream>



class Worker {
public:
    Worker() = default;

    void operator ()();
private:
    thread_local uint32_t
};


#endif //FRPC_WORKER_H
