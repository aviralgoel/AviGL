#pragma region header files
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "util.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "upng.h"
#pragma endregion

void render(void);
void playground(void);
// Function Prototypes
void free_resources(void);
// Number of triangles to be rendered = number of faces (since each triangle corresponds to one face)
triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };
light_t dir_light = { .direction = {-1,-1,-1} };
mat4_t proj_matrix;

bool is_running = false;
int previous_frame_time = 0;
// Rendering mode
enum renderMode {
	RENDER_WIREFRAME,
	RENDER_FILLED,
	RENDER_WIRE_FILLED,
	RENDER_TEXTURED,
	RENDER_TEXTURED_WIRE
};
bool backfaceCulling = true;
bool paintersAlgorithm = true;
int scaleSign = 1;
enum  renderMode mode;

void setup(void) {
	// Function to randomly put some test code for debugging
	//playground();
	// Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
	//load_cube_mesh_data();
	load_obj_file_data("./assets/efa.obj");
	load_png_texture_data("./assets/efa.png");

	//rendering mode
	enum  renderMode mode = RENDER_TEXTURED;
	float fov = degreeToRadian(60);
	float aspect = (float)window_width / (float)window_height;
	float znear = 10;
	float zfar = 100.0;
	proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

	// Manually load the texture data from static uint8 array and cast it into uint32
	//mesh_texture = (uint32_t*)REDBRICK_TEXTURE;
	
	
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		if (event.key.keysym.sym == SDLK_1)
			mode = RENDER_WIREFRAME;
		if (event.key.keysym.sym == SDLK_2)
			mode = RENDER_WIRE_FILLED;
		if (event.key.keysym.sym == SDLK_3)
			mode = RENDER_FILLED;
		if (event.key.keysym.sym == SDLK_4)
			mode = RENDER_WIRE_FILLED;
		// enable texturing
		if (event.key.keysym.sym == SDLK_5)
			mode = RENDER_TEXTURED;
		// enable texture and wiring
		if (event.key.keysym.sym == SDLK_6)
			mode = RENDER_TEXTURED_WIRE;
		if (event.key.keysym.sym == SDLK_p)
			paintersAlgorithm = !paintersAlgorithm;

		break;
	}
}

