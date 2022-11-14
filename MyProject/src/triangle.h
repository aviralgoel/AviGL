#pragma  once
#include <stdint.h>
#include "vector.h"
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
} triangle_t;

triangle_t sortVertsByY(triangle_t unsorted);

