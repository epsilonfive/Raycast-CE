#ifndef INPUT_H
#define INPUT_H

#include <keypadc.h>
#include <stdint.h>

#include "state.h"

#define KEY_INACTIVE     (uint8_t) 0
#define KEY_PRESSED      (uint8_t) (1 << 0)
#define KEY_HELD         (uint8_t) (1 << 1)
#define KEY_RELEASED     (uint8_t) (1 << 2)

//the default keys, these shouldn't be changed
#define NUM_DEFAULT_KEYS 12
#define KEY_ENTER        0
#define KEY_UP           1
#define KEY_DOWN         2
#define KEY_LEFT         3
#define KEY_RIGHT        4
#define KEY_ADD          5
#define KEY_SUB          6
#define KEY_ALPHA        7
#define KEY_MODE         8
#define KEY_TAN          9
#define KEY_WINDOW       10
#define KEY_CLEAR        11

struct key {
    kb_lkey_t key;
    uint8_t state;
};

struct input {
    struct key key[NUM_DEFAULT_KEYS];
};

void updateInput(struct state *state);
void initializeInput(struct input *input);
uint8_t getKeyState(struct input *input, int index);

#endif