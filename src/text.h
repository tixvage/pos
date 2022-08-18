#ifndef TEXT_H_
#define TEXT_H_

#include <stdint.h>


void vga_print_char(unsigned char c, int start_x, int start_y, uint32_t color);
void print_str(const char* str, int start_x, int start_y, uint32_t color);

#endif

