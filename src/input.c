#include "input.h"

static Input g_input;

Input* get_input(void) {
    return &g_input;
}

void input_tick(void) {
    g_input.type = 0;
    g_input.key = 0;
}
