//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_SERVER_H
#define FRPC_SERVER_H

#include <cstdint>
#include <vector>
#include <thread>

/*
 * Server预先派生指定数量的epoll线程，他们监听相同端口
*/
class Server { //注意单例
public:
    Server() = default; //加载配置
    virtual ~Server();
    int Run();

private:
    int BeforeMasterRun();
    int AfterMasterRun();
    int BeforeChildRun();
    int AfterChildRun();
    
    uint32_t _thread_cnt;
    std::string ip;
    uint16_t port;
};


#endif //FRPC_SERVER_H
