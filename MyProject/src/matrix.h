#pragma once
#include "vector.h";
typedef struct 
{
	float m[4][4];
	
} mat4_t;

mat4_t mat4_make_identity(void);
mat4_t mat4_make_rotation_z(const float theta);
mat4_t mat4_make_rotation_y(const float theta);
mat4_t mat4_make_rotation_x(const float theta);
mat4_t mat4_make_scale(float sx, float sy, float sz);
mat4_t mat4_make_translate(float tx, float ty, float tz);
mat4_t mat4_multiply_mat4(mat4_t matA, mat4_t matB);
vec3_t mat4_multiply_vec3(mat4_t mat, vec3_t v);
vec4_t mat4_multiply_vec4 (mat4_t mat, vec4_t v);

