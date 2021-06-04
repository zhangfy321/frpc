#include <stdio.h>
#define WHEEL_SIZE 32 
#define WHEEL_SIZE_BITS 5  // 2 ^ 5 = 32
#define WHEEL_NUM 8

// #define MAX_TIMEOUT 

typedef void* (*pfn_t)(void*);
typedef struct timeout
{
    size_t timeout_tick;    
    pfn_t callback;

    struct timeout* prev_node; //双向链表利于删除
    struct timeout* next_node;

} timeout_t;


typedef struct watch
{
    timeout_t* dial[WHEEL_SIZE];
    size_t hand; //"表盘"指针
} watch_t;


class Timer
{
public:
    Timer()
    {
        _now = 0;
    }
    void timer_init();
    bool add_timeout(size_t timeout_tick, timeout_t *timeout);
    void remove_timeout();
    void update_timeout();

private:

    size_t _interval; //时间间隔
    size_t _now;

    watch_t watches[WHEEL_NUM];

};