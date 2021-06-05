#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <string>
#include <vector>

// #include "trpc/runtime/iomodel/reactor/common/network_address.h"
// #include "trpc/runtime/iomodel/reactor/common/unix_address.h"

namespace comm {

class Socket {
public:
    Socket();

    explicit Socket(int fd, std::string ip, uint16_t port):
    fd(listen_fd_), ip_(ip), port_(port){};

    explicit Socket(const Socket&);

    Socket& operator=(const Socket& other);

    ~Socket();

    int GetFd() const { return listen_fd_; }

    bool IsValid() const { return fd_ != -1; }

    void CloseSocket();

    int Accept(NetworkAddress* peer_addr);

    void Bind(const NetworkAddress& bind_addr);

    void Listen(int backlog = SOMAXCONN);

    int Connect(const NetworkAddress& addr);


    int Recv(void* buff, size_t len, int flag = 0);

    int Send(const void* buff, size_t len, int flag = 0);

    int RecvFrom(void* buff, size_t len, int flag, NetworkAddress* peer_addr);

    int SendTo(const void* buff, size_t len, int flag, const NetworkAddress& peer_addr);

    int Writev(const struct iovec* iov, int iovcnt);

    int SendMsg(const struct msghdr* msg, int flag = 0);

    void SetReuseAddr(); //允许服务器免等待快速重启

    void SetReusePort(); //允许多个io进/线程绑定统一端口 os层负载均衡

    void SetBlock(bool block = false); 

    void SetCloseWait(int delay = 30); //socket关闭时等待缓冲区发送的延时

    void SetNoCloseWait(); //关闭socket时免等待

    void SetCloseWaitDefault(); //设置关闭socket时的默认行为

    void SetTcpNoDelay(); //禁用Nagle算法

    void SetKeepAlive(); //开启KeepAlive


    int GetRecvBufferSize();

    void SetRecvBufferSize(int size);

    int GetSendBufferSize();

    void SetSendBufferSize(int size);

private:
    int listenfd_;
    uint16_t port_;
    std::string ip_;
};

}  // namespace comm
