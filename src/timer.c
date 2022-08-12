#include "timer.h"
#include "isr.h"
#include "screen.h"
#include "ports.h"

#define BASE_FREQ 1193182

uint32_t tick = 0;
double time_since_boot = 0;
uint32_t g_freq = 0;

static uint32_t get_timer_div(void) {
    return BASE_FREQ / g_freq;
}

static void timer_callback(Registers regs) {
    (void) regs;
    //TODO: https://wiki.osdev.org/Programmable_Interval_Timer
    tick++;
    time_since_boot += 1 / (double)get_timer_div();
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
