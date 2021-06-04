#include "timer.h"
// void Timer::timer_init(size_t start_tick, size_t interval)
// {
//     ;
// }

bool Timer::add_timeout(size_t timeout_tick, timeout_t *timeout)
{

    size_t trigger_tick = timeout_tick + _now;

    // if (trigger_tick > MAX) return false;

    unsigned long long total = 1;
    int idx = -1;
    while (total <= trigger_tick)
    {
        total = total << WHEEL_SIZE_BITS;
        ++idx;
    }

    printf("trigger: %lu, total: %lu \n", trigger_tick, total);
    int offset = trigger_tick / ( total >> ( WHEEL_SIZE_BITS ) );
    
    timeout_t cur_timeout;
    watches[idx].dial[offset] = &cur_timeout;

    printf("idx: %d, offset: %d", idx, offset);
    return true;

}   