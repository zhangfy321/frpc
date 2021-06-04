#include <stdio.h>
#include "timer.h"
int main()
{
    // printf("%llu, %lu", 1ULL, 1ULL);
    Timer timer;
    timeout_t t;
    timer.add_timeout(100, &t);

}