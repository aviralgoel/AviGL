#include "clipping.h"
#include <math.h>
#define NUM_PLANES 6

plane_t frutum_planes[NUM_PLANES];
void init_frustum_planes(float fovx, float fovy, float z_near, float z_far)
{
	float cos_fovBy2_X = cos(fovx / 2);
	float sin_fovBy2_X = sin(fovx / 2);

	float cos_fovBy2_Y = cos(fovy / 2);
	float sin_fovBy2_Y = sin(fovy / 2);

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
	frutum_planes[RIGHT_FRUSTUM_PLANE].normal.x = (-1.0f) * (cos_fovBy2_X);
	frutum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frutum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_fovBy2_X;

	// Left Plane : P = (0, 0, 0), N = (cos(fov/2), 0, sin(fov/2))
	frutum_planes[LEFT_FRUSTUM_PLANE].point = origin;
	frutum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_fovBy2_X;
	frutum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frutum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_fovBy2_X;

	// Top Plane : P = (0, 0, 0), N = (0, -cos(fov/2), sin(fov/2))
	frutum_planes[TOP_FRUSTUM_PLANE].point = origin;
	frutum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frutum_planes[TOP_FRUSTUM_PLANE].normal.y = (-1.0f) * (cos_fovBy2_Y);
	frutum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_fovBy2_Y;

	// Bottom Plane : P = (0, 0, 0), N = (0, cos(fov/2), sin(fov/2))
	frutum_planes[BOTTOM_FRUSTUM_PLANE].point = origin;
	frutum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frutum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_fovBy2_Y;
	frutum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_fovBy2_Y;
}

polygon_t create_polygon_from_triangle(vec4_t triangle_to_clipped[3], tex2_t initialTex[3])
{
	// Three vertices (homegenous) of an incoming triangle, which is to be clipped
	vec3_t vertex_0 = vec3_from_vec4(triangle_to_clipped[0]);
	vec3_t vertex_1 = vec3_from_vec4(triangle_to_clipped[1]);
	vec3_t vertex_2 = vec3_from_vec4(triangle_to_clipped[2]);

	polygon_t polygon = {
		.num_vertices = 3,
		.vertices = {vertex_0,vertex_1,vertex_2},
		.texCoods = {initialTex[0], initialTex[1], initialTex[2]}
	};

	return polygon;
}

void create_triangles_from_polygon(polygon_t* poly, triangle_t triangles_after_clipping[], int* numOfTrianglesAfterClipping)
{
	for (int i = 0; i < poly->num_vertices - 2; i++)
	{
		int index0 = 0;
		int index1 = i + 1;
		int index2 = i + 2;

		triangles_after_clipping[i].points[0] = vec4_from_vec3(poly->vertices[index0]);
		triangles_after_clipping[i].points[1] = vec4_from_vec3(poly->vertices[index1]);
		triangles_after_clipping[i].points[2] = vec4_from_vec3(poly->vertices[index2]);
		triangles_after_clipping[i].texcoords[0] = poly->texCoods[index0];
		triangles_after_clipping[i].texcoords[1] = poly->texCoods[index1];
		triangles_after_clipping[i].texcoords[2] = poly->texCoods[index2];
	}
	*numOfTrianglesAfterClipping = poly->num_vertices - 2;
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
	vec3_t plane_point = frutum_planes[plane].point; // P
	vec3_t plane_normal = frutum_planes[plane].normal; // N

	// array of inside vertices
	vec3_t inside_vertices[MAX_NUM_POLY_VERTICES];
	tex2_t inside_texcoords[MAX_NUM_POLY_VERTICES];
	int num_of_inside_vertices = 0;

	// num of vertices in our polygon to be clipped
	int length_of_polygon_vertices = (polygon_to_be_clipped->num_vertices);
	// pointer to a current (initially, first vertex in the polygon) vertex and
	// pointer to a previous (initially, last vertex in the polygon) vertex
	vec3_t* current_vertex = &polygon_to_be_clipped->vertices[0];
	tex2_t* current_texcoord = &polygon_to_be_clipped->texCoods[0];

	vec3_t* previous_vertex = &polygon_to_be_clipped->vertices[length_of_polygon_vertices - 1];
	tex2_t* previous_texcoord = &polygon_to_be_clipped->texCoods[length_of_polygon_vertices - 1];

	vec3_t* last_vertex = &polygon_to_be_clipped->vertices[length_of_polygon_vertices];

	// dotQ1 = n * (Q1-P) ; // dotQ2 = n * (Q2-P)
	// projection of N on Q1-P (if > 0 ==> inside)
	float dotProduct_CV = 0;
	float dotProduct_PV = vec3_dotProduct(vec3_subtract(*previous_vertex, plane_point), plane_normal);

	// loop through all the vertices to check what is inside/outside
	while (current_vertex != last_vertex)
	{
		// dotQ1 = n * (Q1-P) ; // dotQ2 = n * (Q2-P)
		// projection of N on Q1-P (if > 0 ==> inside)
		float dotProduct_CV = vec3_dotProduct(vec3_subtract(*current_vertex, plane_point), plane_normal);
		float dotProduct_PV = vec3_dotProduct(vec3_subtract(*previous_vertex, plane_point), plane_normal);

		// if either one of the vertex is inside-outside || outside-inside
		if (dotProduct_CV * dotProduct_PV < 0)
		{
			// find interpolation factor t (t = Dot1/(DotQ1-DotQ2))
			// find the intersection point I = Q1 + t(Q2-Q1)
			float t = dotProduct_PV / (dotProduct_PV - dotProduct_CV);
			vec3_t intersection_point = vec3_add(*previous_vertex,
				vec3_multiply(vec3_subtract(*current_vertex, *previous_vertex), t));
			tex2_t interpolated_texure;
			interpolated_texure.u = previous_texcoord->u + ((current_texcoord->u - previous_texcoord->u) * t);
			interpolated_texure.v = previous_texcoord->v + ((current_texcoord->v - previous_texcoord->v) * t);
			inside_texcoords[num_of_inside_vertices] = tex2_clone(&interpolated_texure);
			inside_vertices[num_of_inside_vertices] = vec3_clone(&intersection_point);

			num_of_inside_vertices++;
		}
		if (dotProduct_CV > 0)
		{
			inside_vertices[num_of_inside_vertices] = vec3_clone(current_vertex);
			inside_texcoords[num_of_inside_vertices] = tex2_clone(current_texcoord);
			num_of_inside_vertices++;
		}

		dotProduct_PV = dotProduct_CV;
		previous_vertex = current_vertex;
		previous_texcoord = current_texcoord;
		current_vertex++;
		current_texcoord++;
	}
	// copy list of inside vertices into the incoming polygon itself and then return it
	for (int i = 0; i < num_of_inside_vertices; i++)
	{
		polygon_to_be_clipped->vertices[i] = vec3_clone(&inside_vertices[i]);
		polygon_to_be_clipped->texCoods[i] = tex2_clone(&inside_texcoords[i]);
	}
	polygon_to_be_clipped->num_vertices = num_of_inside_vertices;
}