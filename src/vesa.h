#ifndef VESA_H_
#define VESA_H_

#include <stdint.h>

#define VESA_HEIGHT 768
#define VESA_WIDTH 1024

void init_vesa_graphics(uint32_t* multiboot_structure);
uint32_t* get_vesa_fb(void);
uint32_t get_vesa_pitch(void);
void put_pixel_vesa(int x, int y, uint32_t color);

#endif

