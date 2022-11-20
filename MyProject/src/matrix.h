#pragma once
#include "vector.h";
typedef struct 
{
	float m[4][4];
	
} mat4_t;

mat4_t mat4_make_indentity(void);
mat4_t mat4_make_scale(float sx, float sy, float sz);

vec3_t mat4_multiply_vec3(mat4_t mat, vec3_t v);
vec4_t mat4_multiply_vec4 (mat4_t mat, vec4_t v);
