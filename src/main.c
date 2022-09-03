#include "gdt.h"
#include "input.h"
#include "mouse.h"
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

#define	wsize	sizeof(uint32_t)
#define	wmask	(wsize - 1)

void memset_fast3(void* dst0, register int c0, register size_t length) {
	register size_t t;
	register uint8_t *dst;

	dst = dst0;
	t = length / wsize;
	do {
		*(uint32_t *)dst = c0;
		dst += wsize;
	} while (--t != 0);
}

void clear_graphical_screen(uint32_t color) {
    memset_fast3(get_framebuffer(), color, get_vesa_pitch()*VESA_HEIGHT);
}

void render_time(int x, int y) {
    Time time = get_time();
    char buffer[9];
    char *hour_fmt, *minute_fmt, *second_fmt;

    hour_fmt = buffer;
    if (time.hour < 10) {
        buffer[0] = '0';
        hour_fmt = &buffer[1];
    }
    kint_to_ascii(time.hour, hour_fmt);
    buffer[2] = ':';

    minute_fmt = &buffer[3];
    if (time.minute < 10) {
        buffer[3] = '0';
        minute_fmt = &buffer[4];
    }
    kint_to_ascii(time.minute, minute_fmt);
    buffer[5] = ':';

    second_fmt = &buffer[6];
    if (time.second < 10) {
        buffer[6] = '0';
        second_fmt = &buffer[7];
    }
    kint_to_ascii(time.second, second_fmt);

    print_str(buffer, x, y, 0x000000);
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
        render_time(850, 2);
        fill_rect(955, 1, 19, 19, 0xF0F000);
        print_str("?", 960, 2, 0x000000);
        render_mouse();
        if (mouse_pressed_left()) {
            print_str("left pressed", 500, 500, 0x000000);
        }
        input_tick();
        swap_buffers();
        sleep(1000/FPS);
    }
}
