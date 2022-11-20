#include "matrix.h"

mat4_t mat4_make_indentity(void)
{
	/* Visual Representation
		| 1 0 0 0 |
		| 0 1 0 0 |
		| 0 0 1 0 |
		| 0 0 0 1 |
	*/
	mat4_t m = { {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0 ,1}
	} };

	return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
	/* Visual Representation
	| sx 0 0 0 |
	| 0 sy 0 0 |
	| 0 0 sz 0 |
	| 0 0 0 1 |
*/
	mat4_t m = mat4_make_indentity();
	m.m[0][0] = sx; m.m[1][1] = sy; m.m[2][2] = sz;
	return m;
}

vec3_t mat4_multiply_vec3(mat4_t mat, vec3_t v)
{
	vec3_t result = {
	.x = (mat.m[0][0] * v.x) + (mat.m[0][1] * v.y) + (mat.m[0][2] * v.z) + (mat.m[0][4]),
	.y = (mat.m[1][0] * v.x) + (mat.m[1][1] * v.y) + (mat.m[1][2] * v.z) + (mat.m[1][4]),
	.z = (mat.m[2][0] * v.x) + (mat.m[2][1] * v.y) + (mat.m[2][2] * v.z) + (mat.m[2][4])
	};

	return result;
}

vec4_t mat4_multiply_vec4(mat4_t mat, vec4_t v)
{
	vec4_t result = {
		.x = (mat.m[0][0] * v.x) + (mat.m[0][1] * v.y) + (mat.m[0][2] * v.z) + (mat.m[0][3] * v.w),
		.y = (mat.m[1][0] * v.x) + (mat.m[1][1] * v.y) + (mat.m[1][2] * v.z) + (mat.m[1][3] * v.w),
		.z = (mat.m[2][0] * v.x) + (mat.m[2][1] * v.y) + (mat.m[2][2] * v.z) + (mat.m[2][3] * v.w),
		.w = (mat.m[3][0] * v.x) + (mat.m[3][1] * v.y) + (mat.m[3][2] * v.z) + (mat.m[3][3] * v.w),
	};
	return result;
}