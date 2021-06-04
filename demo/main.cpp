#include <stdio.h>
#include "timer.h"
int main()
{
    Timer timer;
    timeout_t t;
    timer.add_timeout(100, &t);

}