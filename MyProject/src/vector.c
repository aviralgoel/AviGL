#include <math.h>
#include "vector.h"

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };
    return rotated_vector;
}

#pragma region Vector TWOD Functions
float vec2_magnitude(vec2_t v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}
#pragma endregion Vector TWOD Functions

#pragma region Vector THREED Functions
float vec3_magnitude(vec3_t v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z*v.z);
}
#pragma endregion Vector THREED Functions
