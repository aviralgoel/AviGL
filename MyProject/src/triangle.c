#include "triangle.h"

/// <summary>
/// accepts a triangle with vertex in any order, return a triangle where y0 is the vertex
/// with highest y value
/// </summary>
/// <param name="unsorted"></param>
/// <returns></returns>
triangle_t sortVertsByY(triangle_t unsorted)
{
	int x0 = unsorted.points[0].x, y0 = unsorted.points[0].y,
		x1 = unsorted.points[1].x, y1 = unsorted.points[1].y,
		x2 = unsorted.points[2].x, y2 = unsorted.points[2].y;

	float z0, z1, z2, w1, w0, w2;
	z0 = unsorted.points[0].z, w0 = unsorted.points[0].w,
		z1 = unsorted.points[1].z, w1 = unsorted.points[1].w,
		z2 = unsorted.points[2].z, w2 = unsorted.points[2].w;
	float li0, li1, li2;
	li0 = unsorted.lightIntensities[0]; li1 = unsorted.lightIntensities[1]; li2 = unsorted.lightIntensities[2];
	// texel coordinates of vertices
	float u0, u1, u2, v0, v1, v2;
	u0 = unsorted.texcoords[0].u; u1 = unsorted.texcoords[1].u; u2 = unsorted.texcoords[2].u;
	v0 = unsorted.texcoords[0].v; v1 = unsorted.texcoords[1].v; v2 = unsorted.texcoords[2].v;

	if (y0 > y1)
	{
		swap(y0, y1);
		swap(x0, x1);
		swap(u0, u1);
		swap(v0, v1);
		swap(w0, w1);
		swap(z0, z1);
		swap(li0, li1);
	}
	if (y0 > y2)
	{
		swap(y0, y2);
		swap(x0, x2);
		swap(u0, u2);
		swap(v0, v2);
		swap(w0, w2);
		swap(z0, z2);
		swap(li0, li2);
	}
	if (y1 > y2)
	{
		swap(y1, y2);
		swap(x1, x2);
		swap(u1, u2);
		swap(v1, v2);
		swap(w1, w2);
		swap(li1, li2);
	}

	triangle_t sorted = {
	.points[0].x = x0, .points[0].y = y0, .points[0].z = z0, .points[0].w = w0,
	.points[1].x = x1, .points[1].y = y1, .points[1].z = z1, .points[1].w = w1,
	.points[2].x = x2, .points[2].y = y2, .points[2].z = z2, .points[2].w = w2,
	.texcoords[0].u = u0, .texcoords[0].v = v0,
	.texcoords[1].u = u1, .texcoords[1].v = v1,
	.texcoords[2].u = u2, .texcoords[2].v = v2,
	.lightIntensities[0] = li0, .lightIntensities[1] = li1, .lightIntensities[2] = li2,
	.color = unsorted.color,
	.texture = unsorted.texture
	};
	return sorted;
}