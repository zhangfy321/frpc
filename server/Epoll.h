//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_EPOLL_H
#define FRPC_EPOLL_H
#include <iostream>
#include <vector>
#include <sys/epoll.h>
#include "tcpsocket.hpp"

class Epoll {
private:
    int _epfd;
public:
    bool Init() {
        //创建epoll

        //接口原型：int epoll_create(int size);
        _epfd = epoll_create(1);
        if (_epfd < 0) {
            perror("epoll create error");
            return false;
        }
        return true;
    }
    bool Add(TcpSocket sock, uint32_t events = 0) {
        sock.SetNonBlock();
        int fd = sock.GetSockFd();

        //定义事件
        struct epoll_event ev;
        ev.events = EPOLLIN | events;
        ev.data.fd = fd;

        //接口原型：epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
        int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
        if (ret < 0) {
            perror("epoll ctrl error");
            return false;
        }
        return true;
    }
    bool Del(TcpSocket sock) {
        int fd = sock.GetSockFd();
        int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
        if (ret < 0) {
            perror("epoll ctrl error");
            return false;
        }
        return true;
    }
    bool Wait(std::vector<TcpSocket> &list, int ms_timeout = 3000) {
        //接口原型：int epoll_wait(int epfd, struct epoll_event *events,
        //  int maxevents, int timeout);
        struct epoll_event evs[10];
        int nfds = epoll_wait(_epfd, evs, 10, ms_timeout);
        if (nfds < 0) {
            perror("epoll wait error");
            return false;
        }else if (nfds == 0) {
            std::cout << "epoll wait timeout\n";
            return false;
        }
        for (int i = 0; i < nfds; i++) {
            int fd = evs[i].data.fd;
            TcpSocket sock;
            sock.SetSockFd(fd);
            list.push_back(sock);
        }
        return true;
    }
};

int main(){
    TcpSocket lst_sock;			//listen socket：监听socket
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind("0.0.0.0", 9000));
    CHECK_RET(lst_sock.Listen());

    Epoll epoll;
    CHECK_RET(epoll.Init());
    CHECK_RET(epoll.Add(lst_sock, EPOLLET));
    while(1) {
        std::vector<TcpSocket> list;
        bool ret = epoll.Wait(list);
        if (ret == false) {
            continue;
        }
        for (int i = 0; i < list.size(); i++) {
            if (list[i].GetSockFd() == lst_sock.GetSockFd()) {
                TcpSocket cli_sock;
                lst_sock.Accept(cli_sock);
                epoll.Add(cli_sock, EPOLLET);
            }else {
                std::string buf;
                list[i].Recv(buf);
                std::cout << "client say: " << buf << std::endl;
            }
        }

    }
    lst_sock.Close();
    return 0;
}



#endif //FRPC_EPOLL_H
