#pragma once

// push to an array
#define array_push(array, value)                                              \
    do {                                                                      \
        (array) = array_hold((array), 1, sizeof(*(array)));                   \
        (array)[array_length(array) - 1] = (value);                           \
    } while (0); // happens only once

// hold the elements in the array
void* array_hold(void* array, int count, int item_size);
int array_length(void* array); // return length of array
void array_free(void* array); // free the memory allocated when array is deleted

