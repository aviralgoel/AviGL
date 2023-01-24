#include "display.h"
#include "light.h"
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
float* z_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

// initialized the window wizard for the renderer
bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Set width and height of the SDL window with the max screen resolution
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = window_width;
	window_height = window_height;

	// Create a SDL Window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_SHOWN
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
void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t))
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			color_buffer[(window_width * y) + x] = color;
		}
	}
}
void clear_z_buffer()
{
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			z_buffer[(window_width * y) + x] = 1.0f;
		}
	}
}
void destroy_window(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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
// paints a pixel on the screen with color
void draw_pixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
		color_buffer[(window_width * y) + x] = color;
	}
}
// maps a pixel coordinate to the texture image using u,v of a given triangle and the pixel inside it.
void draw_texel(int pixelX, int pixelY, triangle_t t, uint32_t* texture)
{
	// the three vertices of the triangle and the Point P where texture needs to be determined.
	vec2_t point_a = { .x = t.points[0].x, .y = t.points[0].y };
	vec2_t point_b = { .x = t.points[1].x, .y = t.points[1].y };
	vec2_t point_c = { .x = t.points[2].x, .y = t.points[2].y };
	vec2_t point_p = { .x = pixelX, .y = pixelY };
	// texel coordinates of vertices
	float u0, u1, u2, v0, v1, v2;
	u0 = t.texcoords[0].u; u1 = t.texcoords[1].u; u2 = t.texcoords[2].u;
	v0 = t.texcoords[0].v; v1 = t.texcoords[1].v; v2 = t.texcoords[2].v;

	// flip the uv coordinates (if necessary, based on obj file format) (default is v grows upwards, but code below makes it flip)
	v0 = 1 - v0;	v1 = 1 - v1;	v2 = 1 - v2;

	float z0, z2, z1, w0, w2, w1;

	z0 = t.points[0].z; 	z1 = t.points[1].z; 	z2 = t.points[2].z;
	w0 = t.points[0].w; 	w1 = t.points[1].w; 	w2 = t.points[2].w;

	// barycentric coordinates of point P
	vec3_t barycentric_coordinates = barycentric_weights(point_a, point_b, point_c, point_p);
	float alpha = barycentric_coordinates.x;
	float beta = barycentric_coordinates.y;
	float gamma = barycentric_coordinates.z;

	// what will be the u,v value of point P, given that we have u,v value of the vertices of the triangle
	float A = w1 * w2 * alpha; float B = w0 * w2 * beta; float C = w1 * w0 * gamma;
	float interpolated_u = (u0 * A + u1 * B + u2 * C) / (A + B + C);
	float interpolated_v = (v0 * A + v1 * B + v2 * C) / (A + B + C);

	// calculating interpolated w (cuz z is not linear) for z buffer values
	float interpolated_reciprocal_w = (A + B + C) / (w1 * w2 * w0);
	interpolated_reciprocal_w = 1 - interpolated_reciprocal_w;
	int zIndex = ((window_width * pixelY) + pixelX) % (window_width * window_height);
	if (interpolated_reciprocal_w < z_buffer[zIndex])
	{
		int tex_x = abs((int)(interpolated_u * (texture_width - 1))) % texture_width;
		int tex_y = abs((int)(interpolated_v * (texture_height - 1))) % texture_height;
		// now we know exact texture color location in the texture image for Point P, let's fetch it
		// based on the u,v value of P, fetch which texture color needs to be here from the texture
		// u,v values are between [0,1] and texture is 64x64, hence scale it. [No Perspective Correction]
		// u/w,v/w values are between [0,1] and texture is 64x64, hence scale it. [Perspective Correction]
		int textureIndex = (texture_width * tex_y) + tex_x;
		// and draw pixel
		draw_pixel(pixelX, pixelY, texture[textureIndex]);
		z_buffer[zIndex] = interpolated_reciprocal_w;
	}
}
void draw_pixel_shaded(int pixelX, int pixelY, triangle_t t, int shadeMode)
{
	if (shadeMode == 0) // flat shading
	{
		draw_pixel(pixelX, pixelY, t.color); return;
	}
	if (shadeMode == 1) // gouraud shading
	{
		// the three vertices of the triangle and the Point P where texture needs to be determined.
		vec2_t point_a = { .x = t.points[0].x, .y = t.points[0].y };
		vec2_t point_b = { .x = t.points[1].x, .y = t.points[1].y };
		vec2_t point_c = { .x = t.points[2].x, .y = t.points[2].y };
		vec2_t point_p = { .x = pixelX, .y = pixelY };

		// light intensity
		float li0, li1, li2;
		li0 = t.lightIntensities[0]; li1 = t.lightIntensities[1]; li2 = t.lightIntensities[1];

		vec3_t barycentric_coordinates = barycentric_weights(point_a, point_b, point_c, point_p);
		float alpha = barycentric_coordinates.x;
		float beta = barycentric_coordinates.y;
		float gamma = barycentric_coordinates.z;

		float w0, w2, w1;

		w0 = t.points[0].w; 	w1 = t.points[1].w; 	w2 = t.points[2].w;

		float A = w1 * w2 * alpha;
		float B = w0 * w2 * beta;
		float C = w1 * w0 * gamma;
		float interpolated_intensity = (li0 * A + li1 * B + li2 * C) / (A + B + C);
		uint32_t interpolated_color = light_apply_intensity(t.color, interpolated_intensity);
		float interpolated_reciprocal_w = (A + B + C) / (w1 * w2 * w0);
		interpolated_reciprocal_w = 1 - interpolated_reciprocal_w;
		int zIndex = ((window_width * pixelY) + pixelX) % (window_width * window_height);
		if (interpolated_reciprocal_w < z_buffer[zIndex])
		{
			draw_pixel(pixelX, pixelY, interpolated_color);
			z_buffer[zIndex] = interpolated_reciprocal_w;
		}
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
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
	vec2_t ac = vec2_subtract(c, a);
	vec2_t ab = vec2_subtract(b, a);
	vec2_t pc = vec2_subtract(c, p);
	vec2_t pb = vec2_subtract(b, p);
	vec2_t ap = vec2_subtract(p, a);

	// area using cross product || AC x AB ||
	float area_of_parallelogram = (ac.x * ab.y) - (ac.y * ab.x);
	// || PC x PB || / areaOfParallelogram
	float alpha = ((pc.x * pb.y) - (pc.y * pb.x)) / area_of_parallelogram;
	// || ACxAP || / areaOfParallelogram
	float beta = ((ac.x * ap.y) - (ac.y * ap.x)) / area_of_parallelogram;

	float gamma = 1 - alpha - beta;

	vec3_t weights = { .x = alpha, .y = beta, .z = gamma };

	return weights;
}
/// <summary>
/// draws a wire frame triangle
/// </summary>
/// <param name="triangle">struct for triangle coordinates</param>
/// <param name="color">wire frame color of triangle</param>
/// <param name="showVertices">true if vertices should be highlighted in GREEN</param>
void draw_triangle(triangle_t triangle, uint32_t color, bool showVertex)
{
	draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, color);
	draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, color);
	draw_line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, color);
	if (showVertex)
	{
		draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, GREEN);
		draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, GREEN);
		draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, GREEN);
	}
}
void draw_triangle_filled(triangle_t triangle, uint32_t fillColor, uint32_t borderColor)
{
	triangle = sortVertsByY(triangle);
	// find the coordinates of line that divides the triangle into flat bottom and flat top
	int x0, x1, x2, y0, y1, y2;
	x0 = triangle.points[0].x; 	x1 = triangle.points[1].x; 	x2 = triangle.points[2].x;
	y0 = triangle.points[0].y; 	y1 = triangle.points[1].y; 	y2 = triangle.points[2].y;
	int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
	int My = y1;
	draw_triangle(triangle, borderColor, false);
	triangle_t flatBottom = {
	.points[0].x = x0, .points[0].y = y0,
	.points[1].x = x1, .points[1].y = y1,
	.points[2].x = Mx, .points[2].y = My };
	triangle_t flatTop = {
	.points[0].x = x1, .points[0].y = y1,
	.points[1].x = x2, .points[1].y = y2,
	.points[2].x = Mx, .points[2].y = My };
	if (y1 == y2) // Pure flat bottom
	{
		fill_flat_bottom(flatBottom, fillColor);
	}
	else if (y0 == y1) // Pure flat top
	{
		fill_flat_top(flatTop, fillColor);
	}
	else // Regular Triangle
	{
		fill_flat_bottom(flatBottom, fillColor);
		fill_flat_top(flatTop, fillColor);
	}
}
void fill_flat_bottom(triangle_t triangle, uint32_t color)
{
	int x0, x1, x2, y0, y1, y2;
	x0 = triangle.points[0].x; 	x1 = triangle.points[1].x; 	x2 = triangle.points[2].x;
	y0 = triangle.points[0].y; 	y1 = triangle.points[1].y; 	y2 = triangle.points[2].y;

	/* Note to self:
	Definition of slope: rise/run => (another iterpretation) how much Y changes if change in x = 1
	Definition of (slope)^-1 => run/rise => (another interpretation) how much X changes if change in y = 1 */
	float invSlope1 = (float)(x0 - x1) / (float)(y0 - y1);
	float invSlope2 = (float)(x0 - x2) / (float)(y0 - y2);
	float x_start = x0;
	float x_end = x0;
	for (int i = y0; i <= y1; i++)
	{
		draw_line(x_start, i, x_end, i, color);
		x_start += invSlope1;
		x_end += invSlope2;
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
void draw_triangle_textured(triangle_t triangle, uint32_t* texture, bool wireframe)
{
	triangle = sortVertsByY(triangle);
	// coordinates of vertices in pixel/screen space
	int x0, x1, x2, y0, y1, y2;
	float z0, z2, z1, w0, w2, w1;
	x0 = triangle.points[0].x; 	x1 = triangle.points[1].x; 	x2 = triangle.points[2].x;
	y0 = triangle.points[0].y; 	y1 = triangle.points[1].y; 	y2 = triangle.points[2].y;

	///////////////////////////////////////////////////////
	// Render the upper part of the triangle (flat-bottom)
	///////////////////////////////////////////////////////
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;
	if (wireframe)
		draw_triangle(triangle, WHITE, false);
	if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				swap(x_start, x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x < x_end; x++) {
				// Draw our pixel with a custom color
				if (x >= 0 && x < window_width && y >= 0 && y < window_height)
					draw_texel(x, y, triangle, texture);
			}
		}
	}

	///////////////////////////////////////////////////////
	// Render the bottom part of the triangle (flat-top)
	///////////////////////////////////////////////////////
	inv_slope_1 = 0;
	inv_slope_2 = 0;

	if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1 != 0) {
		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				swap(x_start, x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x < x_end; x++) {
				/// Draw our pixel with a custom color
				if (x >= 0 && x < window_width && y >= 0 && y < window_height)
					draw_texel(x, y, triangle, texture);
			}
		}
	}
}
void draw_triangle_shaded(triangle_t triangle, uint32_t fillColor, uint32_t borderColor, int shadeMode)
{
	triangle = sortVertsByY(triangle);
	// coordinates of vertices in pixel/screen space
	int x0, x1, x2, y0, y1, y2;
	float z0, z2, z1, w0, w2, w1;
	x0 = triangle.points[0].x; 	x1 = triangle.points[1].x; 	x2 = triangle.points[2].x;
	y0 = triangle.points[0].y; 	y1 = triangle.points[1].y; 	y2 = triangle.points[2].y;

	// draw border
	//draw_triangle(triangle, borderColor, false);

	///////////////////////////////////////////////////////
	// Render the upper part of the triangle (flat-bottom)
	///////////////////////////////////////////////////////
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;

	if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				swap(x_start, x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x < x_end; x++) {
				// Draw our pixel with a custom color
				if (x >= 0 && x < window_width && y >= 0 && y < window_height)
					draw_pixel_shaded(x, y, triangle, shadeMode);
			}
		}
	}

	///////////////////////////////////////////////////////
	// Render the bottom part of the triangle (flat-top)
	///////////////////////////////////////////////////////
	inv_slope_1 = 0;
	inv_slope_2 = 0;

	if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1 != 0) {
		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				swap(x_start, x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x < x_end; x++) {
				/// Draw our pixel with a custom color
				if (x >= 0 && x < window_width && y >= 0 && y < window_height)
					draw_pixel_shaded(x, y, triangle, shadeMode);
			}
		}
	}
}