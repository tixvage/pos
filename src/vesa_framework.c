#include "vesa_framework.h"
#include "std.h"
#include "vesa.h"

uint32_t* back_buffer;

void init_vesa_fb(Memory_Manager* mm) {
    back_buffer = mm_alloc(mm, BUFFER_SIZE);
}

void swap_buffers(void) {
    for(int32_t y = 0; y < VESA_HEIGHT; y++)
        for(int32_t x = 0; x < VESA_WIDTH; x++)
                *(get_vesa_fb() + VESA_WIDTH * y + x) = *(back_buffer + VESA_WIDTH * y + x);
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
