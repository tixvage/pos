#include "keyboard.h"
#include "isr.h"
#include "ports.h"
#include "std.h"
#include "screen.h"
#include <stdint.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "backspace", "tab", "q", "w", "e", 
    "r", "t", "y", "u", "i", "o", "p", "[", "]", "enter", "lctrl", 
    "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", 
    "lshift", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", 
    "/", "rshift", "keypad *", "lalt", "spacebar"};
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
    'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
    'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
    'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

//TODO: graphical mode support
static void keyboard_callback(Registers regs) {
    (void) regs;
    uint8_t scancode = port_byte_in(0x60);

    if (scancode > SC_MAX) return;
    char letter = sc_ascii[(int)scancode];
    char str[2] = {letter, '\0'};
    kstrappend(key_buffer, letter);
    kprint(str);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback); 
}
