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
#pragma endregion


// Function Prototypes
void free_resources(void);
// Number of triangles to be rendered = number of faces (since each triangle corresponds to one face)
triangle_t* triangles_to_render = NULL;


vec3_t camera_position = { .x = 0, .y = 0, .z = -10 };

float fov_factor = 1040;

bool is_running = false;
int previous_frame_time = 0;
// Rendering mode
enum renderMode { WireframePure, WireframeWithVertex, FilledTriangles, WireframeWithFilledTriangles};
bool backfaceCulling = false;
bool paintersAlgorithm = true;
int scaleSign = 1;
enum  renderMode mode;
void playground(void)
{
	mat4_t I = mat4_make_identity();
	mat4_t S = mat4_make_scale(2.0, 2.5, 3.0);

}
void setup(void) {
	
	// Function to randomly put some test code for debugging
	//playground();
	// Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
	load_cube_mesh_data();
	//load_obj_file_data("./assets/cube.obj");

	//rendering mode
	enum  renderMode mode = WireframePure;
	backfaceCulling = true;
	paintersAlgorithm = true;
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
		if (event.key.keysym.sym == SDLK_c)
			backfaceCulling = true;
		if (event.key.keysym.sym == SDLK_d)
			backfaceCulling = false;
		if (event.key.keysym.sym == SDLK_2)
			mode = WireframePure;
		if (event.key.keysym.sym == SDLK_1)
			mode = WireframeWithVertex;
		if (event.key.keysym.sym == SDLK_3)
			mode = FilledTriangles;
		if (event.key.keysym.sym == SDLK_4)
			mode = WireframeWithFilledTriangles;
		if (event.key.keysym.sym == SDLK_p)
			paintersAlgorithm = !paintersAlgorithm;
		if (event.key.keysym.sym == SDLK_l)
			scaleSign = -1;
		if (event.key.keysym.sym == SDLK_m)
			scaleSign = 1;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Function that receives a 3D vector and returns a projected 2D point
////////////////////////////////////////////////////////////////////////////////
vec2_t project(vec3_t point) {
	const vec2_t projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};
	return projected_point;
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

	mesh.rotation.x += 0.03f;
	mesh.rotation.y += 0.03f;
	mesh.rotation.z += 0.03f;
	
	mesh.scale.x += (scaleSign) * 0.02f;
	mesh.scale.y += (scaleSign) * 0.02f;
	mesh.scale.z += (scaleSign ) * 0.02f;

	mesh.translate.x += 0.02f;
	//mesh.translate.y += -0.01f;
	mesh.translate.z = (-1) * (camera_position.z);

	
	mat4_t scaleMatrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translateMatrix = mat4_make_translate(mesh.translate.x, mesh.translate.y, mesh.translate.z);
	mat4_t rotXMatrix = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotYMatrix = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotZMatrix = mat4_make_rotation_z(mesh.rotation.z);
	//mat4_t rotYMatrix = mat4_make_rotation_y(mesh.rotation.y);
	//mat4_t rotZMatrix = mat4_make_rotation_z(mesh.rotation.z);
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

		vec3_t transformed_vertices[3];

		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			// transform (Order => Scale -> Rotate -> Translate)
			mat4_t worldMatrix = mat4_make_identity();
			
			worldMatrix = mat4_multiply_mat4(rotXMatrix, worldMatrix);
			worldMatrix = mat4_multiply_mat4( rotYMatrix, worldMatrix);
			worldMatrix = mat4_multiply_mat4( rotZMatrix, worldMatrix);
			worldMatrix = mat4_multiply_mat4( translateMatrix, worldMatrix);
			worldMatrix = mat4_multiply_mat4(scaleMatrix, worldMatrix);
			transformed_vertex = vec3_from_vec4(mat4_multiply_vec4(worldMatrix, vec4_from_vec3(transformed_vertex)));
			
			
			// save this transformed vertex into an array
			transformed_vertices[j] = transformed_vertex;
		}
		// Back face culling
	
		if (backfaceCulling)
		{
			vec3_t BminusA = vec3_subtract(transformed_vertices[1], transformed_vertices[0]);
			vec3_t CminusA = vec3_subtract(transformed_vertices[2], transformed_vertices[0]);
			vec3_normalize(&CminusA);
			vec3_normalize(&BminusA);
			vec3_t normalToABC = vec3_crossProduct(BminusA, CminusA);
			vec3_normalize(&normalToABC);
			
			vec3_t cameraRay = vec3_subtract(camera_position, transformed_vertices[0]);
			float camRayDotFaceNormal = vec3_dotProduct(cameraRay, normalToABC);
			if (camRayDotFaceNormal < 0)
				continue;
		}
	
		// Loop all three vertices of this current face 
		// and apply perspective divide to the transformed vertices
		// 
		// an empty triangle
		triangle_t projected_triangle;
		for (int j = 0; j < 3; j++) {
			
			// Project the current vertex
			vec2_t projected_point = project(transformed_vertices[j]);

			// translate the projected points to the middle of the screen
			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[j] = projected_point;
			// At this point, we know WHERE on the screen A vertex of a triangle should be painted
		}
		projected_triangle.color = mesh_face.color;
		projected_triangle.avg_depth = (transformed_vertices[0].z + 
			transformed_vertices[1].z + 
			transformed_vertices[2].z) / 3;
		// in an array of projected triangles, where each triangle has 3 points,
		// save those 3 points (ready to be painted) all at once, 
		// Save the projected triangle in the array of triangles to render
		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
		
	}
	int remainingFaces = array_length(triangles_to_render);
	// Sort by Z buffer
	if (paintersAlgorithm)
	{
		quick_sort(triangles_to_render, 0, remainingFaces - 1, sorter_descending);
	}
		

}

void render(void) {
	draw_grid();
	int numOfTriangles = array_length(triangles_to_render);
	// Loop all projected triangles and render them
	for (int i = 0; i < numOfTriangles; i++) {
		triangle_t triangle = triangles_to_render[i];
		//draw_triangle_filled(triangle, RED, PURPLE, true);
		if (mode == WireframePure)
		{
			draw_triangle(triangle, RED, false);
		}
		else if (mode == WireframeWithVertex)
		{
			draw_triangle(triangle, RED, true);
		}
		else if (mode == WireframeWithFilledTriangles)
		{
			draw_triangle_filled(triangle, triangle.color, RED);
		}
		else if (mode == FilledTriangles)
		{
			draw_triangle_filled(triangle, PURPLE, PURPLE);
		}
		
	}

	//triangle_t triangle = { 
	//	.points[2].x = 300, .points[2].y = 100,
	//	.points[1].x = 650, .points[1].y = 400 ,
	//	.points[0].x = 500, .points[0].y = 700 
	//};
	//draw_triangle_filled(triangle, YELLOW, true);

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
	free(color_buffer);
}
