#include "mesh.h"
#include "array.h"
#include <stdio.h>

#define MAX_NUM_OF_MESHES 10

mesh_t mesh = { .vertices = NULL,
				.texcoords = NULL,
				.vertex_normals = NULL,
				.faces = NULL,
				.rotation = {0,0,0},
				.scale = {1.0, 1.0, 1.0},
				.translate = {0, 0, 0},
				.texture = NULL
};

static mesh_t meshes[MAX_NUM_OF_MESHES];
static int total_num_of_meshes = 0;

void load_mesh(const char* obj_file_path, const char* png_file_path, vec3_t _scale, vec3_t _translate, vec3_t _rotate)
{
	mesh_t _mesh;
	_mesh.vertices = NULL;
	_mesh.texcoords = NULL;
	_mesh.vertex_normals = NULL;
	_mesh.faces = NULL;
	// load mesh data from obj file
	load_obj_file_data(obj_file_path, &_mesh);
	// load mesh texture data from png file
	load_png_file_texture(png_file_path, &_mesh);
	// scale, rotate and translate mesh
	_mesh.scale = _scale;
	_mesh.translate = _translate;
	_mesh.rotation = _rotate;

	// add to mesh array
	meshes[total_num_of_meshes] = _mesh;
	total_num_of_meshes++;
}

void load_obj_file_data(char* filename, mesh_t* _mesh) {
	FILE* stream;
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
			array_push(_mesh->vertices, vertex);
		}
		// texture info
		if (strncmp(line, "vt ", 3) == 0) {
			tex2_t _texurecoord;
			sscanf_s(line, "vt %f %f", &_texurecoord.u, &_texurecoord.v);
			array_push(_mesh->texcoords, _texurecoord);
		}
		// vertex normal info
		if (strncmp(line, "vn ", 3) == 0) {
			vec3_t _vn;
			sscanf_s(line, "vn %f %f %f", &_vn.x, &_vn.y, &_vn.z);
			array_push(_mesh->vertex_normals, _vn);
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
				.a_uv = _mesh->texcoords[texture_indices[0] - 1],
				.b_uv = _mesh->texcoords[texture_indices[1] - 1],
				.c_uv = _mesh->texcoords[texture_indices[2] - 1],
				.a_vn = _mesh->vertex_normals[normal_indices[0] - 1],
				.b_vn = _mesh->vertex_normals[normal_indices[0] - 1],
				.c_vn = _mesh->vertex_normals[normal_indices[0] - 1]
			};
			array_push(_mesh->faces, face);
		}
	}
	fclose(stream);
}

void load_png_file_texture(char* filename, mesh_t* _mesh)
{
	upng_t* png_image = upng_new_from_file(filename);
	if (png_image != NULL) {
		upng_decode(png_image);
		if (upng_get_error(png_image) == UPNG_EOK) {
			_mesh->texture = png_image;
		}
	}
}

void free_meshes(void)
{
	for (int i = 0; i < total_num_of_meshes; i++)
	{
		array_free(meshes[i].faces);
		array_free(meshes[i].vertices);
		array_free(meshes[i].texcoords);
		array_free(meshes[i].vertex_normals);
		upng_free(meshes[i].texture);
	}
}

int get_total_num_of_meshes()
{
	return total_num_of_meshes;
}

mesh_t* get_mesh(int index)
{
	return &meshes[index];
}