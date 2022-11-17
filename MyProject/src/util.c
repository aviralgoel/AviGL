#include "util.h"
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
int partition(triangle_t arr[], int l, int r, float pivot, int (*compare)(const void*, const void*)) {
	while (l <= r) {
		while (compare(&arr[l].avg_depth, &pivot) == 1) {
			l++;
		}
		while (compare(&arr[r].avg_depth, &pivot) == -1) {
			r--;
		}
		if (l <= r) {
			swap(arr[l], arr[r]);
			l++;
			r--;
		}
	}
	return l;
}
void quick_sort(triangle_t arr[], int l, int r, int (*compare)(const void*, const void*)) {
	if (l >= r) {
		return;
	}
	float pivot = arr[l + (r - l) / 2].avg_depth;
	int partition_index = partition(arr, l, r, pivot, compare);
	quick_sort(arr, 0, partition_index - 1, compare);
	quick_sort(arr, partition_index, r, compare);
}