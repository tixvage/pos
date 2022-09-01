#ifndef MOUSE_H_
#define MOUSE_H_

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000010
#define PS2Rightbutton 0b00000100
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

#include <stdbool.h>

void init_mouse(void);
void render_mouse(void);
int get_mouse_x(void);
int get_mouse_y(void);
bool mouse_pressed_left(void);
bool mouse_pressed_right(void);

#endif

