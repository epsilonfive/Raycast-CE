#include "raycast.h"

#include <graphx.h>
#include <debug.h>
#include <math.h>
#include <tice.h>

#include "state.h"
#include "fast_trig.h"

#define DX(angle, dy) (FAST_COS(angle) * dy / FAST_SIN(angle))
#define DY(angle, dx) (FAST_SIN(angle) * dx / FAST_COS(angle))
#define RDX(angle, offset) (((angle > (ANGLE_RESOLUTION / 4)) && (angle < 3 * (ANGLE_RESOLUTION / 4)) ? offset : TILE_WIDTH - offset))
#define RDY(angle, offset) (((angle > (ANGLE_RESOLUTION / 2)) ? offset : TILE_WIDTH - offset))

void renderWorld(struct state *state) {
	int angle_start = state->player->angle - (FOV >> 1);
	int current_angle = angle_start;
	if (angle_start < 0) angle_start += ANGLE_RESOLUTION;
	//main graphics loop
	gfx_SetColor(18);
	gfx_Line(state->player->x / 16, state->player->y / 16, (state->player->x + FAST_COS(angle_start)) / 16, (state->player->y + FAST_SIN(angle_start)) / 16);
	gfx_Line(state->player->x / 16, state->player->y / 16, (state->player->x + FAST_COS((angle_start + FOV) % ANGLE_RESOLUTION)) / 16, (state->player->y + FAST_SIN((angle_start + FOV) % ANGLE_RESOLUTION)) / 16);
	gfx_SetColor(224);
	for (int i = 0; i < FOV; i++) {
		//this could maybe cause some problems otherwise
		current_angle = (i + angle_start) % ANGLE_RESOLUTION;
		//start by figuring out where the intersection point is between the player and the nearest wall in both x and y
		int current_x_a = state->player->x, current_y_a = state->player->y;
		int current_x_b = current_x_a, current_y_b = current_y_a;
		int sin = FAST_SIN(current_angle);
		int cos = FAST_COS(current_angle);
		//fix the current x and y (set to edge of cell and stuff)
		if (sin) {
			int rdx = RDX(current_angle, state->player->x % TILE_WIDTH);
			current_x_a += rdx;
			current_y_a += DY(current_angle, rdx);
			if (current_y_a / 16 < LCD_HEIGHT)
				gfx_SetPixel(current_x_a / 16, current_y_a / 16);
		}
		if (cos) {
			int rdy = RDY(current_angle, state->player->y % TILE_HEIGHT);
			current_x_b += DX(current_angle, rdy);
			current_y_b += rdy;
		}
		//slightly faster than using the generic ones probably
		int dx = DX(current_angle, TILE_WIDTH);
		int dy = DY(current_angle, TILE_HEIGHT);
		int temp_x_a = current_x_a, temp_y_a = current_y_a;
		int temp_x_b = current_x_b, temp_y_b = current_y_b;
		dbg_sprintf(dbgout, "dx: %d dy: %d\n", dx, dy);
		//do the check
		while (true) {
			//dbg_sprintf(dbgout, "Checking point (%d, %d) for angle...%d\n", temp_x_a, temp_y_a, current_angle);
			if (state->map->data[temp_y_a / TILE_HEIGHT][temp_x_a / TILE_WIDTH]) {
				//we hit a wall vertically
				//gfx_SetPixel(temp_x_a / 16, temp_y_a / 16);
				//dbg_sprintf(dbgout, "Found wall at (%d, %d) (vertical check)\n", temp_x_a, temp_y_a);
				break;
			}
			if (state->map->data[temp_y_b / TILE_HEIGHT][temp_x_b / TILE_WIDTH]) {
				//hit a wall horizontally
				//dbg_sprintf(dbgout, "Found wall at (%d, %d) (horizontal check)\n", temp_x_b, temp_y_b);
				break;
			}
			//current_x_a and current_y_a will be used to check the horizontal lines, the others for the vertical
			temp_x_a += TILE_WIDTH;
			temp_y_a += dy;
			temp_x_b += dx;
			temp_y_b += TILE_HEIGHT;
			//check if we're out of bounds
			if (temp_x_a < 0 || temp_x_a > TILE_WIDTH * MAP_WIDTH ||
				temp_x_b < 0 || temp_x_b > TILE_WIDTH * MAP_WIDTH)
				break;
			if (temp_y_a < 0 || temp_y_a > TILE_HEIGHT * MAP_HEIGHT ||
				temp_y_b < 0 || temp_y_b > TILE_HEIGHT * MAP_HEIGHT)
				break;
		}
	}
}