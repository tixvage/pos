#ifndef INPUT_H_
#define INPUT_H_

typedef enum Event_Type {
    EVENT_NONE = 0,
    EVENT_KEY_DOWN,
} Event_Type;

typedef struct Input {
    char key;
    Event_Type type;
} Input;

Input* get_input(void);
void input_tick(void);

#endif

