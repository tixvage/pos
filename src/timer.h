#ifndef TIMER_H_
#define TIMER_H_

#include "std.h"

typedef struct Time {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
} Time;

Time get_time(void);
double get_time_since_boot(void);
void init_timer(uint32_t freq);
void sleepd(double seconds);
void sleep(uint64_t mss);

#endif