void update(void) {
	triangles_to_render = NULL;
	// Returns an unsigned 32-bit value representing the number of milliseconds since the SDL library initialized.
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	// Only delay execution if we are running too fast
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}
	previous_frame_time = SDL_GetTicks();

	// rotation per frame
	mesh.rotation.x += 0.0f;
	mesh.rotation.y += 0.01f;
	mesh.rotation.z += 0.0f;

	// translation per frame
	//mesh.translate.x += 0.02f;
	//mesh.translate.y += -0.01f;
	mesh.translate.z = (-1) * (camera_position.z);

	// scale per frame
	mesh.scale.x = 2;
	mesh.scale.y = 2;
	mesh.scale.z = 2;

	mat4_t scaleMatrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translateMatrix = mat4_make_translate(mesh.translate.x, mesh.translate.y, mesh.translate.z);
	mat4_t rotXMatrix = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotYMatrix = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotZMatrix = mat4_make_rotation_z(mesh.rotation.z);

	// Loop all triangle faces of our mesh
	// for a cube there are 6 sides = 6*2 triangular faces
	int numOfFaces = array_length(mesh.faces);
	for (int i = 0; i < numOfFaces; i++) {
		// get the face
		face_t mesh_face = mesh.faces[i];
		// for each face, fetch its three corresponding vertices (vec3)
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		vec4_t transformed_vertices[3];

		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++) {
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

			// transform (Order => Scale -> Rotate -> Translate)
			mat4_t worldMatrix = mat4_make_identity();

			worldMatrix = mat4_multiply_mat4(rotXMatrix, worldMatrix);
			worldMatrix = mat4_multiply_mat4(rotYMatrix, worldMatrix);
			worldMatrix = mat4_multiply_mat4(rotZMatrix, worldMatrix);
			worldMatrix = mat4_multiply_mat4(translateMatrix, worldMatrix);
			worldMatrix = mat4_multiply_mat4(scaleMatrix, worldMatrix);
			// the ultimate world matrix after accounting for R T S
			worldMatrix = worldMatrix;
			// vertex is now transformed but still in world space
			transformed_vertex = mat4_multiply_vec4(worldMatrix, transformed_vertex);

			// save this transformed vertex into an array
			transformed_vertices[j] = transformed_vertex;
		}

		vec3_t BminusA = vec3_subtract(vec3_from_vec4(transformed_vertices[1]), vec3_from_vec4(transformed_vertices[0]));
		vec3_t CminusA = vec3_subtract(vec3_from_vec4(transformed_vertices[2]), vec3_from_vec4(transformed_vertices[0]));
		vec3_normalize(&CminusA);
		vec3_normalize(&BminusA);
		vec3_t normalToABC = vec3_crossProduct(BminusA, CminusA);
		vec3_normalize(&normalToABC);

		// Back face culling
		if (backfaceCulling)
		{
			// Back face culling
			vec3_t cameraRay = vec3_subtract(camera_position, vec3_from_vec4(transformed_vertices[0]));
			float camRayDotFaceNormal = vec3_dotProduct(cameraRay, normalToABC);
			if (camRayDotFaceNormal < 0)
				continue;
		}

		// Flat Shading
		float lightEffect = normalizeInRange(vec3_dotProduct(normalToABC, dir_light.direction), 1, -1);
		mesh_face.color = 0x000000FF; // RED
		mesh_face.color = light_apply_intensity(mesh_face.color, lightEffect);

		// Loop all three vertices of this current face
		// and apply perspective divide to the transformed vertices
		vec4_t projected_points[3];

		// Loop all three vertices to perform projection
		for (int j = 0; j < 3; j++) {
			// Project the current vertex
			// and now we are in image space
			projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);
			// Y value of the model ( lower Y -> bottom of the model, higher Y -> top of the model BUT our raster grid is the opposite
			// lower value is top of the screen and higher value is bottom, therefore, we need to invert/change sign of y-value 0f model
			projected_points[j].y *= -1;

			// Scale into the view
			projected_points[j].x *= (window_width / 2.0);
			projected_points[j].y *= (window_height / 2.0);

			// Translate the projected points to the middle of the screen
			projected_points[j].x += (window_width / 2.0);
			projected_points[j].y += (window_height / 2.0);
		}

		// Calculate the average depth for each face based on the vertices after transformation
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

		triangle_t projected_triangle = {
			.points = {
				{ projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
				{ projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
				{ projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w },
			},
			.texcoords = {
				{mesh_face.a_uv.u, mesh_face.a_uv.v},
				{mesh_face.b_uv.u, mesh_face.b_uv.v},
				{mesh_face.c_uv.u, mesh_face.c_uv.v},
		},
			.color = mesh_face.color,
			.avg_depth = avg_depth
		};

		// Save the projected triangle in the array of triangles to render
		array_push(triangles_to_render, projected_triangle);
	}
	int remainingFaces = array_length(triangles_to_render);
	// Painters Algorithsm
	quicksort_triangles(triangles_to_render, 0, remainingFaces - 1);
}

void render(void) {
	draw_grid();
	int numOfTriangles = array_length(triangles_to_render);
	// Loop all projected triangles and render them
	for (int i = 0; i < numOfTriangles; i++) {
		triangle_t triangle = triangles_to_render[i];
		//draw_triangle_filled(triangle, RED, PURPLE, true);
		if (mode == RENDER_WIREFRAME)
		{
			draw_triangle(triangle, RED, false);
		}
		else if (mode == RENDER_WIRE_FILLED)
		{
			draw_triangle_filled(triangle, triangle.color, 0x000000FF);
		}
		else if (mode == RENDER_FILLED)
		{
			draw_triangle_filled(triangle, PURPLE, PURPLE);
		}
		else if (mode == RENDER_TEXTURED)
		{
			draw_triangle_textured(triangle, mesh_texture, false);
		}
		else if (mode == RENDER_TEXTURED_WIRE)
		{
			draw_triangle_textured(triangle, mesh_texture, true);
		}
	}
	array_free(triangles_to_render);
	render_color_buffer();
	clear_color_buffer(0xFF000000);
	SDL_RenderPresent(renderer);
}
int main(void) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();
	return 0;
}
void free_resources(void)
{
	array_free(mesh.faces);
	array_free(mesh.vertices);
	array_free(mesh.texcoords);
	free(color_buffer);
	free_png_texture(png_texture);
}

// method for me to experiment random stuff
void playground(void)
{
	float x0, x1, x2, y0, y1, y2;
	x0 = 6; 	x1 = 3; 	x2 = 9;
	y0 = 1; 	y1 = 4; 	y2 = 7;

	printf("Flat Bottom Triangle\n");
	//cout << "Flat Bottom Triangle\n";
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;
	float My = y1;
	float Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;
	printf("Mx: %f\t My: %f\n", Mx, My);
	if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(Mx - x0) / abs(My - y0);

	printf("inv_slope1: %f\t inv_slope2: %f\t\n", inv_slope_1, inv_slope_2);

	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			printf("current y: %d\n", y);
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;
			printf("x_start: %d and x_end: %d\n", x_start, x_end);
		}
	}
}