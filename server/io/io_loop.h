#ifndef FRPC_SERVER_IO_IOLOOP_H
#define FRPC_SERVER_IO_IOLOOP_H

#include "server/config/config.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include "comm/macros.h"


namespace frpc
{
class IOLoop
{
public:
    // IOLoop(Config* config): config_(config){
    //     InitSocket();
    // }

    IOLoop(std::string ip, uint16_t port, uint32_t backlog, uint32_t max_conns, bool is_et, uint32_t close_delay) : 
        ip_(ip), port_(port), backlog_(backlog), max_conns_(max_conns), is_et_(is_et), close_delay_(close_delay){};

    ~IOLoop();

    int Run();

private:
    void InitSocket();
    void HandleClose(int fd);
    void HandleRead(int fd);
    void HandleWrite(int fd);

    // Config* config_;
    std::string ip_;
    uint16_t port_;
    int listenfd_;
    uint32_t backlog_;   //已经完成三次握手的连接的队列最大长度
    uint32_t max_conns_; //最大连接数
    bool is_et_;         //et or ft

    uint32_t close_delay_; //关闭socket后，但是还有数据没发送完毕的时候容许逗留的最大秒数
    int epfd_;
};
} //namespace frpc

static inline int setnonblocking(int conn_fd)
{
    int opts = -1;
    opts = fcntl(conn_fd, F_GETFL);
    RETURN_ERROR(opts < 0, "setnonblocking failed");
    opts = opts | O_NONBLOCK;
    return fcntl(conn_fd, F_SETFL, opts);
}

static inline void getipv4addr(uint32_t addr, std::string* addr_str)
{
    std::stringstream stream;
    for (int i = 0; i < 4; ++i){
        stream << (addr & 0xFF);
        if (i != 3) stream << '.';
        addr >>= 8;
    }
    stream >> *addr_str;
}
#endif // FRPC_SERVER_IO_IOLOOP_H