#include "vga_framework.h"
#include "std.h"
#include "vga.h"

static uint8_t back_buffer[320*200*8];

void spaw_buffers(void) {
    kmemcpy(back_buffer, get_fb_segment(), 200 * ((8 | 7) >> 3) * 320);
}

void fill_rect(int x_, int y_, int w, int h, int color) {
    for (int x = x_; x < x_ + w; x++)
        for (int y = y_; y < y_ + h; y++)
            put_pixel(x, y, color);
}

void put_pixel(uint32_t x, uint32_t y, uint8_t color_index) {
    uint8_t* pixel_address = back_buffer + 320 * y + x;
    *pixel_address = color_index;
}
