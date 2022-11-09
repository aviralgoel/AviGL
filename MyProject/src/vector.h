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
#pragma region Vector TWO D Functions
/// <summary>
/// returns the magnitude of a 2D vector
/// </summary>
/// <param name="v">vector</param>
/// <returns></returns>
float vec2_magnitude(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_subtract(vec2_t a, vec2_t b);
vec2_t vec2_multiply(vec2_t a, float s);
vec2_t vec2_divide(vec2_t a, float s);
void vec2_normalize(vec2_t* v);
float vec2_dotProduct(vec2_t a, vec2_t b);

#pragma endregion Vector TWO D Functions
#pragma region Vector THREE D Functions
/// <summary>
/// returns the magnitude of a 3D vector
/// </summary>
/// <param name="v">vector</param>
/// <returns></returns>
float vec3_magnitude(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_subtract(vec3_t a, vec3_t b);
vec3_t vec3_multiply(vec3_t a, float s);
vec3_t vec3_divide(vec3_t a, float s);
vec3_t vec3_crossProduct(vec3_t a, vec3_t b);
void vec3_normalize(vec3_t* v);
float vec3_dotProduct(vec3_t a, vec3_t b);

#pragma endregion Vector THREE D Functions

#endif
