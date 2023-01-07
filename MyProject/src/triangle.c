#include "triangle.h"

/// <summary>
/// accepts a triangle with vertex in any order, return a triangle where y0 &lt; y1 &lt; y2
/// </summary>
/// <param name="unsorted"></param>
/// <returns></returns>
triangle_t sortVertsByY(triangle_t unsorted)
{
	int x0 = unsorted.points[0].x, y0 = unsorted.points[0].y,
		x1 = unsorted.points[1].x, y1 = unsorted.points[1].y,
		x2 = unsorted.points[2].x, y2 = unsorted.points[2].y;

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
	}
	if (y0 > y2)
	{
		swap(y0, y2);
		swap(x0, x2);
		swap(u0, u2);
		swap(v0, v2);
	}
	if (y1 > y2)
	{
		swap(y1, y2);
		swap(x1, x2);
		swap(u1, u2);
		swap(v1, v2);
	}

	triangle_t sorted = {
	.points[0].x = x0, .points[0].y = y0,
	.points[1].x = x1, .points[1].y = y1,
	.points[2].x = x2, .points[2].y = y2,
	.texcoords[0].u = u0, .texcoords[0].v = v0,
	.texcoords[1].u = u1, .texcoords[1].v = v1,
	.texcoords[2].u = u2, .texcoords[2].v = v2
	};
	return sorted;
}