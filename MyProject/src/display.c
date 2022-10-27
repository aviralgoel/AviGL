#pragma region libraries
#include <display.h>
#pragma endregion

#pragma region global variables
// screen resolution 
int window_width = 800;
int window_height = 600;
// window and renderer pointers
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
// pointer to first element of the color buffer array
uint32_t* colorbuffer = NULL;
// pointer to first element of the color buffer texture array
SDL_Texture* color_buffer_texture = NULL;
#pragma endregion

#pragma region methods
void clear_color_buffer(uint32_t color)
{
	for (int i = 0; i < window_height; i++)
	{
		for (int j = 0; j < window_width; j++)
		{
			colorbuffer[(window_width * i) + j] = color;
		}
	}
}
void draw_grid(int columnGap, int rowGap, uint32_t gridColor)
{
	for (int i = 0; i < window_height; i++)
	{
		for (int j = 0; j < window_width; j++)
		{
			if (j % columnGap == 0 || i % rowGap == 0)
				colorbuffer[(window_width * i) + j] = gridColor;
		}
	}

}
void draw_rect(int x0, int y0, int width, int height, uint32_t fillColor)
{	// clamp max width
	if (x0 + width > window_width) width = window_width - x0;
	// clamp max height
	if (y0 + height > window_height) height = window_height - y0;
	// paint the rect
	for (int i = y0; i < y0 + height; i++)
	{
		for (int j = x0; j < x0 + width; j++)
		{
			colorbuffer[(window_width * i) + j] = fillColor;
		}
	}
}
// converts our colorbuffer to SDL's required texture buffer
void render_color_buffer(void)
{
	SDL_UpdateTexture(color_buffer_texture,
		NULL,
		colorbuffer,
		(int)window_width * sizeof(uint32_t));

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
// cleaning up after our renderer
void destroy_window(void)
{
	free(colorbuffer);
	// Destroy the rendering context for a window and free associated textures.
	SDL_DestroyRenderer(renderer);
	// Destroy a window.
	SDL_DestroyWindow(window);
	// Clean up all initialized subsystems. Opposite of SQL_Init()
	SDL_Quit();
}
// initialize an empty window
bool initialize_window(void)
{
	// SDL is composed of eight subsystems 
	// - Audio, CDROM, Event Handling, File I/O, Joystick Handling, Threading, Timers and Video. 
	// Before you can use any of these subsystems they must be initialized by calling SDL_Init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error initializing SDL\n");
		return false;
	}
	// determine the max xres and max yres
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	// 
	// window creation (WIDTH & HEIGHT)
	window = SDL_CreateWindow(
		"My 3D Renderer",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);
	if (!window)
	{
		printf("Error initializing SDL Window\n");
		return false;
	}
	//SDL_CreateRenderer creates renderer for a given window, meaning it is supposed to draw to that window
	// - with some checks, 
	// like opengl or vulkan requires window to be created with specific flags. 
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		printf("Error initializing SDL Renderer\n");
		return false;
	}
	SDL_SetWindowFullscreen(window, SDL_SetWindowFullscreen);
	return true;
}
void draw_pixel(int x, int y, uint32_t color)
{	
	if(x > 0 && x < window_width && y > 0 && y < window_height)
		colorbuffer[(window_width * y) + x] = color;
}

#pragma endregion
