#pragma once
#include "vector.h"

#define MAX_NUM_POLY_VERTICES 10
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
	int num_vertices;
} polygon_t;
void init_frustum_planes(float fov, float z_near, float z_far);
polygon_t create_polygon_from_triangle(vec4_t triangle_to_clipped[3]);
void clip_polygon(polygon_t* polygon_to_be_clipped);
void clip_polygon_against_plane(polygon_t* polygon_to_be_clipped, int plane);