#include "vesa_framework.h"
#include "std.h"
#include "vesa.h"
#include "mmu.h"
#include "vmm.h"

static uint32_t* back_buffer;

void init_vesa_fb(void) {
    uint32_t size = VESA_HEIGHT*get_vesa_pitch();
    back_buffer = aligned_alloc(0x1000, size);
}

extern void memcpy_sse(uint32_t s, uint32_t d, uint32_t sz);

void swap_buffers(void) {
    memcpy_sse((uint32_t) get_vesa_fb(), (uint32_t) back_buffer, (VESA_HEIGHT*VESA_WIDTH) / 32);
/*
    __asm__ volatile("mov %%eax, %%esi;"
                     "mov %%ebx, %%edi;"
                     "rep movsd;" : :
                     "c" (VESA_HEIGHT*VESA_WIDTH), "a" (back_buffer), "b" (get_vesa_fb()));*/
}

void fill_rect(int x_, int y_, int w, int h, uint32_t color) {
    for (int x = x_; x < x_ + w; x++)
        for (int y = y_; y < y_ + h; y++)
            put_pixel(x, y, color);
}

void put_pixel(uint32_t x, uint32_t y, uint32_t color_index) {
    uint32_t* pixel_address = back_buffer + VESA_WIDTH * y + x;
    *pixel_address = color_index;
}

uint32_t* get_framebuffer(void) {
    return back_buffer;
}
