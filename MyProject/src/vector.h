#pragma once

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4_t;

// 2D Vector Functions
vec2_t vec2_new(float x, float y);
float vec2_magnitude(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_from_vec4(vec2_t a);
vec2_t vec2_subtract(vec2_t a, vec2_t b);
vec2_t vec2_multiply(vec2_t a, float s);
vec2_t vec2_divide(vec2_t a, float s);
void vec2_normalize(vec2_t* v);
float vec2_dotProduct(vec2_t a, vec2_t b);

// 3D Vector Functions
vec3_t vec3_new(float x, float y, float z);
vec3_t vec3_clone(vec3_t* ptrToVector);
float vec3_magnitude(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_subtract(vec3_t a, vec3_t b);
vec3_t vec3_multiply(vec3_t a, float s);
vec3_t vec3_divide(vec3_t a, float s);
vec3_t vec3_crossProduct(vec3_t a, vec3_t b);
void vec3_normalize(vec3_t* v);
float vec3_dotProduct(vec3_t a, vec3_t b);
vec3_t vec3_from_vec4(vec4_t vec4);
vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

// 4D Vector Functions
vec4_t vec4_from_vec3(vec3_t vec3);
