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
#define GOLD (0xFFD700)
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
void fill_flat_bottom(triangle_t flatBottom, uint32_t color);
void fill_flat_top(triangle_t flatBottom, uint32_t color);
void draw_triangle_textured(triangle_t triangle, bool wireframe, uint32_t* texture);
void fill_flatTop_textured(int y2, int y1, int x2, int x1, int y0, int x0);
void fill_flatBottom_textured(int y1, int y0, int x1, int x0, int y2, int x2);
#pragma endregion Triangle specific drawing methods
void render_color_buffer(void); 
void clear_color_buffer(uint32_t color);
void destroy_window(void);
