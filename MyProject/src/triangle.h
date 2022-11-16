#pragma  once
#include <stdint.h>
#include "vector.h"

#define swap(x,y) do{ \
unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
	   memcpy(swap_temp, &y, sizeof(x)); \
	   memcpy(&y, &x, sizeof(x)); \
	   memcpy(&x, swap_temp, sizeof(x)); \
	} while (0);
// face is a collection of 3 integers
// each integer represents the memory location of vertex 
typedef struct {
    int a;
    int b;
    int c;
    uint32_t color;
} face_t;

// triangle is a collection of 3 points
// each point is a vec2 which is a collection of 2 floats
typedef struct {
    vec2_t points[3];
    uint32_t color;
    float avg_depth;
} triangle_t;

triangle_t sortVertsByY(triangle_t unsorted);

