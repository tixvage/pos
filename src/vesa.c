#include "vesa.h"
#include "mmu.h"
#include "vmm.h"
#include "vesa_framework.h"

static uint32_t *vesa_fb, vesa_pitch;

void init_vesa_graphics(uint32_t* multiboot_structure) {
    uintptr_t temp_vesa_fb = multiboot_structure[22];
    vesa_pitch = multiboot_structure[24];
    uint32_t size = VESA_HEIGHT*vesa_pitch;
    uintptr_t buff = (uintptr_t) aligned_alloc(0x1000, size);
    for (uint32_t i = 0; i < size/0x1000; i++) {
        Page* p = paging_get_page(buff + 0x1000*i, true, 0);
        *p = (temp_vesa_fb + 0x1000*i) | PAGE_PRESENT | PAGE_RW;
    }

    vesa_fb = (uint32_t*) buff;
}

uint32_t* get_vesa_fb(void) {
    return vesa_fb;
}

uint32_t get_vesa_pitch(void) {
    return vesa_pitch;
}

void put_pixel_vesa(int x, int y, uint32_t color) {
    uint32_t* pixel_addr = vesa_fb + VESA_WIDTH * y + x;
    *pixel_addr = color;
}
