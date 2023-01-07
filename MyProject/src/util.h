#pragma once

#include "triangle.h"

// generic methods that not specific to graphics programming
float normalizeInRange(float value, float max, float min);
int sorter_ascending(const void* first_arg, const void* second_arg);
int sorter_descending(const void* first_arg, const void* second_arg);
float degreeToRadian(float degAngle);
int partition_triangle_array(triangle_t arr[], int first_index, int last_index, float pivot);
void quicksort_triangles(triangle_t arr[], int first_index, int last_index);
