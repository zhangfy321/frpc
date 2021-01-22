//
// Created by fy on 2021/1/22.
//

#ifndef FRPC_EPOLL_H
#define FRPC_EPOLL_H
#include <sys/epoll.h>

enum EventType
{
    EOUT = EPOLLOUT,	  // 写事件
    ECLOSE = EPOLLRDHUP,  // 对端关闭连接或者写半部
    EPRI = EPOLLPRI,	  // 紧急数据到达
    EERR = EPOLLERR,	  // 错误事件
    EET = EPOLLET, 		  // 边缘触发
    EDEFULT = EIN | ECLOSE | EERR | EET
};



#endif //FRPC_EPOLL_H
