#include "gdt.h"
#include "input.h"
#include "mem.h"
#include "mouse.h"
#include "ports.h"
#include "screen.h"
#include "std.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "vesa_framework.h"
#include "text.h"
#include "vesa.h"
#include "multiboot.h"
#include "mmu.h"

#define FPS 1000
#define DT (1.0/(float)FPS)

void clear_graphical_screen(void) {
    kmemset(get_framebuffer(), 0x181818, VESA_HEIGHT*VESA_WIDTH);
}

void kernel_main(void* mb) {
    void* multiboot_structure = phys_to_virt(mb);
    gdt_install();
    isr_install();
    __asm__("sti");

    init_timer(1000);
    init_keyboard();
    init_mouse();
    init_vesa_graphics(multiboot_structure);
    init_vesa_fb(0);
    while (true) {
        clear_graphical_screen();
        print_str("hello font!", 50, 50, 0xFF0000);
        render_mouse();
        input_tick();
        swap_buffers();
        sleep(1000/FPS);
    }
}
