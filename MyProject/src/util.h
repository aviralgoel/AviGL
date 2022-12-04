#pragma once

#include "triangle.h"

#pragma region quick sort
int sorter_ascending(const void* first_arg, const void* second_arg);
int sorter_descending(const void* first_arg, const void* second_arg);
int partition(triangle_t arr[], int l, int r, float pivot, int (*compare)(const void*, const void*));
void quick_sort(triangle_t arr[], int l, int r, int (*compare)(const void*, const void*));
float degreeToRadian(float degAngle);
#pragma endregion quick sort