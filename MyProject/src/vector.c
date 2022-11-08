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

vec2_t vec2_add(vec2_t a, vec2_t b)
{
	vec2_t result = { .x = a.x + b.x, .y = a.y + b.y };
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


float vec2_dotProduct(vec2_t a, vec2_t b)
{
	float result = (a.x * b.x) + (a.y * b.y);
	return result;
}

#pragma endregion Vector TWOD Functions

#pragma region Vector THREED Functions
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


float vec3_dotProduct(vec3_t a, vec3_t b)
{
	float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return result;
}

#pragma endregion Vector THREED Functions