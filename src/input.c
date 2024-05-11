#include "input.h"

#include <stdint.h>
#include <debug.h>
#include <tice.h>
#include <keypadc.h>

#include "state.h"

void updateKeyState(struct key *key) {
    if (kb_IsDown(key->key)) {
        //if the key was active last loop, it was held
        key->state = (key->state == KEY_PRESSED || key->state == KEY_HELD) ? KEY_HELD : KEY_PRESSED;
    } else {
        //if the key was active last loop, it was released
        key->state = (key->state && key->state != KEY_RELEASED) ? KEY_RELEASED : KEY_INACTIVE;
    }
}

void updateInput(struct state *state) {
    kb_Scan();
    for (int i = 0; i < NUM_DEFAULT_KEYS; i++)
        updateKeyState(&state->input->key[i]);
}

//returns the state of the default key at index
uint8_t getKeyState(struct input *input, int index) {
    return input->key[index].state;
}

void initializeInput(struct input *input) {
    //having these be static is okay, I think
    struct key enter =  {.key = kb_KeyEnter};
    struct key up    =  {.key = kb_KeyUp};
    struct key down  =  {.key = kb_KeyDown};
    struct key left  =  {.key = kb_KeyLeft};
    struct key right =  {.key = kb_KeyRight};
    struct key add =    {.key = kb_KeyAdd};
    struct key sub =    {.key = kb_KeySub};
    struct key alpha =  {.key = kb_KeyAlpha};
    struct key mode =   {.key = kb_KeyMode};
    struct key tan =    {.key = kb_KeyTan};
    struct key window = {.key = kb_KeyWindow};
    struct key clear =  {.key = kb_KeyClear};
    struct key *default_key[NUM_DEFAULT_KEYS] = {
        &enter,
        &up,
        &down,
        &left,
        &right,
        &add,
        &sub,
        &alpha,
        &mode,
        &tan,
        &window,
        &clear,
    };
    for (int i = 0; i < NUM_DEFAULT_KEYS; i++) {
        input->key[i].key = default_key[i]->key;
        input->key[i].state = KEY_INACTIVE;
    }
}