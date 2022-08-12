#ifndef TIMER_H_
#define TIMER_H_

#include "std.h"

void init_timer(uint32_t freq);
void sleepd(double seconds);
void sleep(uint64_t mss);

#endif

