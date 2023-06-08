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
	for (int i = 0; i <= side_length; i++)
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

int min_int(int a, int b)
{
	return (a < b ? a : b);
}

int max_int(int a, int b)
{
	return (a > b ? a : b);
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


void draw_triangle_filled(triangle_t triangle, uint32_t fillColor, uint32_t borderColor)
{	
	// find bounding box
	int x0, x1, x2, y0, y1, y2;
	x0 = triangle.points[0].x; 	x1 = triangle.points[1].x; 	x2 = triangle.points[2].x;
	y0 = triangle.points[0].y; 	y1 = triangle.points[1].y; 	y2 = triangle.points[2].y;
	
	int minX = min_int(x0, min_int(x1, x2));
	int maxX = max_int(x0, max_int(x1, x2));

	int minY = min_int(y0, min_int(y1, y2));
	int maxY = max_int(y0, max_int(y1, y2));

	draw_line(minX, minY, maxX, minY, YELLOW); // bottom line
	draw_line(minX, minY, minX, maxY, YELLOW); // left line
	draw_line(maxX, minY, maxX, maxY, YELLOW); // right line
	draw_line(minX, maxY, maxX, maxY, YELLOW); // top line


	// spits out the triangle in order of y coordinates y0 <= y1 <= y2 
	triangle = sortVertsByY(triangle);
	// find the coordinates of line that divides the triangle into flat bottom and flat top
	//int x0, x1, x2, y0, y1, y2;
	x0 = triangle.points[0].x; 	y0 = triangle.points[0].y; // A is the top vertex
	x1 = triangle.points[1].x; 	y1 = triangle.points[1].y; // B is the middle vertex
	x2 = triangle.points[2].x; 	y2 = triangle.points[2].y; // C is the bottom vertex
	
	// For Edge AB
	int deltaX_AB = (x1 - x0);	int deltaY_AB = (y1 - y0);
	int AB_coefficient_A = deltaY_AB;	int AB_coefficient_B = -deltaX_AB;
	int AB_coefficient_C = (deltaX_AB * y0) - (deltaY_AB * x0);

	// for edge BC
	int deltaX_BC = (x2 - x1);	int deltaY_BC = (y2 - y1);
	int BC_coefficient_A = deltaY_BC;	int BC_coefficient_B = -deltaX_BC;
	int BC_coefficient_C = (deltaX_BC * y1) - (deltaY_BC * x1);

	// for edge CA
	int deltaX_CA = (x0 - x2);	int deltaY_CA = (y0 - y2);
	int CA_coefficient_A = deltaY_CA;	int CA_coefficient_B = -deltaX_CA;
	int CA_coefficient_C = (deltaX_CA * y2) - (deltaY_CA * x2);

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			if (
					(
					(((AB_coefficient_A * x) + (AB_coefficient_B * y) + (AB_coefficient_C)) < 0) &&
					(((BC_coefficient_A * x) + (BC_coefficient_B * y) + (BC_coefficient_C)) < 0) &&
					(((CA_coefficient_A * x) + (CA_coefficient_B * y) + (CA_coefficient_C)) < 0)
					) || 
					(
					(((AB_coefficient_A * x) + (AB_coefficient_B * y) + (AB_coefficient_C)) > 0) &&
					(((BC_coefficient_A * x) + (BC_coefficient_B * y) + (BC_coefficient_C)) > 0) &&
					(((CA_coefficient_A * x) + (CA_coefficient_B * y) + (CA_coefficient_C)) > 0)
					)
				)

			{
				draw_pixel(x, y, BLUE);
			}
		}
	}

	//if (y1 == y2)
	//{
	//	triangle_t flatBottom = {
	//	.points[0].x = x0, .points[0].y = y0,
	//	.points[1].x = x1, .points[1].y = y1,
	//	.points[2].x = x2, .points[2].y = y2 };
	//	 //fill_flat_bottom(flatBottom, fillColor);
	//}
	//else if (y0 == y2)
	//{
	//	triangle_t flatTop = {
	//	.points[0].x = x0, .points[0].y = y0,
	//	.points[1].x = x1, .points[1].y = y1,
	//	.points[2].x = x2, .points[2].y = y2 };
	////fill_flat_top(flatTop, fillColor);
	//}
	//else
	//{
	//	int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
	//	int My = y1;
	//	draw_triangle(triangle, borderColor, true);
	//	triangle_t flatBottom = {
	//	.points[0].x = x0, .points[0].y = y0,
	//	.points[1].x = x1, .points[1].y = y1,
	//	.points[2].x = Mx, .points[2].y = My };
	//	triangle_t flatTop = {
	//	.points[0].x = x1, .points[0].y = y1,
	//	.points[1].x = x2, .points[1].y = y2,
	//	.points[2].x = Mx, .points[2].y = My };

	////	fill_flat_bottom(flatBottom, fillColor);
	//	//fill_flat_top(flatTop, fillColor);
	//}
}


void fill_flat_bottom(triangle_t triangle, uint32_t color)
{
	
	int x0, x1, x2, y0, y1, y2;
	x0 = triangle.points[0].x; 	x1 = triangle.points[1].x; 	x2 = triangle.points[2].x;
	y0 = triangle.points[0].y; 	y1 = triangle.points[1].y; 	y2 = triangle.points[2].y;

	/* Note to self:
	Definition of slope: rise/run => (another iterpretation) how much Y changes if change in x = 1
	Definition of (slope)^-1 => run/rise => (another interpretation) how much X changes if change in y = 1 */
	float invSlope1 = (float)(x0 - x1) / (float)(y0 - y1) ;
	float invSlope2 = (float)(x0 - x2) / (float)(y0 - y2) ;
	float x_start = x0;
	float x_end = x0;
	for (int i = y0; i <= y1; i++)
	{	
		draw_line(x_start, i, x_end, i, color);
		x_start += invSlope1;
		x_end +=  invSlope2;
	}


}

void fill_flat_top(triangle_t triangle, uint32_t color)
{
	int x0, x1, x2, y0, y1, y2;
	x0 = triangle.points[0].x; 	x1 = triangle.points[1].x; 	x2 = triangle.points[2].x;
	y0 = triangle.points[0].y; 	y1 = triangle.points[1].y; 	y2 = triangle.points[2].y;
	float invSlope1 = (float)(x0 - x1) / (float)(y0 - y1);
	float invSlope2 = (float)(x2 - x1) / (float)(y2 - y1);
	
	float x_start = x1;
	float x_end = x1;
	for (int i = y1; i >= y0; i--)
	{
		draw_line(x_start, i, x_end, i, color);
		x_start -= invSlope2;
		x_end -= invSlope1;
	}
}
