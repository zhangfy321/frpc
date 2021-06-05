//
// Created by fy on 2021/1/25.
//

#include "bufferpoll.h"


T BufferPool::Get()
{
    if (!_pool.empty())
    {
        auto buff = _pool.front();
        _pool.pop();
        return buff;
    }
    return nullptr;
}

