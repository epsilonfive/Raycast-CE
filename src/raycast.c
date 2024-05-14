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
#define DOT_PRODUCT(x1, y1, x2, y2) (x1 * x2 + y1 * y2)
#define ANGLE_VECTOR_X(angle) (FAST_COS(state->player->angle) * PLAYER_VECTOR_LENGTH)
#define ANGLE_VECTOR_Y(angle) (FAST_SIN(state->player->angle) * PLAYER_VECTOR_LENGTH)





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
			if (dy_temp < TILE_HEIGHT * MAX_ADJUST_TILES) current_y_a += dy_temp;
			else current_y_a += dy_temp;
		}
		//dbg_sprintf(dbgout, "Added %d\n", DY(current_angle, rdx));
		int rdy = RDY(current_angle, state->player->y % TILE_HEIGHT);
		current_y_b += rdy;
		if (sin) {
			int dx_temp = DX(current_angle, rdy);
			if (dx_temp < TILE_WIDTH * MAX_ADJUST_TILES) current_x_b += dx_temp;
			else current_x_b += TILE_WIDTH * MAX_ADJUST_TILES;
		}
		//slightly faster than using the generic ones probably
		int dx = DX(current_angle, ((sin < 0) ? -TILE_WIDTH : TILE_WIDTH));
		int dy = DY(current_angle, ((cos < 0) ? -TILE_HEIGHT : TILE_HEIGHT));
		int temp_x_a = current_x_a, temp_y_a = current_y_a;
		int temp_x_b = current_x_b, temp_y_b = current_y_b;
		bool horizontal_found = false;
		bool vertical_found = false;
		//dbg_sprintf(dbgout, "dx: %d dy: %d What %d %d\n", dx, dy, current_angle, cos * TILE_WIDTH / sin);
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
			if (!vertical_found && state->map->data[temp_y_a / TILE_HEIGHT][(temp_x_a - (cos < 0)) / TILE_WIDTH]) {
				//we hit a wall vertically
				//gfx_SetColor(18);
				//dbg_sprintf(dbgout, "Found wall at (%d, %d) (vertical check)\n", temp_x_a, temp_y_a);
				//vertical_found = true;
				//gfx_SetPixel(temp_x_a / 128, temp_y_a / 128);
				vertical_found = true;
			}
			if (!horizontal_found && state->map->data[(temp_y_b - (sin < 0)) / TILE_HEIGHT][temp_x_b / TILE_WIDTH]) {
				//hit a wall horizontally
				//gfx_SetColor(224);
				//gfx_SetPixel(temp_x_b / 128, temp_y_b / 128);
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
		if (true) { //check to see if we went out of bounds eventually
			//so we have the points we need now
			float player_angle_vector_x = (float) FAST_COS(state->player->angle) * PLAYER_VECTOR_LENGTH;
			float player_angle_vector_y = (float) FAST_SIN(state->player->angle) * PLAYER_VECTOR_LENGTH;
			//differences
			float difference_x_a = (float) temp_x_a - state->player->x;
			float difference_y_a = (float) temp_y_a - state->player->y;
			float difference_x_b = (float) temp_x_b - state->player->x;
			float difference_y_b = (float) temp_y_b - state->player->y;
			//numerators and denominators
			float num_temp_a = DOT_PRODUCT(difference_x_a, difference_y_a, player_angle_vector_x, player_angle_vector_y);
			float num_temp_b = DOT_PRODUCT(difference_x_b, difference_y_b, player_angle_vector_x, player_angle_vector_y);
			float denom_temp = DOT_PRODUCT(player_angle_vector_x, player_angle_vector_y, player_angle_vector_x, player_angle_vector_y);
			//the projection
			float projected_vector_a_x = num_temp_a * player_angle_vector_x / denom_temp;
			float projected_vector_a_y = num_temp_a * player_angle_vector_y / denom_temp;
			float projected_vector_b_x = num_temp_b * player_angle_vector_x / denom_temp;
			float projected_vector_b_y = num_temp_b * player_angle_vector_y / denom_temp;
			//calculate distances
			int dist_a = (int) sqrtf((float) (projected_vector_a_x * projected_vector_a_x + projected_vector_a_y * projected_vector_a_y));
			int dist_b = (int) sqrtf((float) (projected_vector_b_x * projected_vector_b_x + projected_vector_b_y * projected_vector_b_y));
			int dist = (dist_a < dist_b) ? dist_a : dist_b;
			int height = 240 - (dist / 64);
			if (height < 0) height = 0;
			dbg_sprintf(dbgout, "Height was %d %d %d\n", height, num_temp_a, num_temp_b);
			gfx_VertLine(i, 120 - (height / 2), height);
		}
	}
}