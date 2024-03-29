#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <stdio.h>
#include "triangle.h"'
#include "upng.h"

// have 30 frames every second
#define FPS 60
// time in millisecond taken by each frame
#define FRAME_TARGET_TIME (1000 / FPS)

#pragma region Color Macros ABGR
#define RED (0xFF0000FF) // RGB FF0000
#define BLUE (0xFFFF0000) // RGB 0000FF
#define GREEN (0x0000FF00) // RGB 00FF00
#define GOLD (0xFF00D7FF) // RGB FFD700
#define PURPLE (0xFFA020F0)
#define WHITE (0xFFFFFFFF)
#define YELLOW (0xFFFFFF00)
#pragma endregion Color Macros

// supported rendering modes
enum renderMode {
	RENDER_WIREFRAME,
	RENDER_FILLED_FLAT,
	RENDER_FILLED_GOURAUD,
	RENDER_WIRE_FILLED,
	RENDER_TEXTURED,
	RENDER_TEXTURED_WIRE
};

// supported culling methods
enum cull_method {
	CULL_NONE,
	CULL_BACKFACE
};

bool initialize_window(void);
int get_window_width(void);
int get_window_height(void);
int get_cull_method(void);
int get_render_method(void);
void set_render_method(int method);
void set_cull_method(int method);
bool is_cull_backface();

#pragma region drawing primitives
void draw_grid(void);
void draw_circle(float radius, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
// function to draw a texture pixel at (x,y) pixel using interpolation
void draw_texel(int pixelX, int pixelY, triangle_t t);
void draw_pixel_shaded(int pixelX, int pixelY, triangle_t t, int shadeMode);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_line_BLA(int x0, int y0, int x1, int y1, uint32_t color);
#pragma endregion
#pragma region Triangle specific drawing methods
void draw_triangle(triangle_t t, uint32_t color, bool showVertices);
void draw_triangle_filled(triangle_t triangle, uint32_t fillColor, uint32_t borderColor);
void fill_flat_bottom(triangle_t flatBottom, uint32_t color);
void fill_flat_top(triangle_t flatBottom, uint32_t color);
//void draw_triangle_textured(triangle_t triangle, bool wireframe, uint32_t* texture);
void draw_triangle_textured(triangle_t triangle, bool wireframe);
void draw_triangle_shaded(triangle_t triangle, uint32_t fillColor, uint32_t borderColor, int shadeMode);

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);
#pragma endregion
#pragma region other methods
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void clear_z_buffer();
void destroy_window(void);
#pragma endregion
