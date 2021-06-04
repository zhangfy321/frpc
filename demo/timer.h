#include <stdio.h>
#include <string.h>
#include "format.h"
#include <stdlib.h>
#define WHEEL_SIZE 32 
#define WHEEL_SIZE_BITS 5  // 2 ^ 5 = 32
#define WHEEL_NUM 8

// #define MAX_TIMEOUT 

typedef void* (*pfn_t)(void*);
typedef struct timeout
{
    size_t timeout_tick;    
    pfn_t callback;

    struct timeout* prev; //双向链表利于删除
    struct timeout* next;

} timeout_t;


typedef struct watch
{
    timeout_t** dial;
    size_t hand; //"表盘"指针
} watch_t;


class Timer
{
public:
    Timer(size_t now=0, size_t interval=0): 
        _now(now), _interval(interval)
    {
        _watches = (watch_t*)calloc(WHEEL_NUM, sizeof(watch_t));
        if (_watches)
            for (int i = 0; i < WHEEL_NUM; i++)
                _watches[i].dial = (timeout_t**)calloc(WHEEL_SIZE, sizeof(timeout_t*));
    }

    bool add_timeout(size_t timeout_tick, timeout_t *timeout);
    bool remove_timeout();
    bool update_timeout();

private:

    size_t _interval;
    size_t _now;

    watch_t* _watches;

};