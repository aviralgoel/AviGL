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
vec2_t vec2_new(float x, float y)
{
	vec2_t result = { x, y };
	return result;
}
float vec2_magnitude(vec2_t v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b)
{
	vec2_t result = { .x = a.x + b.x, .y = a.y + b.y };
	return result;
}

vec2_t vec2_from_vec4(vec2_t a)
{
	vec2_t result = { .x = a.x, .y = a.y };
	return result;
}

vec2_t vec2_subtract(vec2_t a, vec2_t b)
{
	vec2_t result = { .x = a.x - b.x, .y = a.y - b.y };
	return result;
}
vec2_t vec2_multiply(vec2_t a, float s)
{
	vec2_t result = {
		.x = a.x * s,
		.y = a.y * s
	};
	return result;
}

vec2_t vec2_divide(vec2_t a, float s)
{
	vec2_t result = {
		.x = a.x / s,
		.y = a.y / s
	};
	return result;
}

void vec2_normalize(vec2_t* v)
{
	float magnitude = vec2_magnitude(*v);
	v->x = v->x / magnitude;
	v->y = v->y / magnitude;
}

float vec2_dotProduct(vec2_t a, vec2_t b)
{
	float result = (a.x * b.x) + (a.y * b.y);
	return result;
}

#pragma endregion
#pragma region Vector THREED Functions
vec3_t vec3_new(float x, float y, float z)
{
	vec3_t result = { x, y,z };
	return result;
}
vec3_t vec3_clone(vec3_t* ptrToVector)
{	
	vec3_t result = { ptrToVector->x, ptrToVector->y, ptrToVector->z };
	return result;
}
float vec3_magnitude(vec3_t v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b)
{
	vec3_t result = { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
	return result;
}

vec3_t vec3_subtract(vec3_t a, vec3_t b)
{
	vec3_t result = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};
	return result;
}

vec3_t vec3_multiply(vec3_t a, float s)
{
	vec3_t result = {
		.x = a.x * s,
		.y = a.y * s,
		.z = a.z * s
	};

	return result;
}

vec3_t vec3_divide(vec3_t a, float s)
{
	vec3_t result = {
		.x = a.x / s,
		.y = a.y / s,
		.z = a.z / s
	};

	return result;
}

vec3_t vec3_crossProduct(vec3_t a, vec3_t b)
{
	vec3_t result = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return result;
}

void vec3_normalize(vec3_t* v)
{
	float magnitude = vec3_magnitude(*v);
	v->x = v->x / magnitude;
	v->y = v->y / magnitude;
	v->z = v->z / magnitude;
}

float vec3_dotProduct(vec3_t a, vec3_t b)
{
	float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return result;
}

vec3_t vec3_from_vec4(vec4_t vec4)
{
	const vec3_t result = {
		.x = vec4.x / vec4.w,
		.y = vec4.y / vec4.w,
		.z = vec4.z / vec4.w
	};
	return result;
}

vec4_t vec4_from_vec3(vec3_t vec3)
{
	const vec4_t vec4 = {
	.x = vec3.x,
	.y = vec3.y,
	.z = vec3.z,
	.w = 1
	};
	return vec4;
}

#pragma endregion 