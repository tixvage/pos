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

#define FPS 1000
#define DT (1.0/(float)FPS)

void clear_graphical_screen(void) {
    kmemset(get_framebuffer(), 0x181818, VESA_HEIGHT*VESA_WIDTH*4);
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
        clear_graphical_screen();
        char buffer2[20];
        kint_to_ascii(pmm_total_memory() / 1024 / 1024, buffer2);
        print_str("Available memory: ", 20, 70, 0xFF0000);
        int temp_len = kstrlen(buffer2);
        buffer2[temp_len] = 'm';
        buffer2[temp_len + 1] = 'b';
        buffer2[temp_len + 2] = '\0';
        print_str(buffer2, 180, 70, 0xFF0000);
        render_mouse();
        input_tick();
        swap_buffers();
        sleep(1000/FPS);
    }
}
