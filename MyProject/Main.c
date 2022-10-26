#pragma region libraries
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <stdint.h>
#pragma endregion


#pragma region global variables
int window_width;
int window_height;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool is_running = false;
// pointer to first element of the color buffer array
uint32_t* colorbuffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
#pragma endregion

// initialize an empty window
bool initialize_window(void)
{	
	// SDL is composed of eight subsystems 
	// - Audio, CDROM, Event Handling, File I/O, Joystick Handling, Threading, Timers and Video. 
	// Before you can use any of these subsystems they must be initialized by calling SDL_Init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL\n");
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
		fprintf(stderr, "Error initializing SDL Window\n");
		return false;
	}
	//SDL_CreateRenderer creates renderer for a given window, meaning it is supposed to draw to that window
	// - with some checks, 
	// like opengl or vulkan requires window to be created with specific flags. 
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error initializing SDL Renderer\n");
		return false;
	}
	SDL_SetWindowFullscreen(window, SDL_SetWindowFullscreen);
	return true;
}
#pragma region game loop methods
void setup(void)
{	// allocate the required memory in bytes to hold the color buffer and SDL texture
	colorbuffer = (uint32_t*)malloc(window_width * window_height * sizeof(uint32_t));
	if (colorbuffer == NULL)
	{	
		printf("Error allocating colorbuffer memory!");
	}
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

}
// check for user input
void process_input(void)
{	
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT: // X button is pressed
		is_running = false; break;
	case SDL_KEYDOWN:
		// Escape key is presssed
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			is_running = false;
		}
	default:
		break;
	}

}
void update(void)
{

}
void clear_color_buffer(uint32_t color)
{	
	for (int i = 0 ;i < window_height; i++)
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
	for (int i = y0; i < y0+height; i++)
	{
		for (int j = x0; j < x0+width; j++)
		{
			colorbuffer[(window_width * i) + j] = fillColor;
		}
	}
}
void render_color_buffer(void)
{
	SDL_UpdateTexture(color_buffer_texture,
		NULL,
		colorbuffer,
		(int)window_width * sizeof(uint32_t));

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
void render(void)
{	
	/* Select the color for drawing. It is set to purple here. */
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	/* Clear the entire screen to our selected color. */
	//SDL_RenderClear(renderer);
	draw_grid(20, 20, 0xFFFFFFFF);
	draw_rect(200, 250, 5000, 4500, 0xFFFF0000);
	render_color_buffer();
	
	clear_color_buffer(0xFF88FF99);
	/* like the final draw call	/* Up until now everything was drawn behind the scenes.
	This will show the new, red contents of the window.*/
	SDL_RenderPresent(renderer);
}
#pragma endregion

void destroy_window(void)
{
	free(colorbuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* args[])
{	
	is_running = initialize_window();
	setup();
	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}