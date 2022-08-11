#ifndef IDT_H_
#define IDT_H_

#include <stdint.h>

#define KERNEL_CS 0x08

typedef struct Idt_Gate {
    uint16_t low_offset;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags; 
    uint16_t high_offset;
} __attribute__((packed)) Idt_Gate;

typedef struct Idt_Register {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) Idt_Register;


void set_idt_gate(int n, uint32_t handler);
void set_idt(void);

#endif

