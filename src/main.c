#include "gdt.h"
#include "mouse.h"
#include "screen.h"
#include "std.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "vga.h"

void clear_vga(void) {
    for(int32_t y = 0; y < 200; y++)
        for(int32_t x = 0; x < 320; x++)
            vga_put_pixel(x, y, 0x01);
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
    clear_vga();
    while(1) {
        render_mouse_zort();
    }
}
