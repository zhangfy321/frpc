#include "io_loop.h"
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include "comm/macros.h"
#define MAX_EVENTS 10
namespace frpc
{
int IOLoop::Run()
{
    InitSocket();

    struct epoll_event ev, events[MAX_EVENTS];
    int conn_fd;
    for(;;)
    {   
        int fds_num = epoll_wait(epfd_, events, MAX_EVENTS, -1);

        //https://zhuanlan.zhihu.com/p/149265232
        for (n = 0; n < fds_num; ++n) 
        {
            if (events[n].data.fd == listen_fd_) 
            {
                int conn_fd = accept(listen_fd_, (struct sockaddr *) &addr, &addrlen);
                if (UNLIKELY(conn_fd == -1)) {
                    ERROR("accept ret -1");
                    exit(-1);
                }

                setnonblocking(conn_fd);

                ev.events = EPOLLIN | EPOLLET; //ET模式
                ev.data.fd = conn_fd;
                
                if (UNLIKELY(-1 == epoll_ctl_add(epfd, conn_fd, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP))) {
                    ERROR("epoll_add ret -1");
                    exit(-1);
                }
            }
            else 
            {
                uint32_t ev = events[n].events;
                if (ev & EPOLLIN)
                {
                    //读事件 交给线程池处理
                }

                if (ev & EPOLLOUT)
                {
                    //写事件
                }
                
                if (ev & (EPOLLRDHUP | EPOLLERR | EPOLLHUP))
                {
                    //关闭事件
                }
            }
        }
    }
    return 0;
}

void IOLoop::InitSocket()
{
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htons(ip_); //注意字节序变为大端
    addr.sin_port = htons(port_); 

    int ret = 0;
    //创建sever监听socket
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0); 
    assert(listen_fd_ >= 0);

    int flag = 1;
    //SO_REUSEPORT 是多个epoll线程监听相同地址端口组
    ret = setsockopt(listenfd_, SOL_SOCKET, SO_REUSEPORT, flag, sizeof(flag)) 
    assert(ret == 0);

    //SO_REUSEADDR 使server免等待快速重启
    ret = setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, flag, sizeof(flag)) 
    assert(ret == 0);

    //设置KeepAlive
    setsockopt(listenfd_, SOL_SOCKET, SO_KEEPALIVE, flag, sizeof(flag)) == -1 
    assert(ret == 0);

    //设置TCP_NODELAY (禁用Nagle算法，适用于延时敏感型应用)
    setsockopt(listenfd_, IPPROTO_TCP, TCP_NODELAY, flag, sizeof(flag)) == -1 
    assert(ret == 0);

    //设置CLOSE_DELAY
    struct linger ling;
    ling.l_onoff = close_delay_ != 0;       // 在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
    ling.l_linger = close_delay_;           // 容许逗留的时间为delay秒
    setsockopt(listenfd_, SOL_SOCKET, SO_LINGER, linger, sizeof(linger)) == -1 
    assert(ret == 0);

    //设置socket为非阻塞 todo 怎么理解？
    int flags = fcntl(listen_fd_, F_GETFL, 0); 
    fcntl(listen_fd_, F_SETFL, flags|O_NONBLOCK);

    
    //绑定端口ip
    ret = bind(listen_fd_, (struct sockaddr*)&addr, sizeof(addr)); 
    assert(ret == 0);

    //开启监听
    ret = listen(listen_fd_, backlog_); 
    assert(ret == 0);


    epfd_ = epoll_create1(0);
    assert(epfd_ == 0);

    struct epoll_event ev, events[MAX_EVENTS];
}

}
