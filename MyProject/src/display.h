#pragma once
#include<stdbool.h>
#include<stdint.h>
#include<SDL.h>
#include <stdio.h>

#pragma region global variables
extern int window_width;
extern int window_height;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* colorbuffer;;
extern SDL_Texture* color_buffer_texture;
#pragma endregion

bool initialize_window(void);
void draw_grid(int columnGap, int rowGap, uint32_t gridColor);
void draw_rect(int x0, int y0, int width, int height, uint32_t fillColor);
void draw_pixel(int x, int y, uint32_t color);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void destroy_window(void);
