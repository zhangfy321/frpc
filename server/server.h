//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_SERVER_H
#define FRPC_SERVER_H
#include <mutex>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sock addr_in

using std::mutex;

class Server {
public:
    Server() {
        int opt = 1;
        address.sin_port = htons(8000);     //主机字节顺序转变成网络字节顺序
        address.sin_addr.s_addr = INADDR_ANY;  //绑定到任意地址
        address.sin_family = AF_INET;

        sock_fd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET（IPv4协议） SOCK_STREAM：TCP
        setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        if (bind(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
            // 错误处理
        }
        listen(sock_fd, backlog);
    };
    virtual ~Server();

    void* GetConfig();
    int Run();

private:
    int master();

    void slave();

    int sock_fd;
    struct sockaddr_in address{};

    mutex g_mutex; //如何避免线程获得锁之后崩溃?

//    uint32_t port = 8000;
    uint32_t work_num = 2;
    uint64_t backlog = 65535; //accept等待队列长度最大值

};


#endif //FRPC_SERVER_H
