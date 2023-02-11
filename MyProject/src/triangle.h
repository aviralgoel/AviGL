#pragma  once
#include <stdint.h>
#include "vector.h"
#include "texture.h"
#include "upng.h"

#define swap(x,y) do{ \
unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
	   memcpy(swap_temp, &y, sizeof(x)); \
	   memcpy(&y, &x, sizeof(x)); \
	   memcpy(&x, swap_temp, sizeof(x)); \
	} while (0);
// face is a collection of 3 integers (a, b, & c)
// each integer represents the memory location of vertex position coordinates, vertex texture coordinates (u,v)
// and color of the face of triangle
typedef struct {
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	vec3_t a_vn;
	vec3_t b_vn;
	vec3_t c_vn;
	uint32_t color;
} face_t;

// triangle is a collection of 3 points (for position), color, avg depth for painter's algorithms
// and set of uv coordinates of the 3 points
typedef struct {
	vec4_t points[3];
	uint32_t color;
	float avg_depth;
	tex2_t texcoords[3];
	float lightIntensities[3];
	upng_t* texture;
} triangle_t;

triangle_t sortVertsByY(triangle_t unsorted);
