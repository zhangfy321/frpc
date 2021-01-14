//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_CONFIG_H
#define FRPC_CONFIG_H
#include <string>
using namespace std;

struct config{
    string service_name;
    uint32_t port{};
    short work_mode = 0; // 0:多线程 1:多进程
    uint32_t worker_num = 2;
}Config;


#endif //FRPC_CONFIG_H
