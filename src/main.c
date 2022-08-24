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
#include "multiboot.h"

#define FPS 120
#define DT (1.0/(float)FPS)

void clear_graphical_screen(void) {
    kmemset(get_framebuffer(), 0x181818, VESA_WIDTH*VESA_HEIGHT);
}

void dump_mb_infos(multiboot* boot) {
    int pos_y = 20;
    int pos_x = 20;
    uint32_t color = 0xFF0000;
    print_str(boot->flags & MULTIBOOT_FLAG_MMAP ? "Set" : "Not set", pos_y, pos_x, color);
}

void kernel_main(void* multiboot_structure) {
    multiboot* mb = multiboot_structure;
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
    while (!false) {
        clear_graphical_screen();
        print_str("hello font!", 50, 50, 0xFF0000);
        render_mouse();
        input_tick();
        swap_buffers();
    }
}
