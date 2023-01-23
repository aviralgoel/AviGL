#pragma once
#include "vector.h"
#include "triangle.h"
#include "texture.h"

#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_TRIANGLES_POLY 10
enum {
	LEFT_FRUSTUM_PLANE,
	RIGHT_FRUSTUM_PLANE,
	TOP_FRUSTUM_PLANE,
	BOTTOM_FRUSTUM_PLANE,
	NEAR_FRUSTUM_PLANE,
	FAR_FRUSTUM_PLANE
};

typedef struct {
	vec3_t point;
	vec3_t normal;
} plane_t;

typedef struct {
	vec3_t vertices[MAX_NUM_POLY_VERTICES];
	tex2_t texCoods[MAX_NUM_POLY_VERTICES];
	int num_vertices;
} polygon_t;
void init_frustum_planes(float fovx, float fovy, float z_near, float z_far);
polygon_t create_polygon_from_triangle(vec4_t triangle_to_clipped[3], tex2_t correspondingTex[3]);
void create_triangles_from_polygon(polygon_t* poly, triangle_t triangles_after_clipping[], int* numOfTrianglesAfterClipping);
void clip_polygon(polygon_t* polygon_to_be_clipped);
void clip_polygon_against_plane(polygon_t* polygon_to_be_clipped, int plane);