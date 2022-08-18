#include "vesa.h"

static uint32_t *vesa_fb, *vesa_pitch;

void init_vesa_graphics(uint32_t* multiboot_structure) {
    vesa_fb = (uint32_t *)multiboot_structure[22];
    vesa_pitch = (uint32_t *)multiboot_structure[24];
}

uint32_t* get_vesa_fb(void) {
    return vesa_fb;
}

void put_pixel_vesa(int x, int y, uint32_t color) {
    uint32_t* pixel_addr = vesa_fb + 1024 * y + x;
    *pixel_addr = color;
}
