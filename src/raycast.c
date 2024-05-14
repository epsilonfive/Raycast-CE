#include "raycast.h"

#include <graphx.h>
#include <debug.h>
#include <math.h>
#include <tice.h>

#include "state.h"
#include "fast_trig.h"

#define DX(angle, dy) (FAST_COS(angle) * dy / FAST_SIN(angle))
#define DY(angle, dx) (FAST_SIN(angle) * dx / FAST_COS(angle))
#define RDX(angle, offset) (((angle > (ANGLE_RESOLUTION / 4)) && (angle < 3 * (ANGLE_RESOLUTION / 4)) ? -offset : (TILE_WIDTH - offset)))
#define RDY(angle, offset) (((angle < (ANGLE_RESOLUTION / 2)) ? (TILE_WIDTH - offset) : -offset))

void renderWorld(struct state *state) {
	int angle_start = state->player->angle - (FOV >> 1);
	int current_angle = angle_start;
	if (angle_start < 0) angle_start += ANGLE_RESOLUTION;
	//main graphics loop
	gfx_SetColor(18);
	gfx_Line(state->player->x / 128, state->player->y / 128, (state->player->x + FAST_COS(angle_start)) / 128, (state->player->y + FAST_SIN(angle_start)) / 128);
	gfx_Line(state->player->x / 128, state->player->y / 128, (state->player->x + FAST_COS((angle_start + FOV) % ANGLE_RESOLUTION)) / 128, (state->player->y + FAST_SIN((angle_start + FOV) % ANGLE_RESOLUTION)) / 128);
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
		int rdx = RDX(current_angle, state->player->x % TILE_WIDTH);
		current_x_a += rdx;
		if (cos) {
			int dy_temp = DY(current_angle, rdx);
			// if (dy < TILE_HEIGHT * MAX_ADJUST_TILES)
			current_y_a += dy_temp;
		}
		//dbg_sprintf(dbgout, "Added %d\n", DY(current_angle, rdx));
		int rdy = RDY(current_angle, state->player->y % TILE_HEIGHT);
		current_y_b += rdy;
		if (sin) {
			int dx_temp = DX(current_angle, rdy);
			// if (dx < TILE_WIDTH * MAX_ADJUST_TILES)
			current_x_b += dx_temp;
		}
		//slightly faster than using the generic ones probably
		int dx = DX(current_angle, ((cos < 0) ? -TILE_WIDTH : TILE_WIDTH));
		int dy = DY(current_angle, ((sin < 0) ? -TILE_HEIGHT : TILE_HEIGHT));
		int temp_x_a = current_x_a, temp_y_a = current_y_a;
		int temp_x_b = current_x_b, temp_y_b = current_y_b;
		bool horizontal_found = false;
		bool vertical_found = false;
		dbg_sprintf(dbgout, "dx: %d dy: %d What %d %d\n", dx, dy, current_angle, cos * TILE_WIDTH / sin);
		//do the check
		while (!horizontal_found || !vertical_found) {
			//check if we're out of bounds
			if (temp_x_a < 0 || temp_x_a > TILE_WIDTH * MAP_WIDTH ||
				temp_y_a < 0 || temp_y_a > TILE_HEIGHT * MAP_HEIGHT) {
				vertical_found = true;
			}
			if (temp_x_b < 0 || temp_x_b > TILE_WIDTH * MAP_WIDTH ||
				temp_y_b < 0 || temp_y_b > TILE_HEIGHT * MAP_HEIGHT) {
				horizontal_found = true;
			}
			//dbg_sprintf(dbgout, "Checking point (%d, %d) for angle...%d\n", temp_x_a, temp_y_a, current_angle);
			if (!vertical_found && state->map->data[temp_y_a / TILE_HEIGHT][temp_x_a / TILE_WIDTH]) {
				//we hit a wall vertically
				gfx_SetColor(18);
				//dbg_sprintf(dbgout, "Found wall at (%d, %d) (vertical check)\n", temp_x_a, temp_y_a);
				//vertical_found = true;
				gfx_SetPixel(temp_x_a / 128, temp_y_a / 128);
			} else gfx_SetColor(222);
			if (!horizontal_found && state->map->data[temp_y_b / TILE_HEIGHT][temp_x_b / TILE_WIDTH]) {
				//hit a wall horizontally
				//gfx_SetColor(224);
				//gfx_SetPixel(temp_x_b / 16, temp_y_b / 16);
				//dbg_sprintf(dbgout, "Found wall at (%d, %d) (horizontal check)\n", temp_x_b, temp_y_b);
				horizontal_found = true;
			}
			//current_x_a and current_y_a will be used to check the horizontal lines, the others for the vertical
			if (!vertical_found) {
				if (cos < 0) temp_x_a -= TILE_WIDTH;
				else temp_x_a += TILE_WIDTH;
				temp_y_a += dy;
			}
			if (!horizontal_found) {
				temp_x_b += dx;
				if (sin < 0) temp_y_b -= TILE_HEIGHT;
				else temp_y_b += TILE_HEIGHT;
			}
		}
	}
}