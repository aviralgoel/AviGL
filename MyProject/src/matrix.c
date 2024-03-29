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
	m.m[0][0] = cos(theta); m.m[0][1] = (-1) * sin(theta);
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
	m.m[2][0] = (-1) * sin(theta); m.m[2][2] = cos(theta);
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
// Takes in world space and spits out image space (As far as I understand)
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar) {
	// | 1/tan(fov/2)*(aspect)             0           0              0	   |
	// |                  0  1/tan(fov/2)              0                 0 |
	// |                  0             0     zf/(zf-zn)  (-zf*zn)/(zf-zn) |
	// |                  0             0              1                 0 |
	mat4_t m = { {{ 0 }} };
	m.m[0][0] = aspect * (1 / tan(fov / 2));
	m.m[1][1] = 1 / tan(fov / 2);
	m.m[2][2] = zfar / (zfar - znear);
	m.m[2][3] = (-zfar * znear) / (zfar - znear);
	m.m[3][2] = 1.0;
	return m;
}
vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
	// multiply the projection matrix by our original vector
	vec4_t result = mat4_multiply_vec4(mat_proj, v);

	// perform perspective divide with original z-value that is now stored in w
	if (result.w != 0.0) {
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return result;
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
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
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
mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up)
{
	vec3_t z = vec3_subtract(target, eye); 	vec3_normalize(&z);
	vec3_t x = vec3_crossProduct(up, z);	vec3_normalize(&x);
	vec3_t y = vec3_crossProduct(z, x);

	mat4_t view_matrix = { {
		{x.x, x.y, x.z, -vec3_dotProduct(x, eye)},
		{y.x, y.y, y.z, -vec3_dotProduct(y, eye)},
		{z.x, z.y, z.z, -vec3_dotProduct(z, eye)},
		{0 ,	0,	0,				 1          },
		} };

	return view_matrix;
}