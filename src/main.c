#include "gdt.h"
#include "input.h"
#include "mem.h"
#include "mouse.h"
#include "screen.h"
#include "std.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "vesa_framework.h"
#include "text.h"
#include "vesa.h"

#define FPS 120
#define DT (1.0/(float)FPS)

void clear_graphical_screen(void) {
    for(int32_t y = 0; y < VESA_HEIGHT; y++)
        for(int32_t x = 0; x < VESA_WIDTH; x++)
            put_pixel(x, y, 0x181818);
}

void kernel_main(void* multiboot_structure) {
    clear_screen();
    gdt_install();
    isr_install();
    __asm__("sti");

    init_timer(1000);
    init_keyboard();
    init_mouse();
    uint32_t* memupper = (uint32_t*)(((uint32_t)multiboot_structure) + 8);
    uint32_t heap = 10*1024*1024;
    Memory_Manager mm = init_mm(heap, (*memupper)*1024 - heap - 10*1024);
    init_vesa_graphics(multiboot_structure);
    init_vesa_fb(&mm);
    int i = 0;
    int x = 0;
    while (1) {
        x++;
        clear_graphical_screen();
        fill_rect(i, 20, 20, 20, 0x0000FF);
        fill_rect(x, 200, 20, 20, 0x0000FF);
        print_str("hello font!", 50, 50, 0xFF0000);
        Input* input = get_input();
        switch (input->type) {
            case EVENT_KEY_DOWN: {
                switch (input->key) {
                    case 'D': {
                        i += 10;
                    } break;
                    case 'A': {
                        i -= 10;
                    } break;
                }
            } break;
            default: {

            } break;
        }
        render_mouse();
        input_tick();
        swap_buffers();
    }
}
