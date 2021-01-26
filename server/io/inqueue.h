//
// Created by fy on 2021/1/26.
//

#ifndef FRPC_INQUEUE_H
#define FRPC_INQUEUE_H

#include <cstdint>
#include <atomic>
//单写多读 读指针用原子变量
class InQueue {
public:
    InQueue(){

    }
    ~InQueue(){

    }

    int write();

    int read();
private:
    uint32_t _writeIdx;
    std::atomic<uint32_t> _readIdx;

};


#endif //FRPC_INQUEUE_H
