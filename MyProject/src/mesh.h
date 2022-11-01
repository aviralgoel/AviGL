#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERTICES 8
extern vec3_t mesh_vertices[N_MESH_VERTICES];

#define N_MESH_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern face_t mesh_faces[N_MESH_FACES];

#endif
