#include "timer.h"

bool Timer::add_timeout(size_t timeout_tick, timeout_t *timeout)
{

    // 找到合适的插入位置
    size_t trigger_tick = timeout_tick + _now;

    // if (trigger_tick > MAX) return false;

    unsigned long long total = 1;
    int idx = -1;
    while (total <= trigger_tick)
    {
        total = total << WHEEL_SIZE_BITS;
        ++idx;
    }

    INFO("trigger: %lu, total: %lu", trigger_tick, total);
    int offset = trigger_tick / ( total >> ( WHEEL_SIZE_BITS ) );

    //插入链表中
    timeout_t* head = _watches[idx].dial[offset];
    if (head) //将新超时节点加在链表头部
    {
        timeout->next = head;
        head->prev = timeout;
        _watches[idx].dial[offset] = timeout;
    }
    else head = timeout;

    ERROR("idx: %d, offset: %d", idx, offset);
    return true;

}   


bool Timer::update_timeout()
{
    for(;;)
    {
        _now += _interval;
        for(int i = 0; i < WHEEL_NUM; i++)
        {

        }
    }
}