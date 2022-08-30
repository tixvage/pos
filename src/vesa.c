#include "vesa.h"
#include "mmu.h"

static uint32_t *vesa_fb, *vesa_pitch;

void init_vesa_graphics(uint32_t* multiboot_structure) {
    vesa_fb = (uint32_t *)multiboot_structure[22];
    page_table_set(vesa_fb, vesa_fb, 0x83);
    vesa_pitch = (uint32_t *)multiboot_structure[24];
}

uint32_t* get_vesa_fb(void) {
    return vesa_fb;
}

void put_pixel_vesa(int x, int y, uint32_t color) {
    uint32_t* pixel_addr = vesa_fb + VESA_WIDTH * y + x;
    *pixel_addr = color;
}
