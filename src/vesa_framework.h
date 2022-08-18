#ifndef VESA_FRAMEWORK_H_
#define VESA_FRAMEWORK_H_

#include <stdint.h>

void spaw_buffers(void);
void fill_rect(int x_, int y_, int w, int h, uint32_t color);
void put_pixel(uint32_t x, uint32_t y, uint32_t color_index);
uint32_t* get_framebuffer(void);

#endif
