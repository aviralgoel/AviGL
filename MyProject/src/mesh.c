#include "mesh.h"
#include "array.h"
#include <stdio.h>

mesh_t mesh = { .vertices = NULL,
				.texcoords = NULL,
				.vertex_normals = NULL,
				.faces = NULL,
				.rotation = {0,0,0},
				.scale = {1.0, 1.0, 1.0},
				.translate = {0, 0, 0}
};
// coordinates of vertex in the world space
// every vertex is currently a vec3
vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{.x = -1, .y = -1, .z = -1 }, // 1
	{.x = -1, .y = 1, .z = -1 }, // 2
	{.x = 1, .y = 1, .z = -1 }, // 3
	{.x = 1, .y = -1, .z = -1 }, // 4
	{.x = 1, .y = 1, .z = 1 }, // 5
	{.x = 1, .y = -1, .z = 1 }, // 6
	{.x = -1, .y = 1, .z = 1 }, // 7
	{.x = -1, .y = -1, .z = 1 }  // 8
};

// index array of triangular mesh faces
// every face is a 3 float struct
face_t cube_faces[N_CUBE_FACES] = {
	// front
	{.a = 1, .b = 2, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 1, .b = 3, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// right
	{.a = 4, .b = 3, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 4, .b = 5, .c = 6, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// back
	{.a = 6, .b = 5, .c = 7, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 6, .b = 7, .c = 8, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// left
	{.a = 8, .b = 7, .c = 2, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 8, .b = 2, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// top
	{.a = 2, .b = 7, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 2, .b = 5, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// bottom
	{.a = 6, .b = 8, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 6, .b = 1, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF }
};

void load_cube_mesh_data(void)
{
	for (int i = 0; i < N_CUBE_VERTICES; i++)
	{
		vec3_t cube_vertex = cube_vertices[i];
		array_push(mesh.vertices, cube_vertex);
	}
	for (int i = 0; i < N_CUBE_FACES; i++)
	{
		face_t cube_face = cube_faces[i];
		array_push(mesh.faces, cube_face);
	}
}

FILE* stream;
void load_obj_file_data(char* filename) {
	errno_t err;

	// Open for read (will fail if file "crt_fopen_s.c" doesn't exist)
	err = fopen_s(&stream, filename, "r");
	if (err == 0)
	{
		printf("The file 'crt_fopen_s.c' was opened\n");
	}
	else
	{
		printf("The file 'crt_fopen_s.c' was not opened\n");
	}
	char line[1024];
	//int i = 0;
	while (fgets(line, 1024, stream)) {
		// Vertex information
		if (strncmp(line, "v ", 2) == 0) {
			vec3_t vertex;
			sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			array_push(mesh.vertices, vertex);
		}
		// texture info
		if (strncmp(line, "vt ", 3) == 0) {
			tex2_t _texurecoord;
			sscanf_s(line, "vt %f %f", &_texurecoord.u, &_texurecoord.v);
			array_push(mesh.texcoords, _texurecoord);
		}
		// vertex normal info
		if (strncmp(line, "vn ", 3) == 0) {
			vec3_t _vn;
			sscanf_s(line, "vn %f %f %f", &_vn.x, &_vn.y, &_vn.z);
			array_push(mesh.vertex_normals, _vn);
		}
		// Face information
		if (strncmp(line, "f ", 2) == 0) {
			int vertex_indices[3];
			int texture_indices[3];
			int normal_indices[3];
			sscanf_s(
				line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&vertex_indices[0], &texture_indices[0], &normal_indices[0],
				&vertex_indices[1], &texture_indices[1], &normal_indices[1],
				&vertex_indices[2], &texture_indices[2], &normal_indices[2]
			);
			face_t face = {
				.a = vertex_indices[0],
				.b = vertex_indices[1],
				.c = vertex_indices[2],
				.a_uv = mesh.texcoords[texture_indices[0] - 1],
				.b_uv = mesh.texcoords[texture_indices[1] - 1],
				.c_uv = mesh.texcoords[texture_indices[2] - 1],
				.a_vn = mesh.vertex_normals[normal_indices[0] - 1],
				.b_vn = mesh.vertex_normals[normal_indices[0] - 1],
				.c_vn = mesh.vertex_normals[normal_indices[0] - 1]
			};
			array_push(mesh.faces, face);
		}
	}
}