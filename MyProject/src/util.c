#include "util.h"
#define PI 3.1415926 
float normalizeInRange(float value, float max, float min)
{
	float result = (value - min) / (max - min);
	return result;
}
int sorter_ascending(const void* first_arg, const void* second_arg)
{
	int first = *(int*)first_arg;
	int second = *(int*)second_arg;
	if (first > second)
	{
		return -1;
	}
	else if (first == second)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int sorter_descending(const void* first_arg, const void* second_arg)
{
	int first = *(int*)first_arg;
	int second = *(int*)second_arg;
	if (first < second)
	{
		return -1;
	}
	else if (first == second)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

float degreeToRadian(float degAngle)
{
	return (PI) * (degAngle / 180);
}

int partition_triangle_array(triangle_t arr[], int first_index, int last_index, float pivot)
{
	// Store a swap index
	int i = first_index;

	// Iterate through the current partition, swapping elements larger than the pivot
	for (int j = first_index; j < last_index; j++) {
		if (arr[j].avg_depth > pivot) {
			// Swap if necessary...
			if (i != j) {
				swap(arr[i], arr[j]);
			}

			// Increment our swap index since the current element is larger than our pivot
			i++;
		}
	}

	// Swap our pivot element into place
	swap(arr[i], arr[last_index]);

	// All elements up to arr[i] should be larger than the pivot so return the pivot index
	return i;
}

void quicksort_triangles(triangle_t arr[], int first_index, int last_index)
{
	if (first_index >= last_index) return;
	int pivot = arr[last_index].avg_depth;
	int partition_index = partition_triangle_array(arr, first_index, last_index, pivot);
	quicksort_triangles(arr, first_index, partition_index - 1);
	quicksort_triangles(arr, partition_index + 1, last_index);
}