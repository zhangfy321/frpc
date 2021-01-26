//
// Created by fy on 2021/1/15.
//

#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "base.h"

int init_server_socket(const uint16_t *port)
{
    int fd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, IPPROTO_TCP);    //AF_INET（IPv4协议） SOCK_STREAM：TCP
    if (fd  == -1)
    {
        LOG("create socket error");
        return false;
    }

    //
    /* 设置reuseport 内核级负载均衡
     * 关于reuseport的hash优化 https://blog.csdn.net/dog250/article/details/51510823
     *
    */
    int nReuseAddr = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &nReuseAddr, sizeof(nReuseAddr)); // todo
//    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
//    //设置nonblock
//    int flags;
//    flags = fcntl(fd, F_GETFL, 0); //通过fcntl可以改变已打开的文件性质
//    flags |= O_NONBLOCK;
//    flags |= O_NDELAY;
//    fcntl(fd, F_SETFL, flags);   //todo

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(*port); //主机字节顺序转变成网络字节顺序
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        LOG("bind error");
        return false;
    }

    if ( listen(fd, 10) < 0 )
    {
        LOG("listen error");
        return false;
    }
    return fd;
}


bool connect(int fd, const std::string &ip, uint16_t port)
{
    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    socklen_t len = sizeof(struct sockaddr_in);

    if (connect(fd, (struct sockaddr *) &addr, len) < 0) {
       LOG("connect error");
       return false;
    }
    return true;
}


bool SendWithNoCopy(int fd, const char *buf, size_t len) //todo
{
    /* 只适用于大文件 (10KB 左右) 的场景，小文件场景因为 page pinning 页锁定和等待缓冲区释放的通知消息这些机制，
     * 甚至可能比直接 CPU 拷贝更耗时
    */
    if (setsockopt(fd, SOL_SOCKET, SO_ZEROCOPY, &one, sizeof(one)))
        error(1, errno, "setsockopt zerocopy");
    int ret = send(socket_fd, buffer, sizeof(buffer), MSG_ZEROCOPY);

    /*
     * 不能直接释放buffer，因为这个buffer不一定被读走了，必须读一下Socket的错误队列，确定包已经发走了
    */
    if (recvmsg(socket, &message, MSG_ERRORQUEUE); < 0) {
        LOG("send error");
        return false;
    }
    return true;
}

bool SendFileWithNoCopy(int out_fd, int in_fd, off_t *offset, size_t count)
{
    //ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
    /*
     * in_fd 必须是一个指向文件的文件描述符，且要能支持类 mmap() 内存映射，不能是 Socket 类型
     * off_t 是一个代表了 in_fd 偏移量的指针，指示 sendfile() 该从 in_fd 的哪个位置开始读取
     * 函数返回后，这个指针会被更新成 sendfile() 最后读取的字节位置处，表明此次调用共读取了多少文件数据
     * count 参数则是此次调用需要传输的字节总数
     *
     * 1. 用户进程调用 sendfile() 从用户态陷入内核态 ——(1次系统调用上下文切换)
     * 2. DMA 控制器将数据从硬盘拷贝到内核缓冲区 ——(1次DMA拷贝)
     * 3. CPU 将内核缓冲区中的数据拷贝到套接字缓冲区 ——(1次cpu拷贝)
     * 4. DMA 控制器将数据从套接字缓冲区拷贝到网卡完成数据传输 ——(1次DMA拷贝)
     * 5. sendfile() 返回，上下文从内核态切换回用户态。
     *
     * 缺点：只适用于静态文件的发送
    */
    if ( sendfile(out_fd, in_fd, offset, count) < 0 )
    {
        LOG("send file error");
    }
    return true;
}

bool Send(int fd, const void * buf, size_t len, int flags)
{
    //ssize_t send(int sockfd,const void * buf,size-t len,int flags);
    /*
     * buf为指向要发送数据的缓冲区指针
     * len表示缓冲区buf中要发送的数据的长度
     * flags参数为调用的执行方式（阻塞/非阻塞），当flags设置为0时，可以使用函数write()来代替send()函数
    */
    if ( send(fd, buf, len, flags) < 0 )
    {
        LOG("send file error");
    }
    return true;
}

bool Close(int fd)
{
    close(fd);
    fd = -1;
}

#endif //FRPC_BASESOCKET_HPP