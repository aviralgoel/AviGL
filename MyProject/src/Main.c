#pragma region header files
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <math.h>
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
#include "camera.h"
#include "clipping.h"
#pragma endregion

#define MAX_TRIANGLES_PER_MESH 10000

// Function Prototypes
void render(void);
void playground(void);
void free_resources(void);

// Global Variables
// Number of triangles to be rendered = number of faces (since each triangle corresponds to one face)
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

mat4_t camera_view_matrix; // World to Camera Space Matrix
mat4_t proj_matrix; // Camera Space to NDC Matrix

// lighting in the scene

bool is_running = false;
bool paintersAlgorithm = true;
int previous_frame_time = 0;
float delta_time = 0;

void setup(void) {

	// load mesh data from an obj file and load texture data from PNG file
	load_mesh("./assets/drone.obj", "./assets/drone.png", vec3_new(1, 1, 1), vec3_new(3, 0, 10), vec3_new(0, 0, 0));
	load_mesh("./assets/efa.obj", "./assets/efa.png", vec3_new(1, 1, 1), vec3_new(-3, 0, 8), vec3_new(0, 0, 0));
	load_mesh("./assets/f117.obj", "./assets/f117.png", vec3_new(1, 1, 1), vec3_new(3, -2, 8), vec3_new(0, 0, 0));


	// defaut setting of the renderer
	set_render_method(RENDER_WIREFRAME);
	set_cull_method(CULL_BACKFACE);

	float fovy = degreeToRadian(60);
	float aspectx = (float)get_window_width() / (float)get_window_height();
	float aspecty = (float)get_window_height() / (float)get_window_width();
	float fovx = 2 * atan(tan(fovy / 2) * aspectx);
	float znear = 1;
	float zfar = 100.0;

	// Make a perspective matrix
	proj_matrix = mat4_make_perspective(fovy, aspecty, znear, zfar);

	// Manually load the texture data from static uint8 array and cast it into uint32
	//mesh_texture = (uint32_t*)REDBRICK_TEXTURE;

	//Initialize Frustum Planes with a point and a normal
	init_frustum_planes(fovx, fovy, znear, zfar);

	// intialize lighting
	init_light(vec3_new( 1,1,-1 ));
}

