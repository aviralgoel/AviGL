#include "clipping.h"
#include <math.h>
#define NUM_PLANES 6

plane_t frutum_planes[NUM_PLANES];
void init_frustum_planes(float fov, float z_near, float z_far)
{
	float cos_fovBy2 = cos(fov / 2);
	float sin_fovBy2 = sin(fov / 2);

	vec3_t origin = { 0,0,0 };
	// Near Plane : P = (0, 0, znear), N = (0, 0, 1)
	frutum_planes[NEAR_FRUSTUM_PLANE].point.x = 0;
	frutum_planes[NEAR_FRUSTUM_PLANE].point.y = 0;
	frutum_planes[NEAR_FRUSTUM_PLANE].point.z = z_near;
	frutum_planes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frutum_planes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frutum_planes[NEAR_FRUSTUM_PLANE].normal.z = 1;

	// Far Plane : P = (0, 0, zfar), N = (0, 0, -1)
	frutum_planes[FAR_FRUSTUM_PLANE].point.x = 0;
	frutum_planes[FAR_FRUSTUM_PLANE].point.y = 0;
	frutum_planes[FAR_FRUSTUM_PLANE].point.z = z_far;
	frutum_planes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frutum_planes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frutum_planes[FAR_FRUSTUM_PLANE].normal.z = -1;

	// Right Plane : P = (0, 0, 0), N = (-cos(fov/2), 0, sin(fov/2))
	frutum_planes[RIGHT_FRUSTUM_PLANE].point = origin;
	frutum_planes[RIGHT_FRUSTUM_PLANE].normal.x = (-1.0f) * (cos_fovBy2);
	frutum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frutum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_fovBy2;

	// Left Plane : P = (0, 0, 0), N = (cos(fov/2), 0, sin(fov/2))
	frutum_planes[LEFT_FRUSTUM_PLANE].point = origin;
	frutum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_fovBy2;
	frutum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frutum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_fovBy2;

	// Top Plane : P = (0, 0, 0), N = (0, -cos(fov/2), sin(fov/2))
	frutum_planes[TOP_FRUSTUM_PLANE].point = origin;
	frutum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frutum_planes[TOP_FRUSTUM_PLANE].normal.y = (-1.0f) * (cos_fovBy2);
	frutum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_fovBy2;

	// Bottom Plane : P = (0, 0, 0), N = (0, cos(fov/2), sin(fov/2))
	frutum_planes[BOTTOM_FRUSTUM_PLANE].point = origin;
	frutum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frutum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_fovBy2;
	frutum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_fovBy2;
}

polygon_t create_polygon_from_triangle(vec4_t triangle_to_clipped[3])
{	
	// Three vertices (homegenous) of an incoming triangle, which is to be clipped
	vec3_t vertex_0 = vec3_from_vec4(triangle_to_clipped[0]);
	vec3_t vertex_1 = vec3_from_vec4(triangle_to_clipped[1]);
	vec3_t vertex_2 = vec3_from_vec4(triangle_to_clipped[2]);

	polygon_t polygon = {
		.num_vertices = 3,
		.vertices = {vertex_0,vertex_1,vertex_2}
	};

	return polygon;
}

void clip_polygon(polygon_t* polygon_to_be_clipped)
{
	clip_polygon_against_plane(polygon_to_be_clipped, LEFT_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon_to_be_clipped, RIGHT_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon_to_be_clipped, TOP_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon_to_be_clipped, BOTTOM_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon_to_be_clipped, NEAR_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon_to_be_clipped, FAR_FRUSTUM_PLANE);
}

void clip_polygon_against_plane(polygon_t* polygon_to_be_clipped, int plane)
{
}
