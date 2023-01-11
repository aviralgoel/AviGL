#pragma  once

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern face_t cube_faces[N_CUBE_FACES];

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
	// NOTE: a face is a set of 3 array indices, where each element tells the index of vertex that forms that face
} mesh_t;
extern mesh_t mesh;

void load_cube_mesh_data(void);
// load obj models from storage
void load_obj_file_data(char* filename);