//
// Created by fy on 2021/1/14.
//
#ifndef FRPC_WORKER_THREAD_H
#define FRPC_WORKER_THREAD_H
#include <thread>
#include <mutex>
typedef struct {
    pthread_t thread_tid;
    int64_t  thread_count;
}Thread;

Thread *thread_ptr;

#endif //FRPC_WORKER_THREAD_H
