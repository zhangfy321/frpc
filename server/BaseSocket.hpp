//
// Created by fy on 2021/1/15.
//

#ifndef FRPC_BASESOCKET_HPP
#define FRPC_BASESOCKET_HPP
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define CHECK_RET(ret) if ((ret) == false){return -1;}

class BaseSocket {
public:
    BaseSocket() : _sockfd(-1){}

    bool InitSocket(bool reuse, bool nonblock){
        _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INET（IPv4协议） SOCK_STREAM：TCP
        if (_sockfd < 0) {
            perror("create socket error");
            return false;
        }
        if (reuse){
            int nReuseAddr = 1;
            setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &nReuseAddr, sizeof(nReuseAddr)); // todo
        }
        if (nonblock){
            int iFlags;
            iFlags = fcntl(_sockfd, F_GETFL, 0); //通过fcntl可以改变已打开的文件性质
            iFlags |= O_NONBLOCK;
            iFlags |= O_NDELAY;
            fcntl(_sockfd, F_SETFL, iFlags);   //todo
        }
        return true;
    }

    bool Bind(const std::string &ip, uint16_t port) const{
        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port); //主机字节顺序转变成网络字节顺序
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        int ret = bind(_sockfd, (struct sockaddr*)&addr,sizeof(addr));
        if (ret < 0) {
            perror("bind error");
            return false;
        }
        return true;
    }

    bool Listen(int backlog = 1024) const { //等待accept的队列最大长度
        int ret = listen(_sockfd, backlog);
        if (ret < 0) {
            perror("listen error");
            return false;
        }
        return true;
    }

    bool Connect(const std::string &ip, uint16_t port) {
        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(struct sockaddr_in);

        int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
        if (ret < 0) {
            perror("connect error");
            return false;
        }
        return true;
    }

    bool Accept() {

    }

    bool Recv (char* buf) const {
        int ret = recv(_sockfd, buf, sizeof(*buf), 0);
        if (ret < 0) {
            perror("recv error");
        }
        else if (ret == 0) {
            printf("fd already shutdown");
            return false;
        }
        return true;
    }

    bool Send(const char* buf, size_t len, char mode = 0x01) const{
        // ssize_t send(int sockfd, void *buf, size_t len, int flags);
        int ret = send(_sockfd, buf, len, 0);
        if (ret < 0) {
            perror("send error");
            return false;
        }
        return true;
    }

    bool Close() {
        close(_sockfd);
        _sockfd = -1;
    }

private:
    int _sockfd;
};


#endif //FRPC_BASESOCKET_HPP
