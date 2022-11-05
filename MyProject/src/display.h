#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <stdio.h>

// have 30 frames every second
#define FPS 10
// time in millisecond taken by each frame
#define FRAME_TARGET_TIME (1000 / FPS)

#define YELLOW (0xFFFFFF00)
#define BLUE (0xFF0000FF)
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void); 
void draw_grid(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1);
void render_color_buffer(void); 
void clear_color_buffer(uint32_t color);
void destroy_window(void);
