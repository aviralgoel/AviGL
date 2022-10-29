#include "vector.h"
#include <math.h>



vec3_t vec3_rotate_x(vec3_t original, float angle)
{
	float y_prime = original.y * cos(angle) - original.z * sin(angle);
	float z_prime = original.y * sin(angle) + original.z * cos(angle);
	vec3_t rotated = { .x = original.x, .y = y_prime, .z = z_prime };
	return rotated;
}


vec3_t vec3_rotate_y(vec3_t original, float angle)
{
	float x_prime = original.x * cos(angle) + original.z * sin(angle);
	float z_prime = original.z * cos(angle) - original.x * sin(angle);
	vec3_t rotated = { .x = x_prime, .y = original.y, .z = z_prime };
	return rotated;
}

vec3_t vec3_rotate_z(vec3_t original, float angle)
{
	float x_prime = original.x * cos(angle) - original.y * sin(angle);
	float y_prime = original.y * cos(angle) + original.x * sin(angle);
	vec3_t rotated = { .x = x_prime, .y = y_prime, .z = original.z };
	return rotated;
}
