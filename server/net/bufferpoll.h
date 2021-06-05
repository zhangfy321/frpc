//
// Created by fy on 2021/1/25.
//

#ifndef FRPC_BUFFERPOLL_H
#define FRPC_BUFFERPOLL_H

#include <cstdint>
#include <queue>
#include <iostream>
#include <utility>

using T = std::shared_ptr<char *>;
class BufferPool {
public:
    BufferPool() {

        for (uint32_t i = 0; i < _block_num; i++)
        {
             T block(new char[_block_size], [this](T *p){ _pool.push(*p);});
            _pool.push(block);
        }
    }

    T Get();

private:
    uint16_t _block_size = 8196; //单块大小8KB
    uint32_t _block_num = 200; //总块数
    std::queue<T> _pool;
};

#endif //FRPC_BUFFERPOLL_H
