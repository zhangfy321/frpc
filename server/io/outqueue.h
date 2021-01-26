//
// Created by fy on 2021/1/26.
//

#ifndef FRPC_OUTQUEUE_H
#define FRPC_OUTQUEUE_H

#include <cstdint>
#include <atomic>
//多写单读 读指针用原子变量
class OutQueue {
public:
    OutQueue(){

    }
    int write();

    int read();
private:
    uint32_t _writeIdx = 0;
    std::atomic<uint32_t> _readIdx;
};


#endif //FRPC_OUTQUEUE_H
