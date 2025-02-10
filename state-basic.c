/*
* https://www.youtube.com/watch?v=QyDHxA0286c
*/

#include "hookapi.h"

int64_t hook(uint32_t reserved ) {

    uint16_t count = 0;
    state(SVAR(count), "COUNT", 5);
    TRACEVAR(count);
    ++count;

    state_set(SVAR(count), "COUNT", 5);
    TRACEVAR(count);
    TRACESTR("Accept.c: Called.");
    accept(0,0,0);
    

    _g(1,1);
    return 0;
}