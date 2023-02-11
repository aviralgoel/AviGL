#pragma  once

#include "vector.h"
#include "triangle.h"
#include "upng.h"
/// <summary>
/// Define a struct for dynamic size meshes with array of vertices and faces
/// </summary>
typedef struct {
	vec3_t* vertices; // dynamic array of vec3_t where each element correspond to each vertex of the mesh
	tex2_t* texcoords; // dynamic array of (yet to be mapped) uv (vt from obj) values of the mesh
	vec3_t* vertex_normals; // dynamic array of vec3_t where each element correspond to a vertex normal vector
	face_t* faces; // dynamic array of face_t where each element correspond to each face of the mesh
	vec3_t rotation; // how much the mesh should rotate every frame (w.r.t to original rotation of mesh)
	vec3_t scale; // how much should the mesh be scaled every frame (w.r.t to default scale)
	vec3_t translate; // how much should the mesh be translated every frame (w.r.t to origin)
	upng_t* texture; // PNG parsed texture of the mesh object
	// NOTE: a face is a set of 3 array indices, where each element tells the index of vertex that forms that face
} mesh_t;

// load mesh and png data
void load_mesh(const char* obj_file_path, const char* png_file_path, vec3_t _scale, vec3_t _translate, vec3_t _rotate);

// load obj models from storage
void load_obj_file_data(char* filename, mesh_t* _mesh);
void load_png_file_texture(char* filename, mesh_t* _mesh);
void free_meshes(void);

// getter and setters
int get_total_num_of_meshes();
mesh_t* get_mesh(int index);
