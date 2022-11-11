#include "display.h"
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Set width and height of the SDL window with the max screen resolution
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;

	// Create a SDL Window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	// Create a SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	return true;
}

void draw_grid(void) {
	for (int y = 0; y < window_height; y += 10) {
		for (int x = 0; x < window_width; x += 10) {
			color_buffer[(window_width * y) + x] = 0xFF444444;
		}
	}
}
void draw_circle(float radius, uint32_t color)
{
	int x = window_width / 2, y = window_height / 2;
	for (double i = 0; i < 2 * 3.14; i += 0.02)
	{
		int x1 = radius * cos(i) + x;
		int y1 = radius * sin(i) + y;
		draw_line(x, y, x1, y1, color);
	}
}

void draw_pixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
		color_buffer[(window_width * y) + x] = color;
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int current_x = x + i;
			int current_y = y + j;
			draw_pixel(current_x, current_y, color);
		}
	}
}

void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t))
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
/// <summary>
/// Digital differential analyzer line drawing algorithm
/// </summary>
/// <param name="x0"></param>
/// <param name="y0"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="color"></param>
void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
	float delta_x = x1 - x0;
	float delta_y = y1 - y0;

	int side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;

	float current_x = x0;
	float current_y = y0;
	for (int i = 0; i < side_length; i++)
	{
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}
/// <summary>
/// The Bresenham Line-Drawing Algorithm
/// </summary>
/// <param name="x0"></param>
/// <param name="y0"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="color"></param>
void draw_line_BLA(int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1; 
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;
	int e2 = 0;

	for (;;) { 
		draw_pixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) break; // end condition
		e2 = err << 1;
		if (e2 >= dy) 
		{ 
			err += dy; 
			x0 += sx; 
		} 
		else if (e2 <= dx) 
		{ 
			err += dx; 
			y0 += sy;
		} 
	}
}
/// <summary>
/// draws a wire frame triangle
/// </summary>
/// <param name="triangle">struct for triangle coordinates</param>
/// <param name="color">wire frame color of triangle</param>
/// <param name="showVertices">true if vertices should be highlighted in GREEN</param>


//void draw_filled_triangle(triangle_t triangle, uint32_t color)
//{	
//	triangle = sortVertsByY(triangle);
//	
//}

void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void destroy_window(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void draw_triangle(triangle_t triangle, uint32_t color, bool showVertex)
{
	draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, color);
	draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, color);
	draw_line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, color);
	if(showVertex)
	{
		draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, GREEN);
		draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, GREEN);
		draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, GREEN);
	}
}

void draw_triangle_filled(triangle_t triangle, uint32_t color)
{
	triangle = sortVertsByY(triangle);
	draw_triangle(triangle, color, true);

}
