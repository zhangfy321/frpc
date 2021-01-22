//
// Created by fy on 2021/1/14.
//

#ifndef FRPC_SERVER_H
#define FRPC_SERVER_H

#include <cstdint>
#include <vector>
#include <thread>

/*
 * Server预先派生指定数量的epoll线程，他们监听相同端口，发生IO事件后派生协程来处理
*/
class Server {
public:
    Server() = default;
    virtual ~Server();
    int Run();

private:
    void watch();

    uint32_t _thread_cnt = 10;
    std::vector<std::thread::id> t_ids;
};


#endif //FRPC_SERVER_H
