#include "gdt.h"
#include "mouse.h"
#include "screen.h"
#include "std.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "vga.h"
#include "vga_framework.h"

void clear_vga(void) {
    for(int32_t y = 0; y < 200; y++)
        for(int32_t x = 0; x < 320; x++)
            put_pixel(x, y, 0x01);
}

void kernel_main(const void* multiboot_structure, uint32_t multiboot_magic) {
    (void) multiboot_structure;
    (void) multiboot_magic;

    clear_screen();
    gdt_install();
    isr_install();
    __asm__("sti");

    init_keyboard();
    init_mouse();
    init_vga_graphics();
    //TODO: rendering system
    //TODO: tearing (not that important but :))
    int i = 0;
    while (1) {
        clear_vga();
        i = i > 320 ? 0 : i+1;
        fill_rect(i, 20, 20, 20, 0x03);
        render_mouse_zort();
        spaw_buffers();
    }
}
