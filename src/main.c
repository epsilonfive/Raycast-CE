#include <graphx.h>
#include <tice.h>
#include <debug.h>

#include "state.h"
#include "input.h"
#include "player.h"
#include "map.h"
#include "raycast.h"
#include "fast_trig.h"

int main(void) {
    static struct map map;
    static struct player player;
    static struct input input;
    static struct state state = {
        .map = &map,
        .player = &player,
        .input = &input,
    };
    //initialize graphics
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_ZeroScreen();
    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(255);
    gfx_SetTextTransparentColor(0);
    gfx_PrintStringXY("Initializing...", 5, 5);
    gfx_BlitBuffer();
    //initialize things
    initializeMap(&map);
    initializeInput(&input);
    initializeFastTrig();
    //initialize the timer
    timer_Enable(1, TIMER_CPU, TIMER_0INT, TIMER_UP);
    //main loop
    while (!getKeyState(&input, KEY_CLEAR)) {
        updateInput(&state);
        updatePlayer(&state);
        //update the map eventually, if that's needed
        gfx_FillScreen(255);
        renderWorld(&state);
        renderMap(&map);
        renderPlayer(&player);
        gfx_SwapDraw();
    }
    //end everything
    gfx_End();
    return 0;
}