#ifndef FRPC_SERVER_IO_IOLOOP_H
#define FRPC_SERVER_IO_IOLOOP_H

#include <iostream>
#include <string>
#include "server/config/config.h"

namespace frpc
{
class IOLoop {
public:
    // IOLoop(Config* config): config_(config){
    //     InitSocket();
    // }

    IOLoop(std::string ip, uint16_t port):
        ip_(ip), port_(port){};
    
    ~IOLoop();

    int Run();
private:
    void InitSocket();
    void HandleClose(int fd);
    void HandleRead(int fd);
    void HandleWrite(int fd);

    std::string ip_;
    uint16_t port_;
    int listenfd_;
    int epfd_;

    // Config* config_;
    uint32_t backlog_; //已经完成三次握手的连接的队列最大长度
    uint32_t max_conns_; //最大连接数
    bool is_et_; //et or ft
    uint32_t close_delay_; //关闭socket后，但是还有数据没发送完毕的时候容许逗留的最大秒数

};
} //namespace frpc

#endif // FRPC_SERVER_IO_IOLOOP_H