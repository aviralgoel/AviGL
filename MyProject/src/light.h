#pragma once

#include <stdint.h>
#include "vector.h"

typedef struct {
	vec3_t direction;
} light_t;

// simplified alternative to phong lighting model
uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);
void init_light(vec3_t direction);
vec3_t get_light_direction();
