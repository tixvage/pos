#ifndef VGA_H_
#define VGA_H_

#include <stdint.h>

#define VGA_MISC_PORT 0x3c2
#define CRTC_INDEX_PORT 0x3d4
#define CRTC_DATA_PORT 0x3d5
#define SEQUENCER_INDEX_PORT 0x3c4
#define SEQUENCER_DATA_PORT 0x3c5
#define GC_INDEX_PORT 0x3ce
#define GC_DATA_PORT 0x3cf
#define AC_INDEX_PORT 0x3c0
#define AC_READ_PORT 0x3c1
#define AC_WRITE_PORT 0x3c0
#define AC_RESET_PORT 0x3da

void init_vga_graphics(void);
void vga_put_pixel(uint32_t x, uint32_t y, uint8_t color_index);

#endif

