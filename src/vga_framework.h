#ifndef VGA_FRAMEWORK_H_
#define VGA_FRAMEWORK_H_

#include <stdint.h>

void spaw_buffers(void);
void fill_rect(int x_, int y_, int w, int h, int color);
void put_pixel(uint32_t x, uint32_t y, uint8_t color_index);

#endif
