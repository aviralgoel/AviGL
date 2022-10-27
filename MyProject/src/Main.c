#pragma region libraries
#include <display.h>
#pragma endregion

#pragma region global variables
bool is_running = false; // is the renderer running?
#pragma endregion

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
void render(void)
{	
	// just a method to draw grid
	//draw_grid(20, 20, 0xFFFFFFFF);
	// just a method to draw a rect
	//draw_rect(200, 250, 500, 450, 0xFFFF0000);
	// just a method to draw a single pixel
	//draw_pixel(100, 30, 0xFFFFFF00);

	// converting colorbuffer to SDL's texture needs
	render_color_buffer();
	// clean the colorbuffer for next frame
	clear_color_buffer(0xFF223344);
	// like the final draw call	/* Up until now everything was drawn behind the scenes.
	// This will show the new, red contents of the window
	SDL_RenderPresent(renderer);
}
#pragma endregion

int main(int argc, char* args[])
{	
	is_running = initialize_window();
	setup();

	// game loop
	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}