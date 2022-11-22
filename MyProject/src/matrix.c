#include "matrix.h"
#include <math.h>
mat4_t mat4_make_identity(void)
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

mat4_t mat4_make_rotation_z(const float theta)
{
	/* Visual Representation
	| cos(Th) -sin(Th) 0 0 |
	| sin(Th)  cos(Th) 0 0 |
	|	0		 0	   1 0 |
	|	0		 0     0 1 |
*/
	mat4_t m = mat4_make_identity();
	m.m[0][0] = cos(theta) ; m.m[0][1] = (-1)*sin(theta);
	m.m[1][0] = sin(theta); m.m[1][1] = cos(theta);
	return m;
}
mat4_t mat4_make_rotation_y(const float theta)
{
	/* Visual Representation
	| cos(Th) 0 sin(Th) 0 |
	|	0	  1	   0	0 |
	| -sin(Th) 0 cos(Th) 0 |
	|	0	  0     0   1 |
*/
	mat4_t m = mat4_make_identity();
	m.m[0][0] = cos(theta); m.m[0][2] = sin(theta);
	m.m[2][0] = (-1)*sin(theta); m.m[2][2] = cos(theta);
	return m;
}
mat4_t mat4_make_rotation_x(const float theta)
{
	/* Visual Representation
	| 1		0		0	  0 |
	| 0  cos(Th) -sin(Th) 0 |
	| 0	 sin(Th)  cos(Th) 0 |
	| 0		 0     0	  1	|
*/
	mat4_t m = mat4_make_identity();
	m.m[1][1] = cos(theta); m.m[1][2] = (-1) * sin(theta);
	m.m[2][1] = sin(theta); m.m[2][2] = cos(theta);
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
	mat4_t m = mat4_make_identity();
	m.m[0][0] = sx; m.m[1][1] = sy; m.m[2][2] = sz;
	return m;
}

mat4_t mat4_make_translate(float tx, float ty, float tz)
{	/* Visual Representation
	| 1 0 0 Tx |
	| 0 1 0 Ty |
	| 0 0 1 Tz |
	| 0 0 0 1 |
	*/
	mat4_t tMat = mat4_make_identity();
	tMat.m[0][3] = tx;
	tMat.m[1][3] = ty;
	tMat.m[2][3] = tz;

	return tMat;
}

mat4_t mat4_multiply_mat4(mat4_t matA, mat4_t matB)
{
	mat4_t result = mat4_make_identity();
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 4 ; j++)
		{
			result.m[i][j] = 
				matA.m[i][0] * matB.m[0][j] + 
				matA.m[i][1] * matB.m[1][j] + 
				matA.m[i][2] * matB.m[2][j] + 
				matA.m[i][3] * matB.m[3][j];
		}
	}
	return result;
}

vec3_t mat4_multiply_vec3(mat4_t mat, vec3_t v)
{
	const vec3_t result = {
	.x = (mat.m[0][0] * v.x) + (mat.m[0][1] * v.y) + (mat.m[0][2] * v.z) + (mat.m[0][3]),
	.y = (mat.m[1][0] * v.x) + (mat.m[1][1] * v.y) + (mat.m[1][2] * v.z) + (mat.m[1][3]),
	.z = (mat.m[2][0] * v.x) + (mat.m[2][1] * v.y) + (mat.m[2][2] * v.z) + (mat.m[2][3])
	};

	return result;
}

vec4_t mat4_multiply_vec4(mat4_t mat, vec4_t v)
{
	const vec4_t result = {
		.x = (mat.m[0][0] * v.x) + (mat.m[0][1] * v.y) + (mat.m[0][2] * v.z) + (mat.m[0][3] * v.w),
		.y = (mat.m[1][0] * v.x) + (mat.m[1][1] * v.y) + (mat.m[1][2] * v.z) + (mat.m[1][3] * v.w),
		.z = (mat.m[2][0] * v.x) + (mat.m[2][1] * v.y) + (mat.m[2][2] * v.z) + (mat.m[2][3] * v.w),
		.w = (mat.m[3][0] * v.x) + (mat.m[3][1] * v.y) + (mat.m[3][2] * v.z) + (mat.m[3][3] * v.w),
	};
	return result;
}