#include "gdt.h"
#include "input.h"
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
#include "pmm.h"
#include "vmm.h"

#define FPS 1000
#define DT (1.0/(float)FPS)

void clear_graphical_screen(uint32_t color) {
    for (uint32_t i = 0; i < VESA_HEIGHT*VESA_WIDTH; i++) {
        get_framebuffer()[i] = color;
    }
}

void kernel_main(void* mb) {
    void* multiboot_structure = (void*) phys_to_virt(mb);
    init_pmm(multiboot_structure);
    init_paging();
    gdt_install();
    isr_install();
    __asm__("sti");

    init_timer(1000);
    init_keyboard();
    init_mouse();
    init_vesa_graphics(multiboot_structure);
    init_vesa_fb();
    while (true) {
        clear_graphical_screen(0x0D8A8A);
        fill_rect(0, 0, VESA_WIDTH, 20, 0xFFFFFF);
        fill_rect(5, 1, 18, 18, 0x00FF00);
        fill_rect(8, 4, 12, 12, 0xFF0000);
        fill_rect(11, 7, 6, 6, 0x0000FF);
        print_str("File", 40, 2, 0x000000);
        print_str("Edit", 90, 2, 0x000000);
        print_str("View", 140, 2, 0x000000);
        print_str("Label", 190, 2, 0x000000);
        print_str("Special", 248, 2, 0x000000);
        render_mouse();
        input_tick();
        swap_buffers();
        sleep(1000/FPS);
    }
}
