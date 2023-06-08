#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <stdio.h>
#include "triangle.h"'

// have 30 frames every second
#define FPS 10
// time in millisecond taken by each frame
#define FRAME_TARGET_TIME (1000 / FPS)


#pragma region Color Macros
#define BLUE (0xFF0000FF)
#define GREEN (0xFF00FF00)
#define PURPLE (0xFFA020F0)
#define RED (0xFFFF0000)
#define YELLOW (0xFFFFFF00)
#pragma endregion Color Macros



extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void); 
void draw_grid(void);
void draw_circle(float radius, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_line_BLA(int x0, int y0, int x1, int y1, uint32_t color);
#pragma region Triangle specific drawing methods
void draw_triangle(triangle_t t, uint32_t color, bool showVertices);
void draw_triangle_filled(triangle_t triangle, uint32_t fillColor, uint32_t borderColor);
void draw_triangle_filled(triangle_t triangle, uint32_t color);
void fill_flat_bottom(triangle_t flatBottom, uint32_t color);
void fill_flat_top(triangle_t flatBottom, uint32_t color);
#pragma endregion Triangle specific drawing methods
void render_color_buffer(void); 
void clear_color_buffer(uint32_t color);
void destroy_window(void);
int min_int(int a, int b);
int max_int(int a, int b);
