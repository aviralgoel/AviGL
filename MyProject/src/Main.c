#pragma region header files
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#pragma endregion

#pragma region global variables
#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS]; // 9x9x9 cube
vec2_t projected_points[N_POINTS];
vec3_t camera_position = {.x = 0, .y= 0, .z=-5};
float scale_factor = 512; // how far apart should the points be
#define YELLOW (0xFFFFFF00)
#define PINK (0xFFFFC0CB)
#define PURPLE (0xFFA020F0)
bool is_running = false;
#pragma endregion
////////////////////////////////////////////////////////////////////////////////
// Function that receives a 3D vector and returns a orthographically projected 2D point
////////////////////////////////////////////////////////////////////////////////
vec2_t ortho_Project(vec3_t point) {
	vec2_t projected_point = {
		.x = (scale_factor * point.x),
		.y = (scale_factor * point.y)
	};
	return projected_point;
}
vec2_t perspective_Project(vec3_t point) {
	vec2_t projected_point = {
		.x = (scale_factor * point.x) / point.z,
		.y = (scale_factor * point.y) / point.z
	};
	return projected_point;
}
/**
 * memory allocation for color_buffer and color_buffer_texture. 
 * populating cube_points
 */
void setup(void) {
	// Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	int point_count = 0;

	// Start loading my array of vectors
	// From -1 to 1 (in this 9x9x9 cube)
	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t new_point = { .x = x, .y = y, .z = z };
				cube_points[point_count++] = new_point;
			}
		}
	}
}
/**
 * Polls for user input
 */
void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	// Quit if X button is clicked
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		// Quit if ESC Key is pressed
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		break;
	}
}
/**
 * Does stuff every frame
 */
void update(void) {
	
	// orthographic projection of 3D cube points
	for (int i = 0; i < N_POINTS; i++) {
		vec3_t point = cube_points[i];
		point.z -= camera_position.z;
		// Project the current point
		//vec2_t projected_point = ortho_Project(point);
		vec2_t projected_point = perspective_Project(point);
		// Save the projected 2D vector in the array of projected points
		projected_points[i] = projected_point;
	}
}

/**
 * Draws stuff every frame
 */
void render(void) {
	draw_grid();

	// Loop all projected points and render them
	for (int i = 0; i < N_POINTS; i++) {
		vec2_t projected_point = projected_points[i];
		draw_rect(
			projected_point.x + (window_width / 2),
			projected_point.y + (window_height / 2),
			4,
			4,
			0xFF000000
		);
	}

	render_color_buffer();

	clear_color_buffer(PURPLE);

	SDL_RenderPresent(renderer);
}

int main(void) {
	is_running = initialize_window();

	setup();
	// game loop
	while (is_running) {
		process_input();
		update();
		render();
	}

	// garbage collection
	destroy_window();
	return 0;
}