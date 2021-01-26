//
// Created by fy on 2021/1/26.
//

#ifndef FRPC_OUTQUEUE_H
#define FRPC_OUTQUEUE_H

#include <cstdint>
#include <atomic>
//多写单读 目前暂时采用STL，后续改造为写指针用原子变量CAS操作
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
