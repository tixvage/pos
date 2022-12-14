#include "mouse.h"
#include "isr.h"
#include "ports.h"
#include "std.h"
#include "screen.h"
#include "vesa_framework.h"
#include "vesa.h"
#include <stdint.h>

static void mouse_wait(void) {
    uint64_t timeout = 100000;
    while (timeout--) {
        if ((port_byte_in(0x64) & 0b10) == 0) {
            return;
        }
    }
}

static void mouse_wait_input(void) {
    uint64_t timeout = 100000;
    while (timeout--) {
        if (port_byte_in(0x64) & 0b1) {
            return;
        }
    }
}

static void mouse_write(uint8_t value) {
    mouse_wait();
    port_byte_out(0x64, 0xD4);
    mouse_wait();
    port_byte_out(0x60, value);
}

static uint8_t mouse_read(void) {
    mouse_wait_input();
    return port_byte_in(0x60);
}

//TODO: ah yes data structures
//TODO: proper mouse input system
int mouse_byte_cycle = 0;
bool mouse_packet_ready = false;
int mouse_x = 0, mouse_y = 0, old_mouse_x = 0, old_mouse_y = 0;
uint8_t mouse_packets[4];
bool left_pressed;
bool right_pressed;

static void mouse_callback(Registers regs) {
    (void) regs;
    uint8_t scancode = port_byte_in(0x60);
    
    switch (mouse_byte_cycle) {
        case 0: {
            if (mouse_packet_ready) break;
            if ((scancode & 0b00001000) == 0) break;
            mouse_packets[0] = scancode;
            mouse_byte_cycle++;
        } break;
        case 1: {
            if (mouse_packet_ready) break;
            mouse_packets[1] = scancode;
            mouse_byte_cycle++;
        } break;
        case 2: {
            if (mouse_packet_ready) break;
            mouse_packets[2] = scancode;
            mouse_packet_ready = true;
            mouse_byte_cycle = 0;
        } break;
    }

    if (!mouse_packet_ready) return;

    bool x_n, y_n, x_o, y_o;

    if (mouse_packets[0] & PS2XSign) x_n = true;
    else x_n = false;

    if (mouse_packets[0] & PS2YSign) y_n = true;
    else y_n = false;

    if (mouse_packets[0] & PS2XOverflow) x_o = true;
    else x_o = false;

    if (mouse_packets[0] & PS2YOverflow) y_o = true;
    else y_o = false;

    if (!x_n) {
        mouse_x += mouse_packets[1];
        if (x_o) mouse_x += 255;
    } else {
        mouse_packets[1] = 256 - mouse_packets[1];
        mouse_x -= mouse_packets[1];
        if (x_o) mouse_x -= 255;
    }

    if (!y_n){
        mouse_y -= mouse_packets[2];
        if (y_o) mouse_y -= 255;
    } else {
        mouse_packets[2] = 256 - mouse_packets[2];
        mouse_y += mouse_packets[2];
        if (y_o) mouse_y += 255;
    }

    left_pressed = mouse_packets[0] & PS2Leftbutton;
    right_pressed = mouse_packets[0] & PS2Rightbutton;

    //TODO: limit for other directions?
    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x >= VESA_WIDTH) mouse_x = VESA_WIDTH - 1;
    if (mouse_y >= VESA_HEIGHT) mouse_y = VESA_HEIGHT - 1;
    
    mouse_packet_ready = false;
}

static const char* cursor_ascii = {
    "99\n"
    "9#9\n"
    "9##9\n"
    "9###9\n"
    "9####9\n"
    "9#####9\n"
    "9######9\n"
    "9#######9\n"
    "9########9\n"
    "9#####9999\n"
    "9##9##9\n"
    "9#9 9##9\n"
    "99  9##9\n"
    "     9##9\n"
    "     9##9\n"
    "      99\n"
};

//TODO: ascii tools
static void render_ascii_art(int x, int y) {
    int x_r = x, y_r = y;

    for (int i = 0; i < kstrlen(cursor_ascii); i++) {
        switch (cursor_ascii[i]) {
            case '#': {
                if(x_r >= 0 && y_r >= 0 && x_r < VESA_WIDTH && y_r < VESA_HEIGHT)
                    put_pixel(x_r, y_r, 0xFFFFFF);
            } break;

            case '9': {
                if(x_r >= 0 && y_r >= 0 && x_r < VESA_WIDTH && y_r < VESA_HEIGHT)
                    put_pixel(x_r, y_r, 0x000000);
            } break;
            
            case '\n': {
                x_r = x - 1;
                y_r++;
            } break;
        }
        
        x_r++;
    }
}

void render_mouse(void) {
    //TODO: sensivity?
    
    render_ascii_art(mouse_x, mouse_y);

}

void init_mouse(void) {
    port_byte_out(0x64, 0xA8);
    mouse_wait();
    port_byte_out(0x64, 0x20);
    mouse_wait_input();
    uint8_t status = port_byte_in(0x60);
    status |= 0b10;
    //TODO: do we really need to wait in here https://wiki.osdev.org/Mouse_Input#Wait_for_ACK_from_Mouse
    mouse_wait();
    port_byte_out(0x64, 0x60);
    mouse_wait();
    port_byte_out(0x60, status);

    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();

    register_interrupt_handler(IRQ12, mouse_callback); 
}

int get_mouse_x(void) { return mouse_x; }
int get_mouse_y(void) { return mouse_y; }
bool mouse_pressed_left(void) { return left_pressed; }
bool mouse_pressed_right(void) { return right_pressed; }
