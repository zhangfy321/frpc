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
namespace frpc{

int IOLoop::Run(){

    InitSocket();

    struct epoll_event events[MAX_EVENTS], cur_event;
    struct sockaddr_in cli_addr;
    socklen_t socklen = sizeof(cli_addr);
    int fd = -1;
    uint32_t ev = -1;
    int conn_fd;
    
    for(;;){
        int fds_num = epoll_wait(epfd_, events, MAX_EVENTS, -1);

        //https://zhuanlan.zhihu.com/p/149265232
        for (int n = 0; n < fds_num; ++n) {
            fd = events[n].data.fd;
            ev = events[n].events;

            if (fd == listenfd_){
                //新连接
                conn_fd = accept(listenfd_, reinterpret_cast<sockaddr*>(&cli_addr), &socklen);
                RETURN_ERROR(conn_fd <= 0, "accept ret err");

                // 设置为非阻塞
                int opts = -1;     
                opts = fcntl(conn_fd, F_GETFL);    
                RETURN_ERROR(opts < 0, "setnonblocking failed");
                opts = opts | O_NONBLOCK;  
                int ret = fcntl(conn_fd, F_SETFL, opts);
                RETURN_ERROR( ret < 0, "setnonblocking failed"); 

                //将此fd注册到epoll
                cur_event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
                cur_event.data.fd = fd;
                ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, conn_fd, &cur_event);
                RETURN_ERROR(ret < 0 , "epoll_add ret err");

            } else {
                
                if (ev & EPOLLIN){
                    //读事件 交给线程池处理
                }

                if (ev & EPOLLOUT){
                    //写事件
                }
                
                if (ev & (EPOLLRDHUP | EPOLLERR | EPOLLHUP)){
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
    addr.sin_addr.s_addr = INADDR_ANY; 
    addr.sin_port = htons(port_); //注意字节序变为大端

    int ret = 0;
    //创建sever监听socket
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0); 
    RETURN_ERROR(listenfd_ < 0, "create socker err");

    int flag = 1;
    //SO_REUSEPORT 是多个epoll线程监听相同地址端口组
    ret = setsockopt(listenfd_, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag)); 
    RETURN_ERROR(ret < 0, "set reuseport err");

    //SO_REUSEADDR 使server免等待快速重启
    ret = setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)); 
    RETURN_ERROR(ret < 0, "set reuseaddr err");

    //设置KeepAlive
    ret = setsockopt(listenfd_, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag));
    RETURN_ERROR(ret < 0, "set keepalive err");

    //设置TCP_NODELAY (禁用Nagle算法，适用于延时敏感型应用)
    ret = setsockopt(listenfd_, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    RETURN_ERROR(ret < 0, "set tcp nodelay err");

    //设置CLOSE_DELAY
    struct linger so_linger;
    so_linger.l_onoff = close_delay_ != 0;       // 在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
    so_linger.l_linger = close_delay_;           // 容许逗留的时间为delay秒
    ret = setsockopt(listenfd_, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(linger));
    RETURN_ERROR(ret < 0, "set close delay err");

    //设置socket为非阻塞 todo 怎么理解？
    int flags = fcntl(listenfd_, F_GETFL, 0); 
    fcntl(listenfd_, F_SETFL, flags|O_NONBLOCK);

    
    //绑定端口ip
    ret = bind(listenfd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)); 
    RETURN_ERROR(ret < 0, "bind err");

    //开启监听
    ret = listen(listenfd_, backlog_); 
    RETURN_ERROR(ret < 0, "listen err");


    epfd_ = epoll_create1(0);
    RETURN_ERROR(ret < 0, "create epoll fd err");
}

IOLoop::~IOLoop(){
    close(listenfd_);
}

}

