#include "timer.h"
#include "isr.h"
#include "screen.h"
#include "ports.h"

#define BASE_FREQ 1193182

uint32_t tick = 0;
double time_since_boot = 0;
uint32_t g_freq = 0;
Time time;

static uint32_t get_timer_div(void) {
    return BASE_FREQ / g_freq;
}

static void timer_callback(Registers regs) {
    (void) regs;
    //TODO: https://wiki.osdev.org/Programmable_Interval_Timer
    tick++;
    int before = time_since_boot;
    time_since_boot += 1 / (double)get_timer_div();;
    int after = time_since_boot;
    if (before == after) return;
    if (time.second < 60) {
        time.second++;
    } else if (time.minute < 60) {
        time.minute++;
        time.second = 0;
    } else if (time.hour < 24) {
        time.hour++;
        time.minute = 0;
    }
}

static int get_update_ip(void) {
    port_byte_out(0x70, 0x0A);
    return (port_byte_in(0x71) & 0x80);
}

uint8_t get_RTC_register(int reg) {
    port_byte_out(0x70, reg);
    return port_byte_in(0x71);
}

Time get_time(void) {
    return time;
}

double get_time_since_boot(void) {
    return time_since_boot;
}

void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, timer_callback);
    
    g_freq = freq;
    uint32_t divisor = BASE_FREQ / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);
    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    io_wait();
    port_byte_out(0x40, high);

    //get RTC time
    uint8_t register_b;
    while (get_update_ip());
    uint8_t second = get_RTC_register(0x00);
    uint8_t minute = get_RTC_register(0x02);
    uint8_t hour = get_RTC_register(0x04);
    uint8_t day = get_RTC_register(0x07);
    uint8_t month = get_RTC_register(0x08);
    uint32_t year = get_RTC_register(0x09);

    register_b = get_RTC_register(0x0B);
    if (!(register_b & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
    }

    time = (Time) {second, minute, hour, day, month, year};
}

void sleepd(double seconds) {
    double start_time = time_since_boot;
    while (time_since_boot < start_time + seconds) {
        __asm__ ("hlt");
    }
}

void sleep(uint64_t mss) {
    sleepd((double)mss / 1000);
}
