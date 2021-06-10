#pragma once
#ifndef FRPC_SERVER_SERVER_H
#define FRPC_SERVER_SERVER_H
#include <cstdint>
#include <string>

/*
 * Server预先派生指定数量的epoll线程，他们监听相同端口
*/
namespace frpc
{

class Server
{ //注意单例
public:
    Server();

    Server(uint32_t thread_cnt, std::string ip, uint16_t port) : thread_cnt_(thread_cnt),
                                                                 ip_(ip), port_(port){};

    ~Server(){};

    int Run();

private:
    int BeforeMasterRun();
    int AfterMasterRun();
    int BeforeChildRun();
    int AfterChildRun();

    uint32_t thread_cnt_;
    std::string ip_;
    uint16_t port_;
};

} //namespace frpc

#endif //FRPC_SERVER_SERVER_H