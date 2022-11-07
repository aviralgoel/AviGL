#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);
#pragma region Vector TWOD Functions
/// <summary>
/// returns the magnitude of a 2D vector
/// </summary>
/// <param name="v">vector</param>
/// <returns></returns>
float vec2_magnitude(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_subtract(vec2_t a, vec2_t b);
#pragma endregion Vector TWOD Functions
#pragma region Vector THREED Functions
/// <summary>
/// returns the magnitude of a 3D vector
/// </summary>
/// <param name="v">vector</param>
/// <returns></returns>
float vec3_magnitude(vec3_t v);
#pragma endregion Vector THREED Functions

#endif
