#ifndef VESA_FRAMEWORK_H_
#define VESA_FRAMEWORK_H_

#include "mem.h"
#include <stdint.h>

#define BUFFER_SIZE VESA_WIDTH*VESA_HEIGHT*32

void init_vesa_fb(Memory_Manager* mm);
void swap_buffers(void);
void fill_rect(int x_, int y_, int w, int h, uint32_t color);
void put_pixel(uint32_t x, uint32_t y, uint32_t color_index);
uint32_t* get_framebuffer(void);

#endif