// Check for user input
void process_input(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
		case SDL_QUIT:
			is_running = false; break;
		case SDL_KEYDOWN:
			// quit
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				is_running = false;
				break;
			}
			// rendering modes
			if (event.key.keysym.sym == SDLK_1)
			{
				set_render_method(RENDER_WIREFRAME);
				break;
			}
			if (event.key.keysym.sym == SDLK_2)
			{
				set_render_method(RENDER_WIRE_FILLED);
				break;
			}
			if (event.key.keysym.sym == SDLK_3)
			{
				set_render_method(RENDER_FILLED_FLAT);
				break;
			}
			if (event.key.keysym.sym == SDLK_4)
			{
				set_render_method(RENDER_FILLED_GOURAUD);
				break;
			}
			// enable texturing
			if (event.key.keysym.sym == SDLK_5)
			{
				set_render_method(RENDER_TEXTURED_WIRE);
				break;
			}
			// enable texture and wiring
			if (event.key.keysym.sym == SDLK_6)
			{
				set_render_method(RENDER_TEXTURED);
				break;
			}
			// back face culling
			if (event.key.keysym.sym == SDLK_b)
			{
				set_cull_method(CULL_BACKFACE);
				break;
			}
			if (event.key.keysym.sym == SDLK_n)
			{
				set_cull_method(CULL_NONE);
				break;
			}
			// camera input
			if (event.key.keysym.sym == SDLK_d)
			{
				set_camera_yaw(-5.0f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				set_camera_yaw(5.0f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				set_camera_yaw(-0.00001f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				set_camera_yaw(0.00010f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_w)
			{
				set_camera_position(12 * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				set_camera_position(-12.0f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				set_camera_positionY(12.0f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				set_camera_positionY(-12.0f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_q)
			{
				set_camera_pitch(-2.0f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_e)
			{
				set_camera_pitch(2.0f * delta_time);
				break;
			}
			break;
		}
	}
}

void update(void) {
	num_triangles_to_render = 0;

	// delta_time -> time elapsed between current frame and last frame (in seconds)
	// if this value is too small -> high fps -> transformations will be fine grained
	// if this value is too big -> low fps -> transformations will be corase grained
	delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;
	previous_frame_time = SDL_GetTicks();

	// rotation per frame
	//mesh.rotation.x += 0.2f * delta_time;
	//mesh.rotation.y += 0.2f * delta_time;
	//mesh.rotation.z += 0.2f * delta_time;

	// translation per frame
	//mesh.translate.x += 0.2f * delta_time;
	//mesh.translate.y += -0.01f;
	//mesh.translate.z = 5.0f;

	// scale per frame
	//mesh.scale.x = 2;
	//mesh.scale.y = 2;
	//mesh.scale.z = 2;

	// change camera position per frame
	//camera.position.x += 0.25f * delta_time;
	//camera.position.y += 0.3f * delta_time;

	// camera up direction
	vec3_t up_direction = { 0,1,0 };
	
	vec3_t target = get_camera_lookAt_target();
	// create a camera lookAt/ world-to-camera view matrix
	camera_view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

	
	for (int current_mesh = 0; current_mesh < get_total_num_of_meshes(); current_mesh++)
	{	
		mesh_t* mesh = get_mesh(current_mesh);
		// creating scale, translate, and rotation matrices
		mat4_t scaleMatrix = mat4_make_scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
		mat4_t translateMatrix = mat4_make_translate(mesh->translate.x, mesh->translate.y, mesh->translate.z);
		mat4_t rotXMatrix = mat4_make_rotation_x(mesh->rotation.x);
		mat4_t rotYMatrix = mat4_make_rotation_y(mesh->rotation.y);
		mat4_t rotZMatrix = mat4_make_rotation_z(mesh->rotation.z);
		// Loop all triangle faces of our mesh
		int numOfFaces = array_length(mesh->faces);
		for (int i = 0; i < numOfFaces; i++) {
			// get the face
			face_t mesh_face = mesh->faces[i];
			// for each face, fetch its three corresponding vertices (vec3)
			vec3_t face_vertices[3];
			face_vertices[0] = mesh->vertices[mesh_face.a - 1];
			face_vertices[1] = mesh->vertices[mesh_face.b - 1];
			face_vertices[2] = mesh->vertices[mesh_face.c - 1];

			// for each face, fetch its three corresponding vertices's normals (vec3)
			vec3_t face_vertex_normals[3];
			face_vertex_normals[0] = mesh_face.a_vn;// a
			face_vertex_normals[1] = mesh_face.b_vn;// b
			face_vertex_normals[2] = mesh_face.c_vn;// c

			// store vertices coordinates and vertex normals after being transformed into world space
			vec4_t transformed_vertices[3];
			vec3_t transformed_vertices_normals[3];

			// Loop all three vertices of this current face and apply transformations
			for (int j = 0; j < 3; j++) {
				vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
				vec4_t transformed_normal = vec4_from_vec3(face_vertex_normals[j]);

				// transform (Scale -> Rotate -> Translate)
				mat4_t worldMatrix = mat4_make_identity(); // converts model to world

				worldMatrix = mat4_multiply_mat4(rotXMatrix, worldMatrix);
				worldMatrix = mat4_multiply_mat4(rotYMatrix, worldMatrix);
				worldMatrix = mat4_multiply_mat4(rotZMatrix, worldMatrix);

				// normals are only rotated
				transformed_normal = mat4_multiply_vec4(worldMatrix, transformed_normal);

				worldMatrix = mat4_multiply_mat4(translateMatrix, worldMatrix);
				worldMatrix = mat4_multiply_mat4(scaleMatrix, worldMatrix);

				// vertex coordinates are now transformed but still in world space
				transformed_vertex = mat4_multiply_vec4(worldMatrix, transformed_vertex);

				// vertex coordinates are now transformed but now in camera space
				transformed_vertex = mat4_multiply_vec4(camera_view_matrix, transformed_vertex);

				// save this transformed vertex into an array
				transformed_vertices[j] = transformed_vertex;
				transformed_vertices_normals[j] = vec3_from_vec4(transformed_normal);
			}

			// for backface culling, calculate normal to the triangle face
			vec3_t BminusA = vec3_subtract(vec3_from_vec4(transformed_vertices[1]), vec3_from_vec4(transformed_vertices[0]));
			vec3_t CminusA = vec3_subtract(vec3_from_vec4(transformed_vertices[2]), vec3_from_vec4(transformed_vertices[0]));
			vec3_normalize(&CminusA);
			vec3_normalize(&BminusA);
			vec3_t normalToABC = vec3_crossProduct(BminusA, CminusA);
			// normalize this normal
			vec3_normalize(&normalToABC);

			// Back face culling on/off
			if (is_cull_backface)
			{
				// Back face culling
				vec3_t origin = { 0,0,0 }; // our camera is at the origin of camera space
				vec3_t cameraRay = vec3_subtract(origin, vec3_from_vec4(transformed_vertices[0]));
				float camRayDotFaceNormal = vec3_dotProduct(cameraRay, normalToABC);
				if (camRayDotFaceNormal < 0)
					continue;
			}

			// Flat Shading, based on the dot product between light and the face normal
			float lightEffect = normalizeInRange(vec3_dotProduct(normalToABC, get_light_direction()), 1, -1);
			mesh_face.color = GREEN; // RED
			mesh_face.color = light_apply_intensity(mesh_face.color, lightEffect);

			// Gouraud Shading, calculate light effect for each vertex
			float lightIntensities[3];
			lightIntensities[0] = normalizeInRange(vec3_dotProduct(transformed_vertices_normals[0], get_light_direction()), 1, -1);
			lightIntensities[1] = normalizeInRange(vec3_dotProduct(transformed_vertices_normals[1], get_light_direction()), 1, -1);
			lightIntensities[2] = normalizeInRange(vec3_dotProduct(transformed_vertices_normals[2], get_light_direction()), 1, -1);

			// Camera Frustum Clipping
			// Create a polygon from original triangle to be clipped
			// this polygon will also have triangle's vertices texture uv
			tex2_t intialTexCoords[3];
			intialTexCoords[0] = mesh_face.a_uv;
			intialTexCoords[1] = mesh_face.b_uv;
			intialTexCoords[2] = mesh_face.c_uv;
			polygon_t polygon = create_polygon_from_triangle(transformed_vertices, intialTexCoords);

			// clip that polygon against all six planes
			clip_polygon(&polygon);
			// array to store the triangles that comes out of the clipped polygon
			triangle_t triangles_after_clipping[MAX_NUM_TRIANGLES_POLY];
			int num_triangles_after_clipping = 0;
			// populate triangles_after_clipping
			create_triangles_from_polygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);

			// for every triangle perform projection and perspective divide
			for (int t = 0; t < num_triangles_after_clipping; t++)
			{
				triangle_t triangle_post_clipping = triangles_after_clipping[t];
				// Loop all three vertices to perform projection
				// and apply perspective divide to the transformed vertices
				vec4_t projected_points[3];
				for (int j = 0; j < 3; j++) {
					// Loop all three vertices to perform projection
					// Project the current vertex
					// and now we are in image space
					// perspective divide happens here
					projected_points[j] = mat4_mul_vec4_project(proj_matrix, triangle_post_clipping.points[j]);
					// Y value of the model ( lower Y -> bottom of the model, higher Y -> top of the model BUT our raster grid is the opposite
					// lower value is top of the screen and higher value is bottom, therefore, we need to invert/change sign of y-value 0f model
					projected_points[j].y *= -1;

					// Scale into the view
					projected_points[j].x *= (get_window_width() / 2.0);
					projected_points[j].y *= (get_window_height() / 2.0);

					// Translate the projected points to the middle of the screen
					projected_points[j].x += (get_window_width() / 2.0);
					projected_points[j].y += (get_window_height() / 2.0);
				}

				triangle_t projected_triangle = {
				.points = {
					{ projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
					{ projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
					{ projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w },
				},
				.texcoords = {
					{triangles_after_clipping[t].texcoords[0].u, triangles_after_clipping[t].texcoords[0].v},
					{triangles_after_clipping[t].texcoords[1].u, triangles_after_clipping[t].texcoords[1].v},
					{triangles_after_clipping[t].texcoords[2].u, triangles_after_clipping[t].texcoords[2].v},
				},
				.color = GOLD,
				//.avg_depth = avg_depth,
				.lightIntensities = {
					lightIntensities[0],
					lightIntensities[1],
					lightIntensities[2]
				},
				.texture = mesh->texture
				};

				// Save the projected triangle in the array of triangles to render
				//array_push(triangles_to_render, projected_triangle);
				if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH)
				{
					triangles_to_render[num_triangles_to_render++] = projected_triangle;
				}
				// Calculate the average depth for each face based on the vertices after transformation
				//float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;
			}
		}
		//int remainingFaces = array_length(triangles_to_render);
		// Painters Algorithsm
		//quicksort_triangles(triangles_to_render, 0, remainingFaces - 1);
	}


	
}

void render(void) {
	clear_color_buffer(0xFF000000);
	clear_z_buffer();
	draw_grid();

	//int numOfTriangles = array_length(triangles_to_render);
	// Loop all projected triangles and render them
	for (int i = 0; i < num_triangles_to_render; i++) {
		triangle_t triangle = triangles_to_render[i];
		//draw_triangle_filled(triangle, RED, PURPLE, true);
		if (get_render_method() == RENDER_WIREFRAME)
		{
			draw_triangle(triangle, RED, false);
		}
		else if (get_render_method() == RENDER_WIRE_FILLED)
		{
			draw_triangle(triangle, RED, true);
		}
		else if (get_render_method() == RENDER_FILLED_FLAT)
		{
			draw_triangle_shaded(triangle, triangle.color, BLUE, 0);
		}
		else if (get_render_method() == RENDER_FILLED_GOURAUD)
		{
			draw_triangle_shaded(triangle, triangle.color, RED, 1);
		}
		else if (get_render_method() == RENDER_TEXTURED)
		{
			draw_triangle_textured(triangle, false);
		}
		else if (get_render_method() == RENDER_TEXTURED_WIRE)
		{
			draw_triangle_textured(triangle, true);
		}
	}
	//array_free(triangles_to_render);
	render_color_buffer();

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
	free_meshes();
}

// method for me to experiment random stuff
void playground(void)
{
}