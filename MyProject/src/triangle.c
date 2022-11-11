#include "triangle.h"

#define swap(x,y) do{ \
unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
	   memcpy(swap_temp, &y, sizeof(x)); \
	   memcpy(&y, &x, sizeof(x)); \
	   memcpy(&x, swap_temp, sizeof(x)); \
	} while (0);

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

	if (y0 > y1)
	{
		swap(y0, y1);
		swap(x0, x1);
	}
	if (y0 > y2)
	{
		swap(y0, y2);
		swap(x0, x2);
	}
	if (y1 > y2)
	{
		swap(y1, y2);
		swap(x1, x2);

	}
	
	triangle_t sorted = {
	.points[0].x = x0, .points[0].y = y0,
	.points[1].x = x1, .points[1].y = y1,
	.points[2].x = x2, .points[2].y = y2, 
	};
	return sorted;
}
