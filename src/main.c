#include "gdt.h"
#include "input.h"
#include "mouse.h"
#include "screen.h"
#include "std.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "vga.h"
#include "vga_framework.h"
#include "text.h"

#define FPS 120
#define DT (1.0/(float)FPS)

void clear_vga(void) {
    for(int32_t y = 0; y < 200; y++)
        for(int32_t x = 0; x < 320; x++)
            put_pixel(x, y, 0x01);
}

void putpixeli(uint32_t* mb, int x, int y, uint32_t c) {
    volatile uint32_t* framebuffer = (uint32_t *)mb[22];
    framebuffer[x + (y * mb[24] / 4)] = c;
}

void kernel_main(uint32_t* multiboot_structure) {
    while (1) {
        for (int x = 0; x < 1280; x++)
            for (int y = 0; y < 1024; y++)
                putpixeli(multiboot_structure, x, y, 0xFF181818);
    }

#if 0
    clear_screen();
    gdt_install();
    isr_install();
    __asm__("sti");

    init_keyboard();
    init_mouse();
    init_timer(1000);
    //TODO: tearing (not that important but :))
    init_vga_graphics();
    int i = 0;
    while (1) {
        clear_vga();
        fill_rect(i, 20, 20, 20, 0x03);
        print_str("hello font!", 50, 50);
        Input* input = get_input();
        switch (input->type) {
            case EVENT_KEY_DOWN: {
                switch (input->key) {
                    case 'D': {
                        i++;
                    } break;
                    case 'A': {
                        i--;
                    } break;
                }
            } break;
            default: {

            } break;
        }
        render_mouse();
        input_tick();
        spaw_buffers();
        sleep(1000/FPS);
    }
#endif
}
